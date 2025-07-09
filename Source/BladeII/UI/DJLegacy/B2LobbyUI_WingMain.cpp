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
	//Editor 상황에서 문제가 좀 있어서 여기서도 unsubscribe
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
		UIP_HeroMgmtBase->StartFromLobby(InUIManager, InGM); // 이건 LobbyUISwitcher 에서 모르니 직접 해 주어야..
	}
	if (UIP_WingMenuUpper_MainSymbol.IsValid())
	{
		UIP_WingMenuUpper_MainSymbol->StartFromLobby(InUIManager, InGM);
	}

	// 원래 소모품은 처음에 가져와야 하는데.. 중간에 좀 문제가 생겨서 여기도 확인 차..
	BladeIIGameImpl::GetClientDataStore().SyncingInventoryData(EPCClass::EPC_End); //EPC_End 로 주면 공용 아이템을 요청할 것.

	CachedAirport = CachedLobbyGM->GetLobbyAirport();
	check(CachedAirport);
		
	// Wing UI 화면에서 필요로 하는 추가 날개 데이터를 포함한 요청..
	CachedAirport->ConditionalReqForFullWingData(CachedLobbyGM->GetHeroMgmtModePCClass());
	
	UB2Airport::ConditionalSetWingVisibleOnWingPageOpen(); // 만일 요 페이지를 처음 열었다면 날개 visibility 를 모두 켜 준다.
	
	UpdateStaticWidgets();
	UpdateByCurrentWingData();

	SetLobbyUIHeaderTitle(BladeIIGetLOCText(B2LOC_CAT_WINGTEXT, TEXT("Wing_MainTitle")));
}

void UB2LobbyUI_WingMain::UpdateByCurrentWingData()
{
	EPCClass SelectedPCClass = CachedLobbyGM ? CachedLobbyGM->GetHeroMgmtModePCClass() : EPCClass::EPC_End;
	FB2Wing CurrentWingData;
	// 여기서 실제로 어떤 옵션을 빼올지는 달라질 수 있음. 개선해야 함.
	const bool bGotValidWingData = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWing(SelectedPCClass, CurrentWingData);
	const bool bInSomeAutoTask = CachedAirport ? CachedAirport->IsInAutoEvolOrEnhTask() : false; // True 면 버튼들을 좀 숨겨야
	if (bGotValidWingData)
	{
		if (UIP_WingMenuUpper_MainSymbol.IsValid())
		{
			UIP_WingMenuUpper_MainSymbol->UpdateByCurrentWingData();
			UIP_WingMenuUpper_MainSymbol->SetExtraDisableButtons(bInSomeAutoTask);
		}

		// 강화랑 진화 메뉴는 둘 중 하나만
		const bool bNeedToShowMaxEvolveMenu = NeedToShowMaxEvolveMenu(); // 이 경우는 강화/진화 버튼 모두 제거.
		const bool bNeedToShowEvolveMenu = NeedToShowEvolveMenu();
		
		// 강화 혹은 진화 메뉴 중 하나만. On-demand 로 로딩 및 생성.
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
			// 텍스트 필드에 들어있는 Upgrade전 스텟 값 이미 저장
			BeforeAttPoint = FCString::Atoi(*TB_AttackIncAmount->GetText().ToString());
			TB_AttackIncAmount->SetText(
				FText::FromString(FString::Printf(TEXT("%d"), (int32)CurrentWingData.GetAttackPoint()))
				);
		}
		if (TB_DefenseIncAmount.IsValid())
		{
			// 텍스트 필드에 들어있는 Upgrade전 스텟 값 이미 저장
			BeforeDefPoint = FCString::Atoi(*TB_DefenseIncAmount->GetText().ToString());
			TB_DefenseIncAmount->SetText(
				FText::FromString(FString::Printf(TEXT("%d"), (int32)CurrentWingData.GetDefensePoint()))
				);
		}
		if (TB_HealthIncAmount.IsValid())
		{
			// 텍스트 필드에 들어있는 Upgrade전 스텟 값 이미 저장
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
		// 최대 진화가 된 상태에서는 버튼들 숨긴다.
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
			// 여기도 마찬가지로 날개 업그레이드 상태에 따라 진화 혹은 강화로 표시..
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
				//[@AKI, 170417] 위에서 해주니까 여기서는 인제 하지 말자
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
		// 주요 stat 들은 숨기는 대신 걍 0 으로
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
	{ // 공통 파트에도 버튼이 많으니..
		UIP_HeroMgmtBase->SetExtraDisableButtons(bInSomeAutoTask);
		UIP_HeroMgmtBase->DoMarkRedDot();
	}
}

void UB2LobbyUI_WingMain::PlayOnEnhancedAnim()
{
	// EnhancePoint 아닌 EnhanceLevel 증가시에 불리는 거
	// 파트 쪽에서도 따로 애니메이션 돌림.
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
	UpdateByCurrentWingData(); // 버튼 state 랑 문구들 복구..
}

void UB2LobbyUI_WingMain::UpdateOnAutoEvolveComplete()
{
	if (CreatedEvolutionPart)
	{ // 자동 진화 도중 실패하면서 보여지고 있었을 수 있다.
		CreatedEvolutionPart->ForceStopShowEvolutionFailMessage();
	}
	UpdateByCurrentWingData(); // 버튼 state 랑 문구들 복구..
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
	// 영웅관리 공통 부분으로 캐릭터 별 체크해야 하는 거
	if (UIP_HeroMgmtBase.IsValid())
	{
		for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
		{
			EPCClass CastedPCClass = IntToPCClass(PCI);
			UIP_HeroMgmtBase->ShowNewMark(CastedPCClass, BladeIIGameImpl::GetRedDotManager().ShouldMarkForNewWing(CastedPCClass));
		}
	}
	*/

	// 속성옵션 아이콘 쪽에도 필요..
}

bool UB2LobbyUI_WingMain::NeedToShowEvolveMenu() const
{
	EPCClass SelectedPCClass = CachedLobbyGM ? CachedLobbyGM->GetHeroMgmtModePCClass() : EPCClass::EPC_End;
	FB2Wing CurrentWingData;
	const bool bGotValidWingData = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWing(SelectedPCClass, CurrentWingData);
	if (bGotValidWingData)
	{ // NeedToShowMaxEvolveMenu 인 경우도 포함.
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
{ // 자동강화 / 자동강화해제 / 자동진화 / 자동진화해제 상황에 따라 적절한 걸로 세팅
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
		// 현재 자동강화 혹은 자동진화 도중이면 중단하도록 해야.
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
				if (!NeedToShowMaxEvolveMenu()) // 어차피 이 경우면 버튼 클릭이 되지 않아야 하지만..
				{ // 자동 진화
					// 테스트 가능해지면 위에 DevShowNotReadyYetMessage 없애고 아래 부분 풀어서 테스트
					CachedAirport->BeginAutoEvolution(CachedLobbyGM->GetHeroMgmtModePCClass());
				}
			}
			else
			{ // 자동 강화
				CachedAirport->BeginAutoEnhancement(CachedLobbyGM->GetHeroMgmtModePCClass());
			}
		}

		UpdateByCurrentWingData(); // 자동 강화/진화 시작/종료에 따라 업데이트 할 거리들이 꽤 있다.
	}
}

void UB2LobbyUI_WingMain::OnClickedEnhanceOrEvolveBtn()
{
	EPCClass SelectedPCClass = CachedLobbyGM ? CachedLobbyGM->GetHeroMgmtModePCClass() : EPCClass::EPC_End;
	FB2Wing CurrentWingData;
	// 여기서 실제로 어떤 옵션을 빼올지는 달라질 수 있음. 개선해야 함.
	const bool bGotValidWingData = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWing(SelectedPCClass, CurrentWingData);

	if (bGotValidWingData && !IsUltimatelyEvolvedWing(CurrentWingData))
	{
		int32 SvrOwnerCharType = CliToSvrPCClassType(IntToPCClass(CurrentWingData.OwnerPCClass));

		// 각각의 경우에 사용자 보유 골드나 재료 아이템에 따라 불가능하면 팝업을 띄워야 한다.

		if (IsItTimeToEvolve(CurrentWingData))
		{ // 재화 부족의 경우 사용자 현질 유도.
			if (!UB2Airport::HasEnoughGoldForWingEvolveLocalPlayer(SelectedPCClass)){
				ShortageMGR->PopupGoToShopForGoldMessage();
			}
			else if (!UB2Airport::HasEnoughIngredForWingEvolveLocalPlayer(SelectedPCClass)){
				// 재료 획득 경로 팝업.
				OpenWingIngredAcqPathPopup(EWingIngredAcqElemType::WAET_EvolveIngred);
			}
			else{ // 가능하다고 판단되면 서버로 req.
				data_trader::Retailer::GetInstance().RequestUpgradeWing(SvrOwnerCharType);
			}
		}
		else
		{// 재화 부족의 경우 사용자 현질 유도.
			if (!UB2Airport::HasEnoughGoldForWingEnhanceLocalPlayer(SelectedPCClass)){
				ShortageMGR->PopupGoToShopForGoldMessage();
			}
			else if (!UB2Airport::HasEnoughIngredForWingEnhanceLocalPlayer(SelectedPCClass)){
				// 재료 획득 경로 팝업.
				OpenWingIngredAcqPathPopup(EWingIngredAcqElemType::WAET_EnhanceIngred);
			}
			else{ // 가능하다고 판단되면 서버로 req.
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
	{ // 처음 한번 동적 생성.
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
	{ // 처음 한번 동적 생성.
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
			// SetAutoSize (Size to Content) 하는 건 이쪽의 특수한 사정임. 여기서 생성하려는 UIP 의 이전 배치 상황과 똑같이 맞추려다 보니.
			// 정 안되면 CreateParentPanel 위에 SizeBox 같은 걸 덧댈 수도 있겠지만.
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
	{// Wing UI 화면에서 필요로 하는 추가 날개 데이터를 포함한 요청..
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
		// 여기서 실제로 어떤 옵션을 빼올지는 달라질 수 있음. 개선해야 함.
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
		// 전투력 변경 팝업 출력 
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
{ //이부분 자동강화 때문에 ShowBattleScoreNotifyClass<EPCClass> 대신 강제로 값을 저장해둠. 
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