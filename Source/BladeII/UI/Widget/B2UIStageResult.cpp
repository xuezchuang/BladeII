#include "B2UIStageResult.h"
#include "B2UIStageResultLevelUp.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"
#include "B2UIDocBattle.h"
#include "B2UIDocHero.h"
#include "QuestManager.h"
#include "B2UIQuestDialog.h"
#include "B2StageManager.h"
#include "B2StageGameMode.h"
#include "B2UISlotItem.h"
#include "TutorialManager.h"
#include "BladeIIBlockToSyncNetwork.h"
#include "B2UICBTClear.h"
#include "B2UIHotTimeIcon.h"

#include "Global.h"
#include "BladeIIGameImpl.h"

UB2UIStageResult::UB2UIStageResult(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsSuccessiveLevelUpOpen = false;
	ExpIncreaseStartTime = 0.0f;
	ExpIncreaseSpeed = 1.0f;
	bExpIncreaseStarted = false;
	bExpIncreaseFinished = false;
	CachedWidgetStartTime = 0.0f;

	bButtonClicked = false;
}

void UB2UIStageResult::CacheAssets()
{
	GET_SLOT(UButton,		BTN_Village)
	GET_SLOT(UTextBlock,	TB_BtnVillage)
	GET_SLOT(UButton,		BTN_AreaSelection)
	GET_SLOT(UTextBlock,	TB_BtnAreaSelection)
	GET_SLOT(UButton,		BTN_Restart)
	GET_SLOT(UTextBlock,	TB_BtnRestart)
	GET_SLOT(UButton,		BTN_NextArea)
	GET_SLOT(UTextBlock,	TB_BtnNextArea)

	GET_SLOT(UTextBlock,	TB_Main_TX)
	GET_SLOT(UImage,		IMG_Main_LevelUp)
	if (IMG_Main_LevelUp.IsValid())
	{
		IMG_Main_LevelUp->SetVisibility(ESlateVisibility::Collapsed);
	}
	GET_SLOT(UTextBlock,	TB_MainCharLevel)
	GET_SLOT(UImage,		IMG_PortraitMain)
	GET_SLOT(UProgressBar,	BasicExpBar_MainPlayer)
	GET_SLOT(UProgressBar,	ExtraExpBar_MainPlayer)
	GET_SLOT(UTextBlock,	TB_ExpPercentMainChar)

	GET_SLOT(UTextBlock,	TB_Tag_TX)
	GET_SLOT(UImage,		IMG_Tag_LevelUp)
	if (IMG_Tag_LevelUp.IsValid())
	{
		IMG_Tag_LevelUp->SetVisibility(ESlateVisibility::Collapsed);
	}
	GET_SLOT(UTextBlock,	TB_SubCharLevel)
	GET_SLOT(UImage,		IMG_PortraitSub)
	GET_SLOT(UProgressBar,	BasicExpBar_SubPlayer)
	GET_SLOT(UProgressBar,	ExtraExpBar_SubPlayer)
	GET_SLOT(UTextBlock,	TB_ExpPercentSubChar)

	GET_SLOT(UTextBlock,	TB_CombatTimeText)
	GET_SLOT(UTextBlock,	TB_CombatTime)

	GET_SLOT(UTextBlock,		TB_AcquiredGoldText)
	GET_SLOT(UB2RichTextBlock,	TB_AcquiredGold)

	GET_SLOT(UTextBlock, TB_CombatTimeLeft)
	GET_SLOT(UTextBlock, TB_CombatTimeRight)

	GET_SLOT(UTextBlock, TB_AcquiredGoldLeft)
	GET_SLOT(UB2RichTextBlock, TB_AcquiredGoldRight)
	GET_SLOT(UTextBlock, TB_Boost)

	GET_SLOT(UHorizontalBox, HB_SoldGold)
	GET_SLOT(UTextBlock, TB_SoldGoldLeft)
	GET_SLOT(UB2RichTextBlock, TB_SoldGoldRight)
	GET_SLOT(UTextBlock, TB_SoldBoost)

	GET_SLOT(UTextBlock,	TB_Title)
	GET_SLOT(UTextBlock,	STB_RepeatOpen)
	GET_SLOT(UTextBlock,	STB_BoostOpen)
	GET_SLOT(UTextBlock,	STB_Reward)

	GET_SLOT(UPanelWidget, CP_NewStageInfo)
	
	GET_SLOT(UB2ScrollBox, SB_RewardItems)
	GET_SLOT(UHorizontalBox, HB_AcqItemGroupPanel)

	GET_SLOT(UB2UIStageResultLevelUp, UIP_LevelUp)
	if (UIP_LevelUp.IsValid())
	{
		UIP_LevelUp->Init();
	}
	GET_SLOT(UB2UIRepeatBattleCountDown, UIP_RepeatBattleCountDownP)
	if (UIP_RepeatBattleCountDownP.IsValid())
	{
		UIP_RepeatBattleCountDownP->Init();
		UIP_RepeatBattleCountDownP->SetPauseAble(true);
	}
	GET_SLOT(UB2UIRepeatBattleInGameInfo, UIP_RepeatBattleInGameInfoP)
	if (UIP_RepeatBattleInGameInfoP.IsValid())
	{
		UIP_RepeatBattleInGameInfoP->Init();
	}
	
	WS_ClearResultStar.AddDefaulted(static_cast<int32>(EStageClearCondition::MaxCondition));
	GET_SLOT_BYNAME(UWidgetSwitcher, WS_ClearResultStar[0], WS_ClearResultStar1);
	GET_SLOT_BYNAME(UWidgetSwitcher, WS_ClearResultStar[1], WS_ClearResultStar2);
	GET_SLOT_BYNAME(UWidgetSwitcher, WS_ClearResultStar[2], WS_ClearResultStar3);

	TB_ClearResult.AddDefaulted(static_cast<int32>(EStageClearCondition::MaxCondition));
	GET_SLOT_BYNAME(UB2RichTextBlock, TB_ClearResult[0], TB_ClearResult1);
	GET_SLOT_BYNAME(UB2RichTextBlock, TB_ClearResult[1], TB_ClearResult2);
	GET_SLOT_BYNAME(UB2RichTextBlock, TB_ClearResult[2], TB_ClearResult3);

	GET_SLOT(UB2UIHotTimeIcon, UIP_Hottime);
	GET_SLOT(UTextBlock, TB_ActiveHotTime);
	GET_SLOT(UOverlay, O_ActiveHotTime);
}

void UB2UIStageResult::UpdateStaticText()
{
	if (TB_BtnVillage.IsValid())
		TB_BtnVillage->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("StageClear_Village")));

	if (TB_BtnAreaSelection.IsValid())
		TB_BtnAreaSelection->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("StageClear_SelectArea")));

	if (TB_BtnRestart.IsValid())
		TB_BtnRestart->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("StageClear_Restart")));

	if (TB_BtnNextArea.IsValid())
		TB_BtnNextArea->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("StageClear_NextArea")));

	if (TB_CombatTimeText.IsValid())
		TB_CombatTimeText->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("StageClear_CambatTime")));

	if (TB_AcquiredGoldText.IsValid())
		TB_AcquiredGoldText->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("StageClear_AcquiredGold")));

	if (TB_CombatTimeLeft.IsValid())
		TB_CombatTimeLeft->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("StageClear_CambatTime")));

	if (TB_AcquiredGoldLeft.IsValid())
		TB_AcquiredGoldLeft->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("StageClear_AcquiredGold")));

	if (TB_SoldGoldLeft.IsValid())
		TB_SoldGoldLeft->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("StageClear_SoldGold")));

	if (TB_Title.IsValid())
		TB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("StageClear_CambatStat")));
	
	if (STB_RepeatOpen.IsValid())
		STB_RepeatOpen->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("BattleStageInfo_RepeatBattleOpen")));
	if (STB_BoostOpen.IsValid())
		STB_BoostOpen->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("BattleStageInfo_BoostOpen")));
	if (STB_Reward.IsValid())
		STB_Reward->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ChapterStarRewardReceiveTitle")));
}

void UB2UIStageResult::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Village, &UB2UIStageResult::OnClickBtnVillage);
	BIND_CLICK_FUNC_TO_BTN(BTN_AreaSelection, &UB2UIStageResult::OnClickBtnAreaSelection);
	BIND_CLICK_FUNC_TO_BTN(BTN_Restart, &UB2UIStageResult::OnClickBtnRestart);
	BIND_CLICK_FUNC_TO_BTN(BTN_NextArea, &UB2UIStageResult::OnClickBtnNextArea);

	HandleServerError4015 = HandleServerError4015Class<FB2StartStageInfo>::GetInstance().Subscribe([this](const FB2StartStageInfo& StageInfo) { this->SetButtonRevitalize(); });
	HandleServerError4030 = HandleServerError4030Class<FB2StartStageInfo>::GetInstance().Subscribe([this](const FB2StartStageInfo& StageInfo) { this->SetButtonRevitalize();	});
}

void UB2UIStageResult::DestroySelf(class UB2UIManager* InUIManager) 
{
	 HandleServerError4015Class<FB2StartStageInfo>::GetInstance().Unsubscribe(HandleServerError4015);
	 HandleServerError4030Class<FB2StartStageInfo>::GetInstance().Unsubscribe(HandleServerError4030);

	if (UIP_RepeatBattleInGameInfoP.IsValid())
	{
		UIP_RepeatBattleInGameInfoP->DestroySelf(InUIManager);
	}

	if (UIP_RepeatBattleCountDownP.IsValid())
	{
		UIP_RepeatBattleCountDownP->DestroySelf(InUIManager);
	}
}

#include "B2StageManager.h"
void UB2UIStageResult::Init()
{
	Super::Init();
	
	if (BasicExpBar_MainPlayer.IsValid())
		BasicExpBar_MainPlayer->SetPercent(0.f);

	if (ExtraExpBar_MainPlayer.IsValid())
		ExtraExpBar_MainPlayer->SetPercent(0.f);
	
	if (BasicExpBar_SubPlayer.IsValid())
		BasicExpBar_SubPlayer->SetPercent(0.f);

	if (ExtraExpBar_SubPlayer.IsValid())
		ExtraExpBar_SubPlayer->SetPercent(0.f);
}

void UB2UIStageResult::BindDocAuto()
{
	//경험치 연출
	if (GetWorld())
	{
		// 이게 World pause 상태에서 사용하기 위한 것이므로 GetRealTimeSeconds 를 해야 하긔. 아래 UpdateExpPercent 부르기 전에 이거 먼저 세팅.
		CachedWidgetStartTime = GetWorld()->GetRealTimeSeconds();
	}

	//독배틀에서 캐릭터 데이터를 가져와서 처리
	auto DocBattle = UB2UIDocHelper::GetDocBattle();
	SetDoc(DocBattle);
	if (DocBattle)
	{
		// 실제 main 으로 선택한 캐릭터가 아닌 stage clear 시점의 현재 캐릭터를 가져옴.
		// Exp bar 증가 애니메이션에 필요한 변수들 세팅. Main/Sub 순서대로
		FLocalCharacterData& CharacterDataStore = BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData();

		auto DocHeroMain = UB2UIDocHelper::GetDocHero(PCClassToInt(CharacterDataStore.GetMainPlayerClass()));
		if (DocHeroMain)
		{
			BindDocMainChar(DocHeroMain); 
			InitExpVarSet(EBV_MainPlayer, CharacterDataStore.GetMainPlayerClass());
		}

		// 실제 sub 로 선택한 캐릭터가 아닌 stage clear 시점의 태그 캐릭터를 가져옴.
		auto DocHeroSub = UB2UIDocHelper::GetDocHero(PCClassToInt(CharacterDataStore.GetSubPlayerClass()));
		if (DocHeroSub)
		{
			BindDocSubChar(DocHeroSub);
			InitExpVarSet(EBV_SubPlayer, CharacterDataStore.GetSubPlayerClass());
		}

		UpdateExpPercent(0.f);
	}
	
	//독배틀스테이지에서 클리어타임을 처리
	auto DocBattleStage = UB2UIDocHelper::GetDocBattleStage();
	const int32 MultipleValue = DocBattleStage->GetStageClearMultipleValue();

	if (DocBattleStage)
	{
		SetStageClearGrade(DocBattleStage->GetStageClearConditions(), DocBattleStage->GetServerStageId());
		SetClearTime(DocBattleStage->GetStagePlayTime());
		// 보여주는 순서대로
		SetDropItem(DocBattleStage->GetStageResultBossDropItemsList());
		SetDropItem(DocBattleStage->GetStageClearEtherItemsList(), MultipleValue);
		SetDropItem(DocBattleStage->GetStageClearMaterialItemsList(), MultipleValue);
		SetDropItem(DocBattleStage->GetStageClearRewardItemList(), MultipleValue);

		SetAcquiredGold(DocBattleStage->GetStageTotalGold()); // 스테이지 결과창 골드표시
		SetSoldGold(DocBattleStage->GetStageSoldGold());

		SetHotTimeBuff(DocBattleStage->GetEndGoldBuffRate(), DocBattleStage->GetEndExpBuffRate());

		DocBattleStage->OnRepeatBattleCurrentOnChanged.AddUObject(this, &UB2UIStageResult::OnChangedRepeatBattleCurrentOn);
	}

	if (ExpIncreaseStartTime > 0.0f)
	{
		bExpIncreaseStarted = false; // 이것도 마찬가지로 타이머가 먹지 않으므로 CachedWidgetStartTime 사용해서 따로 카운트할 것.
	}
	else
	{
		bExpIncreaseStarted = true; // 바로 시작.
	}

	CheckFinalStageDisableButton();
}

void UB2UIStageResult::BindDocMainChar(UB2UIDocHero* DocHero)
{
	if (IMG_PortraitMain.IsValid() && DocHero){
		IMG_PortraitMain->SetBrushFromMaterial(UB2UIDocHero::GetPlayerPortraitMaterial(this, DocHero->GetHeroClass()));
	}

	if (TB_Main_TX.IsValid()){
		TB_Main_TX->SetText(BladeIIGameImpl::GetClientDataStore().GetLocalCharClassOrNickName(DocHero->GetHeroClass()));
	}
}

void UB2UIStageResult::BindDocSubChar(UB2UIDocHero* DocHero)
{
	if (IMG_PortraitSub.IsValid() && DocHero){
		IMG_PortraitSub->SetBrushFromMaterial(UB2UIDocHero::GetPlayerPortraitMaterial(this, DocHero->GetHeroClass()));
	}
	if (TB_Tag_TX.IsValid()){
		TB_Tag_TX->SetText(BladeIIGameImpl::GetClientDataStore().GetLocalCharClassOrNickName(DocHero->GetHeroClass()));
	}
}

void UB2UIStageResult::UnbindDoc()
{
	SetDoc(nullptr);

	UB2UIDocBattleStage* DocBattleStage = UB2UIDocHelper::GetDocBattleStage();
	if (DocBattleStage)
	{
		DocBattleStage->OnRepeatBattleCurrentOnChanged.RemoveAll(this);
	}
	if (UIP_RepeatBattleInGameInfoP.IsValid())
	{ // 이 파트가 여러 곳에서 사용되는데 스테이지 마직막에 쓰일 때만 UnbindDoc 을 한다.
		UIP_RepeatBattleInGameInfoP->UnbindDoc();
	}
}
void UB2UIStageResult::SetStageClearGrade(const TArray<bool>& Grade, FServerStageID InServerStageNum)
{
	int32 PassCount = 0;

#if UE_BUILD_DEBUG
	if ( (Grade.Num() != static_cast<int32>(EStageClearCondition::MaxCondition))
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
		&& !FBladeIIBlockToSyncNetwork::GetInstance().DevIsStandaloneMode()
#endif
		)
	{
		BII_CHECK(false);
	}
#endif // UE_BUILD_DEBUG

	for (int32 Index = 0; Index < Grade.Num(); ++Index)
	{
		if (false == WS_ClearResultStar.IsValidIndex(Index) || nullptr == WS_ClearResultStar[Index])
		{
			BII_CHECK(false);
			continue;
		}

		if (Grade[Index]) // 스테이지 조건 충족한다면
		{
			WS_ClearResultStar[Index]->SetActiveWidgetIndex(0);
			++PassCount;
		}
		else
		{
			WS_ClearResultStar[Index]->SetActiveWidgetIndex(1);
		}
	}

	auto DocBattleStage = UB2UIDocHelper::GetDocBattleStage();
	BII_CHECK(DocBattleStage);
	if (DocBattleStage)
	{
		auto NewMenu = BladeIIGameImpl::GetStageDataStore().IsRepeatBattleLocked(DocBattleStage->GetServerStageId());
		if (NewMenu)
		{
			CP_NewStageInfo->SetVisibility((PassCount == 3) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		}
		else
		{
			CP_NewStageInfo->SetVisibility(ESlateVisibility::Hidden);
		}

		if (TB_ClearResult.IsValidIndex(0) && TB_ClearResult[0].IsValid() && Grade.IsValidIndex(0))
		{
			TB_ClearResult[0]->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, Grade[0] ? TEXT("Stage_Grade_Clear_1") : TEXT("Stage_Grade_1")));
		}		
		if (TB_ClearResult.IsValidIndex(1) && TB_ClearResult[1].IsValid() && Grade.IsValidIndex(1))
		{
			TB_ClearResult[1]->SetText(GetStageClearGradeInfoText(DocBattleStage->GetServerStageId(), 3, Grade[1]));
		}
		if (TB_ClearResult.IsValidIndex(2) && TB_ClearResult[2].IsValid() && Grade.IsValidIndex(2))
		{
			TB_ClearResult[2]->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, Grade[2] ? TEXT("Stage_Grade_Clear_3") : TEXT("Stage_Grade_3")));
		}
	}

	SetStageClearGrade_BP(PassCount);
}

void UB2UIStageResult::SetClearTime(int32 InValue)
{
	if (TB_CombatTime.IsValid())
		TB_CombatTime->SetText(FText::FromString(UB2UIManager::FormatSecondsAsTime(InValue, TEXT("hh:mm:ss"))));

	if (TB_CombatTimeRight.IsValid())
		TB_CombatTimeRight->SetText(FText::FromString(UB2UIManager::FormatSecondsAsTime(InValue, TEXT("hh:mm:ss"))));
}

void UB2UIStageResult::SetDropItem(const TArray<FB2Item> DropItemData, int32 MultipleValue)
{
	if (DropItemData.Num() < 1) return;

	RewardItemData.Empty();

	for (const FB2Item& Elem : DropItemData)
	{
		FB2Item RewardItem = Elem;
		// 예전엔 같은 장비는 겹쳐서 보여줬지만 이제 분리해서 보여주기때문에
		// 아이템(장비)는 Multiple Value(= 1 or 3배 모드)로 골드, 보스조각, 재료아이템은 ConsumingAmount로 보여준다
		bool bIsConsuming = IsGoldItem(RewardItem) || IsBossPieceItem(RewardItem) || IsMaterialItem(RewardItem);
		RewardItem.ConsumingAmount = bIsConsuming ? FMath::Max(1, RewardItem.ConsumingAmount) : FMath::Max(1, MultipleValue);
		RewardItemData.Add(RewardItem);
	}

	for (int32 RI = 0; RI < RewardItemData.Num(); ++RI)
	{
		UB2UISlotItem* NewRewardItem = CreateRewardItem();// = CreateWidget<UB2UISlotItem>(GetOwningPlayer(), SkillOptionListClass);
		if (NewRewardItem)
		{
			BattleRewardItems.Add(NewRewardItem);
			NewRewardItem->Init();
			IsEtherItem(RewardItemData[RI]) ? NewRewardItem->BindEther(RewardItemData[RI]) : NewRewardItem->BindDoc(RewardItemData[RI]);
			bool bInvisibleStar = IsGoldItem(RewardItemData[RI]) || IsBossPieceItem(RewardItemData[RI]) || IsMaterialItem(RewardItemData[RI]);
			NewRewardItem->SetVisibleStar(bInvisibleStar ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
			NewRewardItem->SetVisibleStageInfoItemIcon(ESlateVisibility::Collapsed, false);
			// 이 SlotItem 들을 들고 있는 ScrollBox 에 입력을 보내기 위한 셋업
			NewRewardItem->SetupManualScrollBoxSender_byReceiver(this, SB_RewardItems.Get());
		}
	}
}

void UB2UIStageResult::SetAcquiredGold(int32 InValue)
{
	auto DocBattleStage = UB2UIDocHelper::GetDocBattleStage();
	
	if (TB_AcquiredGold.IsValid())
	{
		if (DocBattleStage->GetRepeatBattleBoostOn())
		{
			TB_AcquiredGold->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Stage_BoostResult")),FText::AsNumber(InValue)));
		}
		else
			TB_AcquiredGold->SetText(FText::AsNumber(InValue));
	}

	if (TB_AcquiredGoldRight.IsValid())
	{
		if (TB_Boost.IsValid())
		{
			TB_Boost->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Stage_BoostResult")));
			TB_Boost->SetVisibility(DocBattleStage->GetRepeatBattleBoostOn() ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		}

		TB_AcquiredGoldRight->SetText(FText::AsNumber(InValue));
	}
}

void UB2UIStageResult::SetSoldGold(int32 InValue)
{
	if (HB_SoldGold.IsValid())
		HB_SoldGold->SetVisibility(InValue > 0 ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	if (auto DocBattleStage = UB2UIDocHelper::GetDocBattleStage())
	{
		if (TB_SoldGoldRight.IsValid())
		{
			TB_SoldGoldRight->SetText(FText::AsNumber(InValue));
		}
		if (TB_SoldBoost.IsValid())
		{
			TB_SoldBoost->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Stage_BoostResult")));
			TB_SoldBoost->SetVisibility(DocBattleStage->GetRepeatBattleBoostOn() ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		}
	}	
}

void UB2UIStageResult::SetButtonRevitalize()
{
	bButtonClicked = false;
}

void UB2UIStageResult::SetHotTimeBuff(int32 GoldRate, int32 ExpRate)
{
	bool IsActiveHotTime = GoldRate > 0 || ExpRate > 0;
	
	if (O_ActiveHotTime.IsValid())
		O_ActiveHotTime->SetVisibility(IsActiveHotTime ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	if (UIP_Hottime.IsValid())
		UIP_Hottime->ActiveEffect_BP(IsActiveHotTime);

	if (TB_ActiveHotTime.IsValid())
		TB_ActiveHotTime->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("HotTime_ApplyDesc")));
}

void UB2UIStageResult::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!bIsSuccessiveLevelUpOpen)
	{
		UpdateExpPercent(InDeltaTime);
	}

	UpdateWidgetCustomTimer(); // 여기로 DeltaTime 을 넘겨주는 건 의미가 없을 듯. 자체 타이머 운용. 그나마 NativeTick 이 불리는 게 다행

	UpdateMainCharLevel();
	UpdateSubCharLevel();
}

void UB2UIStageResult::OnOpenComplete()
{
	SetOpenState(EUIOpenState::Opened);
	// 단지 UIWidgetOpenCompleteClass 이벤트 시그널을 여기서 날리지 않기 위해.

	//팝업닫을때 버튼 다시 활성화
	ToolTipCloseTicket = ToolTipCloseClass<>::GetInstance().Subscribe([this]() {
			this->SetButtonRevitalize();
	});

	bShowMainQuestReward = IsShowMainQuestReward();
	//일단 무조건 버튼은 막고 경험치,퀘스트 등등이 진행된 이후에 풀어준다.
	SetButtonClickable(false);
}

void UB2UIStageResult::SetButtonClickable(bool bClickable)
{
	if (BTN_Village.IsValid()) BTN_Village->SetIsEnabled(bClickable);
	if (BTN_AreaSelection.IsValid()) BTN_AreaSelection->SetIsEnabled(bClickable);
	if (BTN_Restart.IsValid()) BTN_Restart->SetIsEnabled(bClickable);
	if (BTN_NextArea.IsValid()) BTN_NextArea->SetIsEnabled(bClickable);
}

void UB2UIStageResult::CheckFinalStageDisableButton()
{
	// 최대 스테이지 도달 시 더 못 가게 막음
	UB2StageInfo* StageInfoTable = StaticFindStageInfo();
	int32 FinalStageNum = StageInfoTable ? StageInfoTable->GetFinalClientStageId() : 0;
	int32 CurrStageID = AB2StageManager::GetCacheStageKeepEssentialData().GetCurrentClientStageId();

	int32 MaxChapterNum = StageInfoTable ? StageInfoTable->GetStartAdventureModeChapterNum() : 0;


	if (FinalStageNum == CurrStageID)
	{
		
		if (BTN_NextArea.IsValid())
			BTN_NextArea->SetIsEnabled(false);
	}
	else
	{
		if (FinalStageNum < CurrStageID &&
			CurrStageID % 10 == 0)
		{
			if (BTN_NextArea.IsValid())
				BTN_NextArea->SetIsEnabled(false);
		}

	}


#if FORCE_LOGGING_IN_ANDROID
	FPlatformMisc::LowLevelOutputDebugStringf(TEXT("=========B2UIStageResultButton========="));
	FPlatformMisc::LowLevelOutputDebugStringf(TEXT("FinalStageNum %d"), FinalStageNum);
	FPlatformMisc::LowLevelOutputDebugStringf(TEXT("CurrStageID %d"), CurrStageID);
#endif

}


void UB2UIStageResult::OnCloseComplete()
{
	Super::OnCloseComplete();
	ToolTipCloseClass<>::GetInstance().Unsubscribe(ToolTipCloseTicket);
	bButtonClicked = false;
}

void UB2UIStageResult::OnLevelUpFXFinish()
{
	if (bShowMainQuestReward)
	{
		OpenQuestRewardDialog();
	}
	else
	{
		OnAllProcessEnd();
	}
}

bool UB2UIStageResult::IsShowMainQuestReward()
{
	if (QuestManager::GetInstance().HasCachedQuest(b2network::B2QuestSlotType::MAIN1))
	{
		const FCompleteQuestData& CachedCompleteQuest = QuestManager::GetInstance().GetCachedCompleteQuest();
		if (CachedCompleteQuest.bValid && CachedCompleteQuest.CompleteQuest->GetType() == MAIN_QUEST)
		{
			return true;
		}
	}

	return false;
}

// 레벨업, 대화, 퀘스트 등등 모든 클리어 관련 연출 진행사항이 끝나고 최종적으로 호출될 함수
void UB2UIStageResult::OnAllProcessEnd()
{
	if (TutorialManager::GetInstance().HasContentOpenTutorial())
	{
		TutorialManager::GetInstance().OpenDialogContentsOpen();
	}

	else
	{
		SetButtonClickable(true);
		CheckFinalStageDisableButton();
	}
}

void UB2UIStageResult::OpenQuestRewardDialog()
{
	FCompleteQuestData CompleteQuest = QuestManager::GetInstance().GetCachedCompleteQuest();

	auto* QuestDialogUI = UB2UIManager::GetInstance()->OpenUI<UB2UIQuestDialog>(UIFName::QuestDialog);
	if (QuestDialogUI != nullptr)
	{
		QuestPtr NextQuest = QuestManager::GetInstance().GetCachedCurrentQuest();

		QuestDialogUI->OpenRewardDialog(*CompleteQuest.CompleteQuest, CompleteQuest.CompleteReward);
		QuestDialogUI->SetDialogCloseDelegate(FOnQuestDialogClosed::CreateLambda([this](){ this->OnQuestDialogClosed(); }));
		QuestDialogUI->SetNextMainQuest(NextQuest);
	}
}

void UB2UIStageResult::OnQuestDialogClosed()
{
	if (QuestManager::GetInstance().HasCachedQuest(b2network::B2QuestSlotType::MAIN1))
	{
		OpenQuestRewardDialog();
		return;
	}

	else
	{
		OnAllProcessEnd();
	}
}

void UB2UIStageResult::OnActualOpenComplete()
{
	UIWidgetOpenCompleteClass<FName>::GetInstance().Signal(this->GetMyUIFName());
	OnUpdateExpPercentPlaySoundStopBP();


	if (IsRepeatBattleOn())
	{ 
		//if (bShowMainQuestReward)	// 반복전투 중이라도 MainQuest가 우선한다.
		//{
		//	CancelOrStopRepeatBattleClass<>::GetInstance().Signal(); // 반복전투 취소
		//	CheckAndOpenLevelUpPart();
		//}
		//else
		UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();
		if ( DocBS )
		{
			// [연속전투] 중 컨텐츠오픈 튜토리얼이 존재할 경우, 연속전투 멈춤
			if ( DocBS->GetRepeatBattleLoopAll() && TutorialManager::GetInstance().HasContentOpenTutorial() )
			{
				CancelOrStopRepeatBattleClass<>::GetInstance().Signal();
				OnAllProcessEnd();
				return;
			}
		
			// [반복전투][연속전투] 일경우, 로비진입(캐릭터 선택) 없이 진행하므로, 카운트다운 디스플레이를 활성화 한다
			SetupOnRepeatBattleStateChange( true, true );
		}
	}
	else
	{
		CheckAndOpenLevelUpPart(); // 결과 UI 오프닝이 끝난 이 시점에서 레벨업 UI 를 체크
	}
}

void UB2UIStageResult::CloseWidgetDelegate()
{
	//if (BTN_AreaSelection.IsValid() && BTN_AreaSelection->bIsEnabled == true)
	//{
	//	OnClickBtnAreaSelection();
	//}
}

void UB2UIStageResult::InitExpVarSet(FExpProgressVars& ExpVarSet, EPCClass PCClass)
{
	//[@SameLine, 180612] explevelinfo doc, datastore -> datastore 개선

	FLocalCharacterData& CharacterDataStore = BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData();

	const int32 StageClear_AfterLevel = CharacterDataStore.GetCharacterLevel(PCClass);
	const float StageClear_AfterExpPercent = CharacterDataStore.GetCharacterExpPercent(PCClass);

	const int32 StageClear_BeforeLevel = CharacterDataStore.GetBeforeCharacterLevel(PCClass);
	const float StageClear_BeforeExpPercent = CharacterDataStore.GetBeforeCharacterExpPercent(PCClass);

	ExpVarSet.BasicLevelToIncrease = StageClear_AfterLevel - StageClear_BeforeLevel;
	ExpVarSet.BasicTargetExpPercent = StageClear_AfterExpPercent * 0.01f; // UI 에서는 0.0 ~ 1.0 스케일.
	ExpVarSet.BasicCurrentExpPercent = StageClear_BeforeExpPercent * 0.01f; // 이건 점차 ExpIncreaseSpeed 에 의해 증가할 것.

	ExpVarSet.bBasicExpIncreaseComplete = false;

	ExpVarSet.ExtraLevelToIncrease = StageClear_AfterLevel - StageClear_BeforeLevel;
	ExpVarSet.ExtraLevelToIncreaseFromBasic = ExpVarSet.ExtraLevelToIncrease - ExpVarSet.BasicLevelToIncrease;

	ExpVarSet.ExtraTargetExpPercent = StageClear_AfterExpPercent * 0.01f;
	ExpVarSet.ExtraCurrentExpPercent = StageClear_AfterExpPercent * 0.01f; // 초기값은 Basic 이나 Extra 나 마찬가지. BasicCurrent 와 같이 증가하다가 이건 좀 더 증가할 수 있음.

	ExpVarSet.bExtraExpIncreaseComplete = false;

	ExpVarSet.CurrentIncreasedLevel = StageClear_BeforeLevel; // 이것도 점차 PostClearLevel 로 증가할 것.
}

/** Most internal functionality for use in UpdateExpPercent. Returns true when current exp is reached to target value. */
bool UpdateExpPercentCommonInternalFunc(int32& InOutLevelToIncrease, int32& InOutCurrentLevel, float InTargetExpPercent, float& InOutCurrentExpPercent, float ExpIncreaseRate, float DeltaSeconds)
{
	if (InOutLevelToIncrease > 0 || InOutCurrentExpPercent < InTargetExpPercent)
	{
		// 흔한 S = Vt
		InOutCurrentExpPercent += FMath::Min(ExpIncreaseRate * DeltaSeconds, 0.99f); // 한 프레임에 1.0 이상 증가하면 곤란

		if (InOutLevelToIncrease > 0)
		{
			if (InOutCurrentExpPercent >= 1.0f)
			{
				// 다음 레벨로 넘어간다.
				--InOutLevelToIncrease;
				++InOutCurrentLevel;
				InOutCurrentExpPercent -= 1.0f;
			}
		}

		if (InOutLevelToIncrease == 0 && InOutCurrentExpPercent >= InTargetExpPercent)
		{
			// 목표 레벨까지 채웠다면 ExpPercent 까지만 맞추면 됨.
			InOutCurrentExpPercent = InTargetExpPercent;
			return true;
		}

		return false;
	}

	return true;
}

void UB2UIStageResult::UpdateExpPercent(float DeltaSeconds)
{
	// Pause 상태에서 사용하는 거라 타이머로 못하고 직접 카운트
	if (bExpIncreaseStarted == false)
	{
		UWorld* TheWorld = GetWorld();
		if (TheWorld && TheWorld->GetRealTimeSeconds() - CachedWidgetStartTime >= ExpIncreaseStartTime)
		{
			bExpIncreaseStarted = true;
			OnUpdateExpPercentPlaySoundStartBP();
		}
	}

	if ((DeltaSeconds == 0.f || bExpIncreaseStarted) && !bExpIncreaseFinished) // DeltaSeconds == 0.f <- Force Update
	{
		const int32 MainLevelBefore = EBV_MainPlayer.CurrentIncreasedLevel; // 이번 업데이트에서 레벨이 증가했는지 체크를 위해.
		const int32 SubLevelBefore = EBV_SubPlayer.CurrentIncreasedLevel;

		const bool bMainFinished = UpdateExpPercentCommon(EBV_MainPlayer, DeltaSeconds);
		const bool bSubFinished = UpdateExpPercentCommon(EBV_SubPlayer, DeltaSeconds);
				
		const bool bMainLevelIncreased = (EBV_MainPlayer.CurrentIncreasedLevel > MainLevelBefore);
		const bool bSubLevelIncreased = (EBV_SubPlayer.CurrentIncreasedLevel > SubLevelBefore);

		if (bMainLevelIncreased) // 레벨업 넘어간 순간에 텍스트 바꿔줌.
		{
			SetLevelUpTextVisible(true);
		}
		if (bSubLevelIncreased)
		{
			SetLevelUpTextVisible(false);
		}

		bExpIncreaseFinished = (bMainFinished && bSubFinished);
		if (bExpIncreaseFinished)
		{
			// 사실상 여기가 opening 효과가 끝나는 시점이 될 것이다. OnOpenComplete 대신 여기서 필요한 event 를 날리도록..
			OnActualOpenComplete();
		}

		//메인캐릭 베이직 Exp
		if (BasicExpBar_MainPlayer.IsValid())
		{
			float Percent = (EBV_MainPlayer.bBasicExpIncreaseComplete && EBV_MainPlayer.ExtraCurrentExpPercent < EBV_MainPlayer.BasicCurrentExpPercent) ? 0.0f : EBV_MainPlayer.BasicCurrentExpPercent;
			BasicExpBar_MainPlayer->SetPercent(Percent);
		}

		//메인캐릭 엑스트라 Exp
		if (ExtraExpBar_MainPlayer.IsValid())
		{
			BasicExpBar_MainPlayer->SetPercent(EBV_MainPlayer.ExtraCurrentExpPercent);
		}

		//서브캐릭 베이직 Exp
		if (BasicExpBar_SubPlayer.IsValid())
		{
			float Percent = (EBV_SubPlayer.bBasicExpIncreaseComplete && EBV_SubPlayer.ExtraCurrentExpPercent < EBV_SubPlayer.BasicCurrentExpPercent) ? 0.0f : EBV_SubPlayer.BasicCurrentExpPercent;
			BasicExpBar_SubPlayer->SetPercent(Percent);
		}

		//서브캐릭 엑스트라 Exp
		if (ExtraExpBar_SubPlayer.IsValid())
		{
			ExtraExpBar_SubPlayer->SetPercent(EBV_SubPlayer.ExtraCurrentExpPercent);
		}

		//메인캐릭 Exp 퍼센트글자
		if (TB_ExpPercentMainChar.IsValid())
		{
			float CurrentMainExpPercent = static_cast<float>(FMath::FloorToInt(EBV_MainPlayer.ExtraCurrentExpPercent * 10000.0f)) / 100.0f;
			TB_ExpPercentMainChar->SetText(FText::FromString(FString::Printf(TEXT("%.2f%%"), CurrentMainExpPercent)));
		}
		
		//서브캐릭 Exp 퍼센트글자
		if (TB_ExpPercentSubChar.IsValid())
		{
			float CurrentSubExpPercent = static_cast<float>(FMath::FloorToInt(EBV_SubPlayer.ExtraCurrentExpPercent * 10000.0f)) / 100.0f;
			TB_ExpPercentSubChar->SetText(FText::FromString(FString::Printf(TEXT("%.2f%%"), CurrentSubExpPercent)));
		}
	}
}

bool UB2UIStageResult::UpdateExpPercentCommon(FExpProgressVars& ExpVarSet, float DeltaSeconds)
{
	if (ExpVarSet.bBasicExpIncreaseComplete == false)
	{
		ExpVarSet.bBasicExpIncreaseComplete =
			UpdateExpPercentCommonInternalFunc(ExpVarSet.BasicLevelToIncrease, ExpVarSet.CurrentIncreasedLevel, ExpVarSet.BasicTargetExpPercent, ExpVarSet.BasicCurrentExpPercent, ExpIncreaseSpeed, DeltaSeconds);

		// 아직 Basic 수치가 target 에 도달하지 못한 상태에서는 Extra 수치를 맞춰준다.
		ExpVarSet.ExtraLevelToIncrease = ExpVarSet.BasicLevelToIncrease + ExpVarSet.ExtraLevelToIncreaseFromBasic;
		ExpVarSet.ExtraCurrentExpPercent = ExpVarSet.BasicCurrentExpPercent;
	}
	else if (ExpVarSet.bExtraExpIncreaseComplete == false)
	{
		ExpVarSet.bExtraExpIncreaseComplete =
			UpdateExpPercentCommonInternalFunc(ExpVarSet.ExtraLevelToIncrease, ExpVarSet.CurrentIncreasedLevel, ExpVarSet.ExtraTargetExpPercent, ExpVarSet.ExtraCurrentExpPercent, ExpIncreaseSpeed, DeltaSeconds);
	}

	return ExpVarSet.bBasicExpIncreaseComplete && ExpVarSet.bExtraExpIncreaseComplete;
}


void UB2UIStageResult::UpdateMainCharLevel()
{
	if (TB_MainCharLevel.IsValid())
		TB_MainCharLevel->SetText(FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("StageClear_Level"))),
		FText::FromString(FString::FromInt(EBV_MainPlayer.CurrentIncreasedLevel))));
}

void UB2UIStageResult::UpdateSubCharLevel()
{
	if (TB_SubCharLevel.IsValid())
		TB_SubCharLevel->SetText(FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("StageClear_Level"))),
		FText::FromString(FString::FromInt(EBV_SubPlayer.CurrentIncreasedLevel))));
}


void UB2UIStageResult::SetupWidgetCustomTimer()
{
	// Pause 상태에서 쓸 것이므로 기본 타이머 못 쓰고 따로 세팅

	for (int32 CTI = 0; CTI < CustomTimedCallBackInfo.Num(); ++CTI)
	{
		if (CustomTimedCallBackInfo[CTI].InvokeTime > 0.0f)
		{
			CustomTimedCallBackInfo[CTI].bIsInvoked = false;
		}
		else
		{
			CustomTimedCallBackInfo[CTI].bIsInvoked = true; // 이미 발동한 걸로 간주.
		}
	}
}

void UB2UIStageResult::SetupWidgetCustomHideTimer()
{
	for (int32 CTI = 0; CTI < CustomTimedHideInfo.Num(); ++CTI)
	{
		FB2UIWidgetCustomTimedHideInfo& ThisHideInfo = CustomTimedHideInfo[CTI];

		UWidget* WidgetToHide = GetWidgetFromName(ThisHideInfo.WidgetNameToHide);

		if (WidgetToHide == NULL)
		{
			continue;
		}

		ThisHideInfo.OriginalVisibility = WidgetToHide->GetVisibility(); // Hide 할 때에도 백업해 놓지만 아래 로직에 따라 HideStop 이 먼저 될 수도 있으니 여기서도 한번.

		// HideStopTime 이 HideStartTime 보다 큰지에 대한 확인은.. 굳이 하지 않는다.

		if (ThisHideInfo.HideStartTime <= 0.0f)
		{
			CustomTimeHideStartCallback(CTI); // 바로 콜백 실행
		}

		if (ThisHideInfo.HideStopTime <= 0.0f)
		{
			CustomTimeHideStopCallback(CTI); // 바로 콜백 실행. 결국 HideStopTime 이 0 이하면 안 숨겨짐
		}
	}
}

void UB2UIStageResult::UpdateWidgetCustomTimer()
{
	UWorld* TheWorld = GetWorld();
	if (TheWorld == NULL)
	{
		return;
	}

	// bIsSuccessiveLevelUpOpen 이라면 한번에 죄다 처리되도록 큰 값으로 세팅. 설마 이걸 넘어가는 일은 없겠지 ㅋ
	const float TimeSinceStart = bIsSuccessiveLevelUpOpen ? 1000000.0f : (TheWorld->GetRealTimeSeconds() - CachedWidgetStartTime);
	// Pause 상태에서도 직접 돌려쓰는 타이머
	for (int32 CTI = 0; CTI < CustomTimedCallBackInfo.Num(); ++CTI)
	{
		FB2UIWidgetCustomTimedCallbackInfo& ThisInfo = CustomTimedCallBackInfo[CTI];
		if (ThisInfo.bIsInvoked == true)
		{
			continue;
		}

		if (ThisInfo.InvokeTime <= TimeSinceStart)
		{
			OriginalCustomTimeCallback(CTI);
			ThisInfo.bIsInvoked = true;
		}
	}

	for (int32 CTI = 0; CTI < CustomTimedHideInfo.Num(); ++CTI)
	{
		FB2UIWidgetCustomTimedHideInfo& ThisHideInfo = CustomTimedHideInfo[CTI];
		
		if (bIsSuccessiveLevelUpOpen)
		{ // SuccessiveOpen 이면 바로 보이도록 처리.
			ThisHideInfo.bIsHidden = true;
			ThisHideInfo.bHiddenDoneOnce = true;
		}

		if (ThisHideInfo.bIsHidden == false)
		{
			// 한번만 HideStart 가 되게. 안 그러면 깜박일 것.
			if (ThisHideInfo.bHiddenDoneOnce == false && ThisHideInfo.HideStartTime <= TimeSinceStart)
			{
				CustomTimeHideStartCallback(CTI);
			}
		}
		else
		{
			if (ThisHideInfo.HideStopTime <= TimeSinceStart)
			{
				CustomTimeHideStopCallback(CTI);
			}
		}
	}
}

void UB2UIStageResult::StartRepeatBattleCountdown( bool CountdownVisibility )
{
	if ( UIP_RepeatBattleCountDownP.IsValid() )
	{
		UIP_RepeatBattleCountDownP->BattleCountDownSetVisibility( CountdownVisibility ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed );
		UIP_RepeatBattleCountDownP->StartCountdown( FRepeatBattleCountDownCB::CreateLambda( [ this ]()
		{
			EndPlayOfSingleRepeatBattleStageClass<>::GetInstance().Signal();
		}));
	}
}
void UB2UIStageResult::StopRepeatBattleCountdown()
{
	if ( UIP_RepeatBattleCountDownP.IsValid() )
	{
		UIP_RepeatBattleCountDownP->BattleCountDownSetVisibility( ESlateVisibility::Collapsed );
		UIP_RepeatBattleCountDownP->StopCountdown();
	}
}

void UB2UIStageResult::SetupOnRepeatBattleStateChange( bool bIsOn, bool CountdownVisibility )
{
	if ( bIsOn )
	{
		StartRepeatBattleCountdown( CountdownVisibility );
	}
	else
	{
		StopRepeatBattleCountdown();
		SetButtonClickable( true );
		CheckFinalStageDisableButton();
	}
	
	// 반복전투 상태였다가 해제되었다고 해서 레벨업 팝업을 굳이 열 필요는 없을 듯.
}

bool UB2UIStageResult::IsRepeatBattleOn()
{
	UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();
	return DocBS ? DocBS->GetRepeatBattleCurrentOn() : false;
}

void UB2UIStageResult::SetLevelUpTextVisible(bool bForMain)
{
	if (bForMain)
	{
		if (IMG_Main_LevelUp.IsValid())
		{
			IMG_Main_LevelUp->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		if (IMG_Tag_LevelUp.IsValid())
		{
			IMG_Tag_LevelUp->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UB2UIStageResult::CheckAndOpenLevelUpPart()
{
	if (IsRepeatBattleOn()){
		return; // 반복전투 도중에는 레벨업 팝업이 열리지 않게 함.
	}

	FLocalCharacterData& CharacterDataStore = BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData();

	EPCClass PCClassMain = CharacterDataStore.GetMainPlayerClass();
	EPCClass PCClassSub = CharacterDataStore.GetSubPlayerClass();

	bool bMainCharLevelUp = false;
	{
		//bMainCharLevelUp = (FMath::Max(DocHeroMain->GetBasicUpgradeLevel(), DocHeroMain->GetExtraUpgradeLevel()) > DocHeroMain->GetCurrentLevel());
		const int32 ClearAfterLevel = CharacterDataStore.GetCharacterLevel(PCClassMain);
		const int32 ClearBeforeLevel = CharacterDataStore.GetBeforeCharacterLevel(PCClassMain);
		bMainCharLevelUp = (ClearAfterLevel > ClearBeforeLevel);
	}

	bool bSubCharLevelUp = false;
	{
		//bSubCharLevelUp = (FMath::Max(DocHeroSub->GetBasicUpgradeLevel(), DocHeroSub->GetExtraUpgradeLevel()) > DocHeroSub->GetCurrentLevel());
		const int32 ClearAfterLevel = CharacterDataStore.GetCharacterLevel(PCClassSub);
		const int32 ClearBeforeLevel = CharacterDataStore.GetBeforeCharacterLevel(PCClassSub);
		bSubCharLevelUp = (ClearAfterLevel > ClearBeforeLevel);
	}

	FServerStageID StageId = AB2StageManager::GetCacheStageKeepEssentialData().GetServerStageID();

	if (ShouldShowCBTVersionContent()
		&& !BladeIIGameImpl::GetStageDataStore().LoadCBTClear()
		&& StageId.Id == 3030)				//CBT용으로만 마지막 악몽3챕터 3스테이지만 검사
	{
		UB2UICBTClear* CBTUI = UB2UIManager::GetInstance()->OpenUI<UB2UICBTClear>(UIFName::CBTClear);
		if (CBTUI)
		{
			CBTUI->SetLevelUpState(bMainCharLevelUp, bSubCharLevelUp);
			CBTUI->DelegateCBTFinish = FOnCBTClearFinish::CreateLambda([this](bool bMain, bool bSub) { this->StageFxResult(bMain, bSub); });
		}

		BladeIIGameImpl::GetStageDataStore().SaveCBTClear(true);

	}
	else
	{
		StageFxResult(bMainCharLevelUp, bSubCharLevelUp);
	}
	/*if (bMainCharLevelUp || bSubCharLevelUp)
	{
		SetIsSuccessiveLevelUpOpen(true);
		
		
		// LevelUp Widget이 필요하다면 Animation이 종료 후 Quest관련 Logic을 호출 할 OnLevelUpFXFinish 함수를 Delegate로 등록
		if (UIP_LevelUp.IsValid())
			UIP_LevelUp->LevelupFinishDelegate = FOnLevelUpFXFinish::CreateLambda([this](){ this->OnLevelUpFXFinish(); });
	}
	else // LevelUp Widget 필요 없다면 바로 Finish호출하여 이후 상황 보여줌
	{
		OnLevelUpFXFinish();
	}*/
}
void UB2UIStageResult::StageFxResult(bool bMain, bool bSub)
{
	if (bMain || bSub)
	{
		SetIsSuccessiveLevelUpOpen(true);


		// LevelUp Widget이 필요하다면 Animation이 종료 후 Quest관련 Logic을 호출 할 OnLevelUpFXFinish 함수를 Delegate로 등록
		if (UIP_LevelUp.IsValid())
			UIP_LevelUp->LevelupFinishDelegate = FOnLevelUpFXFinish::CreateLambda([this]() { this->OnLevelUpFXFinish(); });
	}
	else // LevelUp Widget 필요 없다면 바로 Finish호출하여 이후 상황 보여줌
	{
		OnLevelUpFXFinish();
	}
}
void UB2UIStageResult::SetIsSuccessiveLevelUpOpen(bool bIsSuccessive)
{
	// 원래는 LevelUp 연출 자체는 Clear 연출과 별도면서 LevelUp UI 는 StageResultUI 의 일부로서 작동하도록 만들었을 때 만들었던 거라
	// StageResult UI 오픈과 함께 오프닝 애니메이션을 모두 스킵하고 LevelUp 파트를 열도록 기능이 짜여져 있음.
	// StageResult UI 의 다른 부분이 자연스럽게 끝난 후 LevelUp 파트를 여는 현 방식에서는 그런 코드들이 굳이 없어도 되긴 함

	bIsSuccessiveLevelUpOpen = bIsSuccessive;
	if (bIsSuccessiveLevelUpOpen)
	{ // bIsSuccessiveLevelUpOpen 이 세팅되어 있다면 UpdateWidgetCustomTimer 에서 죄다 돌려버릴 것.
		UpdateWidgetCustomTimer();

		// 경험치랑 레벨도 만땅으로 바로.

		int32 MainPlayerTotalLevelUp = EBV_MainPlayer.BasicLevelToIncrease + EBV_MainPlayer.ExtraLevelToIncreaseFromBasic;
		if (MainPlayerTotalLevelUp > 0){
			SetLevelUpTextVisible(true);
		}
		EBV_MainPlayer.CurrentIncreasedLevel += MainPlayerTotalLevelUp;
		EBV_MainPlayer.BasicLevelToIncrease = EBV_MainPlayer.ExtraLevelToIncrease = EBV_MainPlayer.ExtraLevelToIncreaseFromBasic = 0;
		EBV_MainPlayer.BasicCurrentExpPercent = EBV_MainPlayer.BasicTargetExpPercent;
		EBV_MainPlayer.ExtraCurrentExpPercent = EBV_MainPlayer.ExtraTargetExpPercent;
		EBV_MainPlayer.bBasicExpIncreaseComplete = EBV_MainPlayer.bExtraExpIncreaseComplete = true;

		int32 SubPlayerTotalLevelUp = EBV_SubPlayer.BasicLevelToIncrease + EBV_SubPlayer.ExtraLevelToIncreaseFromBasic;
		if (SubPlayerTotalLevelUp > 0){
			SetLevelUpTextVisible(false);
		}
		EBV_SubPlayer.CurrentIncreasedLevel += SubPlayerTotalLevelUp;
		EBV_SubPlayer.BasicLevelToIncrease = EBV_SubPlayer.ExtraLevelToIncrease = EBV_SubPlayer.ExtraLevelToIncreaseFromBasic = 0;
		EBV_SubPlayer.BasicCurrentExpPercent = EBV_SubPlayer.BasicTargetExpPercent;
		EBV_SubPlayer.ExtraCurrentExpPercent = EBV_SubPlayer.ExtraTargetExpPercent;
		EBV_SubPlayer.bBasicExpIncreaseComplete = EBV_SubPlayer.bExtraExpIncreaseComplete = true;
		UpdateExpPercent(0.0f);

		if (UIP_LevelUp.IsValid())
		{
			TArray<EPCClass> ThisPcClass;
			ThisPcClass.Add(BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass());
			ThisPcClass.Add(BladeIIGameImpl::GetLocalCharacterData().GetSubPlayerClass());

			UIP_LevelUp->StartMe(ThisPcClass);
		}
	}
}

//====================================================================================
//									Click Handler
//====================================================================================
void UB2UIStageResult::OnClickBtnVillage()
{
	if (bButtonClicked)
		return;
	
	if (IsRepeatBattleOn())
	{
		PopUpRepeatBattleStopMsg(GetOwningPlayer()); // 반복 전투 상태에서는 중단 버튼의 역할이 됨.
	}
	else
	{
		bButtonClicked = true;
		if (auto* DocChapter = UB2UIDocHelper::GetDocUICondition())
		{
			DocChapter->SetIsCombineMenu(false);
		}
		GoToVillageClass<>::GetInstance().Signal();
	}
}

void UB2UIStageResult::OnClickBtnAreaSelection()
{
	if (bButtonClicked)
		return;

	if (IsRepeatBattleOn())
	{
		PopUpRepeatBattleStopMsg(GetOwningPlayer()); // 반복 전투 상태에서는 중단 버튼의 역할이 됨.
	}
	else
	{
		bButtonClicked = true;
		GoToMapClass<>::GetInstance().Signal();
	}
}

void UB2UIStageResult::OnClickBtnRestart()
{
	if (bButtonClicked)
		return;

	if (auto* DocBattleStage = UB2UIDocHelper::GetDocBattleStage())
	{
		if (DocBattleStage->GetRepeatBattleBoostRemainCount() < 1 && DocBattleStage->GetRepeatBattleBoostOn())
		{
			UB2UIManager::GetInstance()->OpenMsgPopup(
				EUIMsgPopup::Simple,
				BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_NoticeTab")),
				BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("AdventureMod_TicketShortage")),
				0.f,
				true,
				true,
				EUIMsgPopupButtonGroup::YesOrNo,
				FMsgPopupOnClick::CreateLambda([this]() {UB2UIStageResult::OnClickBtnRestart();})
			);

			AB2StageManager::GetCacheStageKeepEssentialData().SetRepeatBattleStateBoostOn(false);
			DocBattleStage->SetRepeatBattleBoostOn(false);
			return;
		}
	}

	if (IsRepeatBattleOn())
	{
		PopUpRepeatBattleStopMsg(GetOwningPlayer()); // 반복 전투 상태에서는 중단 버튼의 역할이 됨.
	}
	else
	{
		bButtonClicked = true;
		RestartGameClass<>::GetInstance().Signal();
	}

}

void UB2UIStageResult::OnClickBtnNextArea()
{
	if (bButtonClicked)
		return;

	if (IsRepeatBattleOn())
	{
		PopUpRepeatBattleStopMsg(GetOwningPlayer()); // 반복 전투 상태에서는 중단 버튼의 역할이 됨.
	}
	else
	{
		bButtonClicked = true;
		GoToDeferredStageClass<int32>::GetInstance().Signal(INDEX_NONE);	// 특정 StageNum을 입력하지 않으면 다음 Stage로
	}
}

int32 UB2UIStageResult::LoadClearChapterNum()
{
	int32 RetValue = 0;
	GConfig->GetInt(TEXT("ChapterState"), *FString::Printf(TEXT("ClearChapterNum")), RetValue, GB2UserSavedStateIni);
	return RetValue;
}

//====================================================================================
//									Doc Delegate
//====================================================================================

void UB2UIStageResult::OnChangedRepeatBattleCurrentOn(class UB2UIDocBase* Sender, bool bCurrentOn, bool bPrevOn)
{
	if (bCurrentOn != bPrevOn)
	{
		SetupOnRepeatBattleStateChange(bCurrentOn);
	}
}


