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
	//版氰摹 楷免
	if (GetWorld())
	{
		// 捞霸 World pause 惑怕俊辑 荤侩窍扁 困茄 巴捞骨肺 GetRealTimeSeconds 甫 秦具 窍贬. 酒贰 UpdateExpPercent 何福扁 傈俊 捞芭 刚历 技泼.
		CachedWidgetStartTime = GetWorld()->GetRealTimeSeconds();
	}

	//刀硅撇俊辑 某腐磐 单捞磐甫 啊廉客辑 贸府
	auto DocBattle = UB2UIDocHelper::GetDocBattle();
	SetDoc(DocBattle);
	if (DocBattle)
	{
		// 角力 main 栏肺 急琶茄 某腐磐啊 酒囱 stage clear 矫痢狼 泅犁 某腐磐甫 啊廉咳.
		// Exp bar 刘啊 局聪皋捞记俊 鞘夸茄 函荐甸 技泼. Main/Sub 鉴辑措肺
		FLocalCharacterData& CharacterDataStore = BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData();

		auto DocHeroMain = UB2UIDocHelper::GetDocHero(PCClassToInt(CharacterDataStore.GetMainPlayerClass()));
		if (DocHeroMain)
		{
			BindDocMainChar(DocHeroMain); 
			InitExpVarSet(EBV_MainPlayer, CharacterDataStore.GetMainPlayerClass());
		}

		// 角力 sub 肺 急琶茄 某腐磐啊 酒囱 stage clear 矫痢狼 怕弊 某腐磐甫 啊廉咳.
		auto DocHeroSub = UB2UIDocHelper::GetDocHero(PCClassToInt(CharacterDataStore.GetSubPlayerClass()));
		if (DocHeroSub)
		{
			BindDocSubChar(DocHeroSub);
			InitExpVarSet(EBV_SubPlayer, CharacterDataStore.GetSubPlayerClass());
		}

		UpdateExpPercent(0.f);
	}
	
	//刀硅撇胶抛捞瘤俊辑 努府绢鸥烙阑 贸府
	auto DocBattleStage = UB2UIDocHelper::GetDocBattleStage();
	const int32 MultipleValue = DocBattleStage->GetStageClearMultipleValue();

	if (DocBattleStage)
	{
		SetStageClearGrade(DocBattleStage->GetStageClearConditions(), DocBattleStage->GetServerStageId());
		SetClearTime(DocBattleStage->GetStagePlayTime());
		// 焊咯林绰 鉴辑措肺
		SetDropItem(DocBattleStage->GetStageResultBossDropItemsList());
		SetDropItem(DocBattleStage->GetStageClearEtherItemsList(), MultipleValue);
		SetDropItem(DocBattleStage->GetStageClearMaterialItemsList(), MultipleValue);
		SetDropItem(DocBattleStage->GetStageClearRewardItemList(), MultipleValue);

		SetAcquiredGold(DocBattleStage->GetStageTotalGold()); // 胶抛捞瘤 搬苞芒 榜靛钎矫
		SetSoldGold(DocBattleStage->GetStageSoldGold());

		SetHotTimeBuff(DocBattleStage->GetEndGoldBuffRate(), DocBattleStage->GetEndExpBuffRate());

		DocBattleStage->OnRepeatBattleCurrentOnChanged.AddUObject(this, &UB2UIStageResult::OnChangedRepeatBattleCurrentOn);
	}

	if (ExpIncreaseStartTime > 0.0f)
	{
		bExpIncreaseStarted = false; // 捞巴档 付蛮啊瘤肺 鸥捞赣啊 冈瘤 臼栏骨肺 CachedWidgetStartTime 荤侩秦辑 蝶肺 墨款飘且 巴.
	}
	else
	{
		bExpIncreaseStarted = true; // 官肺 矫累.
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
	{ // 捞 颇飘啊 咯矾 镑俊辑 荤侩登绰单 胶抛捞瘤 付流阜俊 静老 锭父 UnbindDoc 阑 茄促.
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

		if (Grade[Index]) // 胶抛捞瘤 炼扒 面练茄促搁
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
	//if (DropItemData.Num() < 1) return;

	//RewardItemData.Empty();

	//for (const FB2Item& Elem : DropItemData)
	//{
	//	FB2Item RewardItem = Elem;
	//	// 抗傈浚 鞍篮 厘厚绰 般媚辑 焊咯玲瘤父 捞力 盒府秦辑 焊咯林扁锭巩俊
	//	// 酒捞袍(厘厚)绰 Multiple Value(= 1 or 3硅 葛靛)肺 榜靛, 焊胶炼阿, 犁丰酒捞袍篮 ConsumingAmount肺 焊咯霖促
	//	bool bIsConsuming = IsGoldItem(RewardItem) || IsBossPieceItem(RewardItem) || IsMaterialItem(RewardItem);
	//	RewardItem.ConsumingAmount = bIsConsuming ? FMath::Max(1, RewardItem.ConsumingAmount) : FMath::Max(1, MultipleValue);
	//	RewardItemData.Add(RewardItem);
	//}

	//for (int32 RI = 0; RI < RewardItemData.Num(); ++RI)
	//{
	//	UB2UISlotItem* NewRewardItem = CreateRewardItem();// = CreateWidget<UB2UISlotItem>(GetOwningPlayer(), SkillOptionListClass);
	//	if (NewRewardItem)
	//	{
	//		BattleRewardItems.Add(NewRewardItem);
	//		NewRewardItem->Init();
	//		IsEtherItem(RewardItemData[RI]) ? NewRewardItem->BindEther(RewardItemData[RI]) : NewRewardItem->BindDoc(RewardItemData[RI]);
	//		bool bInvisibleStar = IsGoldItem(RewardItemData[RI]) || IsBossPieceItem(RewardItemData[RI]) || IsMaterialItem(RewardItemData[RI]);
	//		NewRewardItem->SetVisibleStar(bInvisibleStar ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
	//		NewRewardItem->SetVisibleStageInfoItemIcon(ESlateVisibility::Collapsed, false);
	//		// 捞 SlotItem 甸阑 甸绊 乐绰 ScrollBox 俊 涝仿阑 焊郴扁 困茄 悸诀
	//		NewRewardItem->SetupManualScrollBoxSender_byReceiver(this, SB_RewardItems.Get());
	//	}
	//}
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

	UpdateWidgetCustomTimer(); // 咯扁肺 DeltaTime 阑 逞败林绰 扒 狼固啊 绝阑 淀. 磊眉 鸥捞赣 款侩. 弊唱付 NativeTick 捞 阂府绰 霸 促青

	UpdateMainCharLevel();
	UpdateSubCharLevel();
}

void UB2UIStageResult::OnOpenComplete()
{
	SetOpenState(EUIOpenState::Opened);
	// 窜瘤 UIWidgetOpenCompleteClass 捞亥飘 矫弊澄阑 咯扁辑 朝府瘤 臼扁 困秦.

	//扑诀摧阑锭 滚瓢 促矫 劝己拳
	ToolTipCloseTicket = ToolTipCloseClass<>::GetInstance().Subscribe([this]() {
			this->SetButtonRevitalize();
	});

	bShowMainQuestReward = IsShowMainQuestReward();
	//老窜 公炼扒 滚瓢篮 阜绊 版氰摹,涅胶飘 殿殿捞 柳青等 捞饶俊 钱绢霖促.
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
//	// 弥措 胶抛捞瘤 档崔 矫 歹 给 啊霸 阜澜
//	UB2StageInfo* StageInfoTable = StaticFindStageInfo();
//	int32 FinalStageNum = StageInfoTable ? StageInfoTable->GetFinalClientStageId() : 0;
//	int32 CurrStageID = AB2StageManager::GetCacheStageKeepEssentialData().GetCurrentClientStageId();
//
//	int32 MaxChapterNum = StageInfoTable ? StageInfoTable->GetStartAdventureModeChapterNum() : 0;
//
//
//	if (FinalStageNum == CurrStageID)
//	{
//		
//		if (BTN_NextArea.IsValid())
//			BTN_NextArea->SetIsEnabled(false);
//	}
//	else
//	{
//		if (FinalStageNum < CurrStageID &&
//			CurrStageID % 10 == 0)
//		{
//			if (BTN_NextArea.IsValid())
//				BTN_NextArea->SetIsEnabled(false);
//		}
//
//	}
//
//
//#if FORCE_LOGGING_IN_ANDROID
//	FPlatformMisc::LowLevelOutputDebugStringf(TEXT("=========B2UIStageResultButton========="));
//	FPlatformMisc::LowLevelOutputDebugStringf(TEXT("FinalStageNum %d"), FinalStageNum);
//	FPlatformMisc::LowLevelOutputDebugStringf(TEXT("CurrStageID %d"), CurrStageID);
//#endif

}


void UB2UIStageResult::OnCloseComplete()
{
	Super::OnCloseComplete();
	ToolTipCloseClass<>::GetInstance().Unsubscribe(ToolTipCloseTicket);
	bButtonClicked = false;
}

void UB2UIStageResult::OnLevelUpFXFinish()
{
	//if (bShowMainQuestReward)
	//{
	//	OpenQuestRewardDialog();
	//}
	//else
	//{
	//	OnAllProcessEnd();
	//}
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

// 饭骇诀, 措拳, 涅胶飘 殿殿 葛电 努府绢 包访 楷免 柳青荤亲捞 场唱绊 弥辆利栏肺 龋免瞪 窃荐
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
		//if (bShowMainQuestReward)	// 馆汗傈捧 吝捞扼档 MainQuest啊 快急茄促.
		//{
		//	CancelOrStopRepeatBattleClass<>::GetInstance().Signal(); // 馆汗傈捧 秒家
		//	CheckAndOpenLevelUpPart();
		//}
		//else
		UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();
		if ( DocBS )
		{
			// [楷加傈捧] 吝 牧刨明坷锹 譬配府倔捞 粮犁且 版快, 楷加傈捧 肛勉
			if ( DocBS->GetRepeatBattleLoopAll() && TutorialManager::GetInstance().HasContentOpenTutorial() )
			{
				CancelOrStopRepeatBattleClass<>::GetInstance().Signal();
				OnAllProcessEnd();
				return;
			}
		
			// [馆汗傈捧][楷加傈捧] 老版快, 肺厚柳涝(某腐磐 急琶) 绝捞 柳青窍骨肺, 墨款飘促款 叼胶敲饭捞甫 劝己拳 茄促
			SetupOnRepeatBattleStateChange( true, true );
		}
	}
	else
	{
		CheckAndOpenLevelUpPart(); // 搬苞 UI 坷橇醋捞 场抄 捞 矫痢俊辑 饭骇诀 UI 甫 眉农
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
	//[@SameLine, 180612] explevelinfo doc, datastore -> datastore 俺急

	FLocalCharacterData& CharacterDataStore = BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData();

	const int32 StageClear_AfterLevel = CharacterDataStore.GetCharacterLevel(PCClass);
	const float StageClear_AfterExpPercent = CharacterDataStore.GetCharacterExpPercent(PCClass);

	const int32 StageClear_BeforeLevel = CharacterDataStore.GetBeforeCharacterLevel(PCClass);
	const float StageClear_BeforeExpPercent = CharacterDataStore.GetBeforeCharacterExpPercent(PCClass);

	ExpVarSet.BasicLevelToIncrease = StageClear_AfterLevel - StageClear_BeforeLevel;
	ExpVarSet.BasicTargetExpPercent = StageClear_AfterExpPercent * 0.01f; // UI 俊辑绰 0.0 ~ 1.0 胶纳老.
	ExpVarSet.BasicCurrentExpPercent = StageClear_BeforeExpPercent * 0.01f; // 捞扒 痢瞒 ExpIncreaseSpeed 俊 狼秦 刘啊且 巴.

	ExpVarSet.bBasicExpIncreaseComplete = false;

	ExpVarSet.ExtraLevelToIncrease = StageClear_AfterLevel - StageClear_BeforeLevel;
	ExpVarSet.ExtraLevelToIncreaseFromBasic = ExpVarSet.ExtraLevelToIncrease - ExpVarSet.BasicLevelToIncrease;

	ExpVarSet.ExtraTargetExpPercent = StageClear_AfterExpPercent * 0.01f;
	ExpVarSet.ExtraCurrentExpPercent = StageClear_AfterExpPercent * 0.01f; // 檬扁蔼篮 Basic 捞唱 Extra 唱 付蛮啊瘤. BasicCurrent 客 鞍捞 刘啊窍促啊 捞扒 粱 歹 刘啊且 荐 乐澜.

	ExpVarSet.bExtraExpIncreaseComplete = false;

	ExpVarSet.CurrentIncreasedLevel = StageClear_BeforeLevel; // 捞巴档 痢瞒 PostClearLevel 肺 刘啊且 巴.
}

/** Most internal functionality for use in UpdateExpPercent. Returns true when current exp is reached to target value. */
bool UpdateExpPercentCommonInternalFunc(int32& InOutLevelToIncrease, int32& InOutCurrentLevel, float InTargetExpPercent, float& InOutCurrentExpPercent, float ExpIncreaseRate, float DeltaSeconds)
{
	if (InOutLevelToIncrease > 0 || InOutCurrentExpPercent < InTargetExpPercent)
	{
		// 如茄 S = Vt
		InOutCurrentExpPercent += FMath::Min(ExpIncreaseRate * DeltaSeconds, 0.99f); // 茄 橇饭烙俊 1.0 捞惑 刘啊窍搁 帮鄂

		if (InOutLevelToIncrease > 0)
		{
			if (InOutCurrentExpPercent >= 1.0f)
			{
				// 促澜 饭骇肺 逞绢埃促.
				--InOutLevelToIncrease;
				++InOutCurrentLevel;
				InOutCurrentExpPercent -= 1.0f;
			}
		}

		if (InOutLevelToIncrease == 0 && InOutCurrentExpPercent >= InTargetExpPercent)
		{
			// 格钎 饭骇鳖瘤 盲奎促搁 ExpPercent 鳖瘤父 嘎眠搁 凳.
			InOutCurrentExpPercent = InTargetExpPercent;
			return true;
		}

		return false;
	}

	return true;
}

void UB2UIStageResult::UpdateExpPercent(float DeltaSeconds)
{
	// Pause 惑怕俊辑 荤侩窍绰 芭扼 鸥捞赣肺 给窍绊 流立 墨款飘
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
		const int32 MainLevelBefore = EBV_MainPlayer.CurrentIncreasedLevel; // 捞锅 诀单捞飘俊辑 饭骇捞 刘啊沁绰瘤 眉农甫 困秦.
		const int32 SubLevelBefore = EBV_SubPlayer.CurrentIncreasedLevel;

		const bool bMainFinished = UpdateExpPercentCommon(EBV_MainPlayer, DeltaSeconds);
		const bool bSubFinished = UpdateExpPercentCommon(EBV_SubPlayer, DeltaSeconds);
				
		const bool bMainLevelIncreased = (EBV_MainPlayer.CurrentIncreasedLevel > MainLevelBefore);
		const bool bSubLevelIncreased = (EBV_SubPlayer.CurrentIncreasedLevel > SubLevelBefore);

		if (bMainLevelIncreased) // 饭骇诀 逞绢埃 鉴埃俊 咆胶飘 官层淋.
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
			// 荤角惑 咯扁啊 opening 瓤苞啊 场唱绰 矫痢捞 瞪 巴捞促. OnOpenComplete 措脚 咯扁辑 鞘夸茄 event 甫 朝府档废..
			OnActualOpenComplete();
		}

		//皋牢某腐 海捞流 Exp
		if (BasicExpBar_MainPlayer.IsValid())
		{
			float Percent = (EBV_MainPlayer.bBasicExpIncreaseComplete && EBV_MainPlayer.ExtraCurrentExpPercent < EBV_MainPlayer.BasicCurrentExpPercent) ? 0.0f : EBV_MainPlayer.BasicCurrentExpPercent;
			BasicExpBar_MainPlayer->SetPercent(Percent);
		}

		//皋牢某腐 竣胶飘扼 Exp
		if (ExtraExpBar_MainPlayer.IsValid())
		{
			BasicExpBar_MainPlayer->SetPercent(EBV_MainPlayer.ExtraCurrentExpPercent);
		}

		//辑宏某腐 海捞流 Exp
		if (BasicExpBar_SubPlayer.IsValid())
		{
			float Percent = (EBV_SubPlayer.bBasicExpIncreaseComplete && EBV_SubPlayer.ExtraCurrentExpPercent < EBV_SubPlayer.BasicCurrentExpPercent) ? 0.0f : EBV_SubPlayer.BasicCurrentExpPercent;
			BasicExpBar_SubPlayer->SetPercent(Percent);
		}

		//辑宏某腐 竣胶飘扼 Exp
		if (ExtraExpBar_SubPlayer.IsValid())
		{
			ExtraExpBar_SubPlayer->SetPercent(EBV_SubPlayer.ExtraCurrentExpPercent);
		}

		//皋牢某腐 Exp 欺季飘臂磊
		if (TB_ExpPercentMainChar.IsValid())
		{
			float CurrentMainExpPercent = static_cast<float>(FMath::FloorToInt(EBV_MainPlayer.ExtraCurrentExpPercent * 10000.0f)) / 100.0f;
			TB_ExpPercentMainChar->SetText(FText::FromString(FString::Printf(TEXT("%.2f%%"), CurrentMainExpPercent)));
		}
		
		//辑宏某腐 Exp 欺季飘臂磊
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

		// 酒流 Basic 荐摹啊 target 俊 档崔窍瘤 给茄 惑怕俊辑绰 Extra 荐摹甫 嘎苗霖促.
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
	// Pause 惑怕俊辑 镜 巴捞骨肺 扁夯 鸥捞赣 给 静绊 蝶肺 技泼

	for (int32 CTI = 0; CTI < CustomTimedCallBackInfo.Num(); ++CTI)
	{
		if (CustomTimedCallBackInfo[CTI].InvokeTime > 0.0f)
		{
			CustomTimedCallBackInfo[CTI].bIsInvoked = false;
		}
		else
		{
			CustomTimedCallBackInfo[CTI].bIsInvoked = true; // 捞固 惯悼茄 吧肺 埃林.
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

		ThisHideInfo.OriginalVisibility = WidgetToHide->GetVisibility(); // Hide 且 锭俊档 归诀秦 初瘤父 酒贰 肺流俊 蝶扼 HideStop 捞 刚历 瞪 荐档 乐栏聪 咯扁辑档 茄锅.

		// HideStopTime 捞 HideStartTime 焊促 奴瘤俊 措茄 犬牢篮.. 被捞 窍瘤 臼绰促.

		if (ThisHideInfo.HideStartTime <= 0.0f)
		{
			CustomTimeHideStartCallback(CTI); // 官肺 妮归 角青
		}

		if (ThisHideInfo.HideStopTime <= 0.0f)
		{
			CustomTimeHideStopCallback(CTI); // 官肺 妮归 角青. 搬惫 HideStopTime 捞 0 捞窍搁 救 见败咙
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

	// bIsSuccessiveLevelUpOpen 捞扼搁 茄锅俊 了促 贸府登档废 奴 蔼栏肺 技泼. 汲付 捞吧 逞绢啊绰 老篮 绝摆瘤 せ
	const float TimeSinceStart = bIsSuccessiveLevelUpOpen ? 1000000.0f : (TheWorld->GetRealTimeSeconds() - CachedWidgetStartTime);
	// Pause 惑怕俊辑档 流立 倒妨静绰 鸥捞赣
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
		{ // SuccessiveOpen 捞搁 官肺 焊捞档废 贸府.
			ThisHideInfo.bIsHidden = true;
			ThisHideInfo.bHiddenDoneOnce = true;
		}

		if (ThisHideInfo.bIsHidden == false)
		{
			// 茄锅父 HideStart 啊 登霸. 救 弊矾搁 濒冠老 巴.
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
	
	// 馆汗傈捧 惑怕看促啊 秦力登菌促绊 秦辑 饭骇诀 扑诀阑 被捞 凯 鞘夸绰 绝阑 淀.
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
		return; // 馆汗傈捧 档吝俊绰 饭骇诀 扑诀捞 凯府瘤 臼霸 窃.
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
		&& StageId.Id == 3030)				//CBT侩栏肺父 付瘤阜 厩根3茅磐 3胶抛捞瘤父 八荤
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
		
		
		// LevelUp Widget捞 鞘夸窍促搁 Animation捞 辆丰 饶 Quest包访 Logic阑 龋免 且 OnLevelUpFXFinish 窃荐甫 Delegate肺 殿废
		if (UIP_LevelUp.IsValid())
			UIP_LevelUp->LevelupFinishDelegate = FOnLevelUpFXFinish::CreateLambda([this](){ this->OnLevelUpFXFinish(); });
	}
	else // LevelUp Widget 鞘夸 绝促搁 官肺 Finish龋免窍咯 捞饶 惑炔 焊咯淋
	{
		OnLevelUpFXFinish();
	}*/
}
void UB2UIStageResult::StageFxResult(bool bMain, bool bSub)
{
	if (bMain || bSub)
	{
		SetIsSuccessiveLevelUpOpen(true);


		// LevelUp Widget捞 鞘夸窍促搁 Animation捞 辆丰 饶 Quest包访 Logic阑 龋免 且 OnLevelUpFXFinish 窃荐甫 Delegate肺 殿废
		if (UIP_LevelUp.IsValid())
			UIP_LevelUp->LevelupFinishDelegate = FOnLevelUpFXFinish::CreateLambda([this]() { this->OnLevelUpFXFinish(); });
	}
	else // LevelUp Widget 鞘夸 绝促搁 官肺 Finish龋免窍咯 捞饶 惑炔 焊咯淋
	{
		OnLevelUpFXFinish();
	}
}
void UB2UIStageResult::SetIsSuccessiveLevelUpOpen(bool bIsSuccessive)
{
	// 盔贰绰 LevelUp 楷免 磊眉绰 Clear 楷免苞 喊档搁辑 LevelUp UI 绰 StageResultUI 狼 老何肺辑 累悼窍档废 父甸菌阑 锭 父甸菌带 芭扼
	// StageResult UI 坷锹苞 窃膊 坷橇醋 局聪皋捞记阑 葛滴 胶诺窍绊 LevelUp 颇飘甫 凯档废 扁瓷捞 楼咯廉 乐澜.
	// StageResult UI 狼 促弗 何盒捞 磊楷胶反霸 场抄 饶 LevelUp 颇飘甫 咯绰 泅 规侥俊辑绰 弊繁 内靛甸捞 被捞 绝绢档 登变 窃

	bIsSuccessiveLevelUpOpen = bIsSuccessive;
	if (bIsSuccessiveLevelUpOpen)
	{ // bIsSuccessiveLevelUpOpen 捞 技泼登绢 乐促搁 UpdateWidgetCustomTimer 俊辑 了促 倒妨滚副 巴.
		UpdateWidgetCustomTimer();

		// 版氰摹尔 饭骇档 父顶栏肺 官肺.

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
		PopUpRepeatBattleStopMsg(GetOwningPlayer()); // 馆汗 傈捧 惑怕俊辑绰 吝窜 滚瓢狼 开且捞 凳.
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
		PopUpRepeatBattleStopMsg(GetOwningPlayer()); // 馆汗 傈捧 惑怕俊辑绰 吝窜 滚瓢狼 开且捞 凳.
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
		PopUpRepeatBattleStopMsg(GetOwningPlayer()); // 馆汗 傈捧 惑怕俊辑绰 吝窜 滚瓢狼 开且捞 凳.
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
		PopUpRepeatBattleStopMsg(GetOwningPlayer()); // 馆汗 傈捧 惑怕俊辑绰 吝窜 滚瓢狼 开且捞 凳.
	}
	else
	{
		bButtonClicked = true;
		GoToDeferredStageClass<int32>::GetInstance().Signal(INDEX_NONE);	// 漂沥 StageNum阑 涝仿窍瘤 臼栏搁 促澜 Stage肺
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


