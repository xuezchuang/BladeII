// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIHeroTowerReady.h"
#include "B2StageManager.h"
#include "B2UIHeader.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"
#include "Retailer.h"
#include "B2UIMsgPopupUseResources.h"
#include "B2UIMsgPopupHeroTowerSweep.h"
#include "B2UIRewardEventSet.h"
#include "B2UICharacterSelect.h"
#include "B2UIMsgPopupReward.h"
#include "B2UIShortagePopupManager.h"
#include "B2LobbyInventory.h"
#include "B2UIStageResultLevelUp.h"
#include "B2UIRecommandPower.h"
#include "TutorialManager.h"
#include "B2CombatStatEvaluator.h"
#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"
#include "EventSubsystem.h"


UB2UIHeroTowerReady::UB2UIHeroTowerReady(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MoveToInitFloorWaitTime = 0.0f;
	TotalMoveToInitFloorTime = 1.0f;
	MoveToInitFloorBaseSpeed = 0.0f;
	m_nTempHeroPieceAmount = -1;

	bIsSweep = false;

	CreatedUIP_LevelUp = nullptr;
}

void UB2UIHeroTowerReady::Init()
{
	Super::Init();

	LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::HeroTowerReady);

	m_bIsReadyToScroll = false;
	m_fStartScrollOffSet = 0;
	m_bIsSetFloorInfo = false;
	m_nScrollStartIndex = 0;
	m_fTowerPosForMoveToInitFloor = 1.0f;
	m_fMoveToInitFloorStartTime = GetWorld()->GetTimeSeconds();

	m_eUIState = EHeroTowerReadyUIState::MoveTower;

	SetLobbyUIHeaderTitleByGeneralTextTableKey(TEXT("ChallengeMod_HeroTop"));

	SubscribeEvent();
	SetStaticText();
}

void UB2UIHeroTowerReady::OnSceneOpen(EUIScene InOpenedScene)
{
	Super::OnSceneOpen(InOpenedScene);

	// UIHeader 客 鞍捞 Scene 栏肺 备己窍绰 版快 Init 矫痢俊 窍搁 Scene 俊 器窃等 header 啊 积己捞 救等 惑炔老 荐 乐澜.
	SetLobbyUIHeaderTitleByGeneralTextTableKey(TEXT("ChallengeMod_HeroTop"));
	data_trader::Retailer::GetInstance().RequestGetHeroTower();
}

void UB2UIHeroTowerReady::DestroySelf(class UB2UIManager* InUIManager)
{
	UnsubscribeEvent();
	DestroyLevelUpPart();
	if (UIP_RecommendPower.IsValid())
	{
		UIP_RecommendPower->DestroySelf(InUIManager);
	}
}

void UB2UIHeroTowerReady::BindDocAuto()
{

}

void UB2UIHeroTowerReady::UnbindDoc()
{

}

void UB2UIHeroTowerReady::NativeConstruct()
{
	Super::NativeConstruct();

	if (SB_InputTower.IsValid())
	{
		
		int32 nStartFloor = UB2UIDocHelper::GetDocHeroTower()->GetStartFloor();

		UWidget* pDummyWidget = SB_InputTower->GetChildAt(0);
		SB_InputTower->RemoveChildAt(0);

		for (int32 nCnt = nStartFloor; nCnt < BladeIIGameImpl::GetClientDataStore().GetHeroTowerMaxFloor() + 1; ++nCnt)
		{
			UB2UIWidget* pUI = CreateWidget<UB2UIWidget>(GetWorld(), HeroTowerScrollItemClass);
			SB_InputTower->AddChild(pUI);
		}

		SB_InputTower->AddChild(pDummyWidget);

		ResetTowerScroll();

		m_nScrollStartIndex = SB_InputTower->LastAdjustItemIndex;
	}
	

	//喉废UI 积己(楷免 辆丰饶 力芭且芭烙)
	UB2UIManager::GetInstance()->OpenUI(UIFName::BlockScreen);

	// 家帕滚瓢
	if (BTN_StartSweep.IsValid())
	{
		// 坷疵 涝厘且荐乐唱.
		if (UB2UIDocHelper::GetDocHeroTower()->CanSweepToday())
			BTN_StartSweep->SetIsEnabled(true);
		else
			BTN_StartSweep->SetIsEnabled(false);

		// 扁废摸绝栏搁 厚劝己拳
		if(UB2UIDocHelper::GetDocHeroTower()->GetMaxClearFloor() == 0)
			BTN_StartSweep->SetIsEnabled(false);
	}
}

void UB2UIHeroTowerReady::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{	
	Super::NativeTick(MyGeometry, InDeltaTime);

	CheckUIVisible();
	CheckTowerInitMove();
}

void UB2UIHeroTowerReady::SubscribeEvent()
{
	UnsubscribeEvent();

	Issues.Add(DeliverySweepHeroTowerClass<FB2ResponseSweepHeroTowerPtr>::GetInstance().Subscribe2(
		[this](const FB2ResponseSweepHeroTowerPtr& MsgPtr)
	{
		this->AckHeroTowerSweep(MsgPtr);
	}
	));

	Issues.Add(DeliveryBuyHeroTowerTicketClass<FB2ResponseBuyHeroTowerTicketPtr>::GetInstance().Subscribe2(
		[this](const FB2ResponseBuyHeroTowerTicketPtr& MsgPtr)
	{
		this->ResponseBuyHeroTowerTicket(MsgPtr);
	}
	));
}

void UB2UIHeroTowerReady::UnsubscribeEvent()
{
	Issues.Empty();
}

void UB2UIHeroTowerReady::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UB2Button, BTN_StartSweep);
	GET_SLOT(UB2Button, BTN_StartBattle);
	GET_SLOT(UB2Button, BTN_Ranking);
	GET_SLOT(UB2Button, BTN_MyFloor);
	GET_SLOT(UCanvasPanel, CP_BTN_StartSweep);
	GET_SLOT(UCanvasPanel, CP_BTN_StartBattle);
	GET_SLOT(UB2ScrollBox, SB_InputTower);
	GET_SLOT(UTextBlock, TB_Floor);
	//GET_SLOT(UTextBlock, TB_RecordTime);
	GET_SLOT(UTextBlock, TB_CombatPower);
	GET_SLOT(UTextBlock, TB_RecommandPower);
	GET_SLOT(UHorizontalBox, HB_Rewards);
	GET_SLOT(UTextBlock, TB_TodayEnterCount);

	GET_SLOT(UTextBlock, STB_BattleInfo);
	GET_SLOT(UTextBlock, STB_RewardInfo);
	GET_SLOT(UTextBlock, STB_RecommandPower);
	GET_SLOT(UTextBlock, STB_CombatPower);
	GET_SLOT(UTextBlock, STB_BattleReady);
	GET_SLOT(UTextBlock, STB_Sweep);
	GET_SLOT(UTextBlock, STB_Raking);

	GET_SLOT(UCanvasPanel, X_CP_LevelUpPanel);
	
	GET_SLOT(UB2UIRecommandPower, UIP_RecommendPower);
	if (UIP_RecommendPower.IsValid())
	{
		UIP_RecommendPower->Init();
	}

	if (HB_Rewards.IsValid())
		HB_Rewards->ClearChildren();
}

void UB2UIHeroTowerReady::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_StartSweep, &UB2UIHeroTowerReady::OnClickBtnStartSweep);
	BIND_CLICK_FUNC_TO_BTN(BTN_StartBattle, &UB2UIHeroTowerReady::OnClickBtnStartBattle);
	BIND_CLICK_FUNC_TO_BTN(BTN_Ranking, &UB2UIHeroTowerReady::OnClickBtnRanking);
	BIND_CLICK_FUNC_TO_BTN(BTN_MyFloor, &UB2UIHeroTowerReady::OnClickBtnGoCurrentFloor);
}

void UB2UIHeroTowerReady::OnClickBtnStartBattle()
{
	// 痢八 沥魂 眉农
	if (TutorialManager::GetInstance().IsFinishTutorial(TutorialID_HeroTower) && CheckContentsModeState(b2network::B2ContentsMode::HERO_TOWER))
		return;

	if (auto* pDoc = UB2UIDocHelper::GetDocHeroTower())
	{
		// 涝厘 冉荐 眉农
		if (pDoc->GetDailyLeftFreeTryCount() <= 0)
		{
			CreatePopupBuyTryCount();
			return;
		}

		LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ELobbyScene_HeroTowerCharacterSelectPage);
	}
}

void UB2UIHeroTowerReady::CreatePopupBuyTryCount()
{
	UB2UIMsgPopupUseResources* pUI = UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupUseResources>(EUIMsgPopup::UseGem,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("HeroTowerBuyTryCountConfirm")),
		0.f,
		true,
		true,
		EUIMsgPopupButtonGroup::ConfirmOrCancel,
		FMsgPopupOnClick::CreateLambda([this]() {
		//data_trader::Retailer::GetInstance().RequestBuyHeroTowerTicket();
		this->OnConfirmBuyTryCount();

	})
		);

	if (pUI)
	{
		pUI->SetSubContent(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CounterAttack_BuyTicketSubMsg")));

		// 啊拜. 付胶磐单捞磐看绰单. 傍侥栏肺 官柴. 辑滚尔 鞍篮傍侥荤侩.
		int32 nGemCost = GetHerotowerEnterCost();

		pUI->SetCost(nGemCost);
	}
}

int32 UB2UIHeroTowerReady::GetHerotowerEnterCost()
{
	if (auto* pDoc = UB2UIDocHelper::GetDocHeroTower())
	{
		int32 nTryCount = pDoc->GetDailyTryCount() + 1;
		int32 nDifferFromFreeCount = nTryCount - BladeIIGameImpl::GetClientDataStore().GetMaxDailyFreeTryCountHeroTower();

		// 咯扁吧府搁 救凳. 橇府墨款飘 促敬饶何磐 捞侩秦具窃.
		if (nDifferFromFreeCount <= 0)
			return 0;

		return nTryCount * 10 * nDifferFromFreeCount;
	}
	return 0;
}

//todo delete
void UB2UIHeroTowerReady::OnConfirmBuyTryCount()
{
	if (auto* pDoc = UB2UIDocHelper::GetDocHeroTower())
	{
		// 焊籍荤侩秦辑 涝厘且贰.
		// 巢篮焊籍 眉农绰 秦具摆瘤
		int32 nGemCost = BladeIIGameImpl::GetClientDataStore().GetHeroTowerRetryCost(pDoc->GetDailyTryCount() + 1);
		int32 nCurGem = BladeIIGameImpl::GetClientDataStore().GetGemAmount();

		if (nGemCost > nCurGem)
		{
			// 焊籍 绝澜
			ShortageMGR->PopupGoToShopForGemMessage();
			return;
		}
		data_trader::Retailer::GetInstance().RequestBuyHeroTowerTicket();
	}

}

void UB2UIHeroTowerReady::ResponseBuyHeroTowerTicket(const FB2ResponseBuyHeroTowerTicketPtr& MsgPtr)
{
	if (MsgPtr && MsgPtr->possible_try_count > 0)
	{
		if (auto* pDoc = UB2UIDocHelper::GetDocHeroTower())
		{
			pDoc->m_ptrGetHeroTower->current_free_ticket = MsgPtr->current_free_ticket;
			pDoc->m_ptrGetHeroTower->current_buy_ticket = MsgPtr->current_buy_ticket;
			pDoc->m_ptrGetHeroTower->possible_try_count = MsgPtr->possible_try_count;

			pDoc->SetDailyLeftFreeTryCount(MsgPtr->possible_try_count);
		}
		
		BladeIIGameImpl::GetClientDataStore().ReplaceUserDataWithDoc(EDocUserDataType::Gem, MsgPtr->current_cash);
		LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ELobbyScene_HeroTowerCharacterSelectPage);
	}
}

void UB2UIHeroTowerReady::OnClickBtnStartSweep()
{
	//// 痢八 沥魂 眉农
	//if (TutorialManager::GetInstance().IsFinishTutorial(TutorialID_HeroTower) && CheckContentsModeState(b2network::B2ContentsMode::HERO_TOWER))
	//	return;

	//if (!OnCheckAbleToSweep()) return;

	//UB2UIMsgPopupHeroTowerSweep* pSweepPopup = UB2UIManager::GetInstance()->OpenUI<UB2UIMsgPopupHeroTowerSweep>(UIFName::HeroTowerSweepPopup);

	//if (!pSweepPopup)
	//	return;

	////内膏飘尔 悸泼
	//int32 nClearFloor = UB2UIDocHelper::GetDocHeroTower()->GetMaxClearFloor();
	//bIsSweep = true;

	//// GetSweepItems() 啊 辑滚肺 何磐 罐绰 酒捞袍苞 醚钦阑 备秦淋 2俺 捞惑狼 焊惑老版快 酒贰 SetAddComment甫 眠啊肺 备泅
	//pSweepPopup->SetAddCommentForHeroTower(GetSweepItems());

	//pSweepPopup->SetTitle(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Sweep")));
	//pSweepPopup->SetContent(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("HeroTowerDoYouSweepNFloor?")), FText::AsNumber(nClearFloor)));
	//pSweepPopup->SetTipText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("HeroTower_AutoClearDescription")));
	//pSweepPopup->SetButtonText(EUIMsgPopupButton::Positive, BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("HeroTower_AutoClear"))));

	//pSweepPopup->AddHandler(EUIMsgPopupButton::Positive, FMsgPopupOnClick::CreateLambda([this]() { this->OnHeroTowerSweep(); UB2UIManager::GetInstance()->CloseUI(UIFName::HeroTowerSweepPopup); }));
	//pSweepPopup->AddHandler(EUIMsgPopupButton::Negative, FMsgPopupOnClick::CreateLambda([this]() { bIsSweep = false;  UB2UIManager::GetInstance()->CloseUI(UIFName::HeroTowerSweepPopup); this->BTN_StartSweep->SetIsEnabled(true);  }));
}

bool UB2UIHeroTowerReady::OnCheckAbleToSweep()
{

	if (BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass() == EPCClass::EPC_End || BladeIIGameImpl::GetLocalCharacterData().GetSubPlayerClass() == EPCClass::EPC_End)
	{
		UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupSimple>(EUIMsgPopup::Simple,
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("HeroTower_AutoClearCondition")),
			0.f,
			true,
			true,
			EUIMsgPopupButtonGroup::Confirm
			);

		return false;
	}

	return true;
}

const TMap<int, FRewardItem>& UB2UIHeroTowerReady::GetSweepItems()
{
	int32 ClearFloor = UB2UIDocHelper::GetDocHeroTower()->GetMaxClearFloor();
	SweepItems.Empty();

	for (int i = 0; i < ClearFloor; ++i)
	{
		if (auto* pArray = BladeIIGameImpl::GetClientDataStore().GetHeroTowerRewards(i + 1, true))
		{
			for (FRewardItem& RewardItem : *pArray)
			{
				if (RewardItem.Id != 0)
				{
					if (SweepItems.Contains(RewardItem.Id))
					{
						SweepItems[RewardItem.Id].Count += RewardItem.Count;
					}
					else
					{
						SweepItems.Add(RewardItem.Id, RewardItem);
					}
				}
			}
		}
	}

	return SweepItems;
}

void UB2UIHeroTowerReady::OnClickBtnRanking()
{
	// 珐欧UI 凯扁
	UB2UIManager::GetInstance()->OpenUI(UIFName::HeroTowerRanking);
}

void UB2UIHeroTowerReady::OnClickBtnGoCurrentFloor()
{
	if (SB_InputTower.IsValid())
	{
		SB_InputTower->SetAnimateScroll(true);
		ResetTowerScroll();
	}

	SetFloorInfo();
}

void UB2UIHeroTowerReady::HiddenFloorInfo()
{
	m_bIsSetFloorInfo = false;

	// 见扁绰 局聪皋捞记 犁积
	//PlayAnimationEvent_Anim_Invisible();
}

void UB2UIHeroTowerReady::SetFloorInfo()
{
	if (!m_bIsReadyToScroll)
	{
		// 快急 龋免. 唱吝俊 楷免 唱柯饶肺 颗扁具窃
		SetReadyToScroll();
	}	

	UB2UIDocHeroTower* pDoc = UB2UIDocHelper::GetDocHeroTower();
	
	if (!pDoc)
		return;

	int32 nFloor = pDoc->GetStartFloor() + GetCurrentFloorIndex();

	SetFloorInfoValue(nFloor);
		
	if (BTN_StartBattle.IsValid())
	{
		if (GetCurrentFloorIndex() == 0)
			BTN_StartBattle->SetIsEnabled(true);
		else
			BTN_StartBattle->SetIsEnabled(false);
	}
	
	if (TB_TodayEnterCount.IsValid())
	{
		TB_TodayEnterCount->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("HeroTowerTodayEnterCount")), FText::AsNumber(pDoc->GetDailyLeftFreeTryCount()), FText::AsNumber(BladeIIGameImpl::GetClientDataStore().GetMaxHeroTowerTicketCount())));
		
		if(pDoc->GetDailyLeftFreeTryCount() < 1)
			TB_TodayEnterCount->SetColorAndOpacity(UB2UIManager::GetInstance()->TextColor_Decrease);
		else
			TB_TodayEnterCount->SetColorAndOpacity(FLinearColor::White);
	
	}

	// 见变芭 焊捞绰 局聪皋捞记 犁积
	//PlayAnimationEvent_Anim_Visible();
}

void UB2UIHeroTowerReady::SetFloorInfoValue(int32 nFloor)
{
	if (TB_Floor.IsValid())
	{
		if (FCString::Atoi(*TB_Floor->GetText().ToString()) == nFloor) return;

		TB_Floor->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("HeroTower_NFloor")), FText::AsNumber(nFloor)));
	}

	if (TB_CombatPower.IsValid() && TB_RecommandPower.IsValid())
	{
		int32 MainCharCombatPower = 0;
		int32 SubCharCombatPower = 0;

		int32 MainCharAttack = 0;
		int32 MainCharDefense = 0;
		int32 SubCharAttack = 0;
		int32 SubCharDefense = 0;
		
		auto DocBattle = UB2UIDocHelper::GetDocBattle();
		if (DocBattle)
		{
			auto* DocHero_main = UB2UIDocHelper::GetDocHero(DocBattle->GetCurPCClass());
			auto* DocHero_sub = UB2UIDocHelper::GetDocHero(DocBattle->GetTagPCClass());

			if (DocBattle->GetCurPCClass() != (int32)EPCClass::EPC_End && DocHero_main)
			{
				MainCharCombatPower = DocHero_main->GetCombatPower();
				EPCClass CurPCClass = IntToPCClass(DocBattle->GetCurPCClass());
				MainCharAttack = CombatStatEval::GetPCAttack(CurPCClass);
				MainCharDefense = CombatStatEval::GetPCDefense(CurPCClass);
			}

			if (DocBattle->GetTagPCClass() != (int32)EPCClass::EPC_End && DocHero_sub)
			{
				SubCharCombatPower = DocHero_sub->GetCombatPower();
				EPCClass TagPCClass = IntToPCClass(DocBattle->GetTagPCClass());
				SubCharAttack = CombatStatEval::GetPCAttack(TagPCClass);
				SubCharDefense = CombatStatEval::GetPCDefense(TagPCClass);
			}
		}

		int32 nCurrentCombatPower = MainCharCombatPower + SubCharCombatPower;

		TB_CombatPower->SetText(FText::AsNumber(nCurrentCombatPower));

		int32 RecommandPower = BladeIIGameImpl::GetClientDataStore().GetHeroTowerRecommendedCombat(nFloor);

		TB_RecommandPower->SetText(FText::AsNumber(RecommandPower));

		int32 RecommendAttack = 0;
		int32 RecommendDefense = 0;
		BladeIIGameImpl::GetClientDataStore().GetHeroTowerRecommendAbility(nFloor, RecommendAttack, RecommendDefense);

		int32 CurAttack = MainCharAttack + SubCharAttack;
		int32 CurDefense = MainCharDefense + SubCharDefense;

		if (UIP_RecommendPower.IsValid())
		{
			UIP_RecommendPower->SetInfo(CurAttack, CurDefense, RecommendAttack, RecommendDefense);
		}

		TB_CombatPower->SetColorAndOpacity(GetPowerColor(nCurrentCombatPower, RecommandPower));
	}

	if (HB_Rewards.IsValid())
	{
		//瘤快绊
		TArray<UPanelSlot*> Slots = HB_Rewards->GetSlots();

		for (UPanelSlot* pSlot : Slots)
		{
			if (pSlot)
				HB_Rewards->RemoveChild(pSlot->Content);
		}

		//父甸绊

		UB2UIDocHeroTower* pDoc = UB2UIDocHelper::GetDocHeroTower();

		if (!pDoc)
			return;

		TArray<FRewardItem>* pArray = BladeIIGameImpl::GetClientDataStore().GetHeroTowerRewards(nFloor, pDoc->GetMaxClearFloor() >= nFloor);
		
		if (pArray)
		{
			for (FRewardItem& RewardItem : *pArray)
			{
				if (RewardItem.Id != 0)
				{
					UB2UIRewardIcon* pUIP = CreateWidget<UB2UIRewardIcon>(GetOwningPlayer(), RewardItemClass);
					HB_Rewards->AddChild(pUIP);
					pUIP->Init();
					pUIP->UpdateRewardIcon(RewardItem.Id, FText::AsNumber(RewardItem.Count));
				}
			}
			HB_Rewards->ForceLayoutPrepass();
		}
	}
}

void UB2UIHeroTowerReady::ResetTowerScroll()
{
	//if (SB_InputTower.IsValid())
	//{
	//	SB_InputTower->ScrollToEnd();
	//	SB_InputTower->SetNatualScrollEnd();
	//	// 矫累胶农费 炼例窍搁辑 LastAdjustItemIndex档 嘎苗霖促. 场俊 歹固啊 乐绢辑 窍唱 歹哗淋
	//	SB_InputTower->LastAdjustItemIndex = SB_InputTower->GetSlots().Num() - 2;
	//}
}

int32 UB2UIHeroTowerReady::GetCurrentFloorIndex()
{
	if (SB_InputTower.IsValid())
	{
		return m_nScrollStartIndex - SB_InputTower->LastAdjustItemIndex;
	}
	return 0;
}

void UB2UIHeroTowerReady::CheckUIVisible()
{
	//if (SB_InputTower.IsValid())
	//{
	//	int32 LastAdjustIndex = SB_InputTower->LastAdjustItemIndex;

	//	//UE_LOG(LogBladeII, Log, TEXT("LastAdjustIndex = %d"), LastAdjustIndex);

	//	// 眉农矫累
	//	if (SB_InputTower->IsRightClickScrolling())
	//	{
	//		HiddenFloorInfo();
	//	}
	//	if (!m_bIsSetFloorInfo && !SB_InputTower->IsRightClickScrolling())
	//	{
	//		m_bIsSetFloorInfo = true;

	//		UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(TimeToSetFloor);
	//		UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().SetTimer(TimeToSetFloor, this, &UB2UIHeroTowerReady::SetFloorInfo, 0.1);
	//		//SetFloorInfo();
	//	}
	//}
}

void UB2UIHeroTowerReady::CheckTowerInitMove()
{
	float fElapseTime = GetWorld()->GetTimeSeconds() - m_fMoveToInitFloorStartTime - MoveToInitFloorWaitTime;

	if (fElapseTime <= 0)
	{
		bIsSweep = false;
		return;
	}

	if (m_eUIState == EHeroTowerReadyUIState::Ready)
		return;

	// 1 -> 矫累摸 鳖瘤 捞悼窃
	int32 nGoalFloor = UB2UIDocHelper::GetDocHeroTower()->GetStartFloor();

	//1摸捞搁 楷免措扁绝澜
	if (fElapseTime > TotalMoveToInitFloorTime || nGoalFloor == 1)
	{
		if (m_eUIState != EHeroTowerReadyUIState::Ready)
		{
			UB2UIManager::GetInstance()->CloseUI(UIFName::BlockScreen);
			m_eUIState = EHeroTowerReadyUIState::Ready;
		}
		return;
	}
	//else
	//{
	//	m_eUIState = EHeroTowerReadyUIState::MoveTower;
	//}

	float fAccSpeed = ((1.0f / TotalMoveToInitFloorTime) - MoveToInitFloorBaseSpeed) / TotalMoveToInitFloorTime;
	
	// 殿啊加 开犁积窍绰吧肺..
	float fPositionPercent = 1.0f - ((MoveToInitFloorBaseSpeed + (fAccSpeed * ((TotalMoveToInitFloorTime - fElapseTime) / TotalMoveToInitFloorTime))) * ((TotalMoveToInitFloorTime - fElapseTime) / TotalMoveToInitFloorTime));

	m_fTowerPosForMoveToInitFloor = (fPositionPercent * (nGoalFloor - 1)) + 1.1;
	
	if (bIsSweep)
	{
		//家帕捞绊 格钎摸捞 10摸 檬苞搁 泅犁 焊哥临摸 犁汲沥.
		if (nGoalFloor > 10)
		{
			// 1 2 3 4 5 胶诺 (nGoalFloor-4) (nGoalFloor-3) (nGoalFloor-2) (nGoalFloor-1) (nGoalFloor)
			// fPositionPercent 0.0 ~ 1.0
			int32 nNewGoalFloor = 10;

			m_fTowerPosForMoveToInitFloor = (fPositionPercent * (nNewGoalFloor - 1)) + 1.1;

			if (m_fTowerPosForMoveToInitFloor >= 6)
			{
				m_fTowerPosForMoveToInitFloor += nGoalFloor - nNewGoalFloor;
			}			
		}

// 		FNumberFormattingOptions Options;
// 		Options.SetMaximumFractionalDigits(0);
// 		Options.SetRoundingMode(HalfFromZero);
// 		TB_Floor->SetText(FText::AsNumber(m_fTowerPosForMoveToInitFloor, &Options));

		SetFloorInfoValue(int32(m_fTowerPosForMoveToInitFloor));
	}
}

void UB2UIHeroTowerReady::OnHeroTowerSweep()
{
	SendHeroTowerSweep();	
}

void UB2UIHeroTowerReady::SendHeroTowerSweep()
{
	data_trader::Retailer::GetInstance().RequestSweepHeroTower();
}

void UB2UIHeroTowerReady::AckHeroTowerSweep(const FB2ResponseSweepHeroTowerPtr& MsgPtr)
{
	// 胶农费篮 矫累摸(扁废摸)栏肺 嘎苗仇
	if (SB_InputTower.IsValid())
	{
		SB_InputTower->SetAnimateScroll(false);
		ResetTowerScroll();
	}

	// 鸥况 1摸何磐 档崔摸鳖瘤 棵府绰 楷免
	m_fMoveToInitFloorStartTime = GetWorld()->GetTimeSeconds() - MoveToInitFloorWaitTime;
	m_eUIState = EHeroTowerReadyUIState::MoveTowerSweep;

	UB2UIManager::GetInstance()->OpenUI(UIFName::BlockScreen);

	//TotalMoveToInitFloorTime 檬饶俊 扑诀
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(TimeSweepPopupWait);
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().SetTimer(TimeSweepPopupWait, this, &UB2UIHeroTowerReady::OpenSweepResultPopup, TotalMoveToInitFloorTime);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// colosseum commnet : 家帕焊惑篮 康旷狼 炼阿苞 榜靛肺 侨胶 登绢 乐嚼聪促.
	// 家帕焊惑篮 府况靛 鸥涝 府胶飘 屈怕肺 傈崔 邓聪促.
	for (auto reward : MsgPtr->rewards)
	{
		if (reward->item != nullptr)
		{
			if (reward->item->template_id == FItemRefIDHelper::ITEM_REF_ID_HERO_PIECE) // 康旷狼炼阿
			{
				m_nTempHeroPieceAmount = reward->raw_reward->count;
			}
		}
		BladeIIGameImpl::GetClientDataStore().ApplyRewardInfo(reward);
	}

	if (BTN_StartSweep.IsValid())
		BTN_StartSweep->SetIsEnabled(false);

	auto DocHeroTower = UB2UIDocHelper::GetDocHeroTower();
	if (DocHeroTower)
		UB2UIDocHelper::GetDocHeroTower()->ForcedSetSweepToday(false);

}

void UB2UIHeroTowerReady::OpenSweepResultPopup()
{
	if (m_nTempHeroPieceAmount == -1)
		return;

	UB2UIMsgPopupReward* RewardPopup = UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupReward>(EUIMsgPopup::ItemReward, FText::GetEmpty(), FText::GetEmpty());
	if (RewardPopup != nullptr)
	{
		FText TitleText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("heroTower_AutoClearSuccess"));
		RewardPopup->InitRewardMsgPopupText(TitleText, BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Common_RewardedDesc")));
		RewardPopup->UpdateDescriptionText(FText::GetEmpty());

		for (auto& SweepItemReward : SweepItems)
		{
			int32 itemid = BladeIIGameImpl::GetClientDataStore().GetRewardItemId(SweepItemReward.Key);
			RewardPopup->AddSingleRewardItem(itemid, SweepItemReward.Value.Count);
		}

		RewardPopup->ShowRewardPopup(true);
	}

	m_nTempHeroPieceAmount = -1;
}

float UB2UIHeroTowerReady::GetTowerPosition()
{
	if (m_eUIState == EHeroTowerReadyUIState::MoveTower || m_eUIState == EHeroTowerReadyUIState::MoveTowerSweep)
	{
		UE_LOG(LogBladeII, Log, TEXT("fTowerPos = %f.2"), m_fTowerPosForMoveToInitFloor);
		return m_fTowerPosForMoveToInitFloor;
	}

	if (!SB_InputTower.IsValid())
		return 0.0f;

	float fTowerPos = 0.0f;

	TArray<UPanelSlot*> ScrollSlots = SB_InputTower->GetSlots();

	float fScrollItemSize = 1.0f;

	if (ScrollSlots.Num() >= 2)
	{
		int32 nScrollIndex = 1;
		//fScrollItemSize = ScrollSlots[nScrollIndex]->Content->GetDesiredSize().Y + Cast<UScrollBoxSlot>(ScrollSlots[nScrollIndex])->Padding.Top + Cast<UScrollBoxSlot>(ScrollSlots[nScrollIndex])->Padding.Bottom;
	}
		
	fTowerPos = (m_fStartScrollOffSet - SB_InputTower->GetScrollPhysicalOffset()) / fScrollItemSize;

	//UE_LOG(LogBladeII, Log, TEXT("fTowerPos = %3.2"), fTowerPos);

	// BP俊辑 泅犁 摸阑 舅鞘夸啊 乐绢辑 歹秦霖促.(歹固 皋浆甸档 捞悼捞 鞘夸秦咙..)
	int32 nGoalFloor = UB2UIDocHelper::GetDocHeroTower()->GetStartFloor();

	return fTowerPos + float(nGoalFloor);
}

void UB2UIHeroTowerReady::SetReadyToScroll()
{
	//m_bIsReadyToScroll = true;

	//if (SB_InputTower.IsValid())
	//{
	//	TArray<UPanelSlot*> ScrollSlots = SB_InputTower->GetSlots();

	//	float fScrollItemSize = 1.0f;

	//	if (ScrollSlots.Num() >= 2)
	//	{			
	//		int32 nScrollIndex = 1;
	//		fScrollItemSize = ScrollSlots[nScrollIndex]->Content->GetDesiredSize().Y + Cast<UScrollBoxSlot>(ScrollSlots[nScrollIndex])->Padding.Top + Cast<UScrollBoxSlot>(ScrollSlots[nScrollIndex])->Padding.Bottom;

	//		m_fStartScrollOffSet = (ScrollSlots.Num() - 3) * fScrollItemSize;
	//	}
	//}
}

void UB2UIHeroTowerReady::SetStaticText()
{
	if (STB_BattleInfo.IsValid())
		STB_BattleInfo->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("HeroTower_BattleInfo")));

	if (STB_RewardInfo.IsValid())
		STB_RewardInfo->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("BattleStageInfo_RewardInfo")));

	if (STB_RecommandPower.IsValid())
		STB_RecommandPower->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_RecomCombatPower")));

	if (STB_CombatPower.IsValid())
		STB_CombatPower->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_CurrentCombatpower")));

	if (STB_BattleReady.IsValid())
		STB_BattleReady->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ChallengeMod_TeamMatchBattleReady")));

	if (STB_Sweep.IsValid())
		STB_Sweep->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Sweep")));

	if (STB_Raking.IsValid())
		STB_Raking->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Ranking")));
}


//////////////////////////////////////////////////////////
// 咯扁俊 LevelUpPart 啊 竭蹲嘎霸 乐绰单.. Result 俊 乐绰 吧肺 等 巴 鞍篮单 角荐牢 扒啊..? 咯窍埃 贸澜何磐 肺爹窍瘤绰 臼档废 窃.

void UB2UIHeroTowerReady::CheckAndOpenLevelUpPart()
{
	FLocalCharacterData& CharacterDataStore = BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData();

	TArray<EPCClass> ThisPCClass;

	ThisPCClass.Add(CharacterDataStore.GetMainPlayerClass());
	ThisPCClass.Add(CharacterDataStore.GetSubPlayerClass());

	for (auto pcclass : ThisPCClass)
	{
		//[@SameLine, 180612] explevelinfo doc, datastore -> datastore 俺急
		//UB2UIDocHero* DocHero = UB2UIDocHelper::GetDocHero(PCClassToInt(pcclass));
		//bool bCharLevelUp = (FMath::Max(DocHero->GetBasicUpgradeLevel(), DocHero->GetExtraUpgradeLevel()) > DocHero->GetCurrentLevel());
		const int32 ClearAfterLevel = CharacterDataStore.GetCharacterLevel(pcclass);
		const int32 ClearBeforeLevel = CharacterDataStore.GetBeforeCharacterLevel(pcclass);
		if (ClearAfterLevel > ClearBeforeLevel)
		{
			ConditionalCreateLevelUpPart();

			if (CreatedUIP_LevelUp)
			{
				CreatedUIP_LevelUp->StartMe(ThisPCClass);
				//CreatedUIP_LevelUp->LevelupFinishDelegate = FOnLevelUpFXFinish::CreateLambda([this]() { this->OnLevelUpFXFinish(); });
				return;
			}
		}
	}

	//OnLevelUpFXFinish();
}

void UB2UIHeroTowerReady::ConditionalCreateLevelUpPart()
{
	//if (!CreatedUIP_LevelUp && X_CP_LevelUpPanel.IsValid())
	//{
	//	CreatedUIP_LevelUp = DynLoadAndCreateInCanvasPanelFull<UB2UIStageResultLevelUp>(UIP_LevelUpClass, this, X_CP_LevelUpPanel.Get());
	//	if (CreatedUIP_LevelUp)
	//	{
	//		CreatedUIP_LevelUp->Init();
	//	}
	//}
}
void UB2UIHeroTowerReady::DestroyLevelUpPart()
{
	if (CreatedUIP_LevelUp)
	{
		CreatedUIP_LevelUp->DestroySelf(UB2UIManager::GetInstance());
		CreatedUIP_LevelUp = nullptr;
	}
}