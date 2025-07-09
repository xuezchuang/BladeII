// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
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
	//Editor ��Ȳ���� ������ �� �־ ���⼭�� unsubscribe
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
		UIP_HeroMgmtBase->StartFromLobby(InUIManager, InGM); // �̰� LobbyUISwitcher ���� �𸣴� ���� �� �־��..
	}
	if (UIP_WingMenuUpper_MainSymbol.IsValid())
	{
		UIP_WingMenuUpper_MainSymbol->StartFromLobby(InUIManager, InGM);
	}

	// ���� �Ҹ�ǰ�� ó���� �����;� �ϴµ�.. �߰��� �� ������ ���ܼ� ���⵵ Ȯ�� ��..
	BladeIIGameImpl::GetClientDataStore().SyncingInventoryData(EPCClass::EPC_End); //EPC_End �� �ָ� ���� �������� ��û�� ��.

	CachedAirport = CachedLobbyGM->GetLobbyAirport();
	check(CachedAirport);
		
	// Wing UI ȭ�鿡�� �ʿ�� �ϴ� �߰� ���� �����͸� ������ ��û..
	CachedAirport->ConditionalReqForFullWingData(CachedLobbyGM->GetHeroMgmtModePCClass());
	
	UB2Airport::ConditionalSetWingVisibleOnWingPageOpen(); // ���� �� �������� ó�� �����ٸ� ���� visibility �� ��� �� �ش�.
	
	UpdateStaticWidgets();
	UpdateByCurrentWingData();

	SetLobbyUIHeaderTitle(BladeIIGetLOCText(B2LOC_CAT_WINGTEXT, TEXT("Wing_MainTitle")));
}

void UB2LobbyUI_WingMain::UpdateByCurrentWingData()
{
	EPCClass SelectedPCClass = CachedLobbyGM ? CachedLobbyGM->GetHeroMgmtModePCClass() : EPCClass::EPC_End;
	FB2Wing CurrentWingData;
	// ���⼭ ������ � �ɼ��� �������� �޶��� �� ����. �����ؾ� ��.
	const bool bGotValidWingData = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWing(SelectedPCClass, CurrentWingData);
	const bool bInSomeAutoTask = CachedAirport ? CachedAirport->IsInAutoEvolOrEnhTask() : false; // True �� ��ư���� �� ���ܾ�
	if (bGotValidWingData)
	{
		if (UIP_WingMenuUpper_MainSymbol.IsValid())
		{
			UIP_WingMenuUpper_MainSymbol->UpdateByCurrentWingData();
			UIP_WingMenuUpper_MainSymbol->SetExtraDisableButtons(bInSomeAutoTask);
		}

		// ��ȭ�� ��ȭ �޴��� �� �� �ϳ���
		const bool bNeedToShowMaxEvolveMenu = NeedToShowMaxEvolveMenu(); // �� ���� ��ȭ/��ȭ ��ư ��� ����.
		const bool bNeedToShowEvolveMenu = NeedToShowEvolveMenu();
		
		// ��ȭ Ȥ�� ��ȭ �޴� �� �ϳ���. On-demand �� �ε� �� ����.
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
			// �ؽ�Ʈ �ʵ忡 ����ִ� Upgrade�� ���� �� �̹� ����
			BeforeAttPoint = FCString::Atoi(*TB_AttackIncAmount->GetText().ToString());
			TB_AttackIncAmount->SetText(
				FText::FromString(FString::Printf(TEXT("%d"), (int32)CurrentWingData.GetAttackPoint()))
				);
		}
		if (TB_DefenseIncAmount.IsValid())
		{
			// �ؽ�Ʈ �ʵ忡 ����ִ� Upgrade�� ���� �� �̹� ����
			BeforeDefPoint = FCString::Atoi(*TB_DefenseIncAmount->GetText().ToString());
			TB_DefenseIncAmount->SetText(
				FText::FromString(FString::Printf(TEXT("%d"), (int32)CurrentWingData.GetDefensePoint()))
				);
		}
		if (TB_HealthIncAmount.IsValid())
		{
			// �ؽ�Ʈ �ʵ忡 ����ִ� Upgrade�� ���� �� �̹� ����
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
		// �ִ� ��ȭ�� �� ���¿����� ��ư�� �����.
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
			// ���⵵ ���������� ���� ���׷��̵� ���¿� ���� ��ȭ Ȥ�� ��ȭ�� ǥ��..
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
				//[@AKI, 170417] ������ ���ִϱ� ���⼭�� ���� ���� ����
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
		// �ֿ� stat ���� ����� ��� �� 0 ����
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
	{ // ���� ��Ʈ���� ��ư�� ������..
		UIP_HeroMgmtBase->SetExtraDisableButtons(bInSomeAutoTask);
		UIP_HeroMgmtBase->DoMarkRedDot();
	}
}

void UB2LobbyUI_WingMain::PlayOnEnhancedAnim()
{
	// EnhancePoint �ƴ� EnhanceLevel �����ÿ� �Ҹ��� ��
	// ��Ʈ �ʿ����� ���� �ִϸ��̼� ����.
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
	UpdateByCurrentWingData(); // ��ư state �� ������ ����..
}

void UB2LobbyUI_WingMain::UpdateOnAutoEvolveComplete()
{
	if (CreatedEvolutionPart)
	{ // �ڵ� ��ȭ ���� �����ϸ鼭 �������� �־��� �� �ִ�.
		CreatedEvolutionPart->ForceStopShowEvolutionFailMessage();
	}
	UpdateByCurrentWingData(); // ��ư state �� ������ ����..
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
	// �������� ���� �κ����� ĳ���� �� üũ�ؾ� �ϴ� ��
	if (UIP_HeroMgmtBase.IsValid())
	{
		for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
		{
			EPCClass CastedPCClass = IntToPCClass(PCI);
			UIP_HeroMgmtBase->ShowNewMark(CastedPCClass, BladeIIGameImpl::GetRedDotManager().ShouldMarkForNewWing(CastedPCClass));
		}
	}
	*/

	// �Ӽ��ɼ� ������ �ʿ��� �ʿ�..
}

bool UB2LobbyUI_WingMain::NeedToShowEvolveMenu() const
{
	EPCClass SelectedPCClass = CachedLobbyGM ? CachedLobbyGM->GetHeroMgmtModePCClass() : EPCClass::EPC_End;
	FB2Wing CurrentWingData;
	const bool bGotValidWingData = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWing(SelectedPCClass, CurrentWingData);
	if (bGotValidWingData)
	{ // NeedToShowMaxEvolveMenu �� ��쵵 ����.
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
{ // �ڵ���ȭ / �ڵ���ȭ���� / �ڵ���ȭ / �ڵ���ȭ���� ��Ȳ�� ���� ������ �ɷ� ����
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
		// ���� �ڵ���ȭ Ȥ�� �ڵ���ȭ �����̸� �ߴ��ϵ��� �ؾ�.
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
				if (!NeedToShowMaxEvolveMenu()) // ������ �� ���� ��ư Ŭ���� ���� �ʾƾ� ������..
				{ // �ڵ� ��ȭ
					// �׽�Ʈ ���������� ���� DevShowNotReadyYetMessage ���ְ� �Ʒ� �κ� Ǯ� �׽�Ʈ
					CachedAirport->BeginAutoEvolution(CachedLobbyGM->GetHeroMgmtModePCClass());
				}
			}
			else
			{ // �ڵ� ��ȭ
				CachedAirport->BeginAutoEnhancement(CachedLobbyGM->GetHeroMgmtModePCClass());
			}
		}

		UpdateByCurrentWingData(); // �ڵ� ��ȭ/��ȭ ����/���ῡ ���� ������Ʈ �� �Ÿ����� �� �ִ�.
	}
}

void UB2LobbyUI_WingMain::OnClickedEnhanceOrEvolveBtn()
{
	EPCClass SelectedPCClass = CachedLobbyGM ? CachedLobbyGM->GetHeroMgmtModePCClass() : EPCClass::EPC_End;
	FB2Wing CurrentWingData;
	// ���⼭ ������ � �ɼ��� �������� �޶��� �� ����. �����ؾ� ��.
	const bool bGotValidWingData = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWing(SelectedPCClass, CurrentWingData);

	if (bGotValidWingData && !IsUltimatelyEvolvedWing(CurrentWingData))
	{
		int32 SvrOwnerCharType = CliToSvrPCClassType(IntToPCClass(CurrentWingData.OwnerPCClass));

		// ������ ��쿡 ����� ���� ��峪 ��� �����ۿ� ���� �Ұ����ϸ� �˾��� ����� �Ѵ�.

		if (IsItTimeToEvolve(CurrentWingData))
		{ // ��ȭ ������ ��� ����� ���� ����.
			if (!UB2Airport::HasEnoughGoldForWingEvolveLocalPlayer(SelectedPCClass)){
				ShortageMGR->PopupGoToShopForGoldMessage();
			}
			else if (!UB2Airport::HasEnoughIngredForWingEvolveLocalPlayer(SelectedPCClass)){
				// ��� ȹ�� ��� �˾�.
				OpenWingIngredAcqPathPopup(EWingIngredAcqElemType::WAET_EvolveIngred);
			}
			else{ // �����ϴٰ� �ǴܵǸ� ������ req.
				data_trader::Retailer::GetInstance().RequestUpgradeWing(SvrOwnerCharType);
			}
		}
		else
		{// ��ȭ ������ ��� ����� ���� ����.
			if (!UB2Airport::HasEnoughGoldForWingEnhanceLocalPlayer(SelectedPCClass)){
				ShortageMGR->PopupGoToShopForGoldMessage();
			}
			else if (!UB2Airport::HasEnoughIngredForWingEnhanceLocalPlayer(SelectedPCClass)){
				// ��� ȹ�� ��� �˾�.
				OpenWingIngredAcqPathPopup(EWingIngredAcqElemType::WAET_EnhanceIngred);
			}
			else{ // �����ϴٰ� �ǴܵǸ� ������ req.
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
	{ // ó�� �ѹ� ���� ����.
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
	{ // ó�� �ѹ� ���� ����.
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
	if (CreateParentPanel && InOwnerUserWidget)
	{
		WidgetClassT* NewCreated = DynLoadAndCreateInCanvasPanelFull<WidgetClassT>(InCreateWidgetClass, InOwnerUserWidget, CreateParentPanel);
		if (NewCreated)
		{
			// SetAutoSize (Size to Content) �ϴ� �� ������ Ư���� ������. ���⼭ �����Ϸ��� UIP �� ���� ��ġ ��Ȳ�� �Ȱ��� ���߷��� ����.
			// �� �ȵǸ� CreateParentPanel ���� SizeBox ���� �� ���� ���� �ְ�����.
			UCanvasPanelSlot* CPSlot = Cast<UCanvasPanelSlot>(NewCreated->Slot);
			if (CPSlot)
			{
				CPSlot->SetAutoSize(true);
			}

			NewCreated->InitFromWingMain();
			return NewCreated;
		}
	}
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
	{// Wing UI ȭ�鿡�� �ʿ�� �ϴ� �߰� ���� �����͸� ������ ��û..
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
		// ���⼭ ������ � �ɼ��� �������� �޶��� �� ����. �����ؾ� ��.
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
		// ������ ���� �˾� ��� 
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
{ //�̺κ� �ڵ���ȭ ������ ShowBattleScoreNotifyClass<EPCClass> ��� ������ ���� �����ص�. 
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