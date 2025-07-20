// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UICharacterSelectPage.h"
#include "B2UIManager.h"

#include "B2UIDocHelper.h"
#include "B2UIStartButton.h"
#include "B2UIRepeatBattleCountDown.h"
#include "B2LobbyGameMode.h"
#include "B2ChapterInfo.h"
#include "B2UIRecommandPower.h"
#include "TutorialManager.h"
#include "BladeIIGameImpl.h"
#include "B2UIShortagePopupManager.h"
#include "B2UIHotTimeIcon.h"
#include "HotTimeBuffManager.h"
#include "B2LobbyUI_AutoSell.h"
#include "B2CombatStatEvaluator.h"
#include "Global.h"
#include "B2UIFairyLinkButton.h"
#include "B2UIAutoClearResult.h"
#include "Retailer.h"
#include "B2ServerResultCodeTable.h"
#include "B2UIEtherFullPopup.h"
#include "B2UIUtil.h"
#include "B2UIMsgPopupInventoryFull.h"



void UB2UICharacterSelectPageSlot::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_SelectHeroLevel);
	GET_SLOT(UTextBlock, TB_SelectHeroName);
	GET_SLOT(UTextBlock, TB_SelectCombatPower);

	GET_SLOT(UTextBlock, TB_UnSelectHeroLevel);
	GET_SLOT(UTextBlock, TB_UnSelectHeroName);
	GET_SLOT(UTextBlock, TB_UnSelectCombatPower);
	GET_SLOT(UTextBlock, TB_CharNumber);

	GET_SLOT(UImage, IMG_Arrow);

	GET_SLOT(UB2Button, BTN_Select);
}

void UB2UICharacterSelectPageSlot::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_Select, &UB2UICharacterSelectPageSlot::OnClickSelectButton);
}

void UB2UICharacterSelectPageSlot::Init()
{
	Super::Init();
}

void UB2UICharacterSelectPageSlot::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UICharacterSelectPageSlot::SetHeroInfo(EPCClass HeroClass, int32 Lv, int32 CombatPower)
{
	SlotClass = HeroClass;

	SelectCharacterHero(HeroClass);
	SetHeroName(BladeIIGameImpl::GetClientDataStore().GetLocalCharClassOrNickName(HeroClass));
	SetHeroLv(Lv);
	SetCombatPower(CombatPower);
}

void UB2UICharacterSelectPageSlot::SetHeroName(FText InText)
{
	if (TB_SelectHeroName.IsValid())
		TB_SelectHeroName->SetText(InText);
	if (TB_UnSelectHeroName.IsValid())
		TB_UnSelectHeroName->SetText(InText);
}

void UB2UICharacterSelectPageSlot::SetHeroLv(int32 Lv)
{
	if (TB_SelectHeroLevel.IsValid())
		TB_SelectHeroLevel->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_Level_Abbrev")), FText::AsNumber(Lv)));
	if (TB_UnSelectHeroLevel.IsValid())
		TB_UnSelectHeroLevel->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_Level_Abbrev")), FText::AsNumber(Lv)));
}

void UB2UICharacterSelectPageSlot::SetCombatPower(int32 CombatPower)
{
	if (TB_SelectCombatPower.IsValid())
		TB_SelectCombatPower->SetText(FText::AsNumber(CombatPower));

	if (TB_UnSelectCombatPower.IsValid())
		TB_UnSelectCombatPower->SetText(FText::AsNumber(CombatPower));
}

void UB2UICharacterSelectPageSlot::SlotSelect(int32 State)
{
	// 0 : 厚劝己 1:劝己 2:绰 眠饶俊 惑怕 措厚
	SelectCharacterStateBP(State);
}

void UB2UICharacterSelectPageSlot::SetSlotDelegate(FCharacterSlot Indelegate)
{
	SlotDelegate = Indelegate;
}

void UB2UICharacterSelectPageSlot::OnClickSelectButton()
{
	SlotDelegate.ExecuteIfBound(SlotClass);
}

void UB2UICharacterSelectPageSlot::SetMainHero(bool IsMain)
{
	if (TB_CharNumber.IsValid())
	{
		TB_CharNumber->SetText(FText::AsNumber(IsMain ? 1 : 2)); //皋牢捞搁 1锅 辑宏搁 2锅
		SlotSelect(true);
	}
}

void UB2UICharacterSelectPageSlot::SetSelectArrow(bool IsSelect)
{
	if (IMG_Arrow.IsValid())
		IMG_Arrow->SetVisibility(IsSelect ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UB2UICharacterSelectPageSlot::SetSlotSelectable(bool bSelectable)
{
	if (BTN_Select.IsValid())
		BTN_Select->SetVisibility(bSelectable ? ESlateVisibility::Visible : ESlateVisibility::SelfHitTestInvisible);
}

void UB2UICharacterSelectPage::CacheAssets()
{
	Super::CacheAssets();

	HeroArr.SetNum(GetMaxPCClassNum());

	GET_SLOT_BYNAME(UB2UICharacterSelectPageSlot, HeroArr[PCClassToInt(EPCClass::EPC_Gladiator)], UIP_Gladiator);
	GET_SLOT_BYNAME(UB2UICharacterSelectPageSlot, HeroArr[PCClassToInt(EPCClass::EPC_Assassin)], UIP_Assassin);
	GET_SLOT_BYNAME(UB2UICharacterSelectPageSlot, HeroArr[PCClassToInt(EPCClass::EPC_Wizard)], UIP_Wizard);
	GET_SLOT_BYNAME(UB2UICharacterSelectPageSlot, HeroArr[PCClassToInt(EPCClass::EPC_Fighter)], UIP_Fighter);

	GET_SLOT(UB2Button, BTN_MainHero);
	GET_SLOT(UB2Button, BTN_SubHero);
	GET_SLOT(UTextBlock, TB_TripleCount);

	GET_SLOT(UTextBlock, STB_MainChar);
	GET_SLOT(UTextBlock, STB_SubChar);
	GET_SLOT(UTextBlock, STB_Title);
	GET_SLOT(UTextBlock, STB_DirectionSkip);

	GET_SLOT(UB2Image, IMG_EmptyMain);
	GET_SLOT(UB2Image, IMG_EmptySub);

	GET_SLOT(UB2UIStartButton, UIP_StartBT);
	if (UIP_StartBT.IsValid())
	{
		UIP_StartBT->Init();
		UIP_StartBT->ClickDirectDelegateBindLambda([this]() { PageBlock(true); });
		UIP_StartBT->StartButtonDelegateBindLambda([this]() { BattleStart(); });
		UIP_StartBT->SetStartButtonText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("BattleStageInfo_BattleStart")));
	}

	GET_SLOT(UCheckBox, CB_Triple);
	GET_SLOT(UCheckBox, CB_AutoBattle);
	GET_SLOT(UCheckBox, CB_StoryCheck);
	GET_SLOT(UCheckBox, CB_LoopAll);

	GET_SLOT(UB2Image, IMG_AutoBattleLock);
	GET_SLOT(UB2Image, IMG_TrableAdventureLock);
	GET_SLOT(UB2Image, IMG_StoryLock);

	GET_SLOT(UPanelWidget, HB_TripleNUM);
	GET_SLOT(UPanelWidget, HB_AutoClearNUM);

	GET_SLOT(UB2Button, BTN_CharacterChange);
	//GET_SLOT(UB2UIRepeatBattleCountDown, UIP_RepeatBattleCountDownP);

	GET_SLOT(UB2Button, BTN_ToolTipDisableTrableAdventure);
	GET_SLOT(UB2Button, BTN_ToolTipDisableAutoBattle);
	GET_SLOT(UB2Button, BTN_ToolTipDisableLoopAll);
	GET_SLOT(UB2Button, BTN_ToolTipDisableStorySkip);
	GET_SLOT(UB2Button, BTN_GoHero);

	GET_SLOT(UVerticalBox, VB_MainCharacter);
	GET_SLOT(UVerticalBox, VB_SubCharacter);

	GET_SLOT(UTextBlock, TB_AutoText);
	GET_SLOT(UTextBlock, TB_TripleText);

	GET_SLOT(UB2RichTextBlock, RTB_LockToolTip);

	for (auto El : HeroArr)
	{
		El->Init();
	}

	GET_SLOT(UTextBlock, X_TB_Hero);
	if (X_TB_Hero.IsValid())
	{
		X_TB_Hero->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_CurrentCombatpower")));
	}

	GET_SLOT(UTextBlock, X_TB_HeroPower);
	GET_SLOT(UTextBlock, X_TB_Recommend);
	if (X_TB_Recommend.IsValid())
	{
		X_TB_Recommend->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_RecomCombatPower")));
	}

	GET_SLOT(UTextBlock, TB_LoopAll);

	GET_SLOT(UTextBlock, X_TB_RecommendPower);
	GET_SLOT(UB2UIRecommandPower, UIP_StageRecommendPower);
	if (UIP_StageRecommendPower.IsValid())
	{
		UIP_StageRecommendPower->Init();
	}

	GET_SLOT(UOverlay, O_NoWingSet);
	GET_SLOT(UB2Button, BTN_NotifyNoWingIngame);

	GET_SLOT(UB2ButtonGoodInfoToolTip, BTN_Goods);
	if (BTN_Goods.IsValid())
		BTN_Goods->SetItemInfo(FItemRefIDHelper::ITEM_REF_ID_STAGE_BOOST);

	GET_SLOT(UB2UIHotTimeIcon, UIP_Hottime);
	GET_SLOT(UTextBlock, TB_ActiveHotTime);
	GET_SLOT(UOverlay, O_ActiveHotTime);

	GET_SLOT(UB2Image, IMG_LoopAllLock);

	GET_SLOT(UWidgetSwitcher, WS_AutoSell);
	GET_SLOT(UB2Button, BTN_AutoSell);
	GET_SLOT(UTextBlock, TB_AutoSell);
	GET_SLOT(UB2Button, BTN_ActiveAutoSell);
	GET_SLOT(UTextBlock, TB_ActiveAutoSell);

	GET_SLOT(UB2UIFairyLinkButton, UIP_FairyLinkButton);
	if (UIP_FairyLinkButton.IsValid())
		UIP_FairyLinkButton->Init();

	GET_SLOT(UB2Button, BTN_AutoClear);
	GET_SLOT(UTextBlock, TB_AutoClear);
	GET_SLOT(UTextBlock, TB_AutoClearCount);
	GET_SLOT(UB2Button, BTN_ToolTipDisableAutoClear);
	GET_SLOT(UB2Image, IMG_AutoClear);
	
	GET_SLOT(UImage, FX_ToogleOn);
}

void UB2UICharacterSelectPage::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_MainHero, &UB2UICharacterSelectPage::OnClickMainButton);
	BIND_CLICK_FUNC_TO_BTN(BTN_SubHero, &UB2UICharacterSelectPage::OnClickSubButton);
	BIND_CLICK_FUNC_TO_BTN(BTN_CharacterChange, &UB2UICharacterSelectPage::OnClickBtnChangeMainSub);
	BIND_CLICK_FUNC_TO_BTN(BTN_ToolTipDisableTrableAdventure, &UB2UICharacterSelectPage::OnClickCBTrebleAdventure);
	BIND_CLICK_FUNC_TO_BTN(BTN_ToolTipDisableAutoBattle, &UB2UICharacterSelectPage::OnClickThreeStarToolTipOpen);
	BIND_CLICK_FUNC_TO_BTN(BTN_ToolTipDisableLoopAll, &UB2UICharacterSelectPage::OnClickLoopAllToolTipOpen);
	BIND_CLICK_FUNC_TO_BTN(BTN_ToolTipDisableStorySkip, &UB2UICharacterSelectPage::OnClickOneStarToolTipOpen);
	BIND_CLICK_FUNC_TO_BTN(BTN_GoHero, &UB2UICharacterSelectPage::OnClickGoHero);
	BIND_CLICK_FUNC_TO_BTN(BTN_NotifyNoWingIngame, &UB2UICharacterSelectPage::OnClickBtnNotifyNoWingIngame);
	BIND_CLICK_FUNC_TO_BTN(BTN_AutoSell, &UB2UICharacterSelectPage::OnClickAutoSell);
	BIND_CLICK_FUNC_TO_BTN(BTN_ActiveAutoSell, &UB2UICharacterSelectPage::OnClickAutoSell);
	BIND_CLICK_FUNC_TO_BTN(BTN_AutoClear, &UB2UICharacterSelectPage::OnClickBTNAutoClear);
	BIND_CLICK_FUNC_TO_BTN(BTN_ToolTipDisableAutoClear, &UB2UICharacterSelectPage::OnClickBTNToolTipDisableAutoClear);

	if (CB_Triple.IsValid())
	{
		CB_Triple->OnCheckStateChanged.AddDynamic(this, &UB2UICharacterSelectPage::CheckTriple);
	}

	if (CB_AutoBattle.IsValid())
	{
		CB_AutoBattle->OnCheckStateChanged.AddDynamic(this, &UB2UICharacterSelectPage::CheckAutoBattle);
		//CB_AutoBattle->OnEnabledStateChanged.AddDynamic(this, &UB2UICharacterSelectPage::EnabledAutoBattle);
	}

	if (CB_StoryCheck.IsValid())
	{
		CB_StoryCheck->OnCheckStateChanged.AddDynamic(this, &UB2UICharacterSelectPage::CheckStorySkip);
		//CB_StoryCheck->OnEnabledStateChanged.AddDynamic(this, &UB2UICharacterSelectPage::EnabledStorySkip);
	}

	if (CB_LoopAll.IsValid())
	{
		CB_LoopAll->OnCheckStateChanged.AddDynamic(this, &UB2UICharacterSelectPage::CheckLoopAll);
		//CB_LoopAll->OnEnabledStateChanged.AddDynamic(this, &UB2UICharacterSelectPage::EnabledLoopAll);
	}
}

UB2UICharacterSelectPage::UB2UICharacterSelectPage(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	IsSingleSelect = false;
}

void UB2UICharacterSelectPage::Init()
{
	Super::Init();

	auto DocBattle = UB2UIDocHelper::GetDocBattle();
	auto DocBattleStage = UB2UIDocHelper::GetDocBattleStage();
	if (DocBattle && DocBattleStage)
	{
		ChangeMainClass = DocBattle->GetCurPCClass();
		ChangeSubClass = DocBattle->GetTagPCClass();
		if (ChangeMainClass == ChangeSubClass)
			ChangeSubClass = (ChangeSubClass + 1) % GetMaxPCClassNum();

		ClientStageId = DocBattleStage->GetClientStageId();
		ServerStageId = DocBattleStage->GetServerStageId();
		StageDifficulty = DocBattleStage->GetStageDifficulty();

		SetAutoClear(BladeIIGameImpl::GetStageDataStore().GetStageClearGradeInfo(ServerStageId) >= AutoClearStar);

		RecommendPower = DocBattleStage->GetRecommendPower();
		if (X_TB_RecommendPower.IsValid())
		{
			X_TB_RecommendPower->SetText(FText::AsNumber(RecommendPower));
		}
		int factor = DocBattleStage->GetRepeatBattleBoostOn() ? B2_REPEAT_BATTLE_BOOST_FACTOR : 1;

		UIP_StartBT->SetGoodsNumber(DocBattleStage->GetRequiredStaminaNum() * factor);

		BoostCount = DocBattleStage->GetRepeatBattleBoostRemainCount();

		if (BladeIIGameImpl::GetStageDataStore().IsRepeatBattleLocked(DocBattleStage->GetServerStageId()))
		{
			DocBattleStage->SetRepeatBattleLoopOne(false);
			DocBattleStage->SetRepeatBattleBoostOn(false);
			
			if (CB_Triple.IsValid())
			{
				CB_Triple->SetIsEnabled(false);
				EnabledTriple(false);
			}
			
			if (CB_AutoBattle.IsValid())
			{
				CB_AutoBattle->SetIsEnabled(false);
			}
		}
		else
		{
			if (IMG_TrableAdventureLock.IsValid())
				IMG_TrableAdventureLock->SetVisibility(ESlateVisibility::Collapsed);

			if (HB_TripleNUM.IsValid())
			{
				HB_TripleNUM->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}

			if (BTN_ToolTipDisableTrableAdventure.IsValid())
			{
				BTN_ToolTipDisableTrableAdventure->SetVisibility(BoostCount > 0 ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
				//BTN_ToolTipDisableTrableAdventure->SetIsEnabled(BoostCount > 0);
			}

			if (DocBattleStage->GetRepeatBattleCurrentOn() == false && DocBattleStage->GetRepeatBattleBoostOn() == true)
			{
				DocBattleStage->SetRepeatBattleBoostOn(BoostCount > 0);
				if (CB_Triple.IsValid())
				{
					CB_Triple->SetCheckedState(BoostCount > 0 ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
				}
			}

			if (CB_Triple.IsValid())
			{
				CB_Triple->SetIsEnabled(BoostCount > 0);
			}
		}

		DocBattleStage->OnRepeatBattleCurrentOnChanged.AddUObject(this, &UB2UICharacterSelectPage::OnChangedRepeatBattleCurrentOn);

		if (TB_TripleCount.IsValid())
		{
			int64 InBoostCount = DocBattleStage->GetRepeatBattleBoostMaxCount() > BoostCount ? BoostCount : DocBattleStage->GetRepeatBattleBoostMaxCount();
			TB_TripleCount->SetText(FText::Format(FText::FromString(TEXT("x {0}")), FText::AsNumber(InBoostCount)));
			if (BoostCount <= 0)
			{
				TB_TripleCount->SetColorAndOpacity(FLinearColor::Red);
			}
		}
		
		// condition俊 蝶弗 眉农 钎矫甫 困秦 delegate function 阑 流立 何抚.
		if (CB_AutoBattle.IsValid())
			CheckAutoBattle(DocBattleStage->GetRepeatBattleLoopOne());

		if (CB_Triple.IsValid())
			CheckTriple(DocBattleStage->GetRepeatBattleBoostOn());

		if (CB_LoopAll.IsValid())
			CheckLoopAll(DocBattleStage->GetRepeatBattleLoopAll());
	}

	bool IsActiveHotTime = HotTimeBuffManager::GetInstance().IsActiveHotTimeBuff(EHotTimeEventType::All);
	if (O_ActiveHotTime.IsValid())
		O_ActiveHotTime->SetVisibility(IsActiveHotTime ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	if (UIP_Hottime.IsValid())
		UIP_Hottime->ActiveEffect_BP(IsActiveHotTime);

	if (TB_ActiveHotTime.IsValid())
		TB_ActiveHotTime->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("HotTime_ApplyDesc")));

	if (BTN_Goods.IsValid())
	{
		//BTN_Goods->SetItemInfo(_ItemTemplateId);
	}

	SetCharacterInfo();

	// 捞霸 难柳 惑怕肺 柯 扒 捞固 弥家茄 茄锅 馆汗 傈捧 胶抛捞瘤甫 场郴绊 酒流 巢篮 惑怕俊辑 肺厚肺 柯 芭.
	SetupOnRepeatBattleStateChange(DocBattleStage->GetRepeatBattleCurrentOn());
	UpDateHero();

	SetEnableStageGameStarted(false);

	SaveDocFilterInfo();
}

void UB2UICharacterSelectPage::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);

	if (UIP_RepeatBattleCountDownP.IsValid())
		UIP_RepeatBattleCountDownP->DestroySelf(InUIManager);

	if (UIP_StageRecommendPower.IsValid())
		UIP_StageRecommendPower->DestroySelf(InUIManager);
}

void UB2UICharacterSelectPage::OnOpen(bool RightNow /*= false*/)
{
	Super::OnOpen(RightNow);

	SubscribeEvents();

	// 楷免 掺扁	
	UB2UIDocBattleStage* DocBattleStage = UB2UIDocHelper::GetDocBattleStage();

	// 沥抗 厩根篮 公炼扒 眉农, 叼胶俊捞喉
	// 楷加 傈捧 难林磊
	if (DocBattleStage->GetStageDifficulty() == EStageDifficulty::ESD_Hard
		|| DocBattleStage->GetStageDifficulty() == EStageDifficulty::ESD_VeryHard
		|| DocBattleStage->GetStageDifficulty() == EStageDifficulty::ESD_Hell)
	{
		CheckStorySkip(true);

		if (CB_StoryCheck.IsValid())
		{
			CB_StoryCheck->SetIsEnabled(false);
		}

		if (CB_LoopAll.IsValid())
		{
			CB_LoopAll->SetIsEnabled(true);
		}
	}
	else
	{
		int32 nStarCount = 0;
		for (bool bStar : DocBattleStage->GetStageClearConditions())
		{
			if (bStar)
				nStarCount++;
		}
		if (nStarCount == 0)
		{
			if (CB_StoryCheck.IsValid())
				CB_StoryCheck->SetIsEnabled(false);
		}
		else
		{
			CheckStorySkip(true);
		}

		if (CB_LoopAll.IsValid())
		{
			CB_LoopAll->SetIsEnabled(false);
		}
	}
}

void UB2UICharacterSelectPage::OnClose(bool RightNow /*= false*/)
{
	UnsubscribeEvents();

	Super::OnClose(RightNow);
}

void UB2UICharacterSelectPage::OnSceneOpen(EUIScene InOpenedScene)
{
	Super::OnSceneOpen(InOpenedScene);

	//auto DocBattleStage = UB2UIDocHelper::GetDocBattleStage();
	//ClientStageId = DocBattleStage->GetClientStageId();
	//StageDifficulty = DocBattleStage->GetStageDifficulty();
	//ServerStageId = DocBattleStage->GetServerStageId();

	////Cache StageInfoData
	//UB2StageInfo* StageInfoTable = StaticFindStageInfo();
	//int32 ChapterNum = StageInfoTable ? StageInfoTable->GetChapterNumByClientStageId(ClientStageId) : 0;
	//int32 StageNum = StageInfoTable ? StageInfoTable->GetSubChapterNumByClientStageId(ClientStageId) : 0;

	//FSingleStageInfoData* CachedCurrentStageInfoData = StageInfoTable ? StageInfoTable->GetInfoData(ClientStageId, StageDifficulty) : NULL;

	//if (IsSingleSelect) //true搁 痢飞傈
	//{
	//	SetLobbyUIHeaderTitle(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ChallengeMod_ControlMatch")));
	//}
	//else if (CachedCurrentStageInfoData && DocBattleStage && (!IsSingleSelect)) // CachedCurrentStageInfoData 掘绢柯 矫痢 捞饶咯具
	//{
	//	FText StageDiffText = FText::FromString(FString::Printf(TEXT("Chapter_DiffColor_%d"), (int32)StageDifficulty));
	//	FText StageText = FText::Format(BladeIIGetLOCText(B2LOC_CAT_WORLDNAME, StageDiffText.ToString()),
	//		CachedCurrentStageInfoData->GetLocalizedStageName());

	//	SetLobbyUIHeaderTitle(StageText);
	//}

	//SetAutoSellBtnState();
}

void UB2UICharacterSelectPage::UpdateStaticText()
{
	Super::UpdateStaticText();

	if (STB_MainChar.IsValid())
		STB_MainChar->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CharacterSelect_MainPlayer")));

	if (STB_SubChar.IsValid())
		STB_SubChar->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CharacterSelect_TagPlayer")));

	if (STB_Title.IsValid())
		STB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("BattleStageInfo_ChangeHero")));

	if (STB_DirectionSkip.IsValid())
		STB_DirectionSkip->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("BattleStageInfo_DirectionOff")));

	if (RTB_LockToolTip.IsValid())
		RTB_LockToolTip->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("BattleStageInfo_UnLock")));

	if (TB_LoopAll.IsValid())
		TB_LoopAll->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("BattleStageInfo_ContBattle")));

	if (TB_AutoSell.IsValid())
		TB_AutoSell->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("BattleStageInfo_AutoSell")));

	if (TB_ActiveAutoSell.IsValid())
		TB_ActiveAutoSell->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("BattleStageInfo_AutoSell")));

	SetAutoClearCount();

	if (TB_AutoClear.IsValid())
	{
		TB_AutoClear->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CounterDG_AutoClear_Title")));
	}
}

void UB2UICharacterSelectPage::SubscribeEvents()
{
	UnsubscribeEvents();

	Issues.Add(ToolTipOpenClass<>::GetInstance().Subscribe2(
		[this]()
	{
		CancelOrStopRepeatBattleClass<>::GetInstance().Signal();
	}
	));

	Issues.Add(DeliveryStartStageErrorClass<>::GetInstance().Subscribe2(
		[this]()
	{
		this->SetEnableStageGameStarted(false);

		CheckAutoBattle(false);
	}
	));

	Issues.Add(RefreshAutoSellStateClass<>::GetInstance().Subscribe2(
		[this]()
	{
		this->SetAutoSellBtnState();
	}
	));
	
	CAPTURE_UOBJECT(UB2UICharacterSelectPage);

	Issues.Add(DeliverySweepStageClass<FB2SweepStageInfo>::GetInstance().Subscribe2(
		[Capture](const FB2SweepStageInfo& InSweepStageinfo)
	{
		if (Capture.IsValid())
		{
			Capture->ResponseSweepStage(InSweepStageinfo);
		}
	}
	));

	Issues.Add(DeliverySweepStageErrorClass<FB2SweepStageInfo, int32>::GetInstance().Subscribe2(
		[Capture](const FB2SweepStageInfo& InSweepStageinfo, int32 InErrorCode)
	{
		if (Capture.IsValid())
		{
			Capture->ResponseSweepStageError(InSweepStageinfo, InErrorCode);
		}
	}
	));
}

void UB2UICharacterSelectPage::UnsubscribeEvents()
{
	Issues.Empty();
}

void UB2UICharacterSelectPage::SetChangeCombatPower(int32 InPower, int32 InAttack, int32 InDefense)
{
	//if (X_TB_HeroPower.IsValid())
	//{
	//	X_TB_HeroPower->SetText(FText::AsNumber(InPower));
	//	if (!IsSingleSelect)
	//		X_TB_HeroPower->SetColorAndOpacity(GetPowerColor(InPower, RecommendPower));
	//}

	//auto DocBattleStage = UB2UIDocHelper::GetDocBattleStage();

	//if (UIP_StageRecommendPower.IsValid() && DocBattleStage)
	//{
	//	UIP_StageRecommendPower->SetInfo(InAttack, InDefense, DocBattleStage->GetRecommendAttack(), DocBattleStage->GetRecommendDefense());
	//}

	//CheckStartButton();
}

void UB2UICharacterSelectPage::UpDateHero()
{
	auto DocBattle = UB2UIDocHelper::GetDocBattle();
	bool bCharacterEmptyState = false;

	if (DocBattle)
	{
		if (IMG_EmptyMain.IsValid())
			IMG_EmptyMain->SetVisibility(IntToPCClass(ChangeMainClass) == EPCClass::EPC_End ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		if (IMG_EmptySub.IsValid())
			IMG_EmptySub->SetVisibility(IntToPCClass(ChangeSubClass) == EPCClass::EPC_End ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

		if (VB_MainCharacter.IsValid())
			VB_MainCharacter->SetVisibility(IntToPCClass(ChangeMainClass) == EPCClass::EPC_End ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);

		if (VB_SubCharacter.IsValid())
			VB_SubCharacter->SetVisibility(IntToPCClass(ChangeSubClass) == EPCClass::EPC_End ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);

		if (IsSingleSelect ==  true && IntToPCClass(ChangeMainClass) == EPCClass::EPC_End)
		{
			bCharacterEmptyState = true;
		}
		else if(IsSingleSelect == false && (IntToPCClass(ChangeSubClass) == EPCClass::EPC_End || IntToPCClass(ChangeMainClass) == EPCClass::EPC_End))
		{
			bCharacterEmptyState = true;
		}
	}

	int32 CombatPower = 0;
	int32 CurrentAttack = 0;
	int32 CurrentDefense = 0;

	for (int32 ClassCnt = 0; ClassCnt < GetMaxPCClassNum(); ++ClassCnt)
	{
		auto DocHero = UB2UIDocHelper::GetDocHero(ClassCnt);

		if (DocHero && DocBattle)
		{
			if (ClassCnt == ChangeMainClass)
			{
				HeroArr[ClassCnt]->SetMainHero(true);
			}
			else if (ClassCnt == ChangeSubClass && !IsSingleSelect)
			{
				HeroArr[ClassCnt]->SetMainHero(false);
			}
			else
				HeroArr[ClassCnt]->SlotSelect(false);
			//ClassCnt == ChangeMainClass || (ClassCnt == ChangeSubClass && !IsSingleSelect) ? HeroArr[ClassCnt]->SlotSelect(true) : HeroArr[ClassCnt]->SlotSelect(false);

			HeroArr[ClassCnt]->SetSelectArrow(bCharacterEmptyState);
		}

		if (ClassCnt == ChangeMainClass || (ClassCnt == ChangeSubClass && !IsSingleSelect))
		{
			CombatPower += DocHero->GetCombatPower();
			CurrentAttack += DocHero->GetAttackPower();
			CurrentDefense += DocHero->GetDefensePower();
		}
	}

	if (IsSingleSelect && ChangeMainClass == ChangeSubClass)
	{
		// 教臂伎泛飘搁 辑宏努贰胶 救静瘤父.. 吝汗 救登霸 函版
		ChangeSubClass = (ChangeSubClass + 1) % GetMaxPCClassNum();
	}

	// 朝俺 盎脚捞 鞘夸茄 纳捞胶啊 乐促. 皋浆炼钦眉农
	LobbyUpdateCharacterPartClass<EPCClass>::GetInstance().Signal(IntToPCClass(ChangeMainClass));
	LobbyUpdateCharacterPartClass<EPCClass>::GetInstance().Signal(IntToPCClass(ChangeSubClass));

	LobbySetPCSelectionClass<EPCClass, EPCClass>::GetInstance().Signal(IntToPCClass(ChangeMainClass), IntToPCClass(ChangeSubClass));
	
	SetChangeCombatPower(CombatPower, CurrentAttack, CurrentDefense);
}

void UB2UICharacterSelectPage::SlotDelegateFunction(EPCClass InValue)
{
	int32 EPCEND = GetMaxPCClassNum();
	int32 EPCValue = PCClassToInt(InValue);

	CheckControlTutorialNextStep();

	if (IsSingleSelect)
	{
		if (ChangeMainClass == EPCValue)
		{
			return;
		}
		else
		{
			if (ChangeSubClass == EPCValue)
			{
				ChangeSubClass = ChangeMainClass;
			}

			ChangeMainClass = EPCValue;
		}
	}
	else
	{
		if (ChangeMainClass == EPCValue)
		{
			ChangeMainClass = EPCEND;
		}
		else if (ChangeSubClass == EPCValue)
		{
			ChangeSubClass = EPCEND;
		}
		else if (ChangeMainClass == EPCEND)
		{
			if (ChangeSubClass != EPCValue)
			{
				ChangeMainClass = EPCValue;
			}
		}
		else if (ChangeSubClass == EPCEND)
		{
			if (ChangeMainClass != EPCValue)
			{
				ChangeSubClass = EPCValue;
			}
		}
		else
		{
			return;
		}
	}

	UpDateHero();
}

bool UB2UICharacterSelectPage::CanStartBattle()
{
	bool Result = false;

	if (CheckAbleTime() == false)
	{
		Result = false;
	}
	else
	{
		FLocalCharacterData& CharDataStore = BladeIIGameImpl::GetLocalCharacterData();
		
		if (IsSingleSelect == true && CharDataStore.GetMainPlayerClass() != EPCClass::EPC_End)
		{
			Result = true;
		} 
		else if(IsSingleSelect == false && CharDataStore.GetMainPlayerClass() != EPCClass::EPC_End && CharDataStore.GetSubPlayerClass() != EPCClass::EPC_End)
		{
			Result = true;
		}
	}

	return Result;
}

void UB2UICharacterSelectPage::OnClickThreeStarToolTipOpen()
{
	if (RTB_LockToolTip.IsValid())
		RTB_LockToolTip->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("BattleStageInfo_UnLock")));

	ToolTipStartAni();
}

void UB2UICharacterSelectPage::OnClickLoopAllToolTipOpen()
{
	if (RTB_LockToolTip.IsValid())
	{
		RTB_LockToolTip->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("BattleStageInfo_UnLock3")));
	}

	ToolTipStartAni();
}

void UB2UICharacterSelectPage::OnClickOneStarToolTipOpen()
{
	if (RTB_LockToolTip.IsValid())
		RTB_LockToolTip->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("BattleStageInfo_UnLock2")));

	ToolTipStartAni();
}

void UB2UICharacterSelectPage::OnClickMainButton()
{
	if (IsSingleSelect)
		return;

	if (ChangeMainClass == GetMaxPCClassNum())
		return;

	ChangeMainClass = GetMaxPCClassNum();
	UpDateHero();
}

void UB2UICharacterSelectPage::OnClickSubButton()
{
	if (IsSingleSelect)
		return;

	if (ChangeSubClass == GetMaxPCClassNum())
		return;

	ChangeSubClass = GetMaxPCClassNum();
	UpDateHero();
}

void UB2UICharacterSelectPage::OnClickCBTrebleAdventure()
{
	if (auto* DocBattleStage = UB2UIDocHelper::GetDocBattleStage())
	{
		if (BladeIIGameImpl::GetStageDataStore().IsRepeatBattleLocked(DocBattleStage->GetServerStageId()))
		{
			if (RTB_LockToolTip.IsValid())
				RTB_LockToolTip->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("BattleStageInfo_UnLock")));

			ToolTipStartAni();
		}
		else
		{
			if (BoostCount < 1)
			{
				ShortageMGR->PopupGotoShopForStageBoostMessage();
			}
		}
	}
}

void UB2UICharacterSelectPage::CheckTriple(bool bCheck)
{
	auto DocBattleStage = UB2UIDocHelper::GetDocBattleStage();

	if (bCheck)
	{
		if (CB_Triple.IsValid())
		{
			CB_Triple->SetCheckedState(ECheckBoxState::Checked);
		}
		
		DocBattleStage->SetRepeatBattleBoostOn(true);

		if (TB_TripleText.IsValid())
		{
			TB_TripleText->SetColorAndOpacity(FLinearColor(0.82, 0.80, 0.77, 1));
		}
		
		if (FX_ToogleOn.IsValid())
		{
			FX_ToogleOn->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}
	else
	{
		if (CB_Triple.IsValid())
		{
			CB_Triple->SetCheckedState(ECheckBoxState::Unchecked);
		}

		DocBattleStage->SetRepeatBattleBoostOn(false);
		
		if (TB_TripleText.IsValid())
		{
			TB_TripleText->SetColorAndOpacity(FLinearColor(0.44, 0.45, 0.46, 1));
		}

		if (FX_ToogleOn.IsValid())
		{
			FX_ToogleOn->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if (UIP_StartBT.IsValid())
	{
		const int32 factor = bCheck ? B2_REPEAT_BATTLE_BOOST_FACTOR : 1;
		const int32 DeDuctionCount = 1;

		UIP_StartBT->SetGoodsNumber(DocBattleStage->GetRequiredStaminaNum() * factor, bCheck);
		UIP_StartBT->SetSecondGoodsNumber(bCheck ? DeDuctionCount : 0);
	}
}

void UB2UICharacterSelectPage::EnabledTriple(bool bCheck)
{
	if (IMG_TrableAdventureLock.IsValid())
		IMG_TrableAdventureLock->SetVisibility(bCheck ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);

	if (HB_TripleNUM.IsValid())
	{
		HB_TripleNUM->SetVisibility(bCheck ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}

	if (BTN_ToolTipDisableTrableAdventure.IsValid())
	{
		BTN_ToolTipDisableTrableAdventure->SetVisibility(bCheck ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
		BTN_ToolTipDisableTrableAdventure->SetIsEnabled(!bCheck);
	}
}

void UB2UICharacterSelectPage::CheckAutoBattle(bool bCheck)
{
	if (!TB_AutoText.IsValid())
		return;

	if (CB_LoopAll.IsValid() && CB_LoopAll->GetCheckedState() == ECheckBoxState::Checked)
	{
		UnCheckLoopAll();
	}

	if (bCheck)
	{
		auto DocBattleStage = UB2UIDocHelper::GetDocBattleStage();
		DocBattleStage->SetRepeatBattleLoopOne(true);
		
		if (TB_AutoText.IsValid())
		{
			TB_AutoText->SetColorAndOpacity(FLinearColor(0.98, 0.85, 0, 1));
		}
		
		if (CB_AutoBattle.IsValid())
		{
			CB_AutoBattle->SetCheckedState(ECheckBoxState::Checked);
		}
		
		if (UIP_StartBT.IsValid())
		{
			UIP_StartBT->SetStartButtonText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("B2UIRepeatBattleButton_006")));
		}
	}
	else
	{
		UnCheckAutoBattle();
	}
}

void UB2UICharacterSelectPage::UnCheckAutoBattle()
{
	auto DocBattleStage = UB2UIDocHelper::GetDocBattleStage();
	DocBattleStage->SetRepeatBattleLoopOne(false);
	
	if (TB_AutoText.IsValid())
	{
		TB_AutoText->SetColorAndOpacity(FLinearColor::White);
	}
	
	if (UIP_StartBT.IsValid())
	{
		UIP_StartBT->SetStartButtonText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("BattleStageInfo_BattleStart")));
	}

	if (CB_AutoBattle.IsValid())
	{
		CB_AutoBattle->SetCheckedState(ECheckBoxState::Unchecked);
	}
}

void UB2UICharacterSelectPage::EnabledAutoBattle(bool bCheck)
{
	if (IMG_AutoBattleLock.IsValid())
		IMG_AutoBattleLock->SetVisibility(bCheck ? ESlateVisibility::Hidden : ESlateVisibility::Visible);

	if (BTN_ToolTipDisableAutoBattle.IsValid())
		BTN_ToolTipDisableAutoBattle->SetVisibility(bCheck ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
}

void UB2UICharacterSelectPage::CheckStorySkip(bool bCheck)
{
	UB2UIDocUICondition* DocCon = UB2UIDocHelper::GetDocUICondition();
	if (DocCon)
	{
		if (CB_StoryCheck.IsValid())
		{
			CB_StoryCheck->SetCheckedState(bCheck ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
			if (STB_DirectionSkip.IsValid())
				STB_DirectionSkip->SetColorAndOpacity(bCheck ? FLinearColor::Yellow : FLinearColor::White);
		}

		DocCon->SetbUserSelectedSkipStory(bCheck);

		LobbyUpdateCharacterPartClass<EPCClass>::GetInstance().Signal(IntToPCClass(ChangeMainClass));
		LobbyUpdateCharacterPartClass<EPCClass>::GetInstance().Signal(IntToPCClass(ChangeSubClass));

		// 朝俺x 酒捞能 钎矫力绢 譬配府倔 玻阑锭父(扁夯蔼 Hiddne栏肺 登乐澜)
		TutorialManager& TutorialManagerInstance = TutorialManager::GetInstance();
		if (!TutorialManagerInstance.IsLockTutorial(TutorialID_Wing, 0))
		{
			if (O_NoWingSet.IsValid())
				O_NoWingSet->SetVisibility(bCheck ? ESlateVisibility::Hidden : ESlateVisibility::SelfHitTestInvisible);
		}
	}
}

void UB2UICharacterSelectPage::EnabledStorySkip(bool bCheck)
{
	if (IMG_StoryLock.IsValid())
		IMG_StoryLock->SetVisibility(bCheck ? ESlateVisibility::Hidden : ESlateVisibility::Visible);

	if (BTN_ToolTipDisableStorySkip.IsValid())
		BTN_ToolTipDisableStorySkip->SetVisibility(bCheck ? ESlateVisibility::Hidden : ESlateVisibility::Visible);

	// 眉农等惑怕搁 磊拱艰 救焊烙(眉农等惑怕肺 叼胶俊捞喉 矫难崔扼绰 纳捞胶)
	if (CB_StoryCheck.IsValid() && IMG_StoryLock.IsValid() && CB_StoryCheck->GetCheckedState() == ECheckBoxState::Checked)
		IMG_StoryLock->SetVisibility(ESlateVisibility::Hidden);

	if (CB_StoryCheck.IsValid() && BTN_ToolTipDisableStorySkip.IsValid() && CB_StoryCheck->GetCheckedState() == ECheckBoxState::Checked)
		BTN_ToolTipDisableStorySkip->SetVisibility(ESlateVisibility::Hidden);
}

void UB2UICharacterSelectPage::CheckLoopAll(bool bCheck)
{
	if (CB_AutoBattle.IsValid() && CB_AutoBattle->GetCheckedState() == ECheckBoxState::Checked)
	{
		UnCheckAutoBattle();
	}

	auto DocBattleStage = UB2UIDocHelper::GetDocBattleStage();
	if (DocBattleStage)
	{
		if (bCheck)
		{
			DocBattleStage->SetRepeatBattleLoopAll(true);
			if (TB_LoopAll.IsValid())
			{
				TB_LoopAll->SetColorAndOpacity(FLinearColor(0.98, 0.85, 0, 1));
			}
			
			if (UIP_StartBT.IsValid())
			{
				UIP_StartBT->SetStartButtonText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("BattleStageInfo_ContBattle")));
			}

			if (CB_LoopAll.IsValid())
			{
				CB_LoopAll->SetCheckedState(ECheckBoxState::Checked);
			}
		}
		else
		{
			UnCheckLoopAll();
		}
	}
}

void UB2UICharacterSelectPage::UnCheckLoopAll()
{
	auto DocBattleStage = UB2UIDocHelper::GetDocBattleStage();
	if (DocBattleStage)
	{
		DocBattleStage->SetRepeatBattleLoopAll(false);
	}
	
	if (TB_LoopAll.IsValid())
	{
		TB_LoopAll->SetColorAndOpacity(FLinearColor::White);
	}
	
	if (UIP_StartBT.IsValid())
	{
		UIP_StartBT->SetStartButtonText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("BattleStageInfo_BattleStart")));
	}

	if (CB_LoopAll.IsValid())
	{
		CB_LoopAll->SetCheckedState(ECheckBoxState::Unchecked);
	}
}

void UB2UICharacterSelectPage::EnabledLoopAll(bool bCheck)
{
	if (IMG_LoopAllLock.IsValid())
	{
		IMG_LoopAllLock->SetVisibility(bCheck ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
	}

	if (BTN_ToolTipDisableLoopAll.IsValid())
	{
		BTN_ToolTipDisableLoopAll->SetVisibility(bCheck ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
	}
}

void UB2UICharacterSelectPage::BattleStart()
{
	if (CanStartBattle())
	{
		CheckControlTutorialNextStep();
		SetEnableStageGameStarted(true); // 矫累 滚瓢 努腐 饶 吝汗 努腐捞 救登档废 窍扁 困秦.

		if (auto* GameMode = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(this)))
			GameMode->BattleStart();
	}
}

void UB2UICharacterSelectPage::SetupOnRepeatBattleStateChange(bool bIsOn)
{
	if (bIsOn)
	{
		StartNextRepeatBattleCountdown();
	}
	else
	{
		StopNextRepeatBattleCountdown();
	}

	// 馆汗傈捧 滚瓢篮 泅犁 惑怕俊 蝶扼 悼累捞 馆措肺 官柴.
	/*if (TBS_RepeatBattle.IsValid())
	{
	TBS_RepeatBattle->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL,
	bIsOn ? FString(TEXT("BattleStageInfo_StopRepeatBattle")) : FString(TEXT("BattleStageInfo_RepeatBattle"))
	));
	}*/
}

void UB2UICharacterSelectPage::StartNextRepeatBattleCountdown()
{
	if (!UIP_RepeatBattleCountDownP.IsValid())
	{
		UIP_RepeatBattleCountDownP = UB2UIManager::GetInstance()->OpenUI<UB2UIRepeatBattleCountDown>(UIFName::RepeatBattleCountDown);
		UIP_RepeatBattleCountDownP->SetPauseAble(true);
	}

	if (UIP_RepeatBattleCountDownP.IsValid())
	{
		if (IsBoostOverCount() == true)
		{
			UB2UIDocHelper::GetDocBattleStage()->SetRepeatBattleCurrentOn(false);
			StopNextRepeatBattleCountdown();

			CheckAutoBattle(false);
			CheckTriple(false);
		}
		else if (IsRepeatOverCount() == true)
		{
			UB2UIManager::GetInstance()->OpenMsgPopup(
				EUIMsgPopup::Simple,
				BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_NoticeTab")),
				BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("BattleStageInfo_RepeatBattleComplete")),
				0.f,
				true,
				true,
				EUIMsgPopupButtonGroup::Confirm
			);

			CheckAutoBattle(false);
			UB2UIDocHelper::GetDocBattleStage()->SetRepeatBattleCurrentOn(false);
			StopNextRepeatBattleCountdown();
		}
		else
		{
			UIP_RepeatBattleCountDownP->SetVisibility(ESlateVisibility::Visible);
			UIP_RepeatBattleCountDownP->StartCountdown(FRepeatBattleCountDownCB::CreateLambda([this]() {
				StartNextRepeatBattle(); // 墨款飘促款 促 登搁 矫累窍霸.
			}));
		}
	}
}

void UB2UICharacterSelectPage::StopNextRepeatBattleCountdown()
{
	if (UIP_RepeatBattleCountDownP.IsValid())
	{
		UIP_RepeatBattleCountDownP->StopCountdown();
		UIP_RepeatBattleCountDownP->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UB2UICharacterSelectPage::StartNextRepeatBattle()
{
	if (UIP_RepeatBattleCountDownP.IsValid())
	{
		UIP_RepeatBattleCountDownP->StopCountdown();
	}

	// 捞傈俊 傈捧俊辑 Caching等 Buff绰 葛滴 淬酒霖促. 咯扁鳖瘤 坷搁 Buff 荤侩啊瓷 咯何绰 犬牢 等 惑怕
	UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();
	TArray<EStageBuffType> CurrentBuffList;
	if (DocBS != nullptr)
	{
		CurrentBuffList = DocBS->GetSelectedStageBuffs();
	}

	UB2UIDocUICondition* DocCon = UB2UIDocHelper::GetDocUICondition();
	StartGameStageFromLobbyClass<int32, EStageDifficulty, const TArray<EStageBuffType>&, bool>::GetInstance().Signal(ClientStageId, StageDifficulty, CurrentBuffList, (DocCon ? DocCon->GetbUserSelectedSkipStory() : false));
}

void UB2UICharacterSelectPage::OnClickBtnChangeMainSub()							//Main, Sub 背眉 混妨滴摆嚼聪促...
{
	// UB2UICharacterSelect 俊辑 窍绰 巴苞 吝汗登变 窍绰单 蝶肺 哗扁档 构窍焙..

	UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
	if (DocBattle)
	{// Main/Sub 官层摹扁
		LobbySetPCSelectionClass<EPCClass, EPCClass>::GetInstance().Signal(IntToPCClass(DocBattle->GetTagPCClass()), IntToPCClass(DocBattle->GetCurPCClass()));
		Swap(ChangeMainClass, ChangeSubClass);
		// 急琶 努贰胶啊 函版凳俊 蝶扼 酒赴狼 炼攫档 眉农. 傈捧仿篮 笛捞 钦摹聪 崔扼龙 霸 绝摆瘤父 胶懦器牢飘 鞍篮 芭俊 蝶扼 捞悼 矫 穿备 拳搁栏肺 啊绰瘤 沥档 崔扼瘤摆焙 せ
		UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();
		GameStageCheckForArinConsultingClass<FServerStageID>::GetInstance().Signal(DocBS ? DocBS->GetServerStageId() : FServerStageID());
		PlayChangeChacterAnimBP();

		UpDateHero();
	}
}

void UB2UICharacterSelectPage::OnClickGoHero()
{
	LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_Inventory);
}

void UB2UICharacterSelectPage::OnClickBtnNotifyNoWingIngame()
{
	UB2UIManager::GetInstance()->OpenMsgPopup(
		EUIMsgPopup::Center,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_CinemaWingOff")),
		FText::FromString(TEXT("")),
		1.5f);
}

void UB2UICharacterSelectPage::OnClickAutoSell()
{
	UB2UIManager::GetInstance()->OpenUI<UB2LobbyUI_AutoSell>(UIFName::AutoSell);
}

void UB2UICharacterSelectPage::OnChangedRepeatBattleCurrentOn(class UB2UIDocBase* Sender, bool bCurrentOn, bool bPrevOn)
{
	if (bCurrentOn != bPrevOn && !StageGameStarted)
	{
		// BattleState啊 Prev On -> Current Off 捞扼搁
		if (bCurrentOn == false)
		{
			UB2UIDocBattleStage* DocBS = Cast<UB2UIDocBattleStage>(Sender);
			// 吝窜 荤蜡啊 Gold 何练捞扼搁
			if (DocBS != nullptr && BladeIIGameImpl::GetStageDataStore().IsEnoughGoldToRepeatBattle(ServerStageId) == false)
			{
				// Doc俊 Buff瘤快绊 BuffIcon档 Clear
				TArray<EStageBuffType> EmptyBuffs;
				DocBS->SetSelectedStageBuffs(EmptyBuffs);
				//ClearDisplayBattleStageBuff();
			}
		}

		SetupOnRepeatBattleStateChange(bCurrentOn);
		//UpdateRepeatBattleBtnState(bCurrentOn);
	}
}

void UB2UICharacterSelectPage::OnClickBTNAutoClear()
{
	if (UB2UIManager::GetInstance()->GetUI<UB2UIAutoClearResult>(UIFName::AutoClearResultPopup) == nullptr)
	{
		EPCClass MainChar = EPCClass::EPC_End;
		EPCClass SubChar = EPCClass::EPC_End;
		UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
		if (DocBattle)
		{
			MainChar = IntToPCClass(DocBattle->GetCurPCClass());
			SubChar = IntToPCClass(DocBattle->GetTagPCClass());
		}

		if (MainChar != EPCClass::EPC_End && SubChar != EPCClass::EPC_End && MainChar != SubChar)
		{
			int32 MainPCPresetId = BladeIIGameImpl::GetLocalCharacterData().GetUserAllItems().GetCurrentItemPresetID(MainChar);
			int32 SubPCPresetId = BladeIIGameImpl::GetLocalCharacterData().GetUserAllItems().GetCurrentItemPresetID(SubChar);

			data_trader::Retailer::GetInstance().RequestSweepStage(
				ServerStageId,
				CliToSvrPCClassType(MainChar),
				CliToSvrPCClassType(SubChar),
				MainPCPresetId,
				SubPCPresetId,
				0);
		}
	}
}

void UB2UICharacterSelectPage::OnClickBTNToolTipDisableAutoClear()
{
	if (RTB_LockToolTip.IsValid())
	{
		RTB_LockToolTip->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("BattleStageInfo_UnLock")));
	}

	ToolTipStartAni();
}

bool UB2UICharacterSelectPage::IsBoostOverCount()
{
	auto* DocBattleStage = UB2UIDocHelper::GetDocBattleStage();
	auto IsBoostOver = BoostCount <= 0;
	auto Result = false;

	if (DocBattleStage && DocBattleStage->GetRepeatBattleBoostOn() && IsBoostOver)
	{
		ShortageMGR->PopupGotoShopForStageBoostMessage();
		DocBattleStage->SetRepeatBattleBoostOn(false);

		if (CB_Triple.IsValid())
		{
			CB_Triple->SetCheckedState(ECheckBoxState::Unchecked);
			CB_Triple->SetIsEnabled(false);
		}

		Result = true;
	}

	return Result;
}

bool UB2UICharacterSelectPage::IsRepeatOverCount()
{
	bool Result = false;
	/*
	auto DocBattleStage = UB2UIDocHelper::GetDocBattleStage();
	if (DocBattleStage)
	{
		Result = DocBattleStage->GetRepeatBattleCurrentOn() && DocBattleStage->GetRepeatBattleCurrentRemainCount() <= 0;
	}
	*/
	return Result;
}

void UB2UICharacterSelectPage::CheckStartButton()
{
	FLocalCharacterData& CharDataStore = BladeIIGameImpl::GetLocalCharacterData();
	bool bIsAllSelected = (CharDataStore.GetMainPlayerClass() != EPCClass::EPC_End && CharDataStore.GetSubPlayerClass() != EPCClass::EPC_End);

	if (IsSingleSelect == true && CharDataStore.GetMainPlayerClass() != EPCClass::EPC_End)
	{
		bIsAllSelected = true;
	}

	if (UIP_StartBT.IsValid())
	{
		UIP_StartBT->SetButtonEnabled(bIsAllSelected);
	}
}

bool UB2UICharacterSelectPage::CheckAbleTime()
{
	if (!IsSingleSelect) return true; // IsSingleSelect = true搁 痢飞傈

	auto DocControl = UB2UIDocHelper::GetDocControl();
	if (DocControl)
	{
		TArray<b2network::B2MatchOpenPeriodInfoPtr> OpenTimes;
		bool bIsOpen = BladeIIGameImpl::GetClientDataStore().GetCurrentModeOpenTimes(DocControl->GetPeriodInfos(), OpenTimes);

		if (bIsOpen || IsActiveControlTutorial())
			return true;
	}

	UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
		BladeIIGetLOCText(B2LOC_CAT_ERROR_CODE, TEXT("7603")),
		0.f,
		true,
		true,
		EUIMsgPopupButtonGroup::Confirm
	);

	return false;
}

void UB2UICharacterSelectPage::SetEnableStageGameStarted(bool InEnable)
{
	StageGameStarted = InEnable;
	PageBlock(StageGameStarted);
}

void UB2UICharacterSelectPage::PageBlock(bool bIsBlock)
{
	if (GetRootWidget())
		GetRootWidget()->SetVisibility(bIsBlock ? ESlateVisibility::HitTestInvisible : ESlateVisibility::SelfHitTestInvisible);

	for (int32 SlotIdx = 0; SlotIdx < HeroArr.Num(); SlotIdx++)
	{
		auto& SlotWidget = HeroArr[SlotIdx];
		if (SlotWidget.IsValid())
			SlotWidget->SetSlotSelectable(!bIsBlock);
	}
}

void UB2UICharacterSelectPage::CheckControlTutorialNextStep()
{
	UB2UIManager* pUIManager = UB2UIManager::GetInstance();

	if (pUIManager && pUIManager->GetCurrUIScene() == EUIScene::ControlCharacterSelectPage
		&& IsActiveControlTutorial())
	{
		TutorialManager::GetInstance().GotoNextStep();
	}
}

void UB2UICharacterSelectPage::SetAutoSellBtnState()
{
	SaveDocFilterInfo();

	if (WS_AutoSell.IsValid())
		WS_AutoSell->SetActiveWidgetIndex(IsSetAutoSellBtnState() ? 1 : 0);
}

bool UB2UICharacterSelectPage::IsSetAutoSellBtnState()
{
	bool bCheckedAutoSellCategory = false;
	int32 SaveGradeFilter = 0;
	bool bCheckedAutoSellEtherCategory = false;
	int32 SaveEtherGradeFilter = 0;

	if (GConfig)
	{
		for (int32 i = 0; i < MAX_AUTOSELL_ITEM_CATEGORY; i++)
		{
			GConfig->GetBool(TEXT("AutoSell"), *FString::Printf(TEXT("Category%d"), i), bCheckedAutoSellCategory, GB2UserSavedStateIni);
			if (bCheckedAutoSellCategory)
				break;
		}
		GConfig->GetInt(TEXT("AutoSell"), *FString::Printf(TEXT("Grade")), SaveGradeFilter, GB2UserSavedStateIni);

		GConfig->GetBool(TEXT("AutoSell"), *FString::Printf(TEXT("EtherCategory")), bCheckedAutoSellEtherCategory, GB2UserSavedStateIni);
		GConfig->GetInt(TEXT("AutoSell"), *FString::Printf(TEXT("EtherGrade")), SaveEtherGradeFilter, GB2UserSavedStateIni);
	}

	return (bCheckedAutoSellCategory && (SaveGradeFilter > 0)) || (bCheckedAutoSellEtherCategory && (SaveEtherGradeFilter > 0));
}

void UB2UICharacterSelectPage::SaveDocFilterInfo()
{
	if (GConfig)
	{
		TArray<bool> SaveCategoryFilters;
		SaveCategoryFilters.Empty();
		for (int32 i = 0; i < MAX_AUTOSELL_ITEM_CATEGORY; i++)
		{
			bool bChecked = false;
			GConfig->GetBool(TEXT("AutoSell"), *FString::Printf(TEXT("Category%d"), i), bChecked, GB2UserSavedStateIni);
			SaveCategoryFilters.Add(bChecked);
		}
		int32 SaveGradeFilter = 0;
		GConfig->GetInt(TEXT("AutoSell"), *FString::Printf(TEXT("Grade")), SaveGradeFilter, GB2UserSavedStateIni);

		bool SaveEtherCategoryFilter = false;
		GConfig->GetBool(TEXT("AutoSell"), *FString::Printf(TEXT("EtherCategory")), SaveEtherCategoryFilter, GB2UserSavedStateIni);

		int32 SaveEtherGradeFilter = 0;
		GConfig->GetInt(TEXT("AutoSell"), *FString::Printf(TEXT("EtherGrade")), SaveEtherGradeFilter, GB2UserSavedStateIni);

		if (auto DocSome = UB2UIDocHelper::GetDocSome())
		{
			DocSome->SetOutAutoSellCatergoryFilter(SaveCategoryFilters);
			DocSome->SetOutAutoSellGradeFilter(SaveGradeFilter);
			DocSome->SetOutAutoSellEtherCatergoryFilter(SaveEtherCategoryFilter);
			DocSome->SetOutAutoSellEtherGradeFilter(SaveEtherGradeFilter);
		}
	}
}

void UB2UICharacterSelectPage::SetAutoClear(bool InValue)
{
	if (InValue)
	{
		if (BTN_AutoClear.IsValid())
		{
			BTN_AutoClear->SetIsEnabled(true);
		}

		if (IMG_AutoClear.IsValid())
		{
			IMG_AutoClear->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (HB_AutoClearNUM.IsValid())
		{
			HB_AutoClearNUM->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}

		if (BTN_ToolTipDisableAutoClear.IsValid())
		{
			BTN_ToolTipDisableAutoClear->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	else
	{
		if (BTN_AutoClear.IsValid())
		{
			BTN_AutoClear->SetIsEnabled(false);
		}

		if (IMG_AutoClear.IsValid())
		{
			IMG_AutoClear->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}

		if (HB_AutoClearNUM.IsValid())
		{
			HB_AutoClearNUM->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (BTN_ToolTipDisableAutoClear.IsValid())
		{
			BTN_ToolTipDisableAutoClear->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UB2UICharacterSelectPage::SetAutoClearCount()
{
	if (TB_AutoClearCount.IsValid())
	{
		int64 AutoClearCount = BladeIIGameImpl::GetClientDataStore().GetStageAutoClearTicket();
		TB_AutoClearCount->SetText(FText::Format(FText::FromString(TEXT("x {0}")), FText::AsNumber(AutoClearCount)));
		if (AutoClearCount<=0)
		{
			TB_AutoClearCount->SetColorAndOpacity(FLinearColor::Red);
		}
	}
}

void UB2UICharacterSelectPage::SetCharacterInfo()
{
	for (int32 ClassCnt = 0; ClassCnt < GetMaxPCClassNum(); ++ClassCnt)
	{
		if (auto* DocHero = UB2UIDocHelper::GetDocHero(ClassCnt))
		{
			const EPCClass HeroClassEnum = IntToPCClass(ClassCnt);
			const int32 HeroLevel = BladeIIGameImpl::GetLocalCharacterData().GetCharacterLevel(HeroClassEnum);
			HeroArr[ClassCnt]->SetHeroInfo(HeroClassEnum, HeroLevel, DocHero->GetCombatPower());
			HeroArr[ClassCnt]->SetSlotDelegate(FCharacterSlot::CreateLambda([this](EPCClass InValue) {SlotDelegateFunction(InValue); }));
		}
	}
}

void UB2UICharacterSelectPage::ResponseSweepStage(const FB2SweepStageInfo& InSweepStageinfo)
{
	//b2network::B2AccountInfoPtr Account = GET_TUPLE_DATA(FB2ResponseSweepStage::account_index, InSweepStageinfo);
	//BladeIIGameImpl::GetClientDataStore().SetAccountInfo(Account);
	//BladeIIGameImpl::GetClientDataStore().ClearStage(InSweepStageinfo);
	//SetAutoClearCount();
	//SetCharacterInfo();

	//UB2UIAutoClearResult* Popup = UB2UIManager::GetInstance()->GetUI<UB2UIAutoClearResult>(UIFName::AutoClearResultPopup);
	//if (Popup == nullptr)
	//{
	//	Popup = UB2UIManager::GetInstance()->OpenUI<UB2UIAutoClearResult>(UIFName::AutoClearResultPopup);
	//}

	//if (Popup != nullptr)
	//{
	//	Popup->SetInfo(InSweepStageinfo);
	//}
}

void UB2UICharacterSelectPage::ResponseSweepStageError(const FB2SweepStageInfo& InSweepStageinfo, int32 InErrorCode)
{
	//if (InErrorCode == B2ResultCode::FAILURE_NOT_ENOUGH_STAGE_SWEEP_TICKET)
	//{
	//	UB2UIManager::GetInstance()->GetShortagePopupMGR()->PopupGotoShopForAutoClearTicket();
	//}
	//else if (InErrorCode == B2ResultCode::FAILURE_NOT_ENOUGH_AETHER_INVENTORY_LEFT_SLOT)
	//{
	//	UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
	//	if (UIMgrInst)
	//	{
	//		int32 CurrentCount = GET_TUPLE_DATA(FB2ResponseSweepStage::current_aether_count_index, InSweepStageinfo);
	//		int32 MaxCount = GET_TUPLE_DATA(FB2ResponseSweepStage::current_aether_max_count_index, InSweepStageinfo);
	//		UB2UIEtherFullPopup* Popup = UIMgrInst->OpenUI<UB2UIEtherFullPopup>(UIFName::EtherFullPopup);
	//		Popup->SetEtherFullPopupInfo(CurrentCount, MaxCount);
	//		Popup->SetMoveEtherDelegate([]()
	//		{
	//			UB2UIManager::GetInstance()->CloseUI(UIFName::AutoClearResultPopup);
	//			LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::EtherMain);
	//		});
	//	}
	//}
	//else if (InErrorCode == B2ResultCode::FAILURE_NOT_ENOUGH_INVENTORY_LEFT_SLOT)
	//{
	//	TArray<b2network::B2InventoryCountServerInfoPtr>InvenCountInfo = GET_TUPLE_DATA(FB2ResponseSweepStage::not_enough_inventories_index, InSweepStageinfo);

	//	EPCClass ErrorPCClass = EPCClass::EPC_End;

	//	if (InvenCountInfo.Num() > 0)
	//	{
	//		ErrorPCClass = SvrToCliPCClassType(InvenCountInfo[0]->character_type);
	//	}
	//	if (ErrorPCClass != EPCClass::EPC_End)
	//	{
	//		auto InventoryPopUp = B2UIUtil::OpenInventoryFullPopup(ErrorPCClass, FMsgPopupOnClick::CreateLambda([ErrorPCClass]()
	//		{
	//			UB2UIManager::GetInstance()->CloseUI(UIFName::AutoClearResultPopup);
	//			LobbyHeroMgmtSelectClass<EPCClass>::GetInstance().Signal(ErrorPCClass);
	//			LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_Inventory);
	//			LobbyExternalSetInventoryTabClass<EItemInvenType, bool>::GetInstance().Signal(EItemInvenType::EIIVT_Weapon, false);
	//		}));
	//		if (InventoryPopUp.IsValid() && InvenCountInfo.Num() > 0)
	//		{
	//			InventoryPopUp->SetInventoryItemCount(InvenCountInfo[0]->count, InvenCountInfo[0]->max_count);
	//		}
	//	}
	//}
	//else
	//{
	//	UB2UIManager::GetInstance()->OpenMsgPopupFromErrorCode(InErrorCode);
	//}
}
