// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIPackageStore.h"
#include "B2UIManager.h"
#include "B2UICommonTab.h"
#include "B2RedDotManager.h"
#include "B2AndroidBackManager.h"
#include "BladeIIGameImpl.h"
#include "B2UIBackWidgetBase.h"
#include "Retailer.h"
#include "B2UIHeader.h"
#include "B2UIDocHelper.h"
#include "B2ItemInfo.h"
#include "B2UIPackageTypePanel.h"
#include "B2UIStorePackageDetailPopup.h"
//#include "B2IAPGenericPlatform.h"

UB2UIPackageStore::UB2UIPackageStore(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CurrentStoreCategory = 0;
	Created_UIP_PackageDetailPopup = nullptr;
	Created_UIP_GuestWarningPopup = nullptr;
}

void UB2UIPackageStore::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UWidgetSwitcher, WS_Content);
	GET_SLOT(UVerticalBox, VB_TabList);
	GET_SLOT(UCanvasPanel, X_CP_PackageDetailPopup_Panel);
	GET_SLOT(UCanvasPanel, X_CP_GuestWarningPopup_Panel);
	GET_SLOT(UB2Button, BTN_Probability);
	GET_SLOT(UTextBlock, TB_Probability);

	GET_SLOT(UB2UIPackageTypePanel_Basic, UI_Package_Basic);
	GET_SLOT(UB2UIPackageTypePanel_Flatrate, UI_Package_Flatrate);
	GET_SLOT(UB2UIPackageTypePanel_Level, UI_Package_LevelUp);

	for (int32 i = 0; i < WS_Content->GetChildrenCount(); i++)
	{
		UB2UIPackageTypePanel* ActiveWidget = Cast<UB2UIPackageTypePanel>(WS_Content->GetWidgetAtIndex(i));
		if (ActiveWidget)
			ActiveWidget->Init();
	}
}

void UB2UIPackageStore::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_Probability, &UB2UIPackageStore::OnClickProbability);
}

void UB2UIPackageStore::Init()
{
	Super::Init();
	/*SetLobbyUIHeaderTitle(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyMain_Package")));

	CAPTURE_UOBJECT(UB2UIPackageStore);

	SelectItemToBuyTicket = SelectItemToBuyClass<int32>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(SelectItemToBuy, int32 ProductId)
		Capture->OpenPopupConfirmToBuyItem(ProductId);
	END_CAPTURE_OBJECT()
		);

	DeliveryBuyShopProductTicket = DeliveryBuyShopProductClass<FB2ResponseBuyShopProductPtr>::GetInstance().Subscribe(
		[this](const FB2ResponseBuyShopProductPtr& BuyShopResult) {
		this->UpdatePanelWidget();
	});

	DeliveryCheckRedDotTicket = DeliveryCheckRedDotClass<FB2MessageInfoResponseCheckRedDotPtr>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryCheckRedDot, const FB2MessageInfoResponseCheckRedDotPtr& msgPtr)
		Capture->DoMarkRedDot_Package();
	END_CAPTURE_OBJECT()
		);*/

	//B2InAppPurchase::IB2IAPGenericPlatform::OnPurchaseResultDelegate.BindLambda([this](bool success, b2network::B2BuyShopResultInfoPtr GetProduct) {
	//	UE_LOG(LogBladeII, Warning, TEXT("@@@@@@@ OnPurchaseResultDelegate @@@@@@@>> %d"), success);
	//	CompletedPurchaseBuyGem(success, GetProduct);
	//});
}

void UB2UIPackageStore::DoMarkRedDot_Package()
{
	DoMarkRedDot();

	UWidget* PanelWidget = WS_Content->GetWidgetAtIndex(WS_Content->GetActiveWidgetIndex());
	if (PanelWidget)
	{
		UB2UIPackageTypePanel* ActiveWidget = Cast<UB2UIPackageTypePanel>(PanelWidget);
		if (ActiveWidget)
			ActiveWidget->DoMarkRedDotPanel();
	}
}

void UB2UIPackageStore::DestroySelf(UB2UIManager* InUIManager)
{
//	if (UI_Package_Basic.IsValid())
//		UI_Package_Basic->DestroySelf(InUIManager);
//	if (UI_Package_Flatrate.IsValid())
//		UI_Package_Flatrate->DestroySelf(InUIManager);
//	if (UI_Package_LevelUp.IsValid())
//		UI_Package_LevelUp->DestroySelf(InUIManager);
//
//	DestroyPackageDetailPopup();
//	DestroyGuestWarningPopup();
//
//	SelectItemToBuyClass<int32>::GetInstance().Unsubscribe(SelectItemToBuyTicket);
//	DeliveryBuyShopProductClass<FB2ResponseBuyShopProductPtr>::GetInstance().Unsubscribe(DeliveryBuyShopProductTicket);
//	DeliveryCheckRedDotClass<FB2MessageInfoResponseCheckRedDotPtr>::GetInstance().Unsubscribe(DeliveryCheckRedDotTicket);
//
//	Super::DestroySelf(InUIManager);
}

void UB2UIPackageStore::UpdateStaticText()
{
	if (TB_Probability.IsValid())
	{
		TB_Probability->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_SummonItem_Chance")));
	}
}

void UB2UIPackageStore::OnOpen(bool RightNow)
{
	Super::OnOpen(RightNow);
	CreateStoreCategoryTab();
	
	int32 FirstCategory = 1;
	auto* DocStore = UB2UIDocHelper::GetDocStore();
	if (DocStore)
		FirstCategory = DocStore->GetPackageTabToOpen();

	SetSelectedStoreCategory(FirstCategory);
	DoMarkRedDot_Package();
}

void UB2UIPackageStore::OnClose(bool RightNow /*= false*/)
{
	Super::OnClose(RightNow);
}

void UB2UIPackageStore::CreateStoreCategoryTab()
{
	if (VB_TabList.IsValid())
	{
		VB_TabList->ClearChildren();

		const TArray<FB2ShopPackageTabPtr> OrderList = BladeIIGameImpl::GetClientDataStore().GetShopPackageTabOrder();

		for (FB2ShopPackageTabPtr Elem : OrderList)
		{
			if (!Elem->is_open) continue;

			auto* CreatedWidget = CreateWidget<class UB2UICommonTab>(GetOwningPlayer(), DefaultStoreTab);
			if (CreatedWidget != NULL)
			{
				int32 TabType = Elem->tab_type;
				CreatedWidget->Init();

				CreatedWidget->SetTabSelected(false);
				CreatedWidget->SetIsTabEnable(true);
				CreatedWidget->SetTabName(GetStoreCategoryName(TabType));
				CreatedWidget->SetTabButtonIndex(TabType);
				CreatedWidget->TabButtonDelegateBindLambda(
					[this, TabType]() { OnClickTabButtonDelegate(TabType);
				});
				
				RegisterRedDot(TabType, CreatedWidget);
				
				VB_TabList->AddChild(CreatedWidget);
			}
		}
	}
}

void UB2UIPackageStore::OnClickTabButtonDelegate(int32 ButtonIndex)
{
	SetSelectedStoreCategory(ButtonIndex);
}

FText UB2UIPackageStore::GetStoreCategoryName(int32 StoreCategory)
{
	FString TextString = TEXT("Store_PackageTab_") + FString::FromInt(StoreCategory);
	return BladeIIGetLOCText(B2LOC_CAT_STORE, TextString);
}

void UB2UIPackageStore::InitPanelWidgetByCategory()
{
	int32 CurrntActiveWidgetIndex = 0;
	
	switch (static_cast<EPACKAGE_TAB_TYPE>(CurrentStoreCategory))
	{
	case EPACKAGE_TAB_TYPE::LEVELUP:
		CurrntActiveWidgetIndex = static_cast<int32>(EPACKAGE_PANEL_TYPE::LEVELUP);
		break;
	case EPACKAGE_TAB_TYPE::FLATRATE:
		CurrntActiveWidgetIndex = static_cast<int32>(EPACKAGE_PANEL_TYPE::FLATRATE);
		break;
	default:
		CurrntActiveWidgetIndex = static_cast<int32>(EPACKAGE_PANEL_TYPE::BASIC);
		break;
	}

	UWidget* PanelWidget = WS_Content->GetWidgetAtIndex(CurrntActiveWidgetIndex);
	if (PanelWidget)
	{
		UB2UIPackageTypePanel* ActiveWidget = Cast<UB2UIPackageTypePanel>(PanelWidget);
		if (ActiveWidget)
		{
			ActiveWidget->InitPanelInfo(CurrentStoreCategory);
			ActiveWidget->UpdatePanelInfo();
			ActiveWidget->ButtonOneParamDelegate.BindUObject(this, &UB2UIPackageStore::SubmitConfirmToBuy);
		}

		WS_Content->SetActiveWidgetIndex(CurrntActiveWidgetIndex);
	}

	DoMarkRedDot_Package();
}

void UB2UIPackageStore::UpdatePanelWidget()
{
	if (!WS_Content.IsValid()) return;

	UWidget* PanelWidget = WS_Content->GetWidgetAtIndex(WS_Content->GetActiveWidgetIndex());
	if (PanelWidget)
	{
		UB2UIPackageTypePanel* ActiveWidget = Cast<UB2UIPackageTypePanel>(PanelWidget);
		if (ActiveWidget)
			ActiveWidget->UpdatePanelInfo();
	}

	FB2RedDotManager::RequestCheckRedDot({ RedDotHint::RED_DOT_SHOP_PACKAGE });
}

void UB2UIPackageStore::SetSelectedStoreCategory(int32 StoreCategory)
{
	if (VB_TabList->GetChildrenCount() > 0)
	{
		for (int Idx = 0; Idx < VB_TabList->GetChildrenCount(); Idx++)
		{
			UB2UICommonTab* ChildrenTab = static_cast<UB2UICommonTab*>(VB_TabList->GetChildAt(Idx));

			if (ChildrenTab)
			{
				if (ChildrenTab->GetTabButtonIndex() == StoreCategory)
					ChildrenTab->SetTabSelected(true);
				else
					ChildrenTab->SetTabSelected(false);
			}
		}
	}

	CurrentStoreCategory = StoreCategory;
	InitPanelWidgetByCategory();

	auto* DocStore = UB2UIDocHelper::GetDocStore();
	if (DocStore)
		DocStore->SetPackageTabToOpen(CurrentStoreCategory);
}

void UB2UIPackageStore::SubmitConfirmToBuy(int32 StoreProductID)
{
	const FStoreProductData* PackageProduct = BladeIIGameImpl::GetClientDataStore().GetStoreProductData(StoreProductID);
	if (static_cast<EStoreItemCost>(PackageProduct->CostType) == EStoreItemCost::Cash)
	{
		SubmitConfirmToBuyGem(StoreProductID);
	}
	else
	{
		SubmitConfirmToBuyItem(StoreProductID);
	}
}

void UB2UIPackageStore::SubmitConfirmToBuyItem(int32 StoreProductID)
{
	data_trader::Retailer::GetInstance().RequestBuyShopProduct(StoreProductID);

	ClosePopupConfirmToBuyItem();
}

void UB2UIPackageStore::SubmitConfirmToBuyGem(int32 StoreProductID)
{
	//ClosePopupConfirmToBuyItem();

	//SelectedProductID = StoreProductID;
	//if (B2P_GetKakaoIDPCode() == EPlatformIDPCode::Guest)
	//{
	//	OpenGuestWarningPopup();
	//}
	//else
	//{
	//	ConfirmToBuyGem();
	//}
}

void UB2UIPackageStore::ConfirmToBuyGem()
{
	//B2InAppPurchase::FB2CashProductInfo* ProductInfo = B2InAppPurchase::IB2IAPGenericPlatform::GetInstance()->GetMarketProduct(SelectedProductID);
	//if (ProductInfo)
	//	B2InAppPurchase::IB2IAPGenericPlatform::GetInstance()->RequestPreparePurchase(SelectedProductID, ProductInfo->MarketProductId);
}

void UB2UIPackageStore::CompletedPurchaseBuyGem(bool success, const FB2ResponseBuyShopResultInfoPtr GetProduct)
{
	if (success)
	{
		BladeIIGameImpl::GetClientDataStore().ResponseBuyShopResultInfo(GetProduct);
		UpdatePanelWidget();
	}
	else
	{
		UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupSimple>(EUIMsgPopup::Simple,
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
			BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Failed_Product")),
			0.f,
			true,
			true,
			EUIMsgPopupButtonGroup::Confirm
			);
	}
}

void UB2UIPackageStore::OpenGuestWarningPopup()
{
	ConditionalCreateGuestWarningPopup();

	if (Created_UIP_GuestWarningPopup)
	{
		Created_UIP_GuestWarningPopup->OpenPopup();
	}
}

void UB2UIPackageStore::CloseGuestWarningPopup()
{
	if (Created_UIP_GuestWarningPopup)
	{
		Created_UIP_GuestWarningPopup->ClosePopup();
	}
}

void UB2UIPackageStore::BuyGuestWarningPopup()
{
	if (Created_UIP_GuestWarningPopup)
	{
		Created_UIP_GuestWarningPopup->ClosePopup();
	}

	ConfirmToBuyGem();
}

void UB2UIPackageStore::OpenPopupConfirmToBuyItem(int32 ProductId)
{
	ESTORE_BUYPOPUP_WIDGET_TYPE PopupType = ESTORE_BUYPOPUP_WIDGET_TYPE::END;
	EPACKAGE_TYPE PackageType = EPACKAGE_TYPE::None;

	const FStoreProductData* PackageProduct = BladeIIGameImpl::GetClientDataStore().GetStoreProductData(ProductId);
	int32 PackageID = PackageProduct ? PackageProduct->Product.ProductId : 0;

	UWidget* PanelWidget = WS_Content->GetWidgetAtIndex(WS_Content->GetActiveWidgetIndex());
	if (PanelWidget)
	{
		UB2UIPackageTypePanel* ActiveWidget = Cast<UB2UIPackageTypePanel>(PanelWidget);
		if (ActiveWidget)
		{
			PopupType = ActiveWidget->GetPopupWidgetType(PackageID);
			PackageType = ActiveWidget->GetPackageType(PackageID);
		}
	}

	if (PopupType != ESTORE_BUYPOPUP_WIDGET_TYPE::END)
	{
		ConditionalCreatePackageDetailPopup();

		if (Created_UIP_PackageDetailPopup != nullptr)
		{
			Created_UIP_PackageDetailPopup->Init();
			Created_UIP_PackageDetailPopup->SetPopupInfo(ProductId, static_cast<int32>(PopupType));
			Created_UIP_PackageDetailPopup->SetPackageTypeInfo(static_cast<int32>(PackageType));
			Created_UIP_PackageDetailPopup->ShowMe();
		}
	}
}

void UB2UIPackageStore::DestroyPackageDetailPopup()
{
	if (Created_UIP_PackageDetailPopup)
	{
		Created_UIP_PackageDetailPopup->DestroySelf(UB2UIManager::GetInstance());
		Created_UIP_PackageDetailPopup = nullptr;
	}
}

void UB2UIPackageStore::DestroyGuestWarningPopup()
{
	if (Created_UIP_GuestWarningPopup)
	{
		Created_UIP_GuestWarningPopup->DestroySelf(UB2UIManager::GetInstance());
		Created_UIP_GuestWarningPopup = nullptr;
	}
}

void UB2UIPackageStore::ClosePopupConfirmToBuyItem()
{
	// 摧绰 镑俊辑 ConditionalCreate~~ 甫 且 鞘夸啊 绝促.. 捞繁 内靛绰 酒流 秦寸 颇飘啊 荤侩捞 救登绰 惑炔俊辑档 阂副 荐 乐栏骨肺 积己阑 矫档窍搁 扯厚啊 瞪 荐 乐澜.
	if (Created_UIP_PackageDetailPopup)
		Created_UIP_PackageDetailPopup->CloseMe();
}

void UB2UIPackageStore::ConditionalCreatePackageDetailPopup()
{
	//if (!Created_UIP_PackageDetailPopup && X_CP_PackageDetailPopup_Panel.IsValid())
	//{
	//	Created_UIP_PackageDetailPopup = DynLoadAndCreateInCanvasPanelFull<UB2UIStorePackageDetailPopup>(UIP_PackageDetailPopup_Class, this, X_CP_PackageDetailPopup_Panel.Get());
	//	if (Created_UIP_PackageDetailPopup)
	//	{
	//		Created_UIP_PackageDetailPopup->Init();
	//		Created_UIP_PackageDetailPopup->ButtonOneParamDelegate.BindUObject(this, &UB2UIPackageStore::SubmitConfirmToBuy);
	//	}
	//}
}

void UB2UIPackageStore::ConditionalCreateGuestWarningPopup()
{
	//if (!Created_UIP_GuestWarningPopup && X_CP_GuestWarningPopup_Panel.IsValid())
	//{
	//	Created_UIP_GuestWarningPopup = DynLoadAndCreateInCanvasPanelFull<UB2UIMsgPopupGuestWarning>(UIP_GuestWarningPopup_Class, this, X_CP_GuestWarningPopup_Panel.Get());
	//	if (Created_UIP_GuestWarningPopup)
	//	{
	//		Created_UIP_GuestWarningPopup->Init();

	//		BIND_RELEASE_FUNC_TO_BTN(TWeakObjectPtr<UB2Button>(Created_UIP_GuestWarningPopup->GetSlot<UB2Button>(TEXT("BTN_Cancel"))), &UB2UIPackageStore::CloseGuestWarningPopup);
	//		BIND_RELEASE_FUNC_TO_BTN(TWeakObjectPtr<UB2Button>(Created_UIP_GuestWarningPopup->GetSlot<UB2Button>(TEXT("BTN_Buy"))), &UB2UIPackageStore::BuyGuestWarningPopup);
	//	}
	//}
}

void UB2UIPackageStore::RegisterRedDot(int32 CurrentTabType, UB2UICommonTab* TabWidget)
{
	switch (static_cast<EPACKAGE_TAB_TYPE>(CurrentTabType))
	{
	case EPACKAGE_TAB_TYPE::FLATRATE:
		REGISTER_REDDOT_CONDITION(GetRedDotWidgetFromSubWidget(TabWidget), &UB2UIPackageStore::RedDotCondition_FlatRate)
		break;
	case EPACKAGE_TAB_TYPE::LEVELUP:
		REGISTER_REDDOT_CONDITION(GetRedDotWidgetFromSubWidget(TabWidget), &UB2UIPackageStore::RedDotCondition_Level)
		break;
	}
}

bool UB2UIPackageStore::RedDotCondition_FlatRate()
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_Package_FlatRate();
}

bool UB2UIPackageStore::RedDotCondition_Level()
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_Package_Levelup() || BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_Package_ReachLevel();
}

void UB2UIPackageStore::OnClickProbability()
{
	// 啊铆 犬伏 傍俺
	B2P_ShowKakaoGachaOddsView();
}
