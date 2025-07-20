// Fill out your copyright notice in the Description page of Project Settings.

#include "B2LobbyUI_WingMain.h"
#include "B2LobbyGameMode.h"
#include "B2LobbyUISwitcher.h"
#include "B2UIManager_Lobby.h"
#include "B2Airport.h"
#include "B2LobbyUI_HeroMgmtCommon.h"
#include "Event.h"
#include "Retailer.h"
#include "B2PCClassInfo.h"
#include "B2UIDocHelper.h"
#include "B2UILinkManager.h"
#include "B2UIBattleScoreNotify.h"
#include "B2UIUpgradeStatNotify.h"
#include "B2UIShortagePopupManager.h"
#include "B2AndroidBackManager.h"

#include "BladeIIGameImpl.h"

UB2LobbyUI_WingMain::UB2LobbyUI_WingMain(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CachedAirport = NULL;

	CreatedEnhancementPart = nullptr;
	CreatedEvolutionPart = nullptr;
}

void UB2LobbyUI_WingMain::CacheAssets()
{
	Super::CacheAssets();

	B2AndroidBackManager::GetInstance()->AddBackWidget(this);

	GET_SLOT(UB2LobbyUI_HeroMgmtCommon, UIP_HeroMgmtBase);

	GET_SLOT(UB2WingUIP_MainSymbol, UIP_WingMenuUpper_MainSymbol);

	GET_SLOT(UTextBlock, TB_AttackIncLabel);
	GET_SLOT(UTextBlock, TB_AttackIncAmount);
	GET_SLOT(UTextBlock, TB_DefenseIncLabel);
	GET_SLOT(UTextBlock, TB_DefenseIncAmount);
	GET_SLOT(UTextBlock, TB_HealthIncLabel);
	GET_SLOT(UTextBlock, TB_HealthIncAmount);

	GET_SLOT(UB2Button, BTN_AutoEnhanceOrEvolve);
	BIND_CLICK_FUNC_TO_BTN(BTN_AutoEnhanceOrEvolve, &UB2LobbyUI_WingMain::OnClickedAutoEnhanceOrEvolveBtn);
	GET_SLOT(UB2Button, BTN_EnhanceOrEvolve);
	BIND_CLICK_FUNC_TO_BTN(BTN_EnhanceOrEvolve, &UB2LobbyUI_WingMain::OnClickedEnhanceOrEvolveBtn);
	GET_SLOT(UB2Button, BTN_WingBook);
	BIND_CLICK_FUNC_TO_BTN(BTN_WingBook, &UB2LobbyUI_WingMain::OnClickedWingBookBtn);

	GET_SLOT(UTextBlock, TB_AutoEnhanceOrEvolveBtn);
	GET_SLOT(UTextBlock, TB_EnhanceOrEvolveBtn);
	GET_SLOT(UTextBlock, TB_EnhanceOrEvolveCost);
	GET_SLOT(UTextBlock, TB_WingBookBtn);

	GET_SLOT(UCanvasPanel, X_CP_EnhanceOrEvolutionBase);
}

void UB2LobbyUI_WingMain::BeginDestroy()
{
	//Editor 惑炔俊辑 巩力啊 粱 乐绢辑 咯扁辑档 unsubscribe
	if (CachedLobbyGM)
	{
		LobbySetHeroMgmtModePCSelectionClass<EPCClass>::GetInstance().Unsubscribe(LobbySetHeroMgmtModePCSelectionTicket);
	}

	Super::BeginDestroy();
}

void UB2LobbyUI_WingMain::DestroySelf()
{
	if (UIP_HeroMgmtBase.IsValid())
	{
		UIP_HeroMgmtBase->DestroySelf();
	}
	if (UIP_WingMenuUpper_MainSymbol.IsValid())
	{
		UIP_WingMenuUpper_MainSymbol->DestroySelf();
	}

	LobbySetHeroMgmtModePCSelectionClass<EPCClass>::GetInstance().Unsubscribe(LobbySetHeroMgmtModePCSelectionTicket);
	
	DestroyEnhancementPart();
	DestroyEvolutionPart();

	B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);

	Super::DestroySelf();
}

void UB2LobbyUI_WingMain::StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
{
	Super::StartFromLobby(InUIManager, InGM);

	CAPTURE_UOBJECT(UB2LobbyUI_WingMain);
	LobbySetHeroMgmtModePCSelectionTicket = LobbySetHeroMgmtModePCSelectionClass<EPCClass>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(LobbySetHeroMgmtModePCSelection, EPCClass PCClass)
		Capture->OnHeroMgmtPCChanged(PCClass);
	END_CAPTURE_OBJECT()
		);

	if (UIP_HeroMgmtBase.IsValid())
	{
		UIP_HeroMgmtBase->StartFromLobby(InUIManager, InGM); // 捞扒 LobbyUISwitcher 俊辑 葛福聪 流立 秦 林绢具..
	}
	if (UIP_WingMenuUpper_MainSymbol.IsValid())
	{
		UIP_WingMenuUpper_MainSymbol->StartFromLobby(InUIManager, InGM);
	}

	// 盔贰 家葛前篮 贸澜俊 啊廉客具 窍绰单.. 吝埃俊 粱 巩力啊 积败辑 咯扁档 犬牢 瞒..
	BladeIIGameImpl::GetClientDataStore().SyncingInventoryData(EPCClass::EPC_End); //EPC_End 肺 林搁 傍侩 酒捞袍阑 夸没且 巴.

	CachedAirport = CachedLobbyGM->GetLobbyAirport();
	check(CachedAirport);
		
	// Wing UI 拳搁俊辑 鞘夸肺 窍绰 眠啊 朝俺 单捞磐甫 器窃茄 夸没..
	CachedAirport->ConditionalReqForFullWingData(CachedLobbyGM->GetHeroMgmtModePCClass());
	
	UB2Airport::ConditionalSetWingVisibleOnWingPageOpen(); // 父老 夸 其捞瘤甫 贸澜 凯菌促搁 朝俺 visibility 甫 葛滴 难 霖促.
	
	UpdateStaticWidgets();
	UpdateByCurrentWingData();

	SetLobbyUIHeaderTitle(BladeIIGetLOCText(B2LOC_CAT_WINGTEXT, TEXT("Wing_MainTitle")));
}

void UB2LobbyUI_WingMain::UpdateByCurrentWingData()
{
	EPCClass SelectedPCClass = CachedLobbyGM ? CachedLobbyGM->GetHeroMgmtModePCClass() : EPCClass::EPC_End;
	FB2Wing CurrentWingData;
	// 咯扁辑 角力肺 绢恫 可记阑 哗棵瘤绰 崔扼龙 荐 乐澜. 俺急秦具 窃.
	const bool bGotValidWingData = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWing(SelectedPCClass, CurrentWingData);
	const bool bInSomeAutoTask = CachedAirport ? CachedAirport->IsInAutoEvolOrEnhTask() : false; // True 搁 滚瓢甸阑 粱 见败具
	if (bGotValidWingData)
	{
		if (UIP_WingMenuUpper_MainSymbol.IsValid())
		{
			UIP_WingMenuUpper_MainSymbol->UpdateByCurrentWingData();
			UIP_WingMenuUpper_MainSymbol->SetExtraDisableButtons(bInSomeAutoTask);
		}

		// 碍拳尔 柳拳 皋春绰 笛 吝 窍唱父
		const bool bNeedToShowMaxEvolveMenu = NeedToShowMaxEvolveMenu(); // 捞 版快绰 碍拳/柳拳 滚瓢 葛滴 力芭.
		const bool bNeedToShowEvolveMenu = NeedToShowEvolveMenu();
		
		// 碍拳 趣篮 柳拳 皋春 吝 窍唱父. On-demand 肺 肺爹 棺 积己.
		if (bNeedToShowEvolveMenu)
		{
			SetOpenEnhancementPart(false);
			SetOpenEvolutionPart(true);
			if (CreatedEvolutionPart){
				CreatedEvolutionPart->UpdateByWingData(CurrentWingData);
			}
		}
		else
		{
			SetOpenEnhancementPart(true);
			SetOpenEvolutionPart(false);
			if (CreatedEnhancementPart){
				CreatedEnhancementPart->UpdateByWingData(CurrentWingData);
			}
		}
		
		if (TB_AttackIncAmount.IsValid())
		{
			// 咆胶飘 鞘靛俊 甸绢乐绰 Upgrade傈 胶泡 蔼 捞固 历厘
			BeforeAttPoint = FCString::Atoi(*TB_AttackIncAmount->GetText().ToString());
			TB_AttackIncAmount->SetText(
				FText::FromString(FString::Printf(TEXT("%d"), (int32)CurrentWingData.GetAttackPoint()))
				);
		}
		if (TB_DefenseIncAmount.IsValid())
		{
			// 咆胶飘 鞘靛俊 甸绢乐绰 Upgrade傈 胶泡 蔼 捞固 历厘
			BeforeDefPoint = FCString::Atoi(*TB_DefenseIncAmount->GetText().ToString());
			TB_DefenseIncAmount->SetText(
				FText::FromString(FString::Printf(TEXT("%d"), (int32)CurrentWingData.GetDefensePoint()))
				);
		}
		if (TB_HealthIncAmount.IsValid())
		{
			// 咆胶飘 鞘靛俊 甸绢乐绰 Upgrade傈 胶泡 蔼 捞固 历厘
			BeforeHPPoint = FCString::Atoi(*TB_HealthIncAmount->GetText().ToString());
			TB_HealthIncAmount->SetText(
				FText::FromString(FString::Printf(TEXT("%d"), (int32)CurrentWingData.GetHealthPoint()))
				);
		}

		int32 RequiredGoldCost = 0, RequiredItemCount = 0;
		if (bNeedToShowEvolveMenu)
		{
			int32 MaxFailPoint = 0;
			float SuccessRate = 0.0f;
			BladeIIGameImpl::GetClientDataStore().GetWingEvolveRefData(CurrentWingData, RequiredItemCount, RequiredGoldCost, SuccessRate, MaxFailPoint);
		}
		else
		{
			int32 EnhancePointToAcquire = 0, RequiredEnhancePoint = 0;
			BladeIIGameImpl::GetClientDataStore().GetWingEnhanceRefData(CurrentWingData, RequiredItemCount, RequiredGoldCost, EnhancePointToAcquire, RequiredEnhancePoint);
		}
		// 弥措 柳拳啊 等 惑怕俊辑绰 滚瓢甸 见变促.
		if (BTN_AutoEnhanceOrEvolve.IsValid())
		{
			BTN_AutoEnhanceOrEvolve->SetVisibility(bNeedToShowMaxEvolveMenu ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
		}
		if (BTN_EnhanceOrEvolve.IsValid())
		{
			BTN_EnhanceOrEvolve->SetVisibility(bNeedToShowMaxEvolveMenu ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
			BTN_EnhanceOrEvolve->SetIsEnabled(!bInSomeAutoTask);
		}

		if (!bNeedToShowMaxEvolveMenu)
		{
			// 咯扁档 付蛮啊瘤肺 朝俺 诀弊饭捞靛 惑怕俊 蝶扼 柳拳 趣篮 碍拳肺 钎矫..
			UpdateAutoEnhanceOrEvolveBtnText();

			if (TB_EnhanceOrEvolveBtn.IsValid())
			{
				TB_EnhanceOrEvolveBtn->SetText(
					bNeedToShowEvolveMenu ? BladeIIGetLOCText(B2LOC_CAT_WINGTEXT, TEXT("Wing_Evolution")) :
					BladeIIGetLOCText(B2LOC_CAT_WINGTEXT, TEXT("Wing_Enhancement"))
					);
			}
			if (TB_EnhanceOrEvolveCost.IsValid())
			{
				//[@AKI, 170417] 困俊辑 秦林聪鳖 咯扁辑绰 牢力 窍瘤 富磊
				/*
				int32 RequiredGoldCost = 0;
				if (bNeedToShowEvolveMenu)
				{
					int32 RequiredItemCount = 0, MaxFailPoint = 0;
					float SuccessRate = 0.0f;
					BladeIIGameImpl::GetClientDataStore().GetWingEvolveRefData(CurrentWingData, RequiredItemCount, RequiredGoldCost, SuccessRate, MaxFailPoint);
				}
				else
				{
					int32 RequiredItemCount = 0, EnhancePointToAcquire = 0, RequiredEnhancePoint = 0;
					BladeIIGameImpl::GetClientDataStore().GetWingEnhanceRefData(CurrentWingData, RequiredItemCount, RequiredGoldCost, EnhancePointToAcquire, RequiredEnhancePoint);
				}
				*/
				TB_EnhanceOrEvolveCost->SetText(FText::FromString(FString::FormatAsNumber(RequiredGoldCost)));
			}
		}
	}
	else
	{
		// 林夸 stat 甸篮 见扁绰 措脚 傲 0 栏肺
		if (TB_AttackIncAmount.IsValid()){
			TB_AttackIncAmount->SetText(FText::AsNumber(0));
		}
		if (TB_DefenseIncAmount.IsValid()){
			TB_DefenseIncAmount->SetText(FText::AsNumber(0));
		}
		if (TB_HealthIncAmount.IsValid()){
			TB_HealthIncAmount->SetText(FText::AsNumber(0));
		}
		SetOpenEvolutionPart(false);
		SetOpenEnhancementPart(false);
		if (BTN_AutoEnhanceOrEvolve.IsValid())
		{
			BTN_AutoEnhanceOrEvolve->SetVisibility(ESlateVisibility::Collapsed);
		}
		if (BTN_EnhanceOrEvolve.IsValid())
		{
			BTN_EnhanceOrEvolve->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if (BTN_WingBook.IsValid())
	{
		BTN_WingBook->SetIsEnabled(!bInSomeAutoTask);
	}
	
	if (UIP_HeroMgmtBase.IsValid())
	{ // 傍烹 颇飘俊档 滚瓢捞 腹栏聪..
		UIP_HeroMgmtBase->SetExtraDisableButtons(bInSomeAutoTask);
		UIP_HeroMgmtBase->DoMarkRedDot();
	}
}

void UB2LobbyUI_WingMain::PlayOnEnhancedAnim()
{
	// EnhancePoint 酒囱 EnhanceLevel 刘啊矫俊 阂府绰 芭
	// 颇飘 率俊辑档 蝶肺 局聪皋捞记 倒覆.
	if (CreatedEnhancementPart && !NeedToShowEvolveMenu())
	{
		CreatedEnhancementPart->PlayLevelupAnim();
	}

	SetBattleScoreNotify();
	ShowUpgradeStatNotify();
	PlayOnEnhancedAnim_BP();
}

void UB2LobbyUI_WingMain::BeginShowEvolutionFailMessage()
{
	if (CreatedEvolutionPart)
	{
		CreatedEvolutionPart->BeginShowEvolutionFailMessage();
	}

	PlayOnFailAnim_BP();
}

void UB2LobbyUI_WingMain::OnFailEndAnimation_BP()
{

}

void UB2LobbyUI_WingMain::UpdateOnAutoEnhanceComplete()
{
	UpdateByCurrentWingData(); // 滚瓢 state 尔 巩备甸 汗备..
}

void UB2LobbyUI_WingMain::UpdateOnAutoEvolveComplete()
{
	if (CreatedEvolutionPart)
	{ // 磊悼 柳拳 档吝 角菩窍搁辑 焊咯瘤绊 乐菌阑 荐 乐促.
		CreatedEvolutionPart->ForceStopShowEvolutionFailMessage();
	}
	UpdateByCurrentWingData(); // 滚瓢 state 尔 巩备甸 汗备..
}

void UB2LobbyUI_WingMain::CloseWidgetDelegate()
{
	const bool bAutoEnhancement = CachedAirport ? CachedAirport->IsInAutoEnhancement() : false; 
	const bool bAutoEvolution = CachedAirport ? CachedAirport->IsInAutoEvolution() : false;

	if (bAutoEvolution == true)
	{
		CachedAirport->CustomEndAutoEvolution();
	}
	else if (bAutoEnhancement == true)
	{
		CachedAirport->CustomEndAutoItemEnhancement();
	}
	else if (UIP_HeroMgmtBase.IsValid())
	{
		UIP_HeroMgmtBase->HeaderBackButton();
	}
}

void UB2LobbyUI_WingMain::UpdateStaticWidgets()
{
	if (TB_AttackIncLabel.IsValid())
	{
		TB_AttackIncLabel->SetText(BladeIIGetLOCText(B2LOC_CAT_WINGTEXT, TEXT("Wing_Option_AttackInc")));
	}
	if (TB_DefenseIncLabel.IsValid())
	{
		TB_DefenseIncLabel->SetText(BladeIIGetLOCText(B2LOC_CAT_WINGTEXT, TEXT("Wing_Option_DefenseInc")));
	}
	if (TB_HealthIncLabel.IsValid())
	{
		TB_HealthIncLabel->SetText(BladeIIGetLOCText(B2LOC_CAT_WINGTEXT, TEXT("Wing_Option_HealthInc")));
	}

	if (TB_WingBookBtn.IsValid())
	{
		TB_WingBookBtn->SetText(BladeIIGetLOCText(B2LOC_CAT_WINGTEXT, TEXT("Wing_Book")));
	}
}

void UB2LobbyUI_WingMain::RegisterUIMarkForRedDot()
{
//	BladeIIGameImpl::GetRedDotManager().AddUI(EUICategoryMarkForNew::Wing, this);
}

void UB2LobbyUI_WingMain::UnRegisterUIMarkForRedDot()
{
//	BladeIIGameImpl::GetRedDotManager().RemoveUI(EUICategoryMarkForNew::Wing, this);
}

void UB2LobbyUI_WingMain::DoMarkRedDot()
{
	/*
	// 康旷包府 傍烹 何盒栏肺 某腐磐 喊 眉农秦具 窍绰 芭
	if (UIP_HeroMgmtBase.IsValid())
	{
		for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
		{
			EPCClass CastedPCClass = IntToPCClass(PCI);
			UIP_HeroMgmtBase->ShowNewMark(CastedPCClass, BladeIIGameImpl::GetRedDotManager().ShouldMarkForNewWing(CastedPCClass));
		}
	}
	*/

	// 加己可记 酒捞能 率俊档 鞘夸..
}

bool UB2LobbyUI_WingMain::NeedToShowEvolveMenu() const
{
	EPCClass SelectedPCClass = CachedLobbyGM ? CachedLobbyGM->GetHeroMgmtModePCClass() : EPCClass::EPC_End;
	FB2Wing CurrentWingData;
	const bool bGotValidWingData = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWing(SelectedPCClass, CurrentWingData);
	if (bGotValidWingData)
	{ // NeedToShowMaxEvolveMenu 牢 版快档 器窃.
		return (IsItTimeToEvolve(CurrentWingData) || IsUltimatelyEvolvedWing(CurrentWingData));
	}
	return false;
}

bool UB2LobbyUI_WingMain::NeedToShowMaxEvolveMenu() const
{ 
	EPCClass SelectedPCClass = CachedLobbyGM ? CachedLobbyGM->GetHeroMgmtModePCClass() : EPCClass::EPC_End;
	FB2Wing CurrentWingData;
	const bool bGotValidWingData = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWing(SelectedPCClass, CurrentWingData);
	if (bGotValidWingData)
	{
		return IsUltimatelyEvolvedWing(CurrentWingData);
	}
	return false;
}

void UB2LobbyUI_WingMain::UpdateAutoEnhanceOrEvolveBtnText()
{ // 磊悼碍拳 / 磊悼碍拳秦力 / 磊悼柳拳 / 磊悼柳拳秦力 惑炔俊 蝶扼 利例茄 吧肺 技泼
	if (CachedAirport && TB_AutoEnhanceOrEvolveBtn.IsValid())
	{
		const bool bNeedToShowEvolveMenu = NeedToShowEvolveMenu();

		TB_AutoEnhanceOrEvolveBtn->SetText(
			BladeIIGetLOCText(B2LOC_CAT_WINGTEXT, 
				CachedAirport->IsInAutoEvolution() ? TEXT("Wing_StopAutoEvolve") : 
					(CachedAirport->IsInAutoEnhancement() ? TEXT("Wing_StopAutoEnhance") : 
						(bNeedToShowEvolveMenu ? TEXT("Wing_AutoEvolve") : TEXT("Wing_AutoEnhance")
				))
			));
	}
}

void UB2LobbyUI_WingMain::OnClickedAutoEnhanceOrEvolveBtn()
{
	if (CachedAirport && CachedLobbyGM)
	{
		// 泅犁 磊悼碍拳 趣篮 磊悼柳拳 档吝捞搁 吝窜窍档废 秦具.
		if (CachedAirport->IsInAutoEvolution())
		{
			CachedAirport->CustomEndAutoEvolution();
		}
		else if (CachedAirport->IsInAutoEnhancement())
		{
			CachedAirport->CustomEndAutoItemEnhancement();
		}
		else
		{
			if (NeedToShowEvolveMenu())
			{
				if (!NeedToShowMaxEvolveMenu()) // 绢瞒乔 捞 版快搁 滚瓢 努腐捞 登瘤 臼酒具 窍瘤父..
				{ // 磊悼 柳拳
					// 抛胶飘 啊瓷秦瘤搁 困俊 DevShowNotReadyYetMessage 绝局绊 酒贰 何盒 钱绢辑 抛胶飘
					CachedAirport->BeginAutoEvolution(CachedLobbyGM->GetHeroMgmtModePCClass());
				}
			}
			else
			{ // 磊悼 碍拳
				CachedAirport->BeginAutoEnhancement(CachedLobbyGM->GetHeroMgmtModePCClass());
			}
		}

		UpdateByCurrentWingData(); // 磊悼 碍拳/柳拳 矫累/辆丰俊 蝶扼 诀单捞飘 且 芭府甸捞 蚕 乐促.
	}
}

void UB2LobbyUI_WingMain::OnClickedEnhanceOrEvolveBtn()
{
	EPCClass SelectedPCClass = CachedLobbyGM ? CachedLobbyGM->GetHeroMgmtModePCClass() : EPCClass::EPC_End;
	FB2Wing CurrentWingData;
	// 咯扁辑 角力肺 绢恫 可记阑 哗棵瘤绰 崔扼龙 荐 乐澜. 俺急秦具 窃.
	const bool bGotValidWingData = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWing(SelectedPCClass, CurrentWingData);

	if (bGotValidWingData && !IsUltimatelyEvolvedWing(CurrentWingData))
	{
		int32 SvrOwnerCharType = CliToSvrPCClassType(IntToPCClass(CurrentWingData.OwnerPCClass));

		// 阿阿狼 版快俊 荤侩磊 焊蜡 榜靛唱 犁丰 酒捞袍俊 蝶扼 阂啊瓷窍搁 扑诀阑 剁况具 茄促.

		if (IsItTimeToEvolve(CurrentWingData))
		{ // 犁拳 何练狼 版快 荤侩磊 泅龙 蜡档.
			if (!UB2Airport::HasEnoughGoldForWingEvolveLocalPlayer(SelectedPCClass)){
				ShortageMGR->PopupGoToShopForGoldMessage();
			}
			else if (!UB2Airport::HasEnoughIngredForWingEvolveLocalPlayer(SelectedPCClass)){
				// 犁丰 裙垫 版肺 扑诀.
				OpenWingIngredAcqPathPopup(EWingIngredAcqElemType::WAET_EvolveIngred);
			}
			else{ // 啊瓷窍促绊 魄窜登搁 辑滚肺 req.
				data_trader::Retailer::GetInstance().RequestUpgradeWing(SvrOwnerCharType);
			}
		}
		else
		{// 犁拳 何练狼 版快 荤侩磊 泅龙 蜡档.
			if (!UB2Airport::HasEnoughGoldForWingEnhanceLocalPlayer(SelectedPCClass)){
				ShortageMGR->PopupGoToShopForGoldMessage();
			}
			else if (!UB2Airport::HasEnoughIngredForWingEnhanceLocalPlayer(SelectedPCClass)){
				// 犁丰 裙垫 版肺 扑诀.
				OpenWingIngredAcqPathPopup(EWingIngredAcqElemType::WAET_EnhanceIngred);
			}
			else{ // 啊瓷窍促绊 魄窜登搁 辑滚肺 req.
				data_trader::Retailer::GetInstance().RequestEnhanceWing(SvrOwnerCharType);
			}
		}
	}
}

void UB2LobbyUI_WingMain::OnClickedWingBookBtn()
{
	if (UIP_HeroMgmtBase.IsValid())
	{
		EPCClass WingPCClass = EPCClass::EPC_End;
		WingPCClass = UIP_HeroMgmtBase->GetCurrentPCClass();
		FB2UILinkManager::LinkUIScene(EUIScene::CollectBookMain, static_cast<int32>(WingPCClass), static_cast<int32>(EItemClass::EIC_Wing));
	}
}

void UB2LobbyUI_WingMain::SetOpenEnhancementPart(bool bInOpen)
{
	if (bInOpen)
	{ // 贸澜 茄锅 悼利 积己.
		ConditionalCreateEnhancementPart();
	}
	if (CreatedEnhancementPart)
	{
		CreatedEnhancementPart->SetVisibility(bInOpen ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
}
void UB2LobbyUI_WingMain::SetOpenEvolutionPart(bool bInOpen)
{
	if (bInOpen)
	{ // 贸澜 茄锅 悼利 积己.
		ConditionalCreateEvolutionPart();
	}
	if (CreatedEvolutionPart)
	{
		CreatedEvolutionPart->SetVisibility(bInOpen ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
}

template<class WidgetClassT>
static WidgetClassT* CreateDynamicWingUIPartCommon(TSoftClassPtr<WidgetClassT>& InCreateWidgetClass, UB2LobbyUI_WingMain* InOwnerUserWidget, UCanvasPanel* CreateParentPanel)
{
	//if (CreateParentPanel && InOwnerUserWidget)
	//{
	//	WidgetClassT* NewCreated = DynLoadAndCreateInCanvasPanelFull<WidgetClassT>(InCreateWidgetClass, InOwnerUserWidget, CreateParentPanel);
	//	if (NewCreated)
	//	{
	//		// SetAutoSize (Size to Content) 窍绰 扒 捞率狼 漂荐茄 荤沥烙. 咯扁辑 积己窍妨绰 UIP 狼 捞傈 硅摹 惑炔苞 度鞍捞 嘎眠妨促 焊聪.
	//		// 沥 救登搁 CreateParentPanel 困俊 SizeBox 鞍篮 吧 怠搭 荐档 乐摆瘤父.
	//		UCanvasPanelSlot* CPSlot = Cast<UCanvasPanelSlot>(NewCreated->Slot);
	//		if (CPSlot)
	//		{
	//			CPSlot->SetAutoSize(true);
	//		}

	//		NewCreated->InitFromWingMain();
	//		return NewCreated;
	//	}
	//}
	return nullptr;
}

void UB2LobbyUI_WingMain::ConditionalCreateEnhancementPart()
{
	if (!CreatedEnhancementPart && X_CP_EnhanceOrEvolutionBase.IsValid())
	{
		CreatedEnhancementPart = CreateDynamicWingUIPartCommon<UB2WingUIP_Enhancement>(UIP_Enhancement_WidgetClass, this, X_CP_EnhanceOrEvolutionBase.Get());
	}
}
void UB2LobbyUI_WingMain::ConditionalCreateEvolutionPart()
{
	if (!CreatedEvolutionPart && X_CP_EnhanceOrEvolutionBase.IsValid())
	{
		CreatedEvolutionPart = CreateDynamicWingUIPartCommon<UB2WingUIP_Evolution>(UIP_Evolution_WidgetClass, this, X_CP_EnhanceOrEvolutionBase.Get());
	}
}

void UB2LobbyUI_WingMain::DestroyEnhancementPart()
{
	if (CreatedEnhancementPart)
	{
		CreatedEnhancementPart->DestroySelf();
		CreatedEnhancementPart = nullptr;
	}
}
void UB2LobbyUI_WingMain::DestroyEvolutionPart()
{
	if (CreatedEvolutionPart)
	{
		CreatedEvolutionPart->DestroySelf();
		CreatedEvolutionPart = nullptr;
	}
}

void UB2LobbyUI_WingMain::OnHeroMgmtPCChanged(EPCClass InSelectedPCClass)
{
	UpdateByCurrentWingData();

	if (CachedAirport)
	{// Wing UI 拳搁俊辑 鞘夸肺 窍绰 眠啊 朝俺 单捞磐甫 器窃茄 夸没..
		CachedAirport->ConditionalReqForFullWingData(InSelectedPCClass);
	}
}

void OpenWingIngredAcqPathPopup(EWingIngredAcqElemType InPopupMainReason)
{
	int32 reasonID = FItemRefIDHelper::GetWingResourceRefIDByAcqElemType(InPopupMainReason);
	ShortageMGR->CheckContentsShortageShortcut(reasonID);
}

void UB2LobbyUI_WingMain::ShowUpgradeStatNotify()
{
	if (UB2UIUpgradeStatNotify* pUI = UB2UIManager::GetInstance()->OpenUI<UB2UIUpgradeStatNotify>(UIFName::UpgradeStatNotify))
	{
		//pUI->SetAutoClose();

		EPCClass SelectedPCClass = CachedLobbyGM ? CachedLobbyGM->GetHeroMgmtModePCClass() : EPCClass::EPC_End;
		FB2Wing CurrentWingData;
		// 咯扁辑 角力肺 绢恫 可记阑 哗棵瘤绰 崔扼龙 荐 乐澜. 俺急秦具 窃.
		const bool bValidWingData = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWing(SelectedPCClass, CurrentWingData);
		if (bValidWingData)
		{
			pUI->ClearList();
			
			FText ValueName = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("IncreaseStatAttack"));
			pUI->AddListItem(ValueName, FText::FromString("+"+FString::FromInt(CurrentWingData.GetAttackPoint()- BeforeAttPoint)), true);
			
			ValueName = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("IncreaseStatDefense"));
			pUI->AddListItem(ValueName, FText::FromString("+"+FString::FromInt(CurrentWingData.GetDefensePoint() - BeforeDefPoint)), true);
			
			ValueName = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("IncreaseStatHealth"));
			pUI->AddListItem(ValueName, FText::FromString("+"+FString::FromInt(CurrentWingData.GetHealthPoint() - BeforeHPPoint)), true);

			ShowBattleScoreNotify();
		}
	}
}

void UB2LobbyUI_WingMain::ShowBattleScoreNotify()
{
	if (TempBattleScoreBefore != TempBattleScoreAfter && TempBattleScoreBefore >= 0  && TempBattleScoreAfter >= 0)
	{
		EPCClass SelectedPCClass = CachedLobbyGM ? CachedLobbyGM->GetHeroMgmtModePCClass() : EPCClass::EPC_End;
		// 傈捧仿 函版 扑诀 免仿 
		UB2UIBattleScoreNotify* BattleScoreNotifyUI = UB2UIManager::GetInstance()->OpenUI<UB2UIBattleScoreNotify>(UIFName::BattleScoreNotify);
		if (BattleScoreNotifyUI != nullptr)
		{
			BattleScoreNotifyUI->SetAmount(TempBattleScoreAfter, TempBattleScoreBefore, SelectedPCClass);

			TempBattleScoreBefore = -1;
			TempBattleScoreAfter = -1;
		}
	}
}

void UB2LobbyUI_WingMain::SetBattleScoreNotify()
{ //捞何盒 磊悼碍拳 锭巩俊 ShowBattleScoreNotifyClass<EPCClass> 措脚 碍力肺 蔼阑 历厘秦狄. 
	UB2UIDocHero* DocHero = UB2UIDocHelper::GetDocHero((int32)CachedLobbyGM->GetHeroMgmtModePCClass());

	if (DocHero)
	{
		TempBattleScoreBefore = DocHero->GetPreCombatPower();
		TempBattleScoreAfter = DocHero->GetCombatPower();

		if(TempBattleScoreBefore != TempBattleScoreAfter)
			DocHero->SetChangedCombatPower(true);
	}
}

void UB2LobbyUI_WingMain::BeginShowWingEvolutionSuccessMessage()
{
	PlayOnSoundEvolution_BP();
}