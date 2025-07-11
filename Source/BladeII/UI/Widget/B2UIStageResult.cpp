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
	//����ġ ����
	if (GetWorld())
	{
		// �̰� World pause ���¿��� ����ϱ� ���� ���̹Ƿ� GetRealTimeSeconds �� �ؾ� �ϱ�. �Ʒ� UpdateExpPercent �θ��� ���� �̰� ���� ����.
		CachedWidgetStartTime = GetWorld()->GetRealTimeSeconds();
	}

	//����Ʋ���� ĳ���� �����͸� �����ͼ� ó��
	auto DocBattle = UB2UIDocHelper::GetDocBattle();
	SetDoc(DocBattle);
	if (DocBattle)
	{
		// ���� main ���� ������ ĳ���Ͱ� �ƴ� stage clear ������ ���� ĳ���͸� ������.
		// Exp bar ���� �ִϸ��̼ǿ� �ʿ��� ������ ����. Main/Sub �������
		FLocalCharacterData& CharacterDataStore = BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData();

		auto DocHeroMain = UB2UIDocHelper::GetDocHero(PCClassToInt(CharacterDataStore.GetMainPlayerClass()));
		if (DocHeroMain)
		{
			BindDocMainChar(DocHeroMain); 
			InitExpVarSet(EBV_MainPlayer, CharacterDataStore.GetMainPlayerClass());
		}

		// ���� sub �� ������ ĳ���Ͱ� �ƴ� stage clear ������ �±� ĳ���͸� ������.
		auto DocHeroSub = UB2UIDocHelper::GetDocHero(PCClassToInt(CharacterDataStore.GetSubPlayerClass()));
		if (DocHeroSub)
		{
			BindDocSubChar(DocHeroSub);
			InitExpVarSet(EBV_SubPlayer, CharacterDataStore.GetSubPlayerClass());
		}

		UpdateExpPercent(0.f);
	}
	
	//����Ʋ������������ Ŭ����Ÿ���� ó��
	auto DocBattleStage = UB2UIDocHelper::GetDocBattleStage();
	const int32 MultipleValue = DocBattleStage->GetStageClearMultipleValue();

	if (DocBattleStage)
	{
		SetStageClearGrade(DocBattleStage->GetStageClearConditions(), DocBattleStage->GetServerStageId());
		SetClearTime(DocBattleStage->GetStagePlayTime());
		// �����ִ� �������
		SetDropItem(DocBattleStage->GetStageResultBossDropItemsList());
		SetDropItem(DocBattleStage->GetStageClearEtherItemsList(), MultipleValue);
		SetDropItem(DocBattleStage->GetStageClearMaterialItemsList(), MultipleValue);
		SetDropItem(DocBattleStage->GetStageClearRewardItemList(), MultipleValue);

		SetAcquiredGold(DocBattleStage->GetStageTotalGold()); // �������� ���â ���ǥ��
		SetSoldGold(DocBattleStage->GetStageSoldGold());

		SetHotTimeBuff(DocBattleStage->GetEndGoldBuffRate(), DocBattleStage->GetEndExpBuffRate());

		DocBattleStage->OnRepeatBattleCurrentOnChanged.AddUObject(this, &UB2UIStageResult::OnChangedRepeatBattleCurrentOn);
	}

	if (ExpIncreaseStartTime > 0.0f)
	{
		bExpIncreaseStarted = false; // �̰͵� ���������� Ÿ�̸Ӱ� ���� �����Ƿ� CachedWidgetStartTime ����ؼ� ���� ī��Ʈ�� ��.
	}
	else
	{
		bExpIncreaseStarted = true; // �ٷ� ����.
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
	{ // �� ��Ʈ�� ���� ������ ���Ǵµ� �������� �������� ���� ���� UnbindDoc �� �Ѵ�.
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

		if (Grade[Index]) // �������� ���� �����Ѵٸ�
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
		// ������ ���� ���� ���ļ� ���������� ���� �и��ؼ� �����ֱ⶧����
		// ������(���)�� Multiple Value(= 1 or 3�� ���)�� ���, ��������, ���������� ConsumingAmount�� �����ش�
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
			// �� SlotItem ���� ��� �ִ� ScrollBox �� �Է��� ������ ���� �¾�
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

	UpdateWidgetCustomTimer(); // ����� DeltaTime �� �Ѱ��ִ� �� �ǹ̰� ���� ��. ��ü Ÿ�̸� ���. �׳��� NativeTick �� �Ҹ��� �� ����

	UpdateMainCharLevel();
	UpdateSubCharLevel();
}

void UB2UIStageResult::OnOpenComplete()
{
	SetOpenState(EUIOpenState::Opened);
	// ���� UIWidgetOpenCompleteClass �̺�Ʈ �ñ׳��� ���⼭ ������ �ʱ� ����.

	//�˾������� ��ư �ٽ� Ȱ��ȭ
	ToolTipCloseTicket = ToolTipCloseClass<>::GetInstance().Subscribe([this]() {
			this->SetButtonRevitalize();
	});

	bShowMainQuestReward = IsShowMainQuestReward();
	//�ϴ� ������ ��ư�� ���� ����ġ,����Ʈ ����� ����� ���Ŀ� Ǯ���ش�.
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
	// �ִ� �������� ���� �� �� �� ���� ����
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

// ������, ��ȭ, ����Ʈ ��� ��� Ŭ���� ���� ���� ��������� ������ ���������� ȣ��� �Լ�
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
		//if (bShowMainQuestReward)	// �ݺ����� ���̶� MainQuest�� �켱�Ѵ�.
		//{
		//	CancelOrStopRepeatBattleClass<>::GetInstance().Signal(); // �ݺ����� ���
		//	CheckAndOpenLevelUpPart();
		//}
		//else
		UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();
		if ( DocBS )
		{
			// [��������] �� ���������� Ʃ�丮���� ������ ���, �������� ����
			if ( DocBS->GetRepeatBattleLoopAll() && TutorialManager::GetInstance().HasContentOpenTutorial() )
			{
				CancelOrStopRepeatBattleClass<>::GetInstance().Signal();
				OnAllProcessEnd();
				return;
			}
		
			// [�ݺ�����][��������] �ϰ��, �κ�����(ĳ���� ����) ���� �����ϹǷ�, ī��Ʈ�ٿ� ���÷��̸� Ȱ��ȭ �Ѵ�
			SetupOnRepeatBattleStateChange( true, true );
		}
	}
	else
	{
		CheckAndOpenLevelUpPart(); // ��� UI �������� ���� �� �������� ������ UI �� üũ
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
	//[@SameLine, 180612] explevelinfo doc, datastore -> datastore ����

	FLocalCharacterData& CharacterDataStore = BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData();

	const int32 StageClear_AfterLevel = CharacterDataStore.GetCharacterLevel(PCClass);
	const float StageClear_AfterExpPercent = CharacterDataStore.GetCharacterExpPercent(PCClass);

	const int32 StageClear_BeforeLevel = CharacterDataStore.GetBeforeCharacterLevel(PCClass);
	const float StageClear_BeforeExpPercent = CharacterDataStore.GetBeforeCharacterExpPercent(PCClass);

	ExpVarSet.BasicLevelToIncrease = StageClear_AfterLevel - StageClear_BeforeLevel;
	ExpVarSet.BasicTargetExpPercent = StageClear_AfterExpPercent * 0.01f; // UI ������ 0.0 ~ 1.0 ������.
	ExpVarSet.BasicCurrentExpPercent = StageClear_BeforeExpPercent * 0.01f; // �̰� ���� ExpIncreaseSpeed �� ���� ������ ��.

	ExpVarSet.bBasicExpIncreaseComplete = false;

	ExpVarSet.ExtraLevelToIncrease = StageClear_AfterLevel - StageClear_BeforeLevel;
	ExpVarSet.ExtraLevelToIncreaseFromBasic = ExpVarSet.ExtraLevelToIncrease - ExpVarSet.BasicLevelToIncrease;

	ExpVarSet.ExtraTargetExpPercent = StageClear_AfterExpPercent * 0.01f;
	ExpVarSet.ExtraCurrentExpPercent = StageClear_AfterExpPercent * 0.01f; // �ʱⰪ�� Basic �̳� Extra �� ��������. BasicCurrent �� ���� �����ϴٰ� �̰� �� �� ������ �� ����.

	ExpVarSet.bExtraExpIncreaseComplete = false;

	ExpVarSet.CurrentIncreasedLevel = StageClear_BeforeLevel; // �̰͵� ���� PostClearLevel �� ������ ��.
}

/** Most internal functionality for use in UpdateExpPercent. Returns true when current exp is reached to target value. */
bool UpdateExpPercentCommonInternalFunc(int32& InOutLevelToIncrease, int32& InOutCurrentLevel, float InTargetExpPercent, float& InOutCurrentExpPercent, float ExpIncreaseRate, float DeltaSeconds)
{
	if (InOutLevelToIncrease > 0 || InOutCurrentExpPercent < InTargetExpPercent)
	{
		// ���� S = Vt
		InOutCurrentExpPercent += FMath::Min(ExpIncreaseRate * DeltaSeconds, 0.99f); // �� �����ӿ� 1.0 �̻� �����ϸ� ���

		if (InOutLevelToIncrease > 0)
		{
			if (InOutCurrentExpPercent >= 1.0f)
			{
				// ���� ������ �Ѿ��.
				--InOutLevelToIncrease;
				++InOutCurrentLevel;
				InOutCurrentExpPercent -= 1.0f;
			}
		}

		if (InOutLevelToIncrease == 0 && InOutCurrentExpPercent >= InTargetExpPercent)
		{
			// ��ǥ �������� ä���ٸ� ExpPercent ������ ���߸� ��.
			InOutCurrentExpPercent = InTargetExpPercent;
			return true;
		}

		return false;
	}

	return true;
}

void UB2UIStageResult::UpdateExpPercent(float DeltaSeconds)
{
	// Pause ���¿��� ����ϴ� �Ŷ� Ÿ�̸ӷ� ���ϰ� ���� ī��Ʈ
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
		const int32 MainLevelBefore = EBV_MainPlayer.CurrentIncreasedLevel; // �̹� ������Ʈ���� ������ �����ߴ��� üũ�� ����.
		const int32 SubLevelBefore = EBV_SubPlayer.CurrentIncreasedLevel;

		const bool bMainFinished = UpdateExpPercentCommon(EBV_MainPlayer, DeltaSeconds);
		const bool bSubFinished = UpdateExpPercentCommon(EBV_SubPlayer, DeltaSeconds);
				
		const bool bMainLevelIncreased = (EBV_MainPlayer.CurrentIncreasedLevel > MainLevelBefore);
		const bool bSubLevelIncreased = (EBV_SubPlayer.CurrentIncreasedLevel > SubLevelBefore);

		if (bMainLevelIncreased) // ������ �Ѿ ������ �ؽ�Ʈ �ٲ���.
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
			// ��ǻ� ���Ⱑ opening ȿ���� ������ ������ �� ���̴�. OnOpenComplete ��� ���⼭ �ʿ��� event �� ��������..
			OnActualOpenComplete();
		}

		//����ĳ�� ������ Exp
		if (BasicExpBar_MainPlayer.IsValid())
		{
			float Percent = (EBV_MainPlayer.bBasicExpIncreaseComplete && EBV_MainPlayer.ExtraCurrentExpPercent < EBV_MainPlayer.BasicCurrentExpPercent) ? 0.0f : EBV_MainPlayer.BasicCurrentExpPercent;
			BasicExpBar_MainPlayer->SetPercent(Percent);
		}

		//����ĳ�� ����Ʈ�� Exp
		if (ExtraExpBar_MainPlayer.IsValid())
		{
			BasicExpBar_MainPlayer->SetPercent(EBV_MainPlayer.ExtraCurrentExpPercent);
		}

		//����ĳ�� ������ Exp
		if (BasicExpBar_SubPlayer.IsValid())
		{
			float Percent = (EBV_SubPlayer.bBasicExpIncreaseComplete && EBV_SubPlayer.ExtraCurrentExpPercent < EBV_SubPlayer.BasicCurrentExpPercent) ? 0.0f : EBV_SubPlayer.BasicCurrentExpPercent;
			BasicExpBar_SubPlayer->SetPercent(Percent);
		}

		//����ĳ�� ����Ʈ�� Exp
		if (ExtraExpBar_SubPlayer.IsValid())
		{
			ExtraExpBar_SubPlayer->SetPercent(EBV_SubPlayer.ExtraCurrentExpPercent);
		}

		//����ĳ�� Exp �ۼ�Ʈ����
		if (TB_ExpPercentMainChar.IsValid())
		{
			float CurrentMainExpPercent = static_cast<float>(FMath::FloorToInt(EBV_MainPlayer.ExtraCurrentExpPercent * 10000.0f)) / 100.0f;
			TB_ExpPercentMainChar->SetText(FText::FromString(FString::Printf(TEXT("%.2f%%"), CurrentMainExpPercent)));
		}
		
		//����ĳ�� Exp �ۼ�Ʈ����
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

		// ���� Basic ��ġ�� target �� �������� ���� ���¿����� Extra ��ġ�� �����ش�.
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
	// Pause ���¿��� �� ���̹Ƿ� �⺻ Ÿ�̸� �� ���� ���� ����

	for (int32 CTI = 0; CTI < CustomTimedCallBackInfo.Num(); ++CTI)
	{
		if (CustomTimedCallBackInfo[CTI].InvokeTime > 0.0f)
		{
			CustomTimedCallBackInfo[CTI].bIsInvoked = false;
		}
		else
		{
			CustomTimedCallBackInfo[CTI].bIsInvoked = true; // �̹� �ߵ��� �ɷ� ����.
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

		ThisHideInfo.OriginalVisibility = WidgetToHide->GetVisibility(); // Hide �� ������ ����� ������ �Ʒ� ������ ���� HideStop �� ���� �� ���� ������ ���⼭�� �ѹ�.

		// HideStopTime �� HideStartTime ���� ū���� ���� Ȯ����.. ���� ���� �ʴ´�.

		if (ThisHideInfo.HideStartTime <= 0.0f)
		{
			CustomTimeHideStartCallback(CTI); // �ٷ� �ݹ� ����
		}

		if (ThisHideInfo.HideStopTime <= 0.0f)
		{
			CustomTimeHideStopCallback(CTI); // �ٷ� �ݹ� ����. �ᱹ HideStopTime �� 0 ���ϸ� �� ������
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

	// bIsSuccessiveLevelUpOpen �̶�� �ѹ��� �˴� ó���ǵ��� ū ������ ����. ���� �̰� �Ѿ�� ���� ������ ��
	const float TimeSinceStart = bIsSuccessiveLevelUpOpen ? 1000000.0f : (TheWorld->GetRealTimeSeconds() - CachedWidgetStartTime);
	// Pause ���¿����� ���� �������� Ÿ�̸�
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
		{ // SuccessiveOpen �̸� �ٷ� ���̵��� ó��.
			ThisHideInfo.bIsHidden = true;
			ThisHideInfo.bHiddenDoneOnce = true;
		}

		if (ThisHideInfo.bIsHidden == false)
		{
			// �ѹ��� HideStart �� �ǰ�. �� �׷��� ������ ��.
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
	
	// �ݺ����� ���¿��ٰ� �����Ǿ��ٰ� �ؼ� ������ �˾��� ���� �� �ʿ�� ���� ��.
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
		return; // �ݺ����� ���߿��� ������ �˾��� ������ �ʰ� ��.
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
		&& StageId.Id == 3030)				//CBT�����θ� ������ �Ǹ�3é�� 3���������� �˻�
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
		
		
		// LevelUp Widget�� �ʿ��ϴٸ� Animation�� ���� �� Quest���� Logic�� ȣ�� �� OnLevelUpFXFinish �Լ��� Delegate�� ���
		if (UIP_LevelUp.IsValid())
			UIP_LevelUp->LevelupFinishDelegate = FOnLevelUpFXFinish::CreateLambda([this](){ this->OnLevelUpFXFinish(); });
	}
	else // LevelUp Widget �ʿ� ���ٸ� �ٷ� Finishȣ���Ͽ� ���� ��Ȳ ������
	{
		OnLevelUpFXFinish();
	}*/
}
void UB2UIStageResult::StageFxResult(bool bMain, bool bSub)
{
	if (bMain || bSub)
	{
		SetIsSuccessiveLevelUpOpen(true);


		// LevelUp Widget�� �ʿ��ϴٸ� Animation�� ���� �� Quest���� Logic�� ȣ�� �� OnLevelUpFXFinish �Լ��� Delegate�� ���
		if (UIP_LevelUp.IsValid())
			UIP_LevelUp->LevelupFinishDelegate = FOnLevelUpFXFinish::CreateLambda([this]() { this->OnLevelUpFXFinish(); });
	}
	else // LevelUp Widget �ʿ� ���ٸ� �ٷ� Finishȣ���Ͽ� ���� ��Ȳ ������
	{
		OnLevelUpFXFinish();
	}
}
void UB2UIStageResult::SetIsSuccessiveLevelUpOpen(bool bIsSuccessive)
{
	// ������ LevelUp ���� ��ü�� Clear ����� �����鼭 LevelUp UI �� StageResultUI �� �Ϻημ� �۵��ϵ��� ������� �� ������� �Ŷ�
	// StageResult UI ���°� �Բ� ������ �ִϸ��̼��� ��� ��ŵ�ϰ� LevelUp ��Ʈ�� ������ ����� ¥���� ����.
	// StageResult UI �� �ٸ� �κ��� �ڿ������� ���� �� LevelUp ��Ʈ�� ���� �� ��Ŀ����� �׷� �ڵ���� ���� ��� �Ǳ� ��

	bIsSuccessiveLevelUpOpen = bIsSuccessive;
	if (bIsSuccessiveLevelUpOpen)
	{ // bIsSuccessiveLevelUpOpen �� ���õǾ� �ִٸ� UpdateWidgetCustomTimer ���� �˴� �������� ��.
		UpdateWidgetCustomTimer();

		// ����ġ�� ������ �������� �ٷ�.

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
		PopUpRepeatBattleStopMsg(GetOwningPlayer()); // �ݺ� ���� ���¿����� �ߴ� ��ư�� ������ ��.
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
		PopUpRepeatBattleStopMsg(GetOwningPlayer()); // �ݺ� ���� ���¿����� �ߴ� ��ư�� ������ ��.
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
		PopUpRepeatBattleStopMsg(GetOwningPlayer()); // �ݺ� ���� ���¿����� �ߴ� ��ư�� ������ ��.
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
		PopUpRepeatBattleStopMsg(GetOwningPlayer()); // �ݺ� ���� ���¿����� �ߴ� ��ư�� ������ ��.
	}
	else
	{
		bButtonClicked = true;
		GoToDeferredStageClass<int32>::GetInstance().Signal(INDEX_NONE);	// Ư�� StageNum�� �Է����� ������ ���� Stage��
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


