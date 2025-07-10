// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIMiniStorePopup.h"
#include "B2UIManager.h"
#include "BladeIIGameImpl.h"
#include "B2UIStoreItemSlot.h"
//#include "B2IAPGenericPlatform.h"
#include "B2UIBackWidgetBase.h"
#include "B2GameInstance.h"



void UB2UIMiniStorePopup::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_Title);
	GET_SLOT(UHorizontalBox, HB_ItemList);
	GET_SLOT(UB2Button, BTN_Close);

	GET_SLOT(UCanvasPanel, X_CP_ConfirmToBuyPopup_Gem_Panel);
	GET_SLOT(UCanvasPanel, X_CP_GuestWarningPopup_Panel);
}

void UB2UIMiniStorePopup::UpdateStaticText()
{
	if (TB_Title.IsValid())
		TB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Mini_title")));
}

void UB2UIMiniStorePopup::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIMiniStorePopup::OnClickBTN_Close);
}

UB2UIMiniStorePopup::UB2UIMiniStorePopup(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Created_UIP_ConfirmToBuyPopup_Gem = nullptr;
}

void UB2UIMiniStorePopup::Init()
{
	Super::Init();

	SubscribeEvents();

	SetProductData();
}

void UB2UIMiniStorePopup::SubscribeEvents()
{
	UnsubscribeEvents();

	Issues.Add(SelectItemToBuyClass<int32>::GetInstance().Subscribe2(
		[this](int32 ProductId)
	{
		this->OpenPopupConfirmToBuyItem(ProductId);
	}
	));
}

void UB2UIMiniStorePopup::UnsubscribeEvents()
{
	Issues.Empty();
}

void UB2UIMiniStorePopup::DestroySelf(UB2UIManager* InUIManager)
{
	DestroyConfirmToBuyPopup_Gem();

	UnsubscribeEvents();

	Super::DestroySelf(InUIManager);
}

void UB2UIMiniStorePopup::SetProductData()
{
	ItemSlots.Empty();

	//if (HB_ItemList.IsValid())
	//{
	//	HB_ItemList->ClearChildren();

	//	TArray<FStoreProductData> ProductList;
	//	BladeIIGameImpl::GetClientDataStore().GetStoreProductList(static_cast<uint32>(EStoreCategory::Goods), static_cast<uint32>(EGoodsCategory::GEM), ProductList);

	//	for (auto& Elem : ProductList)
	//	{
	//		B2InAppPurchase::FB2CashProductInfo* ProductInfo = B2InAppPurchase::IB2IAPGenericPlatform::GetInstance()->GetMarketProduct(Elem.StoreProductId);
	//		if (ProductInfo)
	//		{
	//			auto* ProductData = BladeIIGameImpl::GetClientDataStore().GetStoreProductData(ProductInfo->ShopProductId);
	//			if (ProductData)
	//				SetProductListItem(ProductData);
	//		}
	//	}
	//}
}

void UB2UIMiniStorePopup::SetProductListItem(const FStoreProductData* ProductItem)
{
	auto* CreatedWidget = CreateWidget<UB2UIStoreItemSlot>(GetOwningPlayer(), DefaultItemSlot);
	if (CreatedWidget != NULL)
	{
		auto* AddedSlot = Cast<UHorizontalBoxSlot>(HB_ItemList->AddChild(CreatedWidget));
		check(AddedSlot);
		CreatedWidget->Init();

		// Product
		int32 SlotWidgetNum = static_cast<uint32>(GetProductWidget(ProductItem->StoreProductId));
		int32 SlotIconEffectNum = static_cast<uint32>(GetProductIconEffect(ProductItem->StoreProductId));
		CreatedWidget->CreateSlotWidget(SlotWidgetNum, GetProductIcon(ProductItem->StoreProductId, ProductItem->Product.ProductId), SlotIconEffectNum);
		CreatedWidget->SetData(ProductItem->StoreProductId,
			GetProductIcon(ProductItem->StoreProductId, ProductItem->Product.ProductId),
			GetProductName(ProductItem->StoreProductId, ProductItem->Product.ProductId),
			ProductItem->Product.ProductCount);

		// Bonus
		UMaterialInstance* BonusIcon = nullptr;
		FText BonusName;
		GetBonusIconAndName(static_cast<EStoreItemType>(ProductItem->Bonus.ProductType), BonusIcon, BonusName, ProductItem->Bonus.ProductId);
		CreatedWidget->SetBonusData(ProductItem->Bonus.ProductCount > 0, BonusIcon, BonusName, ProductItem->Bonus.ProductCount, ProductItem->CalculatedBonusCount);

		// Restrict
		CreatedWidget->SetRestrictData(ProductItem->StoreProductId, ProductItem->RestrictType, ProductItem->RestrictCount);

		const FStoreFundsSale* FundsSale = BladeIIGameImpl::GetClientDataStore().GetStoreFundsSaleData(ProductItem->StoreProductId);
		if (FundsSale)
		{
			// Funds Sale
			bool bIsInclude = UB2GameInstance::IncludeTimeNow(FundsSale->StartTime, FundsSale->EndTime);
			if (FundsSale->DiscountRate > 0 && bIsInclude)
				CreatedWidget->SetFundsSale(FundsSale->DiscountRate);
			else
				// Product Mark
				CreatedWidget->SetProductMark(ProductItem->StoreProductId);
		}
		else
		{
			// Product Mark
			CreatedWidget->SetProductMark(ProductItem->StoreProductId);
		}

		// Cash Bonus
		const FStoreCashBonusData* CashBonus = BladeIIGameImpl::GetClientDataStore().GetStoreCashBonusData(ProductItem->StoreProductId);
		if (CashBonus)
		{
			bool bIsInclude = UB2GameInstance::IncludeTimeNow(CashBonus->StartTime, CashBonus->EndTime);
			if (CashBonus->IncreaseRate > 0 && bIsInclude)
				CreatedWidget->SetCashBonus(CashBonus->IncreaseRate);
		}

		// Cost
		CreatedWidget->SetCost(static_cast<EStoreItemCost>(ProductItem->CostType), ProductItem->Cost, ProductItem->CalculatedPrice);

		ItemSlots.Add(CreatedWidget);
	}
}

void UB2UIMiniStorePopup::OpenPopupConfirmToBuyItem(int32 ProductId)
{
	SelectedProductIdToBuy = ProductId;

	//auto* ProductData = BladeIIGameImpl::GetClientDataStore().GetStoreProductData(ProductId);
	//check(ProductData);
	//if (ProductData->Product.ProductType == static_cast<int32>(EStoreItemType::Gem))
	//{
	//	ConditionalCreateConfirmToBuyPopup_Gem();
	//	if (Created_UIP_ConfirmToBuyPopup_Gem)
	//	{
	//		auto* Icon = Created_UIP_ConfirmToBuyPopup_Gem->GetSlot<UImage>(TEXT("IMG_ProductIcon"));
	//		if (Icon)
	//		{
	//			Icon->SetBrushFromMaterial(GetProductIcon(ProductData->StoreProductId, ProductData->Product.ProductId));
	//		}

	//		auto* ContentText = Created_UIP_ConfirmToBuyPopup_Gem->GetSlot<UB2RichTextBlock>(TEXT("TB_Content"));
	//		if (ContentText)
	//		{
	//			ContentText->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Confirm_Buy_Item")), GetProductName(ProductData->StoreProductId, ProductData->Product.ProductId), FText::AsNumber(ProductData->Product.ProductCount)));
	//		}

	//		B2InAppPurchase::FB2CashProductInfo* ProductInfo = B2InAppPurchase::IB2IAPGenericPlatform::GetInstance()->GetMarketProduct(SelectedProductIdToBuy);
	//		TWeakObjectPtr<UB2UIStoreProductCost> Cost = Created_UIP_ConfirmToBuyPopup_Gem->GetSlot<UB2UIStoreProductCost>(TEXT("UIP_CostToBuy"));
	//		if (Cost.IsValid())
	//		{
	//			Cost->Init();
	//			Cost->SetCashCost(ProductInfo->Price, ProductInfo->CurrencyCode);
	//		}

	//		SetBonusData_ConfirmBuy(ProductData->Bonus.ProductCount > 0, static_cast<EStoreItemType>(ProductData->Bonus.ProductType), ProductData->Bonus.ProductCount, ProductData->CalculatedBonusCount);
	//		Created_UIP_ConfirmToBuyPopup_Gem->OpenPopup();
	//	}
	//}
}

void UB2UIMiniStorePopup::ConditionalCreateConfirmToBuyPopup_Gem()
{
	//if (!Created_UIP_ConfirmToBuyPopup_Gem && X_CP_ConfirmToBuyPopup_Gem_Panel.IsValid())
	//{
	//	Created_UIP_ConfirmToBuyPopup_Gem = DynLoadAndCreateInCanvasPanelFull<UB2UIBackWidgetBase>(UIP_ConfirmToBuyPopup_Gem_Class, this, X_CP_ConfirmToBuyPopup_Gem_Panel.Get());
	//	if (Created_UIP_ConfirmToBuyPopup_Gem)
	//	{
	//		Created_UIP_ConfirmToBuyPopup_Gem->Init();

	//		BIND_RELEASE_FUNC_TO_BTN(TWeakObjectPtr<UB2Button>(Created_UIP_ConfirmToBuyPopup_Gem->GetSlot<UB2Button>(TEXT("BTN_Positive"))), &UB2UIMiniStorePopup::SubmitConfirmToBuyGem);
	//		BIND_RELEASE_FUNC_TO_BTN(TWeakObjectPtr<UB2Button>(Created_UIP_ConfirmToBuyPopup_Gem->GetSlot<UB2Button>(TEXT("BTN_Negative"))), &UB2UIMiniStorePopup::CloseConfirmToBuyPopupGemPopup);

	//		B2InAppPurchase::IB2IAPGenericPlatform::OnPurchaseResultDelegate.BindLambda([this](bool success, b2network::B2BuyShopResultInfoPtr GetProduct) {
	//			UE_LOG(LogBladeII, Warning, TEXT("@@@@@@@ OnPurchaseResultDelegate @@@@@@@>> %d"), success);
	//			CompletedPurchaseBuyGem(success, GetProduct);
	//		});
	//	}
	//}
}

void UB2UIMiniStorePopup::SetBonusData_ConfirmBuy(bool bShowBonus, EStoreItemType BonusItemType, int32 InItemCount, int32 InBonusCount)
{
	int32 CalculatedBonusCount = InBonusCount > 0 ? InBonusCount : InItemCount;

	if (Created_UIP_ConfirmToBuyPopup_Gem)
	{
		UB2RichTextBlock* TB_Bonus = Created_UIP_ConfirmToBuyPopup_Gem->GetSlot<UB2RichTextBlock>(TEXT("TB_Bonus"));
		check(TB_Bonus);
		if (TB_Bonus)
		{
			if (bShowBonus)
			{
				TB_Bonus->SetVisibility(ESlateVisibility::HitTestInvisible);

				if (BonusItemType == EStoreItemType::BladePoint)
				{
					TB_Bonus->SetText(FText::Format(
						BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Bonus_Notification")), BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Bladepoint")), FText::AsNumber(CalculatedBonusCount)
					));
				}
				else if (BonusItemType == EStoreItemType::Gem)
				{
					TB_Bonus->SetText(FText::Format(
						BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Bonus_Notification")), BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Gem")), FText::AsNumber(CalculatedBonusCount)
					));
				}
				else if (BonusItemType == EStoreItemType::Gold)
				{
					TB_Bonus->SetText(FText::Format(
						BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Bonus_Notification")), BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Gold")), FText::AsNumber(CalculatedBonusCount)
					));
				}
				else if (BonusItemType == EStoreItemType::DarkCrystal)
				{
					TB_Bonus->SetText(FText::Format(
						BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Bonus_Notification")), BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_DarkCrystal")), FText::AsNumber(CalculatedBonusCount)
					));
				}
			}
			else
			{
				TB_Bonus->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
}

void UB2UIMiniStorePopup::CompletedPurchaseBuyGem(bool success, const FB2ResponseBuyShopResultInfoPtr GetProduct)
{
	if (success)
	{
		BladeIIGameImpl::GetClientDataStore().ResponseBuyShopResultInfo(GetProduct);
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

void UB2UIMiniStorePopup::SubmitConfirmToBuyGem()
{
	CloseConfirmToBuyPopupGemPopup();

	if (B2P_GetKakaoIDPCode() == EPlatformIDPCode::Guest)
	{
		OpenGuestWarningPopup();
	}
	else
	{
		ConfirmToBuyGem();
	}
}

void UB2UIMiniStorePopup::CloseConfirmToBuyPopupGemPopup()
{
	if (Created_UIP_ConfirmToBuyPopup_Gem)
	{
		Created_UIP_ConfirmToBuyPopup_Gem->ClosePopup();
	}
}

void UB2UIMiniStorePopup::DestroyConfirmToBuyPopup_Gem()
{
	if (Created_UIP_ConfirmToBuyPopup_Gem)
	{
		Created_UIP_ConfirmToBuyPopup_Gem->DestroySelf(UB2UIManager::GetInstance());
		Created_UIP_ConfirmToBuyPopup_Gem = nullptr;
	}
}

void UB2UIMiniStorePopup::ConfirmToBuyGem()
{
	//B2InAppPurchase::FB2CashProductInfo* ProductInfo = B2InAppPurchase::IB2IAPGenericPlatform::GetInstance()->GetMarketProduct(SelectedProductIdToBuy);
	//if (ProductInfo)
	//	B2InAppPurchase::IB2IAPGenericPlatform::GetInstance()->RequestPreparePurchase(SelectedProductIdToBuy, ProductInfo->MarketProductId);
}

void UB2UIMiniStorePopup::ConditionalCreateGuestWarningPopup()
{
	if (!Created_UIP_GuestWarningPopup && X_CP_GuestWarningPopup_Panel.IsValid())
	{
		Created_UIP_GuestWarningPopup = DynLoadAndCreateInCanvasPanelFull<UB2UIMsgPopupGuestWarning>(UIP_GuestWarningPopup_Class, this, X_CP_GuestWarningPopup_Panel.Get());
		if (Created_UIP_GuestWarningPopup)
		{
			Created_UIP_GuestWarningPopup->Init();

			BIND_RELEASE_FUNC_TO_BTN(TWeakObjectPtr<UB2Button>(Created_UIP_GuestWarningPopup->GetSlot<UB2Button>(TEXT("BTN_Cancel"))), &UB2UIMiniStorePopup::CloseGuestWarningPopup);
			BIND_RELEASE_FUNC_TO_BTN(TWeakObjectPtr<UB2Button>(Created_UIP_GuestWarningPopup->GetSlot<UB2Button>(TEXT("BTN_Buy"))), &UB2UIMiniStorePopup::BuyGuestWarningPopup);
		}
	}
}

void UB2UIMiniStorePopup::OpenGuestWarningPopup()
{
	ConditionalCreateGuestWarningPopup();

	if (Created_UIP_GuestWarningPopup)
	{
		Created_UIP_GuestWarningPopup->OpenPopup();
	}
}

void UB2UIMiniStorePopup::CloseGuestWarningPopup()
{
	if (Created_UIP_GuestWarningPopup)
	{
		Created_UIP_GuestWarningPopup->ClosePopup();
	}
}

void UB2UIMiniStorePopup::BuyGuestWarningPopup()
{
	if (Created_UIP_GuestWarningPopup)
	{
		Created_UIP_GuestWarningPopup->ClosePopup();
	}

	ConfirmToBuyGem();
}


void UB2UIMiniStorePopup::OnClickBTN_Close()
{
	CloseSelf();
}

////////////////////////////////////////////////////////////////////////////////////////////
ESTORE_SLOT_WIDGET_TYPE UB2UIMiniStorePopup::GetProductWidget(int32 ProductId)
{
	auto* ProductData = ProductIconData->GetDefaultObject<UB2UIStoreProductIconData>();
	if (ProductData)
		return ProductData->GetProductWidget(ProductId);

	return ESTORE_SLOT_WIDGET_TYPE::NONE;
}

UMaterialInstance* UB2UIMiniStorePopup::GetProductIcon(int32 ProductId, int32 OptionalItemId)
{
	auto* ProductData = ProductIconData->GetDefaultObject<UB2UIStoreProductIconData>();
	if (ProductData)
		return ProductData->GetProductIcon(ProductId, OptionalItemId);

	return nullptr;
}

ESTORE_SLOT_EFFECT_TYPE UB2UIMiniStorePopup::GetProductIconEffect(int32 ProductId)
{
	auto* ProductData = ProductIconData->GetDefaultObject<UB2UIStoreProductIconData>();
	if (ProductData)
		return ProductData->GetProductIconEffect(ProductId);

	return ESTORE_SLOT_EFFECT_TYPE::NONE;
}

FText UB2UIMiniStorePopup::GetProductName(int32 ProductId, int32 OptionalItemId)
{
	auto* ProductData = ProductIconData->GetDefaultObject<UB2UIStoreProductIconData>();
	if (ProductData)
		return ProductData->GetProductName(ProductId, OptionalItemId);

	return FText::FromString(TEXT("Unknown Product"));
}

void UB2UIMiniStorePopup::GetBonusIconAndName(EStoreItemType BonusType, UMaterialInstance*& OutBonusIcon, FText& OutBonusName, int32 OptionalItemTemplateId)
{
	auto* ProductData = ProductIconData->GetDefaultObject<UB2UIStoreProductIconData>();
	if (ProductData)
		ProductData->GetBonusIconAndName(BonusType, OutBonusIcon, OutBonusName, OptionalItemTemplateId);
}
