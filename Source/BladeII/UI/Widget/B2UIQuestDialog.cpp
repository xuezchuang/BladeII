#include "BladeII.h"
#include "B2UIQuestDialog.h"
#include "B2RichTextBlock.h"
#include "QuestDataTable.h"
#include "QuestManager.h"
#include "B2ItemInfo.h"
#include "B2UIManager.h"
#include "Quest.h"
#include "Retailer.h"
#include "BladeIIGameMode.h"
#include "B2MissionInfo.h"
#include "B2UIRewardNotice.h"
#include "B2UIMsgPopupReward.h"
#include "B2UIRewardEventSet.h"
#include "B2UIDocHelper.h"
#include "B2UIDocSome.h"
#include "B2UILinkManager.h"
#include "TutorialManager.h"
#include "BladeIIUtil.h"
#include "B2UIMailRewardPopUp.h"
#include "B2UIMission.h"


#include "BladeIIGameImpl.h"

#define DEFAULT_NPCINDEX 100000

UB2UIQuestDialog::UB2UIQuestDialog(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	MaxCharShowInterval = 0.15f;
	bShowServerRewardItem = false;
	ProgressColorHex = TEXT("ffffff");
	IsInStageMode = false;
	NextMainQuestPtr = nullptr;
	NextMainQuestSlot = 0;
	CurrRewardIconIdx = 0;
	MissionID = 0;
	CurRewardButtonType = EQuestRewardButtonType::Max;
	ShowCompleteNoticeDelayTime = 0.0f;
}

void UB2UIQuestDialog::CacheAssets()
{
	RewardIcons.AddUninitialized(4);

	GET_SLOT(UB2RichTextBlock, RTB_NpcName);
	GET_SLOT(UB2RichTextBlock, RTB_QuestName1);
	GET_SLOT(UB2RichTextBlock, RTB_QuestName2);
	GET_SLOT(UB2RichTextBlock, RTB_QuestTitle);
	GET_SLOT(UB2RichTextBlock, RTB_Dialog);
	GET_SLOT(UHorizontalBox, HB_QuestName);
	GET_SLOT(UOverlay, O_Dialog);
	GET_SLOT(UOverlay, O_Reward);
	GET_SLOT(UImage, IMG_Npc);
	GET_SLOT(UButton, BTN_Dialogue_Next);
	GET_SLOT(UImage, IMG_Dialogue_Next);
	GET_SLOT(UButton, BTN_Result);
	GET_SLOT(UButton, BTN_Shortcut);
	GET_SLOT(UTextBlock, TB_SuccessReward);
	GET_SLOT(UTextBlock, TB_ConfirmText);
	GET_SLOT(UTextBlock, TB_ConfirmText2);
	GET_SLOT(UButton, BTN_Close);
	GET_SLOT_BYNAME(UB2UIRewardIcon, RewardIcons[0], UIP_RewardIcon1);
	GET_SLOT_BYNAME(UB2UIRewardIcon, RewardIcons[1], UIP_RewardIcon2);
	GET_SLOT_BYNAME(UB2UIRewardIcon, RewardIcons[2], UIP_RewardIcon3);
	GET_SLOT_BYNAME(UB2UIRewardIcon, RewardIcons[3], UIP_RewardIcon4);

	for (auto Ele : RewardIcons)
	{
		if (Ele.IsValid())
		{
			Ele->Init();
		}
	}

	GET_SLOT(UB2UIRewardNotice, QuestCompleteNotice);
	if (QuestCompleteNotice.IsValid())
	{
		QuestCompleteNotice->Init();
	}

	GET_SLOT(UB2Button, BTN_GiveUp);
	GET_SLOT(UTextBlock, TB_GiveUp);

	QuestSlotIndex = 0;
	DivisionTable = 0;
}

void UB2UIQuestDialog::UpdateStaticText()
{
	if (TB_SuccessReward.IsValid())
	{
		TB_SuccessReward->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Quest_CompleteRewards"))));
	}
	if (TB_ConfirmText.IsValid())
	{
		TB_ConfirmText->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Quest_ShortOut"))));
	}
	if (TB_ConfirmText2.IsValid())
	{
		TB_ConfirmText2->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Quest_Confirm"))));
	}
	if (TB_GiveUp.IsValid())
	{
		TB_GiveUp->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Quest_abandon"))));
	}
}

void UB2UIQuestDialog::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Dialogue_Next, &UB2UIQuestDialog::OnClickDialogueNext);
	BIND_CLICK_FUNC_TO_BTN(BTN_Result, &UB2UIQuestDialog::OnClickConfirm);
	BIND_CLICK_FUNC_TO_BTN(BTN_Shortcut, &UB2UIQuestDialog::OnClickConfirm);
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIQuestDialog::OnClickClose);
	BIND_CLICK_FUNC_TO_BTN(BTN_GiveUp, &UB2UIQuestDialog::OnClickGiveUp);
}

void UB2UIQuestDialog::NativeConstruct()
{
	Super::NativeConstruct();

	ManualLastRealTime = 0;
	ManualRealDeltaTime = 0;
}

void UB2UIQuestDialog::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	UWorld* World = GetWorld();

	UpdateManualTimeCounter();

	bool IsLastChar = false;
	if (ProgressiveDialogText.Tick(ManualLastRealTime, InDeltaTime, IsLastChar))
		FinalApplyDialogText(IsLastChar);
}

void UB2UIQuestDialog::UpdateManualTimeCounter()
{
	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		float ThisRealTime = TheWorld->GetRealTimeSeconds();
		ManualRealDeltaTime = ThisRealTime - ManualLastRealTime;
		ManualLastRealTime = ThisRealTime;
	}
	else
	{
		ManualRealDeltaTime = 0.0f;
		ManualLastRealTime = 0.0f;
	}
}

void UB2UIQuestDialog::UpdateDialog()
{
	UpdateRewardItem(DialogType);
}

void UB2UIQuestDialog::OpenDialogBySlotIndex(int32 QuestSlot)
{
	DialogType = EDialogType::Quest;
	QuestSlotIndex = QuestSlot;
	QuestPtr QuestPtrInfo = QuestManager::GetInstance().GetQuest(QuestSlotIndex);
	if (QuestPtrInfo != nullptr)
	{
		QuestID = QuestPtrInfo->GetQuestId();
		QuestType = QuestPtrInfo->GetType();
		DivisionTable = QuestPtrInfo->GetType();
		DivisionStat = QuestPtrInfo->GetState();
		
		switch (DivisionStat)
		{
		case EQS_NOT_ACCEPTED:
			if (QuestType == DAILY_QUEST)
				OnClickConfirm();
			break;
		case EQS_PROGRESS:
			SetOpenQuestID(QuestPtrInfo->GetQuestId());
			
			break;
		case EQS_COMPLETED:
			SetRewardQuestID(QuestPtrInfo->GetQuestId());
			break;
		}
	}

	bool IsStartMode = DivisionStat != EQS_COMPLETED;
	SetPlaySound(IsStartMode);
}

void UB2UIQuestDialog::OpenRewardDialog(const class Quest& QuestRef, const TArray<b2network::B2RewardPtr>& ServerRewardInfo)
{
	DialogType = EDialogType::Quest;
	QuestID = QuestRef.GetQuestId();
	QuestType = QuestRef.GetType();
	DivisionTable = QuestRef.GetType();
	DivisionStat = QuestRef.GetState();

	// MainQuest는 서버에서 별다른 완료처리 없이 바로 다음퀘스트를 이어보내준다.
	// 여기까지 들어온 QuestRef는 실제로 서버에서 완료됐다 하더라도 EQS_PROGRESS 상태
	// 따라서 UI Flag만 EQS_COMPLETED로 변경 해 준다.
	if (QuestType == MAIN_QUEST && QuestRef.GetState() == EQS_PROGRESS)
		DivisionStat = EQS_COMPLETED;

	bShowServerRewardItem = true;
	RewardInfo = ServerRewardInfo;
	SetRewardQuestID(QuestRef.GetQuestId());
	bShowServerRewardItem = false;

	SetPlaySound(false);
}

void UB2UIQuestDialog::OpenAutoClearQuest()
{
	QuestManager::GetInstance().UpdateQuestState(QuestSlotIndex, EQS_COMPLETED);
	SetNextMainQuest(QuestManager::GetInstance().GetCachedCurrentQuest());
	OpenDialogBySlotIndex(QuestSlotIndex);
}

void UB2UIQuestDialog::SetToggleQuestTitleWidget(bool IsVisibleQuestTitle)
{
	if (HB_QuestName.IsValid())
		HB_QuestName->SetVisibility(IsVisibleQuestTitle ? ESlateVisibility::Hidden : ESlateVisibility::HitTestInvisible);
}

void UB2UIQuestDialog::SetNextMainQuest(QuestPtr NextMainQuest)
{
	if (NextMainQuest != nullptr && NextMainQuest->GetType() == MAIN_QUEST)
	{
		NextMainQuestPtr = NextMainQuest;
	}
}

void UB2UIQuestDialog::SetDialogCloseDelegate(FOnQuestDialogClosed&& CloseDelegate)
{
	OnQuestDialogDelegate = CloseDelegate;
}

void UB2UIQuestDialog::OpenMissionCompleteDialog(int32 InMissionID)
{
	DialogType = EDialogType::Mission;
	MissionID = InMissionID;
	SetMissionDialogText(MissionID);
	SetNpc(DEFAULT_NPCINDEX);

	MissionManager::GetInstance().FlushCachedCompleteMission(EMissionType::Max);
}

void UB2UIQuestDialog::SetMissionDialogText(int32 InMissionID)
{
	if (MissionPtr Mission = MissionManager::GetInstance().GetMission(InMissionID))
	{
		FString DescKey = GetMissionDescKey(Mission->MissionID);
		FText DescText = FText::FromString(TEXT("Unknown"));
		if (FText::FindText(B2LOC_CAT_MISSIONTEXT, DescKey, DescText, nullptr) == false)
		{
			DescKey = GetMissionDescKey(Mission->MissionID, Mission->RewardStep + 1);
			DescText = BladeIIGetLOCText(B2LOC_CAT_MISSIONTEXT, DescKey);
		}

		DescText = FText::Format(FText::FromString(TEXT("[{0}] {1}")), GetMissionTypeText(InMissionID), DescText);
		DescText = FText::Format(DescText, FText::AsNumber(GenerateGoalCount(Mission)));
		DescText = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Mission_Complete")), DescText);

		FText::Format(FText::FromString(TEXT("[{0}] {1}")), GetMissionTypeText(InMissionID), DescText);

		FText QuestTitleText;

		// Note : 기본 Title을 검색해보고 없다면 Step을 넣어서 한번 더 찾아준다.
		if (FText::FindText(B2LOC_CAT_MISSIONTEXT, GetMissionTitleKey(InMissionID), QuestTitleText, nullptr) == false)
			QuestTitleText = MissionManager::GetInstance().GetMissionTitle(InMissionID, Mission->CurrentStep);

		SetQuestNameText(DescText);
		SetQuestTitleText(QuestTitleText);
	}

	DialogArr.Empty();
	PageCount = 0;
	DialogArr.Add(BladeIIGetLOCText(B2LOC_CAT_MISSIONTEXT, TEXT("MissionCompleteDescription")).ToString());

	if (IMG_Dialogue_Next.IsValid())
		IMG_Dialogue_Next->SetVisibility(ESlateVisibility::Hidden);

	OpenDialog();
}

int32 UB2UIQuestDialog::GenerateGoalCount(MissionPtr Mission)
{
	if (Mission->MissionType == EMissionType::Serial)
	{
		b2network::B2mdSerialMissionInfoPtr	MD_MissionData = MissionManager::GetInstance().GetSerialMissionMasterInfo(MissionID, Mission->CurrentStep);

		if (MD_MissionData)
		{
			if (MD_MissionData->event_type == b2network::B2GameEventType::ENHANCE_EQUIPMENT)
				return MD_MissionData->condition1;
		}
	}

	return Mission->GoalCount;
}

void UB2UIQuestDialog::AddRewardIcon(ERewardType Type, int32 Count)
{
	if (Count > 0 && RewardIcons.IsValidIndex(CurrRewardIconIdx))
	{
		if (RewardIcons[CurrRewardIconIdx].IsValid())
		{
			RewardIcons[CurrRewardIconIdx]->UpdateRewardIcon(Type, Count);
			RewardIcons[CurrRewardIconIdx]->SetVisibility(ESlateVisibility::Visible);
			CurrRewardIconIdx++;
		}
	}
}

void UB2UIQuestDialog::AddRewardIcon(const int InRewardId, int32 Count)
{
	if (Count > 0 && RewardIcons.IsValidIndex(CurrRewardIconIdx))
	{
		if (RewardIcons[CurrRewardIconIdx].IsValid())
		{
			RewardIcons[CurrRewardIconIdx]->UpdateRewardIcon(InRewardId, Count);
			RewardIcons[CurrRewardIconIdx]->SetVisibility(ESlateVisibility::Visible);
			CurrRewardIconIdx++;
		}
	}
}

void UB2UIQuestDialog::SetRewardButtonType(EQuestRewardButtonType RewardButtonType)
{
	CurRewardButtonType = RewardButtonType;
}

void UB2UIQuestDialog::SetVisibilityRewardButtonType(EQuestRewardButtonType RewardButtonType)
{
	if (BTN_Shortcut.IsValid() == false || BTN_Result.IsValid() == false || TB_ConfirmText2.IsValid() == false || BTN_GiveUp.IsValid() == false)
	{
		return;
	}

	switch (RewardButtonType)
	{
	case EQuestRewardButtonType::Max:
		BTN_GiveUp->SetVisibility(ESlateVisibility::Hidden);
		BTN_Shortcut->SetVisibility(ESlateVisibility::Hidden);
		BTN_Result->SetVisibility(ESlateVisibility::Hidden);
		break;
	case EQuestRewardButtonType::Shortcut:
		BTN_GiveUp->SetVisibility(ESlateVisibility::Hidden);
		BTN_Shortcut->SetVisibility(ESlateVisibility::Visible);
		BTN_Result->SetVisibility(ESlateVisibility::Hidden);
		break;
	case EQuestRewardButtonType::GiveUp:
		BTN_GiveUp->SetVisibility(ESlateVisibility::Visible);
		BTN_Shortcut->SetVisibility(ESlateVisibility::Visible);
		BTN_Result->SetVisibility(ESlateVisibility::Hidden);
		break;
	case EQuestRewardButtonType::GetReward:
		BTN_GiveUp->SetVisibility(ESlateVisibility::Hidden);
		BTN_Shortcut->SetVisibility(ESlateVisibility::Hidden);
		BTN_Result->SetVisibility(ESlateVisibility::Visible);
		TB_ConfirmText2->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Quest_Confirm"))));
		break;
	case EQuestRewardButtonType::GetAchivement:
		BTN_GiveUp->SetVisibility(ESlateVisibility::Hidden);
		BTN_Shortcut->SetVisibility(ESlateVisibility::Hidden);
		BTN_Result->SetVisibility(ESlateVisibility::Visible);
		TB_ConfirmText2->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Quest_GoToAchievement"))));
		break;
	}
}

void UB2UIQuestDialog::SetOpenQuestID(int32 id)
{
	FQuestCommonInfo* QuestInfo = nullptr;
	UB2QuestInfoTable* QuestTable = StaticFindQuestTable();

	if (QuestTable != nullptr)
	{
		DivisionTable == MAIN_QUEST ? QuestInfo = QuestTable->GetQuestMainInfo(id) : QuestInfo = QuestTable->GetQuestDailyInfo(id);
		if (QuestInfo != nullptr)
		{
			SetQuestTitleText(BladeIIGetLOCText(B2LOC_CAT_QUESTTEXT, FString::FromInt(QuestInfo->QuestName)));
			SetToggleQuestTitleWidget(true);
			SetDialogText(QuestInfo->StartDialog);
			SetNpc(QuestInfo->StartNPC);

			OpenDialog();
		}
	}
}

void UB2UIQuestDialog::SetRewardQuestID(int32 id)
{
	FQuestCommonInfo* QuestInfo = nullptr;
	UB2QuestInfoTable* QuestTable = StaticFindQuestTable();

	if (QuestTable != nullptr)
	{
		QuestInfo = (DivisionTable == MAIN_QUEST) ? QuestTable->GetQuestMainInfo(id) : QuestTable->GetQuestDailyInfo(id);
		if (QuestInfo != nullptr)
		{
			SetQuestTitleText(BladeIIGetLOCText(B2LOC_CAT_QUESTTEXT, FString::FromInt(QuestInfo->QuestName)));
			SetToggleQuestTitleWidget(true);
			SetDialogText(QuestInfo->CompleteDialog);
			SetNpc(QuestInfo->CompleteNPC);

			OpenDialog();
		}
	}
}

void UB2UIQuestDialog::SetCloseQuestID(int32 id)
{
}

void UB2UIQuestDialog::SetQuestTitleText(const FText& TitleText)
{
	TArray<FText> ParsedTextList;
	ParseTextFromSpecialChar(TitleText, TEXT("/div"), ParsedTextList);

	if (ParsedTextList.IsValidIndex(0))
	{
		if (RTB_QuestTitle.IsValid())
			RTB_QuestTitle->SetText(ParsedTextList[0]);
	}
}

void UB2UIQuestDialog::SetQuestNameText(const FText& TitleText)
{
	TArray<FText> ParsedTextList;
	ParseTextFromSpecialChar(TitleText, TEXT("/div"), ParsedTextList);

	if (ParsedTextList.IsValidIndex(0))
	{
		if (RTB_QuestName1.IsValid())
			RTB_QuestName1->SetText(ParsedTextList[0]);
	}

	if (RTB_QuestName2.IsValid())
	{
		if (ParsedTextList.IsValidIndex(1))
			RTB_QuestName2->SetText(ParsedTextList[1]);
		else
			RTB_QuestName2->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UB2UIQuestDialog::SetQuestNameText(const FText& TitleText, const FString& Strnum)
{
	FString Temp = TitleText.ToString();

	FString ProgressRichText = FString::Printf(TEXT("<span color=\"#%s\">%s</>"), *ProgressColorHex, *Strnum);

	SetQuestNameText(FText::FromString(Temp + TEXT(" ") + ProgressRichText));
}

void UB2UIQuestDialog::SetDialogText(int32 index)
{
	DialogArr.Empty();
	PageCount = 0;
	// 페이지넘김 키워드 /page
	FString str = BladeIIGetLOCText(B2LOC_CAT_QUESTTEXT, FString::FromInt(index)).ToString();///임시 스트링
	FText OrgText = BladeIIGetLOCText(B2LOC_CAT_QUESTTEXT, FString::FromInt(index));
	TArray<FText> ParsedTextList;

	ParseTextFromSpecialChar(OrgText, TEXT("/page"), ParsedTextList);
	for (const auto& ParsedText : ParsedTextList)
		DialogArr.Add(ParsedText.ToString());

	if (IMG_Dialogue_Next.IsValid())
		IMG_Dialogue_Next->SetVisibility(ESlateVisibility::Hidden);
}

void UB2UIQuestDialog::SetNpc(int32 index)
{
	if (IMG_Npc.IsValid())
	{
		//엔피씨는 10만번대부터 시작이라... 우선은 이렇게...
		int32 NPCImageIndex = index % 100000;
		UTexture2D* LoadedNPCImage = GetNPCImageTexture(NPCImageIndex);
		if (LoadedNPCImage)
		{
			IMG_Npc->SetBrushFromTexture(LoadedNPCImage);
		}
	}

	if (RTB_NpcName.IsValid())
		RTB_NpcName->SetText(BladeIIGetLOCText(B2LOC_CAT_QUESTTEXT, FString::FromInt(index)));
}

void UB2UIQuestDialog::SetRewardItem()
{
	// 초기화
	SetRewardButtonType(EQuestRewardButtonType::Max); // 보상 받기 버튼은 초기화 후 아래에서 변경하도록 하자.
	SetVisibilityRewardButtonType(EQuestRewardButtonType::Max);

	// 업데이트
	UpdateRewardItem(DialogType);
}

void UB2UIQuestDialog::SetQuestCompleteMessage()
{
	if (QuestCompleteNotice.IsValid() == false)
		return;

	if (CurRewardButtonType == EQuestRewardButtonType::Max)
	{
		QuestCompleteNotice->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	FString QuestClearGeneralTextKey;
	if (DialogType == EDialogType::Quest)
	{
		if (DivisionStat == EQS_COMPLETED)
		{
			if(QuestType == MAIN_QUEST || QuestType == DAILY_QUEST)
				QuestClearGeneralTextKey = TEXT("Quest_QuestComplete");
		}
	}
	else if (DialogType == EDialogType::Mission)
	{
		QuestClearGeneralTextKey = TEXT("Quest_AchievementComplete");
	}

	if (QuestClearGeneralTextKey.IsEmpty())
	{
		QuestCompleteNotice->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	if (QuestCompleteNotice->IsVisible() == false)
	{
		QuestCompleteNotice->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		QuestCompleteNotice->SetTextAndStartAnimation(BladeIIGetLOCText(B2LOC_CAT_GENERAL, QuestClearGeneralTextKey).ToString(), ShowCompleteNoticeDelayTime);
	}
}

void UB2UIQuestDialog::UpdateRewardItem(EDialogType InDialogType)
{
	switch (InDialogType)
	{
	case EDialogType::Quest:
	{
		UB2QuestInfoTable* QuestInfoTable = StaticFindQuestTable();
		FQuestCommonInfo* QuestTable = nullptr;
		if (QuestInfoTable != nullptr)
			QuestTable = (QuestType == MAIN_QUEST) ? QuestInfoTable->GetQuestMainInfo(QuestID) : QuestInfoTable->GetQuestDailyInfo(QuestID);

		FText ProgressText;
		if (QuestType == MAIN_QUEST)
		{
			b2network::B2mdMainQuestInfoPtr QuestMasterInfo = QuestManager::GetInstance().GetMainQuestMasterInfo(QuestID);
			if (QuestMasterInfo != nullptr)
			{
				ProgressText = DivisionStat == EQS_PROGRESS ? GetProgressText(QuestID, QuestType, QuestMasterInfo->goal_count) : GetProgressCompletedText(QuestID, QuestType);
				RewardArr = QuestMasterInfo->rewards;
			}
		}
		else if (QuestType == DAILY_QUEST)
		{
			b2network::B2mdDailyQuestInfoPtr QuestMasterInfo = QuestManager::GetInstance().GetDailyQuestMasterInfo(QuestID);
			if (QuestMasterInfo != nullptr)
			{
				ProgressText = GetProgressText(QuestID, QuestType, QuestMasterInfo->goal_count);
				RewardArr = QuestMasterInfo->rewards;
			}
		}

		QuestPtr QuestPtrInfo = QuestManager::GetInstance().GetQuest(QuestSlotIndex);
		if (QuestTable != nullptr)
		{
			FText QuestText = DivisionStat == EQS_COMPLETED ? ProgressText : QuestPtrInfo ? GetProgressText(QuestPtrInfo) : ProgressText;
			SetQuestNameText(BladeIIGetLOCText(B2LOC_CAT_QUESTTEXT, FString::FromInt(QuestTable->OutLine)), QuestText.ToString());
		}

		if (bShowServerRewardItem)
		{
			SetDisplayServerReward(RewardInfo);
		}
		else
		{
			for (int32 i = 0; i < 4; ++i)
			{
				if (RewardIcons[i].IsValid())
				{
					if (RewardArr.IsValidIndex(i) && RewardArr[i]->raw_reward->id != 0)
						RewardIcons[i]->UpdateRewardIcon(RewardArr[i]);
					else
						RewardIcons[i]->SetVisibility(ESlateVisibility::Collapsed);
				}
			}
		}

		if (BTN_Close.IsValid())
		{
			if ((QuestType == MAIN_QUEST) && (DivisionStat == EQS_COMPLETED))
			{
				BTN_Close->SetVisibility(ESlateVisibility::Hidden);
			}
			else
			{
				BTN_Close->SetVisibility(ESlateVisibility::Visible);
			}

		}

		bool IsProgress = false;
		b2network::B2mdMainQuestInfoPtr QuestMasterInfo = QuestManager::GetInstance().GetMainQuestMasterInfo(QuestID);
		if (QuestMasterInfo != nullptr && QuestTable != nullptr && DivisionStat == EQS_PROGRESS && QuestMasterInfo->goal_count > QuestPtrInfo->GetProgressCount())
		{
			IsProgress = true;
		}

		bool IsSubQuest = false;
		if (QuestPtrInfo != nullptr && QuestPtrInfo->GetQuestSlot() == b2network::B2QuestSlotType::MAIN2)
		{
			IsSubQuest = true;
		}

		if (IsSubQuest == true && IsProgress == true)
		{
			SetRewardButtonType(EQuestRewardButtonType::GiveUp);
		}
		else
		{
			SetRewardButtonType((DivisionStat == EQS_COMPLETED) ? EQuestRewardButtonType::GetReward : EQuestRewardButtonType::Shortcut);
		}
	}
	break;
	case EDialogType::Mission:
	{
		if (auto MissionPtr = MissionManager::GetInstance().GetMission(MissionID))
		{
			if (RewardIcons.Num() > 3)
			{
				if (RewardIcons[0].IsValid())
					RewardIcons[0]->UpdateRewardIcon(MissionPtr->RewardID, MissionPtr->RewardCount);
				if (RewardIcons[1].IsValid())
				{
					if (MissionPtr->CompletePoint != 0)
						RewardIcons[1]->UpdateRewardIconByItemIndex(FItemRefIDHelper::GetGoodsID_MissionPoint(), FText::AsNumber(MissionPtr->CompletePoint));
					else
						RewardIcons[1]->SetVisibility(ESlateVisibility::Collapsed);
				}
				if (RewardIcons[2].IsValid())
					RewardIcons[2]->SetVisibility(ESlateVisibility::Collapsed);
				if (RewardIcons[3].IsValid())
					RewardIcons[3]->SetVisibility(ESlateVisibility::Collapsed);
			}
		}

		if (BTN_Close.IsValid())
		{
			SetClose(true);
			BTN_Close->SetVisibility(ESlateVisibility::Visible);
		}

		SetRewardButtonType(EQuestRewardButtonType::GetAchivement);
	}
	break;
	}
}

void UB2UIQuestDialog::SetDisplayServerReward(const TArray<b2network::B2RewardPtr>& ServerRewardInfo)
{
	for (int32 i = 0; i < 4; ++i)
	{
		if (RewardIcons[i].IsValid())
		{
			if (ServerRewardInfo.IsValidIndex(i) && ServerRewardInfo[i]->raw_reward->id != 0)
				RewardIcons[i]->UpdateRewardIcon(ServerRewardInfo[i]);
			else
				RewardIcons[i]->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UB2UIQuestDialog::FinalApplyDialogText(bool IsStringFilledUp)
{
	if (RTB_Dialog.IsValid())
		RTB_Dialog->SetText(FText::FromString(ProgressiveDialogText.GetCurrentString()));

	if (IMG_Dialogue_Next.IsValid())
		IMG_Dialogue_Next->SetVisibility(IsStringFilledUp ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);

	//if (CheckIsVisibleBtnNext)
	//{
	//	// 마지막 페이지의 마지막 글자면 Next표시를 지워준다.
	//	if ((DialogArr.Num() - 1 == PageCount))
	//	{
	//		if (BTN_Dialogue_Next.IsValid())
	//			BTN_Dialogue_Next->SetVisibility(ESlateVisibility::Collapsed);
	//	}
	//}
}

void UB2UIQuestDialog::OpenDialog(bool CheckCompleteQuestNotice)
{
	O_Dialog->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	BTN_Dialogue_Next->SetVisibility(ESlateVisibility::Visible);
	O_Reward->SetVisibility(ESlateVisibility::Hidden);
	HB_QuestName->SetVisibility(ESlateVisibility::Hidden);

	if (CheckCompleteQuestNotice)
	{
		SetRewardItem();
		SetQuestCompleteMessage();
	}

	UpdateManualTimeCounter();

	BII_CHECK(DialogArr.Num() > 0);
	ProgressiveDialogText.SetWholeText(DialogArr[PageCount], TotalFlowTime, MaxCharShowInterval, ManualLastRealTime);
	FinalApplyDialogText();
}

void UB2UIQuestDialog::OnClickDialogueNext()
{
	// 글이 끝나지 않았을때는 풀텍스트를 보여주자.
	if (!ProgressiveDialogText.IsStringFilledUp())
	{
		ProgressiveDialogText.ApplyToWholeString();
		FinalApplyDialogText(true);
		return;
	}

	++PageCount;

	if (DialogArr.Num() <= PageCount)
		OnClickSkip();
	else
		OpenDialog(false);
}

void UB2UIQuestDialog::OnClickConfirm()
{
	bool bCloseQuestDialog = true;

	if (DialogType == EDialogType::Quest)
	{
		FQuestCommonInfo* QuestCommonInfo = nullptr;
		if (UB2QuestInfoTable* QuestTable = StaticFindQuestTable())
			QuestCommonInfo = QuestType == MAIN_QUEST ? QuestTable->GetQuestMainInfo(QuestID) : QuestTable->GetQuestDailyInfo(QuestID);

		switch (DivisionStat)
		{
		case EQS_NOT_ACCEPTED:
			data_trader::Retailer::GetInstance().RequestAcceptQuest(QuestSlotIndex);
			break;
		case EQS_COMPLETED:
		{
			if (QuestType != MAIN_QUEST)
			{
				// MainQuest의 경우 DivisionStat을 강제로 EQS_COMPLETED로 돌리며
				// 서버에 보상 요청을 할 필요도 없다.
				bCloseQuestDialog = false;
				StopCurrentDialogSound();
				data_trader::Retailer::GetInstance().RequestReceiveQuestReward(QuestSlotIndex); 
				break;
			}
			else if (QuestSlotIndex == b2network::B2QuestSlotType::MAIN2)
			{
				bCloseQuestDialog = false;
				StopCurrentDialogSound();
				data_trader::Retailer::GetInstance().RequestReceiveQuestReward(QuestSlotIndex); 
				break;
			}
			else if (QuestType == MAIN_QUEST && NextMainQuestPtr != nullptr)
			{
				NextMainQuestSlot = NextMainQuestPtr->GetQuestSlot();
				NextMainQuestPtr = nullptr;
				IsInStageMode = true;

				if (auto* RewardMailPopUp = UB2UIManager::GetInstance()->OpenRewardMailPopUp(RewardArr))
				{
					RewardMailPopUp->SetConfirmCallback(FMsgPopupOnClick::CreateLambda([this]() { this->OnClickMsgReward(); }));
				}
				else
				{
					if (UB2UIMsgPopupReward* RewardPopup = UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupReward>(EUIMsgPopup::ItemReward, FText::GetEmpty(), FText::GetEmpty()))
					{
						RewardPopup->AddRewardItems(RewardArr);
						RewardPopup->SetConfirmCallback(FMsgPopupOnClick::CreateLambda([this]() { this->OnClickMsgReward(); }));
						RewardPopup->ShowRewardPopup();
					}
				}
				return;
			}
		}
		break;
		case EQS_PROGRESS:
			if (QuestCommonInfo != nullptr)
			{
				if (QuestManager::GetInstance().IsAutoClearQuest(QuestSlotIndex, QuestID))
				{
					OpenAutoClearQuest();
					return;
				}
				else if (IsInStageMode)	// Stage Result 상황에서 MainQuest 바로가기
				{
					if (TutorialManager::GetInstance().HasContentOpenTutorial() == false)
					{
						auto& StageDataStore = BladeIIGameImpl::GetStageDataStore();

						FServerStageID ServerStageID = FServerStageID(QuestCommonInfo->Optional1);
						int32 ClientStageID = StageDataStore.GetClientStageId(ServerStageID);

						int32 ActNumber = StageDataStore.GetActIdByClientStageId(ClientStageID);
						if (StageDataStore.GetEventSceneChapter(ActNumber) != INDEX_NONE)
						{
							if (UB2UIDocChapter* DocChapter = UB2UIDocHelper::GetDocChapter())
							{
								DocChapter->SetShowChapterEventScene(true);
								DocChapter->SetDeferredServerStageID(ServerStageID);
							}
						}

						GoToDeferredStageClass<int32>::GetInstance().Signal(ClientStageID);
						bCloseQuestDialog = false;
					}
				}
				else // Lobby 상황에서 바로가기
				{
					FB2UILinkManager::LinkUIScene(QuestCommonInfo->LinkScene, QuestCommonInfo->Optional1, QuestCommonInfo->Optional2);
				}
			}
			break;
		}
	}
	else if (DialogType == EDialogType::Mission)
	{
		if (UB2UIDocSome* SomeDoc = UB2UIDocHelper::GetDocSome())
		{
			int32 MissionType = 0;
			if (MissionPtr Mission = MissionManager::GetInstance().GetMission(MissionID))
			{
				MissionType = static_cast<int32>(Mission->MissionType);

				auto MissionUI = UB2UIManager::GetInstance()->GetUI<UB2UIMission>(UIFName::Mission);		//미션 안에서 탭을 바꿔야 하는경우
				if (MissionUI)
				{
					MissionUI->ChangeTapShortcut(Mission->MissionType);
				}
			}

			SomeDoc->SetCompleteMissionType(MissionType);
		}

		UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::Mission);
	}

	if (bCloseQuestDialog)
	{
		StopCurrentDialogSound();
		UB2UIManager::GetInstance()->CloseUI(UIFName::QuestNotice);
		UB2UIManager::GetInstance()->CloseUI(UIFName::QuestDialog);
	}

	OnQuestDialogDelegate.ExecuteIfBound();
	OnQuestDialogDelegate.Unbind();
}

void UB2UIQuestDialog::CloseWidgetDelegate()
{
	if(BTN_Close.IsValid() && BTN_Close->GetVisibility() == ESlateVisibility::Visible)
	{
		OnClickClose();
	}
	else if (
		(BTN_Shortcut.IsValid() && BTN_Shortcut->GetVisibility() == ESlateVisibility::Visible) ||
		(BTN_Result.IsValid() && BTN_Result->GetVisibility() == ESlateVisibility::Visible)
		)
	{
		OnClickConfirm();
	}
	else if (BTN_Dialogue_Next.IsValid())
	{
		OnClickDialogueNext();
	}
	
}

void UB2UIQuestDialog::DestroySelf(class UB2UIManager* InUIManager)
{
	for (auto Ele : RewardIcons)
	{
		if (Ele.IsValid())
		{
			Ele->DestroySelf(InUIManager);
		}
	}

	if (QuestCompleteNotice.IsValid())
	{
		QuestCompleteNotice->DestroySelf(InUIManager);
	}
}

void UB2UIQuestDialog::OnClickClose()
{
	if (QuestManager::GetInstance().IsAutoClearQuest(QuestSlotIndex, QuestID))
	{
		OpenAutoClearQuest();
		return;
	}

	StopCurrentDialogSound();

	UB2UIManager::GetInstance()->CloseUI(UIFName::QuestDialog);

	OnQuestDialogDelegate.ExecuteIfBound();
	OnQuestDialogDelegate.Unbind();
}

void UB2UIQuestDialog::OnClickSkip()
{
	// 다이얼로그 꺼주고, 보상창 켜주자.
	O_Dialog->SetVisibility(ESlateVisibility::Hidden);
	BTN_Dialogue_Next->SetVisibility(ESlateVisibility::Collapsed);
	O_Reward->SetVisibility(ESlateVisibility::Visible);
	HB_QuestName->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	// 타이틀명 변경
	SetToggleQuestTitleWidget(false);

	// 보상받기/바로가기 버튼 켜주자.
	SetVisibilityRewardButtonType(CurRewardButtonType);
}

void UB2UIQuestDialog::OnClickGiveUp()
{
	UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
		BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("SensitiveNoti_Notification"))),
		BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Quest_abandonConfirm"))),
		0.f,
		true,
		true,
		EUIMsgPopupButtonGroup::YesOrNo,
		FMsgPopupOnClick::CreateLambda([this]() {
		this->StopCurrentDialogSound();
		data_trader::Retailer::GetInstance().RequestGiveUpQuest(b2network::B2QuestSlotType::MAIN2);
	})
	);
}

void UB2UIQuestDialog::OnClickMsgReward()
{
	if (NextMainQuestSlot)
	{
		QuestManager::GetInstance().FlushCachedCompleteQuest();

		QuestPtr QuestPtrInfo = QuestManager::GetInstance().GetQuest(NextMainQuestSlot);
		if (QuestPtrInfo != nullptr)
		{
			OpenDialogBySlotIndex(NextMainQuestSlot);
			NextMainQuestSlot = 0;
		}
		else
		{
			OnClickClose();
		}
	}
}

void UB2UIQuestDialog::SetPlaySound(bool IsStartMode)
{
	StopCurrentDialogSound(); // 중단부터

	UB2QuestInfoTable* QuestInfoTable = StaticFindQuestTable();
	UB2QuestNPCSoundInfo *QuestSoundInfoTable = StaticFindQuestNPCSoundTable();

	FQuestCommonInfo* QuestTable = nullptr;
	if (QuestInfoTable != nullptr)
		QuestTable = (QuestType == MAIN_QUEST) ? QuestInfoTable->GetQuestMainInfo(QuestID) : QuestInfoTable->GetQuestDailyInfo(QuestID);

	USoundCue* DialogSound = nullptr;
	if (QuestTable)
	{
		FName SoundName = IsStartMode ?  QuestTable->StartNpcSoundKey : QuestTable->CompleteNpcSoundKey;
		int32 NpcCode = IsStartMode ? QuestTable->StartNPC : QuestTable->CompleteNPC;
		DialogSound = QuestSoundInfoTable->GetNPCSound(SoundName, NpcCode);
	}

	if (DialogSound)
	{
		CurrentPlayingDialogSound = UGameplayStatics::SpawnSound2D(this, DialogSound);
	}

}

USoundCue* UB2UIQuestDialog::GetNpcSound(bool IsStartMode)
{
	// 퀘스트만 사운드 출력
	if (DialogType == EDialogType::Quest)
	{
		UB2QuestInfoTable* QuestInfoTable = StaticFindQuestTable();
		UB2QuestNPCSoundInfo *QuestSoundInfoTable = StaticFindQuestNPCSoundTable();

		FQuestCommonInfo* QuestTable = nullptr;
		if (QuestInfoTable != nullptr)
			QuestTable = (QuestType == MAIN_QUEST) ? QuestInfoTable->GetQuestMainInfo(QuestID) : QuestInfoTable->GetQuestDailyInfo(QuestID);

		USoundCue* DialogSound = nullptr;
		if (QuestTable)
		{
			FName SoundName = IsStartMode ? QuestTable->StartNpcSoundKey : QuestTable->CompleteNpcSoundKey;
			int32 NpcCode = IsStartMode ? QuestTable->StartNPC : QuestTable->CompleteNPC;
			return QuestSoundInfoTable->GetNPCSound(SoundName, NpcCode);
		}

	}

	return nullptr;
}

void UB2UIQuestDialog::StopCurrentDialogSound()
{
	if (CurrentPlayingDialogSound)
	{
		//if (!CurrentPlayingDialogSound->IsPendingKill())
		{
			CurrentPlayingDialogSound->Stop();
			CurrentPlayingDialogSound->ConditionalBeginDestroy();
		}
	}
	CurrentPlayingDialogSound = NULL;
}

UTexture2D* UB2UIQuestDialog::GetNPCImageTexture(int32 InIndex)
{
	if (NPCImage.IsValidIndex(InIndex))
	{
		TSoftObjectPtr<UTexture2D>& WantedTAsset = NPCImage[InIndex];
		return WantedTAsset.LoadSynchronous();
	}
	return nullptr;
}

#undef DEFAULT_NPCINDEX