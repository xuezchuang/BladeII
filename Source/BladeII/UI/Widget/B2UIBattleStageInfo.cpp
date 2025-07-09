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
	// 이거 StaticText 세팅 블루프린트에 많이 있음.

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
	if (CachedCurrentStageInfoData && DocBattleStage) // CachedCurrentStageInfoData 얻어온 시점 이후여야
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

		SetHeaderTitleAsChapterNum(); // CachedCurrentStageInfoData 얻어온 이후..
		SetStageDiffulty();

		if (CachedCurrentStageInfoData)
		{
			SetStageMainTexture(CachedCurrentStageInfoData->GetStageTexture());
			SetStageStory(CachedCurrentStageInfoData->GetLocalizedStageStory());
			SetNPCTexture(CachedCurrentStageInfoData->GetNPCTexture());
		}

		// 보스 조각이 보상 아이템 보다 먼저 나와야 되서 앞에 추가 함
		FStageFixedDropsInfo* const BossPiece = DocBattleStage->GetBossDropItemsList();
		AddBossPieceRewardItem(BossPiece);
		// 장비보다 에테르 조각이 먼저
		const TArray<FB2AetherItemPreviewInfo>& AetherItems = DocBattleStage->GetAetherItemList();
		for (int AetherCnt = 0; AetherCnt < AetherItems.Num(); ++AetherCnt)
		{
			AddAetherItem(AetherItems[AetherCnt]);
		}
		// 재료 아이템 Preview
		const TArray<FB2MaterialItemPreviewInfo>& MaterialItems = DocBattleStage->GetMaterialItemList();
		for (int MaterialCnt = 0; MaterialCnt < MaterialItems.Num(); ++MaterialCnt)
		{
			AddMaterialItem(MaterialItems[MaterialCnt]);
		}
		// 보상아이템 처리. 바뀔일 없으므로 Delegate처리 필요없음
		const TArray<FB2RewardItemPreviewInfo>& RewardItems = DocBattleStage->GetRewardItemList();
		for (int RewardCnt = 0; RewardCnt < RewardItems.Num(); ++RewardCnt)
		{
			AddRewardItem(RewardItems[RewardCnt]);
		}

		//SetRepeatMissionReward(DocBattleStage->GetRepeatMissionReward());

		SetStageGradeInfo(DocBattleStage->GetStageClearConditions());

		// 초기진입은 무조건 초기화
		TArray<EStageBuffType> StageBuffs;
		DocBattleStage->SetSelectedStageBuffs(StageBuffs);
	}
	
	auto DocBattle = UB2UIDocHelper::GetDocBattle();
	if (DocBattle)
	{
		//메인영웅과 섭영웅의 데이터를 가져옴
		MainHeroDoc = UB2UIDocHelper::GetDocHero(DocBattle->GetCurPCClass());
		SubHeroDoc = UB2UIDocHelper::GetDocHero(DocBattle->GetTagPCClass());
		
		//메인영웅과 섭영우이 바뀌었을때 처리
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

	// UIHeader 와 같이 Scene 으로 구성하는 경우 Init 시점에 하면 Scene 에 포함된 header 가 생성이 안된 상황일 수 있음.
	SetHeaderTitleAsChapterNum();
}

void UB2UIBattleStageInfo::AddBossPieceRewardItem(const FStageFixedDropsInfo* _BossPieceData)
{
	//보스 조각이 나오지 않는 Stage에서는 nullptr로 해주고 있어서 return함
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

		// 스테이지 보상 아이템 추가 정보를 넣어줌
		FB2UISlotItemStageRewardInfo ExtraStageRewardInfo;
		ExtraStageRewardInfo.PrimPointMaxValue = _BossPieceData->Max_Gain;
		ExtraStageRewardInfo.PrimPointMinValue = _BossPieceData->Min_Gain;
		int convert_itemidx = FItemRefIDHelper::GetRelicIDFromBossPieceRefID(_BossPieceData->Iteminfo_idx);
		ExtraStageRewardInfo.RandomOptionCount = convert_itemidx;//여기는 스트링 들어갈 자리. 자리를 억지로 만든 느낌이 있긴 하다...
		NewRewardItem->SetExtraStageRewardInfo(ExtraStageRewardInfo);

		NewRewardItem->ShowCount(false);
		NewRewardItem->OnSlotItemClickDelgate.BindDynamic(this, &UB2UIBattleStageInfo::OnClickBtnBossPieceItem);

		// 이 SlotItem 들을 들고 있는 ScrollBox 에 입력을 보내기 위한 셋업
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

		// 스테이지 보상 아이템 추가 정보를 넣어줌
		FB2UISlotItemStageRewardInfo ExtraStageRewardInfo;
		ExtraStageRewardInfo.PrimPointMaxValue = InItemData.PrimPointMaxValue;
		ExtraStageRewardInfo.PrimPointMinValue = InItemData.PrimPointMinValue;
		ExtraStageRewardInfo.RandomOptionCount = InItemData.RandomOptionCount;
		NewRewardItem->SetExtraStageRewardInfo(ExtraStageRewardInfo);

		NewRewardItem->ShowCount(false);
		NewRewardItem->OnSlotItemClickDelgate.BindDynamic(this, &UB2UIBattleStageInfo::OnClickBtnRewardItem);

		// 이 SlotItem 들을 들고 있는 ScrollBox 에 입력을 보내기 위한 셋업
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

		// 이 SlotItem 들을 들고 있는 ScrollBox 에 입력을 보내기 위한 셋업
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

		// 이 SlotItem 들을 들고 있는 ScrollBox 에 입력을 보내기 위한 셋업
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

	if (!CheckAndInvokeArinConsulting()) // 마지막으로 Arin 의 조언 체크
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
	// 스테이지 시작 버튼 눌렀을 때 불리는 거 가정.
	
	// 일정 횟수 이전에 무시했던 경우. 이걸 기본적인 Arin 의 조언 상태 체크 이전에 실행해서 스킵 횟수 차감이 매번 실행되도록 함.
	// Arin 의 조언이 발동하는 상황에서만 이걸 검사할 경우 자칫 너무 오래 Arin 의 조언을 못 보게 될 수도 있다.
	UB2UIDocUICondition* DocCondition = UB2UIDocHelper::GetDocUICondition();
	if (DocCondition && DocCondition->ShouldIgnorePreCombatArinConsulting())
	{ // 하나 차감하고 이번엔 스킵
		DocCondition->SetPreCombatArinConsultIgnoreCount(DocCondition->GetPreCombatArinConsultIgnoreCount() - 1);
		return false;
	}

	UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();
	// ArinConsultingSubject 는 이 페이지 들어올때 내지는 영웅관리나 캐릭터 선택하면서 세팅되어야 함.
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

void UB2UIBattleStageInfo::OnClickBtnChangeMainSub()							//Main, Sub 교체 살려두겠습니다...
{
	// UB2UICharacterSelect 에서 하는 것과 중복되긴 하는데 따로 빼기도 뭐하군..

	UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
	if (DocBattle)
	{// Main/Sub 바꿔치기
		LobbySetPCSelectionClass<EPCClass, EPCClass>::GetInstance().Signal(IntToPCClass(DocBattle->GetTagPCClass()), IntToPCClass(DocBattle->GetCurPCClass()));

		// 선택 클래스가 변경됨에 따라 아린의 조언도 체크. 전투력은 둘이 합치니 달라질 게 없겠지만 스킬포인트 같은 거에 따라 이동 시 누구 화면으로 가는지 정도 달라지겠군 ㅋ
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
	StageGameStarted = true; // 스테이지 시작 버튼 같은 걸 클릭 시 여길 통과하게 해서 몇몇 중요한 버튼들이 반복 클릭되지 않도록 한다.

	// 단, 간혹 특수한 사정으로 (서버측 사정) 실제로 스테이지 런칭이 안될 수 있으니 어느 정도 시간이 흐른 후에는 리셋시키도록.
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