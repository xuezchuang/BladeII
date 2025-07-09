#include "BladeII.h"
#include "B2UIBattleStageInfo.h"
#include "B2UISlotItem.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"
#include "B2UIDocUser.h"
#include "B2UIDocBattle.h"
#include "B2UIDocUICondition.h"
#include "B2StageInfo.h"
#include "B2ItemInfo.h"
#include "Event.h"
#include "B2ClientDataStore.h"
#include "Retailer.h"
#include "B2UISlotStageBuffItem.h"
#include "B2UIMsgPopupReward.h"
#include "B2UIHeader.h"
#include "B2UIStartButton.h"
#include "B2StageManager.h"
#include "B2ChapterInfo.h"
#include "B2UIShortagePopupManager.h"
#include "B2UIBattleStageInfoToolTip.h"
#include "Global.h"
#include "BladeIIGameImpl.h"

UB2UIBattleStageInfo::UB2UIBattleStageInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	BattleRewardItems.Empty();
	CachedCurrentStageInfoData = nullptr;
	ClientStageId = INDEX_NONE;
	ServerStageId = FServerStageID();
	StageDifficulty = EStageDifficulty::ESD_Normal;

	StageGameStarted = false;
}

void UB2UIBattleStageInfo::CacheAssets()
{
	GET_SLOT(UB2Button, BTN_BattleReady);
	GET_SLOT(UB2ScrollBox, SB_RewardItems);
	
	GET_SLOT(UB2RichTextBlock, TB_StageStory);
	GET_SLOT(UB2Image, IMG_MainTexture);
	GET_SLOT(UB2Image, IMG_NPC);
	GET_SLOT(UB2RichTextBlock, TB_TitleName);
	GET_SLOT(UWidgetSwitcher, WS_ChangeDiffImage);
	GET_SLOT(UB2UIBattleStageInfoToolTip, UIP_BattleStageInfoToolTip);
	if (UIP_BattleStageInfoToolTip.IsValid())
	{
		UIP_BattleStageInfoToolTip->Init();
	}

	WS_ClearResultStar.AddDefaulted(static_cast<int32>(EStageClearCondition::MaxCondition));
	GET_SLOT_BYNAME(UWidgetSwitcher, WS_ClearResultStar[0], WS_ClearResultStar1);
	GET_SLOT_BYNAME(UWidgetSwitcher, WS_ClearResultStar[1], WS_ClearResultStar2);
	GET_SLOT_BYNAME(UWidgetSwitcher, WS_ClearResultStar[2], WS_ClearResultStar3);

	TB_ClearResult.AddDefaulted(static_cast<int32>(EStageClearCondition::MaxCondition));
	GET_SLOT_BYNAME(UB2RichTextBlock, TB_ClearResult[0], TB_ClearResult1);
	GET_SLOT_BYNAME(UB2RichTextBlock, TB_ClearResult[1], TB_ClearResult2);
	GET_SLOT_BYNAME(UB2RichTextBlock, TB_ClearResult[2], TB_ClearResult3);
}

void UB2UIBattleStageInfo::UpdateStaticText()
{
	// �̰� StaticText ���� �������Ʈ�� ���� ����.

}

void UB2UIBattleStageInfo::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_BattleReady, &UB2UIBattleStageInfo::OnClickBattleReady)
}

void UB2UIBattleStageInfo::Init()
{
	Super::Init();
	SubscribeEvent();
}

void UB2UIBattleStageInfo::DestroySelf(class UB2UIManager* InUIManager)
{
	UnsubscribeEvent();

	StageGameStartedResetCB();

	if (UIP_BattleStageInfoToolTip.IsValid())
	{
		UIP_BattleStageInfoToolTip->DestroySelf(InUIManager);
	}

	Super::DestroySelf(InUIManager);
}

void UB2UIBattleStageInfo::BeginDestroy()
{
	UnsubscribeEvent();
	Super::BeginDestroy();
}

void UB2UIBattleStageInfo::SubscribeEvent()
{
	CAPTURE_UOBJECT(UB2UIBattleStageInfo);

	CancelStageGameStartTicket = CancelStartGameStageClass<>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT()
			Capture->CancelStageGameStart();
		END_CAPTURE_OBJECT()
		);
}

void UB2UIBattleStageInfo::UnsubscribeEvent()
{
	CancelStartGameStageClass<>::GetInstance().Unsubscribe(CancelStageGameStartTicket);
}

void UB2UIBattleStageInfo::SetHeaderTitleAsChapterNum()
{
	UB2UIDocBattleStage* DocBattleStage = UB2UIDocHelper::GetDocBattleStage();
	if (CachedCurrentStageInfoData && DocBattleStage) // CachedCurrentStageInfoData ���� ���� ���Ŀ���
	{
		FText StageDiffText = FText::FromString(FString::Printf(TEXT("Chapter_DiffColor_%d"), (int32)StageDifficulty));
		FText StageText = FText::Format(BladeIIGetLOCText(B2LOC_CAT_WORLDNAME, StageDiffText.ToString()), CachedCurrentStageInfoData->GetLocalizedStageName());
		
		SetLobbyUIHeaderTitle(StageText);
	}
}

void UB2UIBattleStageInfo::SetStageDiffulty()
{
	if (TB_TitleName.IsValid())
	{
		UB2ChapterInfo* ChapterInfoTable = StaticFindChapterInfo();

		FText StageDiffText = FText::FromString(FString::Printf(TEXT("Chapter_DiffColor_%d"), (int32)StageDifficulty));
		FText StageText = FText::Format(BladeIIGetLOCText(B2LOC_CAT_WORLDNAME, StageDiffText.ToString()), ChapterInfoTable->GetLocalizedDifficultyName(StageDifficulty));
		TB_TitleName->SetText(StageText);
	}

	WS_ChangeDiffImage->SetActiveWidgetIndex((int32)StageDifficulty - 1);
}

void UB2UIBattleStageInfo::BindDocAuto()
{
	UnbindDoc();

	auto DocBattleStage = UB2UIDocHelper::GetDocBattleStage();
	SetDoc(DocBattleStage);
	if (DocBattleStage)
	{
		ClientStageId = DocBattleStage->GetClientStageId();
		StageDifficulty = DocBattleStage->GetStageDifficulty();
		ServerStageId = DocBattleStage->GetServerStageId();

		//Cache StageInfoData

		UB2StageInfo* StageInfoTable = StaticFindStageInfo();
		int32 ChapterNum = StageInfoTable ? StageInfoTable->GetChapterNumByClientStageId(ClientStageId) : 0;
		int32 StageNum = StageInfoTable ? StageInfoTable->GetSubChapterNumByClientStageId(ClientStageId) : 0;
		
		CachedCurrentStageInfoData = StageInfoTable ? StageInfoTable->GetInfoData(ClientStageId, StageDifficulty) : NULL;

		SetHeaderTitleAsChapterNum(); // CachedCurrentStageInfoData ���� ����..
		SetStageDiffulty();

		if (CachedCurrentStageInfoData)
		{
			SetStageMainTexture(CachedCurrentStageInfoData->GetStageTexture());
			SetStageStory(CachedCurrentStageInfoData->GetLocalizedStageStory());
			SetNPCTexture(CachedCurrentStageInfoData->GetNPCTexture());
		}

		// ���� ������ ���� ������ ���� ���� ���;� �Ǽ� �տ� �߰� ��
		FStageFixedDropsInfo* const BossPiece = DocBattleStage->GetBossDropItemsList();
		AddBossPieceRewardItem(BossPiece);
		// ��񺸴� ���׸� ������ ����
		const TArray<FB2AetherItemPreviewInfo>& AetherItems = DocBattleStage->GetAetherItemList();
		for (int AetherCnt = 0; AetherCnt < AetherItems.Num(); ++AetherCnt)
		{
			AddAetherItem(AetherItems[AetherCnt]);
		}
		// ��� ������ Preview
		const TArray<FB2MaterialItemPreviewInfo>& MaterialItems = DocBattleStage->GetMaterialItemList();
		for (int MaterialCnt = 0; MaterialCnt < MaterialItems.Num(); ++MaterialCnt)
		{
			AddMaterialItem(MaterialItems[MaterialCnt]);
		}
		// ��������� ó��. �ٲ��� �����Ƿ� Delegateó�� �ʿ����
		const TArray<FB2RewardItemPreviewInfo>& RewardItems = DocBattleStage->GetRewardItemList();
		for (int RewardCnt = 0; RewardCnt < RewardItems.Num(); ++RewardCnt)
		{
			AddRewardItem(RewardItems[RewardCnt]);
		}

		//SetRepeatMissionReward(DocBattleStage->GetRepeatMissionReward());

		SetStageGradeInfo(DocBattleStage->GetStageClearConditions());

		// �ʱ������� ������ �ʱ�ȭ
		TArray<EStageBuffType> StageBuffs;
		DocBattleStage->SetSelectedStageBuffs(StageBuffs);
	}
	
	auto DocBattle = UB2UIDocHelper::GetDocBattle();
	if (DocBattle)
	{
		//���ο����� �������� �����͸� ������
		MainHeroDoc = UB2UIDocHelper::GetDocHero(DocBattle->GetCurPCClass());
		SubHeroDoc = UB2UIDocHelper::GetDocHero(DocBattle->GetTagPCClass());
		
		//���ο����� �������� �ٲ������ ó��
		DocBattle->OnCurPCClassChanged.AddUObject(this, &UB2UIBattleStageInfo::OnChangedCurPCClass);
		DocBattle->OnTagPCClassChanged.AddUObject(this, &UB2UIBattleStageInfo::OnChangedTagPCClass);

		UpdateHerosPower();
	}
}

void UB2UIBattleStageInfo::SetStageGradeInfo(const TArray<bool>& Grade)
{
	for (int32 Index = 0; Index < Grade.Num(); ++Index)
	{
		if (false == WS_ClearResultStar.IsValidIndex(Index) || nullptr == WS_ClearResultStar[Index])
		{
			BII_CHECK(false);
			continue;
		}

		WS_ClearResultStar[Index]->SetActiveWidgetIndex(Grade[Index] ? 0 : 1);
	}

	if (TB_ClearResult.IsValidIndex(0) && TB_ClearResult[0].IsValid())
	{
		TB_ClearResult[0]->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, Grade[0] ? TEXT("Stage_Grade_Clear_1") : TEXT("Stage_Grade_1")));
	}
	if (const auto* DocBattleStage = UB2UIDocHelper::GetDocBattleStage())
	{
		if (TB_ClearResult.IsValidIndex(1) && TB_ClearResult[1].IsValid())
		{
			TB_ClearResult[1]->SetText(GetStageClearGradeInfoText(DocBattleStage->GetServerStageId(), 3, Grade[1]));
		}
	}
	if (TB_ClearResult.IsValidIndex(2) && TB_ClearResult[2].IsValid())
	{
		TB_ClearResult[2]->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, Grade[2] ? TEXT("Stage_Grade_Clear_3") : TEXT("Stage_Grade_3")));
	}
}

void UB2UIBattleStageInfo::UnbindDoc()
{
	ClearRewardItems();

	auto DocBattleStage = UB2UIDocHelper::GetDocBattleStage();
	if (DocBattleStage)
	{
		DocBattleStage->OnBattleDifficultyChanged.RemoveAll(this);
		DocBattleStage->OnRepeatBattleCurrentOnChanged.RemoveAll(this);
		DocBattleStage->OnRepeatMissionCurNumChanged.RemoveAll(this);
		DocBattleStage->OnRepeatMissionMaxNumChanged.RemoveAll(this);
		DocBattleStage->OnRepeatMissionRewardChanged.RemoveAll(this);
		DocBattleStage->OnRepeatBattleBoostOnChanged.RemoveAll(this);
	}

	auto DocBattle = UB2UIDocHelper::GetDocBattle();
	if (DocBattle)
	{
		DocBattle->OnCurPCClassChanged.RemoveAll(this);
		DocBattle->OnTagPCClassChanged.RemoveAll(this);
	}

	UB2UIDocUser* DocUser = UB2UIDocHelper::GetDocUser();
	if (DocUser)
	{ 
		DocUser->OnGoldChanged.RemoveAll(this);
	}

	for (UB2UISlotItem* RewardSlotItem : BattleRewardItems)
	{
		if (RewardSlotItem)
		{
			RewardSlotItem->UnbindDoc();
		}
	}

	ClientStageId = INDEX_NONE;
	ServerStageId = FServerStageID();

	MainHeroDoc = nullptr;
	SubHeroDoc = nullptr;
	SetDoc(nullptr);	
}

void UB2UIBattleStageInfo::OnSceneOpen(EUIScene InOpenedScene)
{
	Super::OnSceneOpen(InOpenedScene);

	// UIHeader �� ���� Scene ���� �����ϴ� ��� Init ������ �ϸ� Scene �� ���Ե� header �� ������ �ȵ� ��Ȳ�� �� ����.
	SetHeaderTitleAsChapterNum();
}

void UB2UIBattleStageInfo::AddBossPieceRewardItem(const FStageFixedDropsInfo* _BossPieceData)
{
	//���� ������ ������ �ʴ� Stage������ nullptr�� ���ְ� �־ return��
	if (!_BossPieceData) return;

	UB2UISlotItem* NewRewardItem = CreateRewardItem();
	if (NewRewardItem)
	{
		BattleRewardItems.Add(NewRewardItem);
		NewRewardItem->Init();
		FB2Item b2item;
		b2item.ItemRefID = _BossPieceData->Iteminfo_idx;
		b2item.ItemClass = EItemClass::EIC_BossPiece;
		NewRewardItem->BindDoc(b2item);
		NewRewardItem->SetVisibleStar(ESlateVisibility::Collapsed);
		NewRewardItem->SetVisibleStageInfoItemIcon(ESlateVisibility::HitTestInvisible, true);
		NewRewardItem->SetStageInfoStarGrade(_BossPieceData->Min_Gain, _BossPieceData->Max_Gain);

		// �������� ���� ������ �߰� ������ �־���
		FB2UISlotItemStageRewardInfo ExtraStageRewardInfo;
		ExtraStageRewardInfo.PrimPointMaxValue = _BossPieceData->Max_Gain;
		ExtraStageRewardInfo.PrimPointMinValue = _BossPieceData->Min_Gain;
		int convert_itemidx = FItemRefIDHelper::GetRelicIDFromBossPieceRefID(_BossPieceData->Iteminfo_idx);
		ExtraStageRewardInfo.RandomOptionCount = convert_itemidx;//����� ��Ʈ�� �� �ڸ�. �ڸ��� ������ ���� ������ �ֱ� �ϴ�...
		NewRewardItem->SetExtraStageRewardInfo(ExtraStageRewardInfo);

		NewRewardItem->ShowCount(false);
		NewRewardItem->OnSlotItemClickDelgate.BindDynamic(this, &UB2UIBattleStageInfo::OnClickBtnBossPieceItem);

		// �� SlotItem ���� ��� �ִ� ScrollBox �� �Է��� ������ ���� �¾�
		NewRewardItem->SetupManualScrollBoxSender_byReceiver(this, SB_RewardItems.Get());
	}
}

void UB2UIBattleStageInfo::AddRewardItem(const FB2RewardItemPreviewInfo& InItemData)
{
	UB2UISlotItem* NewRewardItem = CreateRewardItem();
	if (NewRewardItem)
	{
		BattleRewardItems.Add(NewRewardItem);
		NewRewardItem->Init();
		NewRewardItem->BindDoc(InItemData);
		NewRewardItem->SetVisibleStar(ESlateVisibility::Collapsed);
		NewRewardItem->SetVisibleStageInfoItemIcon(ESlateVisibility::HitTestInvisible, false);

		// �������� ���� ������ �߰� ������ �־���
		FB2UISlotItemStageRewardInfo ExtraStageRewardInfo;
		ExtraStageRewardInfo.PrimPointMaxValue = InItemData.PrimPointMaxValue;
		ExtraStageRewardInfo.PrimPointMinValue = InItemData.PrimPointMinValue;
		ExtraStageRewardInfo.RandomOptionCount = InItemData.RandomOptionCount;
		NewRewardItem->SetExtraStageRewardInfo(ExtraStageRewardInfo);

		NewRewardItem->ShowCount(false);
		NewRewardItem->OnSlotItemClickDelgate.BindDynamic(this, &UB2UIBattleStageInfo::OnClickBtnRewardItem);

		// �� SlotItem ���� ��� �ִ� ScrollBox �� �Է��� ������ ���� �¾�
		NewRewardItem->SetupManualScrollBoxSender_byReceiver(this, SB_RewardItems.Get());
	}
}

void UB2UIBattleStageInfo::AddAetherItem(const FB2AetherItemPreviewInfo& InItemData)
{
	UB2UISlotItem* NewRewardItem = CreateRewardItem();
	if (NewRewardItem)
	{
		BattleRewardItems.Add(NewRewardItem);
		NewRewardItem->Init();
		NewRewardItem->BindEther(InItemData);
		NewRewardItem->SetVisibleStar(ESlateVisibility::Collapsed);
		NewRewardItem->SetVisibleStageInfoItemIcon(ESlateVisibility::HitTestInvisible, false);

		NewRewardItem->ShowCount(false);
		NewRewardItem->OnSlotItemClickDelgate.BindDynamic(this, &UB2UIBattleStageInfo::OnClickBtnEtherItem);

		// �� SlotItem ���� ��� �ִ� ScrollBox �� �Է��� ������ ���� �¾�
		NewRewardItem->SetupManualScrollBoxSender_byReceiver(this, SB_RewardItems.Get());
	}
}

void UB2UIBattleStageInfo::AddMaterialItem(const FB2MaterialItemPreviewInfo& InItemData)
{
	UB2UISlotItem* NewRewardItem = CreateRewardItem();
	if (NewRewardItem)
	{
		BattleRewardItems.Add(NewRewardItem);
		NewRewardItem->Init();
		
		FB2Item ItemData;
		ItemData.ItemRefID = InItemData.ItemRefID;
		ItemData.ConsumingAmount = InItemData.ItemDropCount;
		ItemData.InventoryType = EItemInvenType::EIIVT_Consumables;
		ItemData.ItemClass = EItemClass::EIC_End;

		NewRewardItem->BindDoc(ItemData);
		NewRewardItem->SetVisibleStar(ESlateVisibility::Collapsed);
		NewRewardItem->SetVisibleStageInfoItemIcon(ESlateVisibility::Collapsed, false);

		NewRewardItem->ShowCount(false);
		NewRewardItem->OnSlotItemClickDelgate.BindDynamic(this, &UB2UIBattleStageInfo::OnClickBtnMaterialItem);

		// �� SlotItem ���� ��� �ִ� ScrollBox �� �Է��� ������ ���� �¾�
		NewRewardItem->SetupManualScrollBoxSender_byReceiver(this, SB_RewardItems.Get());
	}
}

void UB2UIBattleStageInfo::ClearRewardItems()
{
	for (auto RewarItem : BattleRewardItems)
	{
		RewarItem->UnbindDoc();
		RewarItem->OnSlotItemClickDelgate.Unbind();
	}

	BattleRewardItems.Empty();

	RemoveAllRewardItems();
}

void UB2UIBattleStageInfo::ArinCheck()
{
	if (StageGameStarted)
		return;

	if (!CheckAndInvokeArinConsulting()) // ���������� Arin �� ���� üũ
	{
		//UIP_StartBT->StartCloseAni();
	}
}

void UB2UIBattleStageInfo::UpdateHerosPower()
{
	auto DocBattle = UB2UIDocHelper::GetDocBattle();
	if (DocBattle)
	{
		MainHeroDoc = UB2UIDocHelper::GetDocHero(DocBattle->GetCurPCClass());
		SubHeroDoc = UB2UIDocHelper::GetDocHero(DocBattle->GetTagPCClass());
	}
}

void UB2UIBattleStageInfo::SetStageMainTexture(UTexture2D* MainTexture)
{
	if (IMG_MainTexture.IsValid())
		IMG_MainTexture->SetBrushFromTexture(MainTexture);
}

void UB2UIBattleStageInfo::SetNPCTexture(UTexture2D* NPCTexture)
{
	if (IMG_NPC.IsValid())
		IMG_NPC->SetBrushFromTexture(NPCTexture);
}

void UB2UIBattleStageInfo::SetStageStory(FText StageStory)
{
	if (TB_StageStory.IsValid())
		TB_StageStory->SetText(StageStory);
}

bool UB2UIBattleStageInfo::CheckAndInvokeArinConsulting()
{
	// �������� ���� ��ư ������ �� �Ҹ��� �� ����.
	
	// ���� Ƚ�� ������ �����ߴ� ���. �̰� �⺻���� Arin �� ���� ���� üũ ������ �����ؼ� ��ŵ Ƚ�� ������ �Ź� ����ǵ��� ��.
	// Arin �� ������ �ߵ��ϴ� ��Ȳ������ �̰� �˻��� ��� ��ĩ �ʹ� ���� Arin �� ������ �� ���� �� ���� �ִ�.
	UB2UIDocUICondition* DocCondition = UB2UIDocHelper::GetDocUICondition();
	if (DocCondition && DocCondition->ShouldIgnorePreCombatArinConsulting())
	{ // �ϳ� �����ϰ� �̹��� ��ŵ
		DocCondition->SetPreCombatArinConsultIgnoreCount(DocCondition->GetPreCombatArinConsultIgnoreCount() - 1);
		return false;
	}

	UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();
	// ArinConsultingSubject �� �� ������ ���ö� ������ ���������� ĳ���� �����ϸ鼭 ���õǾ�� ��.
	EArinConsultPreCombat_MainSubject ArinConsultSubject = DocBS ? DocBS->GetArinConsultingSubject() : EArinConsultPreCombat_MainSubject::ARCPCMS_None;

	if (ArinConsultSubject != EArinConsultPreCombat_MainSubject::ARCPCMS_None)
	{
		UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
		if (UIMgrInst)
		{
			UIMgrInst->OpenUI(UIFName::ArinConsult_PreCombat);
			return true;
		}
	}
	return false;
}

void UB2UIBattleStageInfo::ShowRewardItemInfo(const FB2RewardItemPreviewInfo& InRewardInfoData)
{
	if (UIP_BattleStageInfoToolTip.IsValid())
	{
		UIP_BattleStageInfoToolTip->ShowRewardItemInfo(InRewardInfoData);
	}
}

void UB2UIBattleStageInfo::ShowBossPieceInfo(const FB2RewardItemPreviewInfo& InRewardInfoData)
{
	if (UIP_BattleStageInfoToolTip.IsValid())
	{
		UIP_BattleStageInfoToolTip->ShowBossPieceInfo(InRewardInfoData);
	}
}

void UB2UIBattleStageInfo::ShowEtherItemInfo(const FB2RewardItemPreviewInfo& InRewardInfoData)
{
	if (UIP_BattleStageInfoToolTip.IsValid())
	{
		UIP_BattleStageInfoToolTip->ShowEtherItemInfo(InRewardInfoData);
	}
}

void UB2UIBattleStageInfo::ShowMaterialItemInfo(const FB2RewardItemPreviewInfo& InRewardInfoData)
{
	if (UIP_BattleStageInfoToolTip.IsValid())
	{
		UIP_BattleStageInfoToolTip->ShowMaterialItemInfo(InRewardInfoData);
	}
}

void UB2UIBattleStageInfo::HideRewardItemInfo()
{
	if (UIP_BattleStageInfoToolTip.IsValid())
	{
		UIP_BattleStageInfoToolTip->HideToolTip();
	}
}

void UB2UIBattleStageInfo::OnOpenComplete()
{
	Super::OnOpenComplete();
}

void UB2UIBattleStageInfo::OnCloseComplete()
{
	Super::OnCloseComplete();
	StageGameStarted = false;
}

//====================================================================================
//									Click Handler
//====================================================================================

void UB2UIBattleStageInfo::OnClickBtnChangeMainSub()							//Main, Sub ��ü ����ΰڽ��ϴ�...
{
	// UB2UICharacterSelect ���� �ϴ� �Ͱ� �ߺ��Ǳ� �ϴµ� ���� ���⵵ ���ϱ�..

	UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
	if (DocBattle)
	{// Main/Sub �ٲ�ġ��
		LobbySetPCSelectionClass<EPCClass, EPCClass>::GetInstance().Signal(IntToPCClass(DocBattle->GetTagPCClass()), IntToPCClass(DocBattle->GetCurPCClass()));

		// ���� Ŭ������ ����ʿ� ���� �Ƹ��� ���� üũ. �������� ���� ��ġ�� �޶��� �� �������� ��ų����Ʈ ���� �ſ� ���� �̵� �� ���� ȭ������ ������ ���� �޶����ڱ� ��
		UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();
		GameStageCheckForArinConsultingClass<FServerStageID>::GetInstance().Signal(DocBS ? DocBS->GetServerStageId() : FServerStageID());
		PlayChangeChacterAnimBP();
	}
}

void UB2UIBattleStageInfo::OnClickBtnBossPieceItem(UB2UISlotItem* ClickedItem)
{
	if (StageGameStarted)
	return;

	if (ClickedItem)
	{
		ShowBossPieceInfo(ClickedItem->GetStageRewardPreviewInfo());
	}
}

void UB2UIBattleStageInfo::OnClickBtnRewardItem(UB2UISlotItem* ClickedItem)
{
	if (StageGameStarted)
		return;

	if (ClickedItem)
	{
		ShowRewardItemInfo(ClickedItem->GetStageRewardPreviewInfo());
	}
}

void UB2UIBattleStageInfo::OnClickBtnEtherItem(UB2UISlotItem* ClickedItem)
{
	if (StageGameStarted)
		return;

	if (ClickedItem)
	{
		ShowEtherItemInfo(ClickedItem->GetStageRewardPreviewInfo());
	}
}

void UB2UIBattleStageInfo::OnClickBtnMaterialItem(UB2UISlotItem* ClickedItem)
{
	if (StageGameStarted)
		return;

	if (ClickedItem)
	{
		ShowMaterialItemInfo(ClickedItem->GetStageRewardPreviewInfo());
	}
}

void UB2UIBattleStageInfo::OnClickBtnHideRewardItemInfoSet()
{
	HideRewardItemInfo();
}

void UB2UIBattleStageInfo::OnClickBattleReady()
{
	LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::CharacterSelectPage);
}

//====================================================================================
//									Doc Delegate
//====================================================================================

void UB2UIBattleStageInfo::OnChangedCurPCClass(class UB2UIDocBase* Sender, int32 CurPCClass, int32 PrevCurPCClass)
{
	UpdateHerosPower();
}

void UB2UIBattleStageInfo::OnChangedTagPCClass(class UB2UIDocBase* Sender, int32 TagPCClass, int32 PrevTagPCClass)
{
	UpdateHerosPower();
}

void UB2UIBattleStageInfo::MarkStageGameStarted()
{
	StageGameStarted = true; // �������� ���� ��ư ���� �� Ŭ�� �� ���� ����ϰ� �ؼ� ��� �߿��� ��ư���� �ݺ� Ŭ������ �ʵ��� �Ѵ�.

	// ��, ��Ȥ Ư���� �������� (������ ����) ������ �������� ��Ī�� �ȵ� �� ������ ��� ���� �ð��� �帥 �Ŀ��� ���½�Ű����.
	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		TheWorld->GetTimerManager().SetTimer(StageGameStartedResetTH, this, &UB2UIBattleStageInfo::StageGameStartedResetCB, 5.0f, false);
	}
}

void UB2UIBattleStageInfo::StageGameStartedResetCB()
{
	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		TheWorld->GetTimerManager().ClearTimer(StageGameStartedResetTH);
	}
	StageGameStarted = false;
}