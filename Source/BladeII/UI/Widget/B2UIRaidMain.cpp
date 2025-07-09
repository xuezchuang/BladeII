// Fill out your copyright notice in the Description page of Project Settings.
#include "BladeII.h"

#include "B2UIHeader.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"
#include "B2UIDocHero.h"
#include "Retailer.h"
#include "TutorialManager.h"
#include "BladeIIGameImpl.h"

#include "B2ItemInfo.h"
#include "B2RewardInfo.h"
#include "B2UIRewardEventSet.h"
#include "B2ClientDataStore.h"
#include "B2UIMsgPopupRaidPenalty.h"
#include "B2UIPVPEntryHero.h"
#include "B2UIRecommandPower.h"
#include "B2CombatStatEvaluator.h"
#include "B2UIFairyLinkButton.h"
#include "B2UICommonTab.h"
#include "B2UICounterAttackSelectCharacterSlot.h"
#include "B2UIRaidJoin.h"

#include "B2UIRaidMain.h"

void UB2UIRaidMain::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2Button, BTN_GoToHeroMgmtMode);
	GET_SLOT(UButton, BTN_MakeRoom);
	GET_SLOT(UButton, BTN_QuickStart);

	GET_SLOT(UTextBlock, STB_RecommandPower);
	GET_SLOT(UTextBlock, STB_CombatPower);
	GET_SLOT(UTextBlock, STB_MakeRoom);
	GET_SLOT(UTextBlock, STB_QuickStart);
	GET_SLOT(UTextBlock, STB_RewardInfo);
	GET_SLOT(UTextBlock, STB_Reward_Step);
	GET_SLOT(UTextBlock, STB_Reward_Exp);
	GET_SLOT(UTextBlock, STB_GoToHeroMgmtMode);
	GET_SLOT(UTextBlock, STB_EntenHero);
	GET_SLOT(UTextBlock, STB_RaidSelect);
	GET_SLOT(UB2RichTextBlock, STB_JoinExplanation);

	GET_SLOT(UTextBlock, TB_ExceptionItem);
	GET_SLOT(UTextBlock, TB_DailyEnter);
	GET_SLOT(UTextBlock, TB_Reward_Main);
	GET_SLOT(UTextBlock, TB_Reward_Contribution);
	GET_SLOT(UTextBlock, TB_CombatPower);
	GET_SLOT(UTextBlock, TB_RecommandPower);
	GET_SLOT(UTextBlock, TB_BossTitle);
	GET_SLOT(UTextBlock, TB_BossDesc);
	GET_SLOT(UTextBlock, TB_BlockDesc);

	GET_SLOT(UImage, IMG_RaidBossBG);
	GET_SLOT(UImage, IMG_BossImg);

	GET_SLOT(UWidgetSwitcher, SW_ButtonSet);

	GET_SLOT(UB2UIFairyLinkButton, UIP_FairyLinkButton);
	if (UIP_FairyLinkButton.IsValid())
		UIP_FairyLinkButton->Init();

	GET_SLOT(UB2UIRewardIcon, UIP_Reward_Contribution);
	if (UIP_Reward_Contribution.IsValid())
		UIP_Reward_Contribution->Init();

	GET_SLOT(UB2UIRewardIcon, UIP_Reward_Main);
	if (UIP_Reward_Main.IsValid())
		UIP_Reward_Main->Init();

	GET_SLOT(UB2UIRewardIcon, UIP_Reward_Step);
	if (UIP_Reward_Step.IsValid())
		UIP_Reward_Step->Init();

	GET_SLOT(UB2UIRewardIcon, UIP_Reward_Exp);
	if (UIP_Reward_Exp.IsValid())
		UIP_Reward_Exp->Init();

	GET_SLOT(UB2UIRecommandPower, UIP_RecommendPower);
	if (UIP_RecommendPower.IsValid())
	{
		UIP_RecommendPower->Init();
	}

	HeroEntryList.SetNumUninitialized(4);
	GET_SLOT_BYNAME(UB2UIPVPEntryHero, HeroEntryList[0], HeroEntry_0);
	GET_SLOT_BYNAME(UB2UIPVPEntryHero, HeroEntryList[1], HeroEntry_1);
	GET_SLOT_BYNAME(UB2UIPVPEntryHero, HeroEntryList[2], HeroEntry_2);
	GET_SLOT_BYNAME(UB2UIPVPEntryHero, HeroEntryList[3], HeroEntry_3);

	for (auto ThisSlot : HeroEntryList)
	{
		if (ThisSlot.IsValid())
		{
			ThisSlot->Init();
			ThisSlot->FOnClickBtnSelect.BindUObject(this, &UB2UIRaidMain::OnClickEntrySlot);
		}
	}

	GET_SLOT(UB2UIDungeonDifficulty, UIP_DungeonLevelofDifficulty);
	if (UIP_DungeonLevelofDifficulty.IsValid())
	{
		CAPTURE_UOBJECT(UB2UIRaidMain);
		UIP_DungeonLevelofDifficulty->Init();
		UIP_DungeonLevelofDifficulty->SetDifficultyDelegate(
			USE_CAPTURE_OBJECT(int32 InDifficulty)
			Capture->ChangeDifficulty(InDifficulty);
		END_CAPTURE_OBJECT());
	}

	TopTabs.Empty();
	TopTabs.SetNum(static_cast<int32>(ERaidBossIndex::BOSS_SARGON));
	GET_SLOT_BYNAME(UB2UICommonTab, TopTabs[0], TAB_Harang);
	GET_SLOT_BYNAME(UB2UICommonTab, TopTabs[1], TAB_Sargon);

	for (auto TabItem : TopTabs)
	{
		TabItem->Init();
	}

}

void UB2UIRaidMain::UpdateStaticText()
{
	SetLobbyUIHeaderTitleByGeneralTextTableKey(TEXT("ChallengeMod_Raid"));

	if (STB_RecommandPower.IsValid())
		STB_RecommandPower->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_RecomCombatPower")));
	if (STB_CombatPower.IsValid())
		STB_CombatPower->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_CurrentCombatpower")));
	if (STB_MakeRoom.IsValid())
		STB_MakeRoom->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raid_MakeRoom")));
	if (STB_QuickStart.IsValid())
		STB_QuickStart->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_QuickMatch")));
	if (STB_RewardInfo.IsValid())
		STB_RewardInfo->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("BattleStageInfo_RewardInfo")));
	if (STB_GoToHeroMgmtMode.IsValid())
		STB_GoToHeroMgmtMode->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyHeroMgmt_EquipMgmtButton")));
	if (STB_EntenHero.IsValid())
		STB_EntenHero->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PVP1on1_EnterHero")));
	if (STB_JoinExplanation.IsValid())
		STB_JoinExplanation->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raid_JoinCounterExplanation")));
	if (STB_RaidSelect.IsValid())
		STB_RaidSelect->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("RaidMain_DifficultyChoice")));

	if (STB_Reward_Step.IsValid())
		STB_Reward_Step->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raid_DifficultyReward")));
	if (STB_Reward_Exp.IsValid())
		STB_Reward_Exp->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raid_EXPReward")));
	
	if (STB_Reward_Exp.IsValid())
		STB_Reward_Exp->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raid_EXPReward")));

	if (TB_ExceptionItem.IsValid())
		TB_ExceptionItem->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raid_ExceptionItem")));
}

void UB2UIRaidMain::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_GoToHeroMgmtMode, &UB2UIRaidMain::OnClickBTN_GoToHeroMgmtMode);
	BIND_CLICK_FUNC_TO_BTN(BTN_MakeRoom, &UB2UIRaidMain::OnClickBTN_MakeRoom);
	BIND_CLICK_FUNC_TO_BTN(BTN_QuickStart, &UB2UIRaidMain::OnClickBTN_QuickStart);

	for (int32 i = 0; i < TopTabs.Num(); i++)
	{
		int32 ButtonIndex = i + 1;
		TopTabs[i]->SetTabButtonIndex(ButtonIndex);
		TopTabs[i]->TabButtonDelegateBindLambda(
			[this, ButtonIndex]() { OnClickTAB_Boss(ButtonIndex);
		});
	}
}

void UB2UIRaidMain::Init()
{
	Super::Init();

	CurrentRaidStep = 1;
	CurrentBossType = 1;
	if (auto* DocBattle = UB2UIDocHelper::GetDocBattle())
		CurrentPCClass = IntToPCClass(DocBattle->GetCurPCClass());

	InitEntrySlot();

	CheckRaidAble();
	SelectRaidDefaultBoss();

	TutorialManager& TutorialMgr = TutorialManager::GetInstance();
	if (TutorialMgr.IsCurrentTutorial(TutorialID_Raid))
	{
		if (SW_ButtonSet.IsValid())
			SW_ButtonSet->SetActiveWidgetIndex(true);
		SetButtonEnable(true);
		return;
	}

	SetButtonEnable(false);
}

void UB2UIRaidMain::DestroySelf(UB2UIManager* InUIManager)
{
	for (auto ThisSlot : HeroEntryList)
	{
		if (ThisSlot.IsValid())
			ThisSlot->DestroySelf(InUIManager);
	}

	if (UIP_RecommendPower.IsValid())
	{
		UIP_RecommendPower->DestroySelf(InUIManager);
	}

	Super::DestroySelf(InUIManager);
}

void UB2UIRaidMain::OnClickBTN_GoToHeroMgmtMode()
{
	if (auto* DocBattle = UB2UIDocHelper::GetDocBattle())
		LobbyHeroMgmtSelectClass<EPCClass>::GetInstance().Signal(IntToPCClass(DocBattle->GetCurPCClass()));

	LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_Inventory);
}

void UB2UIRaidMain::OnClickBTN_MakeRoom()
{
	StartGame(ERaidJoinType::MakeRoom);
}

void UB2UIRaidMain::OnClickBTN_QuickStart()
{
	StartGame(ERaidJoinType::QuickStart);
}

void UB2UIRaidMain::OnClickTAB_Boss(int32 BossNum)
{
	CurrentBossType = BossNum;
	
	OnClickEntrySlot(CurrentPCClass);
	
	SetBossInfo();
	SetMaxDifficulty();

	for (int32 i = 0; i < TopTabs.Num(); i++)
	{
		bool bIsSelected = (i + 1 == BossNum);
		TopTabs[i]->SetTabSelected(bIsSelected);
	}
}

void UB2UIRaidMain::OnClickEntrySlot(EPCClass InEntry)
{
	CurrentPCClass = InEntry;

	for (int32 i = 0; i < PCClassToInt(EPCClass::EPC_End); ++i)
	{
		HeroEntryList[i]->SetVisibilityWidgets(IntToPCClass(i) == InEntry ? EEntryType::EntryType_MainChar : EEntryType::EntryType_None);
	}

	UpdateEntryInfo(InEntry);
	SelectRaidDefaultStep();
}

void UB2UIRaidMain::SelectRaidDefaultBoss()
{
	auto* DocRaid = UB2UIDocHelper::GetDocRaid();
	if (!DocRaid)
		return;

	int32 BossDefaultIndex = 0;

	for (int32 i = 0; i < TopTabs.Num(); i++)
	{
		TArray<int32> DayOpens;
		bool bIsDayOpen = BladeIIGameImpl::GetClientDataStore().GetIsRaidBossOpen(DocRaid->GetPeriodInfos(), i+1, DayOpens);
		if (bIsDayOpen)
		{
			BossDefaultIndex = i;
			break;
		}
	}

	if(TopTabs.IsValidIndex(BossDefaultIndex))
		TopTabs[BossDefaultIndex]->ForcedClickTab();
}

void UB2UIRaidMain::SelectRaidDefaultStep()
{
	if (UIP_DungeonLevelofDifficulty.IsValid())
	{
		int32 CurrentCombatPower = CombatStatEval::GetPCCombatPower(CurrentPCClass);
		int32 RecommendStep = 1;
		const TArray<FRaidMasterData>* RaidData = BladeIIGameImpl::GetClientDataStore().GetRaidMasterDataList(CurrentBossType);
		if (RaidData)
		{
			for (FRaidMasterData Ele : *RaidData)
			{
				if (Ele.RecommendCombatPoint <= CurrentCombatPower)
				{
					RecommendStep = Ele.RaidStep;
				}
			}
		}
		if (auto* DocRaid = UB2UIDocHelper::GetDocRaid())
		{
			if (CurrentBossType == DocRaid->GetLastRaidType() && DocRaid->GetLastRaidStep() <= RecommendStep)
			{
				RecommendStep = DocRaid->GetLastRaidStep();
			}
		}

		UIP_DungeonLevelofDifficulty->SetCurrentDifficulty(RecommendStep);
	}
}

void UB2UIRaidMain::InitEntrySlot()
{
	FLocalCharacterData& CharacterDataStore = BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData();

	for (int32 i = 0; i < GetMaxPCClassNum(); ++i)
	{
		if (HeroEntryList.IsValidIndex(i))
		{
			const int32 CharLevel = CharacterDataStore.GetCharacterLevel(IntToPCClass(i));
			HeroEntryList[i]->SetEntryInfo(IntToPCClass(i), EEntryType::EntryType_None, CharLevel);
		}
	}
}

void UB2UIRaidMain::SetMaxDifficulty()
{
	auto* DocRaid = UB2UIDocHelper::GetDocRaid();
	int32 MaxDifficultyNumber = DocRaid ? BladeIIGameImpl::GetClientDataStore().GetRaidMaxStep(CurrentBossType) : 1;
	UIP_DungeonLevelofDifficulty->SetMaxDifficulty(MaxDifficultyNumber);
}

void UB2UIRaidMain::UpdateEntryInfo(EPCClass InEntry)
{
	int32 CurrentCombatPower = CombatStatEval::GetPCCombatPower(InEntry);

	if (TB_CombatPower.IsValid())
		TB_CombatPower->SetText(FText::AsNumber(CurrentCombatPower));

	if (CurrentCombatPower >= RecomandCombatPower)
		TB_CombatPower->SetColorAndOpacity(UB2UIManager::GetInstance()->TextColor_Increase);
	else
		TB_CombatPower->SetColorAndOpacity(UB2UIManager::GetInstance()->TextColor_Decrease);
}

void UB2UIRaidMain::ChangeDifficulty(int32 InDifficulty)
{
	CurrentRaidStep = InDifficulty;

	SetRaidRewardInfo(CurrentBossType, CurrentRaidStep);
	
	const TArray<FRaidMasterData>* RaidData = BladeIIGameImpl::GetClientDataStore().GetRaidMasterDataList(CurrentBossType);
	if (RaidData)
	{
		for (FRaidMasterData Ele : *RaidData)
		{
			if (Ele.RaidStep == InDifficulty)
			{
				RefreshRecommendStats(Ele.RecommendCombatPoint, Ele.RecommendAttack, Ele.RecommendDefense);
				break;
			}
		}
	}

	CheckRaidAble();
}

void UB2UIRaidMain::RefreshRecommendStats(int32 RecommandPower, int32 RecommendAttack, int32 RecommendDefense)
{
	RecomandCombatPower = RecommandPower;

	if (TB_CombatPower.IsValid() && TB_RecommandPower.IsValid())
	{
		int32 CurrentCombatPower = CombatStatEval::GetPCCombatPower(CurrentPCClass);

		if (CurrentCombatPower >= RecommandPower)
			TB_CombatPower->SetColorAndOpacity(UB2UIManager::GetInstance()->TextColor_Increase);
		else
			TB_CombatPower->SetColorAndOpacity(UB2UIManager::GetInstance()->TextColor_Decrease);

		TB_RecommandPower->SetText(FText::AsNumber(RecommandPower));
	}

	if (UIP_RecommendPower.IsValid())
	{
		int32 CurrentAttack = CombatStatEval::GetPCAttack(CurrentPCClass);
		int32 CurrentDefense = CombatStatEval::GetPCDefense(CurrentPCClass);

		UIP_RecommendPower->SetInfo(CurrentAttack, CurrentDefense, RecommendAttack, RecommendDefense);
	}
}

void UB2UIRaidMain::SetRaidRewardInfo(int32 RaidType, int32 RaidStep)
{
	const TArray<FRaidMasterData>* RaidMasterData = BladeIIGameImpl::GetClientDataStore().GetRaidMasterDataList(RaidType);

	if (RaidMasterData == nullptr)
		return;

	int32 DifficultyIdx = FMath::Max(RaidStep - 1, 0);
	if (RaidMasterData->IsValidIndex(DifficultyIdx) == false)
		return;

	FRaidMasterData RewardInfo = (*RaidMasterData)[DifficultyIdx];

	if (RewardInfo.MainRewardID == 0)
		SetItemRewardAssessory(RewardInfo.RewardEquipItemMinGrade, RewardInfo.RewardEquipItemMaxGrade);
	else
		SetItemReward(RewardInfo.MainRewardID);

	if (UIP_Reward_Exp.IsValid())
	{
		int32 ExpNum = BladeIIGameImpl::GetClientDataStore().GetExpectedExpOfRaidGame(RaidType, RaidStep);
		UIP_Reward_Exp->UpdateRewardIconByItemIndex(FItemRefIDHelper::GetGoodsID_EXP(), FText::AsNumber(ExpNum));
	}

	if (UIP_Reward_Step.IsValid())
	{
		FString CountString = (RewardInfo.StepRewardMinCount == RewardInfo.StepRewardMaxCount) ?
			FString::FromInt(RewardInfo.StepRewardMinCount) : (FString::FromInt(RewardInfo.StepRewardMinCount) + "~" + FString::FromInt(RewardInfo.StepRewardMaxCount));

		UIP_Reward_Step->UpdateRewardIcon(RewardInfo.StepRewardID, FText::FromString(CountString));
	}
}

void UB2UIRaidMain::ForcedStartGame(int32 RaidType, int32 RaidStep, ERaidJoinType RaidJoinType, EPCClass InPcClass, bool IsMatchMaking)
{
	if (auto* DocRaid = UB2UIDocHelper::GetDocRaid())
	{
		DocRaid->ClearRaidJoinData();
		DocRaid->SetAlreadyExitUser(false);
		DocRaid->SetOpenedRaidJoinType(RaidJoinType);
		DocRaid->SetLastRaidType(RaidType);
		DocRaid->SetLastRaidStep(CheckAbleRaidStep(RaidStep));
	}

	if (auto* DocBattle = UB2UIDocHelper::GetDocBattle())
	{
		int32 ChangeMainClass = DocBattle->GetCurPCClass();
		int32 ChangeSubClass = DocBattle->GetTagPCClass();

		if (ChangeSubClass == PCClassToInt(InPcClass))
		{
			DocBattle->SetTagPCClass(PCClassToInt(InPcClass));
		}

		DocBattle->SetCurPCClass(PCClassToInt(InPcClass));

		LobbySetMainPCClassClass<EPCClass>::GetInstance().Signal(InPcClass);
	}

	LobbyStartMatchClass<bool>::GetInstance().Signal(IsMatchMaking);

	// 버튼 비활성화 및 타이머 세팅..
	SetButtonEnable(false);

	GetWorld()->GetTimerManager().ClearTimer(ButtonFreeHandle);
	GetWorld()->GetTimerManager().SetTimer(ButtonFreeHandle, this, &UB2UIRaidMain::ButtonFree, 5.0f, false);

}

void UB2UIRaidMain::StartGame(ERaidJoinType JoinType)
{
	// 일반적인 시작.

	if (!CheckRaidAbleTime())
	{
		ShowRaidNotOpenMsgPopup();
		return;
	}
	if (CheckRaidPenalty())
		return;

	if (auto* DocRaid = UB2UIDocHelper::GetDocRaid())
	{
		DocRaid->ClearRaidJoinData();
		DocRaid->SetAlreadyExitUser(false);
		DocRaid->SetOpenedRaidJoinType(JoinType);
		DocRaid->SetLastRaidType(CurrentBossType);
		DocRaid->SetLastRaidStep(CurrentRaidStep);
		DocRaid->SetRaidJoinType(JoinType);
	}

	if (auto* DocBattle = UB2UIDocHelper::GetDocBattle())
	{
		int32 ChangeMainClass = DocBattle->GetCurPCClass();
		int32 ChangeSubClass = DocBattle->GetTagPCClass();

		if (ChangeSubClass == PCClassToInt(CurrentPCClass))
		{
			DocBattle->SetTagPCClass(ChangeMainClass);
		}

		DocBattle->SetCurPCClass(PCClassToInt(CurrentPCClass));

		LobbySetMainPCClassClass<EPCClass>::GetInstance().Signal(CurrentPCClass);
	}

	LobbyStartMatchClass<bool>::GetInstance().Signal(true);

	// 버튼 비활성화 및 타이머 세팅..
	SetButtonEnable(false);

	GetWorld()->GetTimerManager().ClearTimer(ButtonFreeHandle);
	GetWorld()->GetTimerManager().SetTimer(ButtonFreeHandle, this, &UB2UIRaidMain::ButtonFree, 5.0f, false);

}

void UB2UIRaidMain::ButtonFree()
{
	SetButtonEnable(true);
}

void UB2UIRaidMain::SetBossInfo()
{
	UTexture2D* BossBGImg = nullptr;
	UTexture2D* BossDescImg = nullptr;

	for (FBossImageInfo Item : BossImageArray)
	{
		if (static_cast<int32>(Item.BossIndex) == CurrentBossType)
		{
			BossBGImg = Item.BossBGImage;
			BossDescImg = Item.BossDescImage;
			break;
		}
	}

	if (IMG_RaidBossBG.IsValid())
		IMG_RaidBossBG->SetBrushFromTexture(BossBGImg);

	if (IMG_BossImg.IsValid())
		IMG_BossImg->SetBrushFromTexture(BossDescImg);

	if (TB_BossDesc.IsValid())
	{
		FString BossText = TEXT("RaidMain_BossDesc") + FString::FromInt(CurrentBossType);
		TB_BossDesc->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, BossText));
	}
	if (TB_BossTitle.IsValid())
	{
		FString BossText = TEXT("RaidMain_BossName") + FString::FromInt(CurrentBossType);
		TB_BossTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, BossText));
	}
}

int32 UB2UIRaidMain::CheckAbleRaidStep(int32 inStep)
{
	auto* UIDocRaid = UB2UIDocHelper::GetDocRaid();
	const ERaidGotoWhere MatchType = UIDocRaid->GetReturnToPageType();
	const bool IsRematch = (MatchType == ERaidGotoWhere::Rematch);

	if (!IsRematch) // 파티원 다시하기와 초대하기는 지정해준 스텝 그대로.
		return inStep;

	int32 RaidStep = inStep;
	int32 RecommandStep = 1;
	int32 MyCombat =  CombatStatEval::GetPCCombatPower(CurrentPCClass);
	int32 MaxDifficultyNumber = BladeIIGameImpl::GetClientDataStore().GetRaidMaxStep(CurrentBossType);

	const TArray<FRaidMasterData>* RaidData = BladeIIGameImpl::GetClientDataStore().GetRaidMasterDataList(CurrentBossType);
	if (RaidData)
	{
		for (FRaidMasterData Ele : *RaidData)
		{
			if (MyCombat <= Ele.RecommendCombatPoint)
			{
				RecommandStep = Ele.RaidStep;
			}
			else if (Ele.RaidStep == MaxDifficultyNumber && MyCombat >= Ele.RecommendCombatPoint) // 내 전투력이 MAX단계보다 높을경우 MAX단계로 고정.
			{
				RecommandStep = MaxDifficultyNumber;
			}
		}
	}

	if (RecommandStep <= inStep)
		RaidStep = RecommandStep;

	return RaidStep;
}

void UB2UIRaidMain::CheckRaidAble()
{
	ERaidBlockCode blockCode = ERaidBlockCode::ABLEJOIN;
	auto* UIDocRaid = UB2UIDocHelper::GetDocRaid();
	TArray<int32> DayOpens;
	TArray<b2network::B2MatchOpenPeriodInfoPtr> OpenTimes;

	bool bIsDayOpen = BladeIIGameImpl::GetClientDataStore().GetIsRaidBossOpen(UIDocRaid->GetPeriodInfos(), CurrentBossType, DayOpens);
	bool bAbleCombat = CombatStatEval::GetPCCombatPower(CurrentPCClass) >= RecomandCombatPower;

	if (SW_ButtonSet.IsValid() && TB_BlockDesc.IsValid())
	{
		if (!bIsDayOpen)/// 날짜 체크
		{
			SW_ButtonSet->SetActiveWidgetIndex(false);
			TB_BlockDesc->SetText(GetBlockMessageText(ERaidBlockCode::NOT_ABLE_DAY, GetLockedDayText(DayOpens)));
		}
		else if(!bAbleCombat)//  전투력 체크
		{
			SW_ButtonSet->SetActiveWidgetIndex(false);
			TB_BlockDesc->SetText(GetBlockMessageText(ERaidBlockCode::NOT_COMBATPOWER, FText::AsNumber(RecomandCombatPower)));
		}
		else // 시간체크
		{
			CheckRaidAbleTime();
			SW_ButtonSet->SetActiveWidgetIndex(true);
			return;
		}
	}
}

bool UB2UIRaidMain::CheckRaidPenalty()
{
	auto* DocRaid = UB2UIDocHelper::GetDocRaid();
	if (DocRaid)
	{
		bool bIsPenalty = DocRaid->IsPanelty();
		if (bIsPenalty) // 패널티 팝업 추가.
		{
			UB2UIMsgPopupRaidPenalty* pPopupUI = UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupRaidPenalty>(EUIMsgPopup::RaidPenalty,
				BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Notification")),
				BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raid_PenaltyNotice")),
				0.0f,
				true,
				true,
				EUIMsgPopupButtonGroup::Confirm);

			if (pPopupUI)
				pPopupUI->StartTimer(DocRaid->GetPaneltyTime().GetTotalSeconds());
		}

		return bIsPenalty;
	}

	return false;
}

bool UB2UIRaidMain::CheckRaidAbleTime()
{
	auto* DocRaid = UB2UIDocHelper::GetDocRaid();
	if (DocRaid)
	{
		if (DocRaid->GetRaidOpenByType(CurrentBossType))
		{
			SetButtonEnable(true);
			return true;
		}
	}

	SetButtonEnable(false);
	return false;
}

void UB2UIRaidMain::ShowRaidNotOpenMsgPopup()
{
	UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
		BladeIIGetLOCText(B2LOC_CAT_ERROR_CODE, TEXT("7410")),
		0.f,
		true,
		true,
		EUIMsgPopupButtonGroup::Confirm
	);
}

void UB2UIRaidMain::SetButtonEnable(bool bIsEnable)
{
	if (BTN_MakeRoom.IsValid())
		BTN_MakeRoom->SetIsEnabled(bIsEnable);
	if (BTN_QuickStart.IsValid())
		BTN_QuickStart->SetIsEnabled(bIsEnable);
}

void UB2UIRaidMain::UpdateRaidRewardInfo(int32 RaidType, int32 RaidStep)
{
	const TArray<FRaidMasterData>* RaidMasterData = BladeIIGameImpl::GetClientDataStore().GetRaidMasterDataList(RaidType);

	if (RaidMasterData == nullptr)
		return;

	int32 DifficultyIdx = FMath::Max(RaidStep - 1, 0);
	if (RaidMasterData->IsValidIndex(DifficultyIdx) == false)
		return;

	FRaidMasterData RewardInfo = (*RaidMasterData)[DifficultyIdx];

	if (RewardInfo.MainRewardID == 0)
		SetItemRewardAssessory(RewardInfo.RewardEquipItemMinGrade, RewardInfo.RewardEquipItemMaxGrade);
	else
		SetItemReward(RewardInfo.MainRewardID);

	if (UIP_Reward_Exp.IsValid())
	{
		int32 ExpNum = BladeIIGameImpl::GetClientDataStore().GetExpectedExpOfRaidGame(RaidType, RaidStep);
		UIP_Reward_Exp->UpdateRewardIconByItemIndex(FItemRefIDHelper::GetGoodsID_EXP(), FText::AsNumber(ExpNum));
	}

	if (UIP_Reward_Step.IsValid())
	{
		FString CountString = (RewardInfo.StepRewardMinCount == RewardInfo.StepRewardMaxCount) ?
			FString::FromInt(RewardInfo.StepRewardMinCount) : (FString::FromInt(RewardInfo.StepRewardMinCount) + "~" + FString::FromInt(RewardInfo.StepRewardMaxCount));

		UIP_Reward_Step->UpdateRewardIcon(RewardInfo.StepRewardID, FText::FromString(CountString));
	}
}

void UB2UIRaidMain::SetItemRewardAssessory(int32 ItemMinGrade, int32 ItemMaxGrade)
{
	auto* ItemMiscInfo = StaticFindItemMiscInfo();
	if (!ItemMiscInfo)
		return;

	FText GradeText = FText::FromString(GetEquipItemGradeText(ItemMinGrade, ItemMaxGrade));
	FText CategoryText = FText::FromString(GetEquipItemTypeText(EEquipCategoryType::ACCESSORY));
	FText EleText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raid_RewardDesc3"));

	if (TB_Reward_Main.IsValid())
		TB_Reward_Main->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raid_RewardDesc1")), GradeText, CategoryText));

	if (UIP_Reward_Main.IsValid())
	{
		UIP_Reward_Main->UpdateRewardIconByItemIndex(FItemRefIDHelper::ITEM_REF_ID_RANDOM_ACCESSORY, FText::GetEmpty());
		UIP_Reward_Main->SetGrade(ItemMinGrade);
	}

	if (UIP_Reward_Contribution.IsValid())
		UIP_Reward_Contribution->UpdateRewardIconByItemIndex(FItemRefIDHelper::ITEM_REF_ID_RANDOM_BONUS_ACCESSORY, FText::GetEmpty());

	if (TB_Reward_Contribution.IsValid())
		TB_Reward_Contribution->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raid_ContributionReward")));
}

void UB2UIRaidMain::SetItemReward(int32 RewardId)
{
	FText RewardName = FText::GetEmpty();
	FSingleRewardInfoData* RewardInfo = StaticFindRewardInfo() ? StaticFindRewardInfo()->GetInfoData(RewardId) : nullptr;
	if (RewardInfo)
		RewardName = BladeIIGetLOCText(RewardInfo->TextCategory, RewardInfo->RewardNameForTextTable);

	if (UIP_Reward_Main.IsValid())
		UIP_Reward_Main->UpdateRewardIcon(RewardId, FText::GetEmpty());

	if (TB_Reward_Main.IsValid())
		TB_Reward_Main->SetText(RewardName);

	if (UIP_Reward_Contribution.IsValid())
		UIP_Reward_Contribution->UpdateRewardIcon(RewardId, FText::GetEmpty());

	if (TB_Reward_Contribution.IsValid())
		TB_Reward_Contribution->SetText(RewardName);
}

FText UB2UIRaidMain::GetLockedDayText(TArray<int32> ModeOpenDays)
{
	FString TimeText = FString();
	TArray<FText> ta_OpenDays;
	ta_OpenDays.Empty();

	if (ModeOpenDays.Num() > 0)
	{
		for (auto EachItem : ModeOpenDays)
		{
			ta_OpenDays.Add(BladeIIGetLOCText(B2LOC_CAT_GENERAL, SvrToCliDayString(EachItem)));
		}

		for (int32 i = 0; i < ta_OpenDays.Num(); i++)
		{
			TimeText += ta_OpenDays[i].ToString();
			if (i != (ta_OpenDays.Num() - 1))
				TimeText += FString(TEXT(", "));
		}
	}
	else
	{
		TimeText += BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raid_EnterDungeon")).ToString();
	}

	return FText::FromString(TimeText);
}

FText UB2UIRaidMain::GetLockedTimeText(TArray<b2network::B2MatchOpenPeriodInfoPtr> OpenTimes)
{
	TArray<FText> ta_OpenTimes;

	if (OpenTimes.Num() > 0)
	{
		for (auto EachItem : OpenTimes)
		{
			FNumberFormattingOptions Options;
			Options.MinimumIntegralDigits = 2;
			ta_OpenTimes.Add(FText::Format(FText::FromString("{0} : {1} ~ {2} : {3}"),
				FText::AsNumber(EachItem->begin_hour, &Options), FText::AsNumber(EachItem->begin_min, &Options),
				FText::AsNumber(EachItem->end_hour, &Options), FText::AsNumber(EachItem->end_min, &Options)));
		}
	}
	else
	{
		return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raid_EnterDungeon"));
	}

	FString TimeText = "";

	for (int32 i = 0; i < ta_OpenTimes.Num(); i++)
	{
		TimeText += ta_OpenTimes[i].ToString();
		if (i != (ta_OpenTimes.Num() - 1))
			TimeText += FString("\n");
	}

	return FText::FromString(TimeText);
}

FText UB2UIRaidMain::GetBlockMessageText(ERaidBlockCode eCode, FText AddMsg)
{
	FString BlockText = TEXT("RaidMain_BlockMsg") + FString::FromInt(static_cast<int32>(eCode));
	return FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, BlockText), AddMsg);
}

FString UB2UIRaidMain::GetEquipItemGradeText(int32 ItemMinGrade, int32 ItemMaxGrade)
{
	if (ItemMaxGrade == ItemMinGrade)
	{
		return FString::FromInt(ItemMinGrade);
	}

	return FString::Printf(TEXT("%d~%d"), ItemMinGrade, ItemMaxGrade);
}

FString UB2UIRaidMain::GetEquipItemText(EEquipCategoryType EquipCategory, int32 ItemMinGrade, int32 ItemMaxGrade)
{
	auto* ItemMiscInfo = StaticFindItemMiscInfo();
	if (!ItemMiscInfo)
		return FString();

	return FString::Printf(TEXT("%d~%d %s"), ItemMinGrade, ItemMaxGrade, *(ItemMiscInfo->GetEquipCategoryName(EquipCategory).ToString()));
}

FString UB2UIRaidMain::GetEquipItemTypeText(EEquipCategoryType EquipCategory)
{
	auto* ItemMiscInfo = StaticFindItemMiscInfo();
	if (!ItemMiscInfo)
		return FString();

	return ItemMiscInfo->GetEquipCategoryName(EquipCategory).ToString();
}