// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UILobby_CostumeMain.h"

#include "B2LobbyUI_HeroMgmtCommon.h"
#include "B2LobbyGameMode.h"
#include "B2AndroidBackManager.h"
#include "BladeIIGameImpl.h"
#include "B2UIManager.h"

void UB2UILobby_CostumeMain::StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
{
	Super::StartFromLobby(InUIManager, InGM);

	if (HeroMgmtBaseNRef)
	{
		HeroMgmtBaseNRef->StartFromLobby(InUIManager, InGM); // 이건 LobbyUISwitcher 에서 모르니 직접 해 주어야..		
	}

	DoMarkRedDot();

	InitCostumeMain();
}

void UB2UILobby_CostumeMain::DestroySelf()
{
	ReleaseCostumeMain();

	BladeIIGameImpl::GetCostumeDataStore().SetCachedCostumePageType(ECostumePage::Inven);
	BladeIIGameImpl::GetClientDataStore().RequestSyncToServerCostumeVisible();
	Super::DestroySelf();
}

void UB2UILobby_CostumeMain::DoMarkRedDot()
{
	Super::DoMarkRedDot();

	if (HeroMgmtBaseNRef)
		HeroMgmtBaseNRef->DoMarkRedDot();
}

void UB2UILobby_CostumeMain::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateInputDelay(InDeltaTime);
}

void UB2UILobby_CostumeMain::CloseWidgetDelegate()
{
	if (BladeIIGameImpl::GetCostumeDataStore().GetCachedCostumePageType() != FirstInPage)
	{
		UpdateCostumePage(FirstInPage);
		return;
	}

	if (UB2UIManager* UIManager = UB2UIManager::GetInstance())
	{
		EUIScene PrevUIScene = UIManager->GetCurrUIScene();

		UIManager->ChangeUISceneBack();
	}
}

void UB2UILobby_CostumeMain::CostumeMain_CacheAssets()
{
	GET_SLOT(UCanvasPanel, CP_CostumePage);

	GET_SLOT(UCanvasPanel, X_LeftPopupPanel);
	GET_SLOT(UCanvasPanel, X_RightPopupPanel);

	GET_SLOT(UWidgetSwitcher, WS_InvenOrStoreOpenBtn);

	GET_SLOT(UB2Button, BTN_OpenInvenPage);
	GET_SLOT(UB2RichTextBlock, RTB_OpenInvenPageBtn);

	GET_SLOT(UB2Button, BTN_OpenStorePage);
	GET_SLOT(UB2RichTextBlock, RTB_OpenStorePageBtn);
}

void UB2UILobby_CostumeMain::CostumeMain_BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_OpenInvenPage, &UB2UILobby_CostumeMain::OnClickBTN_OpenInvenPage);
	BIND_CLICK_FUNC_TO_BTN(BTN_OpenStorePage, &UB2UILobby_CostumeMain::OnClickBTN_OpenStorePage);

	REGISTER_REDDOT_CONDITION(GetRedDotWidget(BTN_OpenInvenPage.Get()), &UB2UILobby_CostumeMain::RedDotCondition_Inven);
}

void UB2UILobby_CostumeMain::CostumeMain_UpdateStaticText()
{
	if (RTB_OpenInvenPageBtn.IsValid())
	{
		RTB_OpenInvenPageBtn->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Costume_Tab_Closet")));
	}

	if (RTB_OpenStorePageBtn.IsValid())
	{
		RTB_OpenStorePageBtn->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Costume_Tab_Store")));
	}
}

void UB2UILobby_CostumeMain::SubscribeEvents()
{
	UnsubscribeEvents();

	//TWeakObjectPtr<UB2UILobby_CostumeMain> Capture(this);


	//Issues.Add(RefreshCostumeHeroMgmtRedDotClass<>::GetInstance().Subscribe2(
	//	[Capture]()
	//{
	//	if (Capture.IsValid())
	//		Capture->DoMarkRedDot();
	//}
	//));

	//Issues.Add(DeliveryCheckRedDotClass<FB2MessageInfoResponseCheckRedDotPtr>::GetInstance().Subscribe2(
	//	[Capture](FB2MessageInfoResponseCheckRedDotPtr RedDotInfo)
	//{
	//	if (RedDotInfo.get() == nullptr)
	//		return;

	//	if (RedDotInfo->is_valid_equip && Capture.IsValid())
	//		Capture->DoMarkRedDot();
	//}
	//));
}

void UB2UILobby_CostumeMain::UnsubscribeEvents()
{
	Issues.Empty();
}

void UB2UILobby_CostumeMain::InitCostumeMain()
{
	CostumeMain_CacheAssets();
	CostumeMain_BindDelegates();
	CostumeMain_UpdateStaticText();

	SubscribeEvents();

	SetLobbyUIHeaderTitle(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyMain_Costume")));

	B2AndroidBackManager::GetInstance()->AddBackWidget(this);

	InitCostumePage();

	InitInputDelay();
}

void UB2UILobby_CostumeMain::InitCostumePage()
{
	const int32 iArrayPageMax = static_cast<int32>(ECostumePage::Max);
	ArrayCostumePage.SetNum(iArrayPageMax);

	const ECostumePage eInitCostumePageType = BladeIIGameImpl::GetCostumeDataStore().GetCachedCostumePageType();
	FirstInPage = eInitCostumePageType;
	UpdateCostumePage(eInitCostumePageType);
}

void UB2UILobby_CostumeMain::ReleaseCostumeMain()
{
	UnsubscribeEvents();

	if (HeroMgmtBaseNRef)
	{
		HeroMgmtBaseNRef->DestroySelf();
	}

	for (int32 i = 0; i < ArrayCostumePage.Num(); ++i)
	{
		if (ArrayCostumePage.IsValidIndex(i) && ArrayCostumePage[i] != nullptr)
			ArrayCostumePage[i]->DestroySelf(nullptr);
	}
	ArrayCostumePage.Empty();

	B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);

	CloseAllStandaloneUIFromUIManager();
}

void UB2UILobby_CostumeMain::UpdateCostumePage(const ECostumePage& eCostumePageType)
{
	//int32 iCostumePageIndex = static_cast<int32>(eCostumePageType);

	//UpdateCostumePageValidity(iCostumePageIndex);
	//UpdateCostumePagesVisibility(iCostumePageIndex);

	//switch (eCostumePageType)
	//{
	//case ECostumePage::Inven:
	//	{
	//		CostumeEquipSlotVisibleClass<bool>::GetInstance().Signal(true);
	//	}break;

	//case ECostumePage::Store:
	//	{
	//		CostumeEquipSlotVisibleClass<bool>::GetInstance().Signal(false);
	//		BladeIIGameImpl::GetRedDotManager().RequestCheckRedDot({ RedDotHint::RED_DOT_EQUIP });
	//	}break;
	//}

	//if (WS_InvenOrStoreOpenBtn.IsValid())
	//{
	//	WS_InvenOrStoreOpenBtn->SetActiveWidgetIndex(iCostumePageIndex);
	//}

	//BladeIIGameImpl::GetCostumeDataStore().SetCachedCostumePageType(eCostumePageType);
	//
	//CloseAllStandaloneUIFromUIManager();
}

void UB2UILobby_CostumeMain::UpdateCostumePageValidity(const int32& iCostumePageIndex)
{
	if (ArrayCostumePage.IsValidIndex(iCostumePageIndex) && CostumePageSubClass.IsValidIndex(iCostumePageIndex))
	{
		if (ArrayCostumePage[iCostumePageIndex] == nullptr)
		{
			ArrayCostumePage[iCostumePageIndex] = CreateDynamicCostumePage(&(CostumePageSubClass[iCostumePageIndex]));
		}
	}
}

void UB2UILobby_CostumeMain::InitInputDelay()
{
	fCurrentInputDelay = fMaxInputDelay;
}

void UB2UILobby_CostumeMain::UpdateInputDelay(const float& InDeltaTime)
{
	if (fCurrentInputDelay < fMaxInputDelay)
	{
		fCurrentInputDelay += InDeltaTime;
	}
}

bool UB2UILobby_CostumeMain::IsEnableInputDelay()
{
	bool IsEnableInput = false;

	if (fCurrentInputDelay >= fMaxInputDelay)
	{
		IsEnableInput = true;
		fCurrentInputDelay = 0.f;
	}
	
	return IsEnableInput;
}

void UB2UILobby_CostumeMain::CloseAllStandaloneUIFromUIManager()
{
	//열린 팝업들의 close 처리가 어려워 팝업류 일괄 close 추가
	if (UB2UIManager* pUIManager = UB2UIManager::GetInstance())
	{
		pUIManager->CloseAllStandaloneUIs();
	}
}

void UB2UILobby_CostumeMain::UpdateCostumePagesVisibility(const int32& iVisiblePageIndex)
{
	const int32 iArrayPageMax = ArrayCostumePage.Num();
	for (int32 i = 0; i < iArrayPageMax; ++i)
	{
		bool bVisible = false;

		if (i == iVisiblePageIndex)
		{
			bVisible = true;
		}

		if (ArrayCostumePage.IsValidIndex(i) && ArrayCostumePage[i] != nullptr)
		{
			if (bVisible)
			{
				ArrayCostumePage[i]->SetVisibility(ESlateVisibility::Visible);
				ArrayCostumePage[i]->OnOpen(true);
			}
			else
			{
				ArrayCostumePage[i]->OnClose();
			}
		}
	}
}

UB2UIWidget* UB2UILobby_CostumeMain::CreateDynamicCostumePage(const TSoftClassPtr<UB2UIWidget>* pCreateWidgetClass)
{
	//if (CP_CostumePage.IsValid() == false)
	//{
		return nullptr;
	//}

	//UB2UIWidget* CreatedWidget = DynLoadAndCreateInCanvasPanelFull<UB2UIWidget>(*pCreateWidgetClass, this, CP_CostumePage.Get());
	//if (CreatedWidget != nullptr)
	//	CreatedWidget->Init();

	//return CreatedWidget;
}

UCanvasPanel* UB2UILobby_CostumeMain::GetDetailPopupParentPanel(bool bOfLeftSide) const
{
	return bOfLeftSide ? X_LeftPopupPanel.Get() :
		// 우측팝업은 HeroMgmt 공통파트의 헤더파트에서 나오는 모드 알림을 가리지 않도록 HeroMgmt 쪽 Panel 을 사용한다.
		// 여기 widget 들 배치 시 순서 및 z-order 주의해야 함.
		(HeroMgmtBaseNRef ? HeroMgmtBaseNRef->GetInfoPopupParentPanel(bOfLeftSide) : X_RightPopupPanel.Get());
}

void UB2UILobby_CostumeMain::OnClickBTN_OpenInvenPage()
{
	if (IsEnableInputDelay())
	{
		UpdateCostumePage(ECostumePage::Inven);
	}
}

void UB2UILobby_CostumeMain::OnClickBTN_OpenStorePage()
{
	if (IsEnableInputDelay())
	{
		UpdateCostumePage(ECostumePage::Store);
	}
}

bool UB2UILobby_CostumeMain::RedDotCondition_Inven()
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_Costume();
}
