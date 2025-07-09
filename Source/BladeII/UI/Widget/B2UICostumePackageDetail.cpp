// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UICostumeItemSlot.h"
#include "B2UIStoreProductCost.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"
//#include "B2IAPGenericPlatform.h"
#include "B2GameInstance.h"
#include "BladeIIGameImpl.h"
#include "B2UIStoreEventLabel.h"
#include "B2UICostumePackageDetail.h"

void UB2UICostumePackageDetail::CacheAssets()
{
	GET_SLOT(UCanvasPanel, CP_PopupMain);

	GET_SLOT(UTextBlock, TB_Title);
	GET_SLOT(UTextBlock, TB_PackageName);
	GET_SLOT(UTextBlock, TB_PackageDescription);
	GET_SLOT(UTextBlock, TB_BuyDescription);
	GET_SLOT(UTextBlock, TB_RestrictComplete);
	GET_SLOT(UTextBlock, TB_Buy);
	GET_SLOT(UTextBlock, TB_Period);

	UIP_CostumeItemSlots.SetNum(3);
	GET_SLOT_BYNAME(UB2UICostumeItemSlot, UIP_CostumeItemSlots[0], UIP_CostumeItemSlot0);
	GET_SLOT_BYNAME(UB2UICostumeItemSlot, UIP_CostumeItemSlots[1], UIP_CostumeItemSlot1);
	GET_SLOT_BYNAME(UB2UICostumeItemSlot, UIP_CostumeItemSlots[2], UIP_CostumeItemSlot2);
	for (auto & ItemSlot : UIP_CostumeItemSlots)
	{
		if (ItemSlot.IsValid())
		{
			ItemSlot->Init();
			ItemSlot->SetOpenItemInfoPopup(true);
		}
	}

	GET_SLOT(UB2Button, BTN_ItemToBuy);
	GET_SLOT(UB2Button, BTN_Close);

	GET_SLOT(UB2UIStoreProductCost, UIP_Cost);
	if (UIP_Cost.IsValid())
		UIP_Cost->Init();

	GET_SLOT(UB2UIStoreEventLabel, UIP_Store_Event_Label);
	if (UIP_Store_Event_Label.IsValid())
		UIP_Store_Event_Label->Init();
}
void UB2UICostumePackageDetail::UpdateStaticText()
{
	if (TB_Title.IsValid())
		TB_Title->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_STORE), FString(TEXT("Store_CostumeDetail"))));
	
	if (TB_BuyDescription.IsValid())
		TB_BuyDescription->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_STORE), FString(TEXT("Store_PackageBuyDescription"))));

	if (TB_Buy.IsValid())
		TB_Buy->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Buy"))));
}

void UB2UICostumePackageDetail::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UICostumePackageDetail::OnClickClose)
	BIND_CLICK_FUNC_TO_BTN(BTN_ItemToBuy, &UB2UICostumePackageDetail::OnClickBuyToItem)
}

void UB2UICostumePackageDetail::CloseWidgetDelegate()
{
	OnClickClose();
}

void UB2UICostumePackageDetail::OnClickClose()
{
	DestroySelf(UB2UIManager::GetInstance());

	if (UB2UIManager* pUIManager = UB2UIManager::GetInstance())
	{
		pUIManager->CloseAllStandaloneUIs();
	}
}

void UB2UICostumePackageDetail::OnClickBuyToItem()
{
	SelectItemToBuyClass<int32>::GetInstance().Signal(StoreProductID);

}

void UB2UICostumePackageDetail::SetPosition(const FVector2D v2PopupPosition, const float fPopupSizeX)
{
	if (CP_PopupMain.IsValid())
	{
		CP_PopupMain->SetRenderTranslation(v2PopupPosition);

		if (UCanvasPanelSlot* pCanvasPanelSlot = Cast<UCanvasPanelSlot>(CP_PopupMain->Slot))
		{
			pCanvasPanelSlot->SetOffsets(FMargin(0, fTopBezel_BP, fPopupSizeX, 0));
		}
	}
}

void UB2UICostumePackageDetail::SetPackageData(int32 ProductID, const FText& ProductName, const FText &ProductDescription)
{
	const FStoreProductData* PackageProductData = BladeIIGameImpl::GetClientDataStore().GetStoreProductData(ProductID);
	
	if (PackageProductData == nullptr)
	{
		OnClickClose();
		return;
	}
		
	StoreProductID = ProductID;
	
	if (TB_PackageName.IsValid())
		TB_PackageName->SetText(ProductName);

	if (TB_PackageDescription.IsValid())
		TB_PackageDescription->SetText(ProductDescription);

	RefreshData();
}

void UB2UICostumePackageDetail::RefreshData()
{
	const FStoreProductData* PackageProductData = BladeIIGameImpl::GetClientDataStore().GetStoreProductData(StoreProductID);

	if (PackageProductData == nullptr)
	{
		OnClickClose();
		return;
	}

	int32 PackageID = PackageProductData->Product.ProductId;

	if (UB2UIDocStore* DocStore = UB2UIDocHelper::GetDocStore())
	{
		int64 StartTime = 0;
		int64 EndTime = 0;

		bool IsLimitedPackage = DocStore->GetProductSalePeriod(StoreProductID, StartTime, EndTime);
		
		FDateTime StartDateTime = FDateTime::FromUnixTimestamp(StartTime / 1000);
		FDateTime EndDateTime = FDateTime::FromUnixTimestamp(EndTime / 1000);
		FTimespan RemainTime = FDateTime::FromUnixTimestamp(EndTime / 1000) - UB2GameInstance::GetUTCNow();

		int32 UsedCount = DocStore->GetReceivedRestrictCount(StoreProductID);
		bool HaveItem = PackageProductData->RestrictCount == 0 ? false : (PackageProductData->RestrictCount - UsedCount <= 0);

		if (BTN_ItemToBuy.IsValid())
			BTN_ItemToBuy->SetIsEnabled((!IsLimitedPackage || RemainTime.GetSeconds() > 0) && !HaveItem);

		if (TB_Buy.IsValid())
			TB_Buy->SetVisibility(HaveItem ? ESlateVisibility::Hidden : ESlateVisibility::HitTestInvisible);

		if (UIP_Cost.IsValid())
		{
			UIP_Cost->SetVisibility(HaveItem ? ESlateVisibility::Hidden : ESlateVisibility::HitTestInvisible);
			if (!HaveItem)
			{
				if (static_cast<EStoreItemCost>(PackageProductData->CostType) == EStoreItemCost::Cash)
				{
				//	B2InAppPurchase::FB2CashProductInfo* ProductInfo = B2InAppPurchase::IB2IAPGenericPlatform::GetInstance()->GetMarketProduct(PackageProductData->StoreProductId);
				//	if (ProductInfo != nullptr)
				//		UIP_Cost->SetCashCost(ProductInfo->Price, ProductInfo->CurrencyCode);
				}
				else
				{
					UIP_Cost->SetCost(static_cast<EStoreItemCost>(PackageProductData->CostType), PackageProductData->Cost, PackageProductData->CalculatedPrice);
				}
			}
		}

		if (TB_Period.IsValid())
		{
			TB_Period->SetVisibility(IsLimitedPackage ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

			TB_Period->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("EventAttendance_Period2")),
				FText::FromString(FString::FromInt(EndDateTime.GetYear())), FText::FromString(FString::FromInt(EndDateTime.GetMonth())), FText::FromString(FString::FromInt(EndDateTime.GetDay()))
			));
		}

		if (TB_RestrictComplete.IsValid())
		{
			TB_RestrictComplete->SetVisibility(HaveItem ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
			TB_RestrictComplete->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Restrict_Complete")));
		}

		int32 MarkType = b2network::B2ShopMarkType::MARK_NONE;
		MarkType = DocStore->GetProductMark(PackageProductData->StoreProductId);

		if (UIP_Store_Event_Label.IsValid())
			UIP_Store_Event_Label->SetStoreEventLabel(MarkType);
	}

	TArray<FPackageProductData> OutList;
	BladeIIGameImpl::GetClientDataStore().GetPackageProductList(PackageID, OutList);

	if (OutList.Num() > UIP_CostumeItemSlots.Num())
	{
		OnClickClose();
		return;
	}

	for (int32 number = 0; number < UIP_CostumeItemSlots.Num(); ++number)
	{
		if (!OutList.IsValidIndex(number))
		{
			if (UIP_CostumeItemSlots[number].IsValid())
				UIP_CostumeItemSlots[number]->SetVisibility(ESlateVisibility::Hidden);
			continue;
		}

		if (UIP_CostumeItemSlots[number].IsValid())
		{
			UIP_CostumeItemSlots[number]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			//FB2Item
			FB2Item CreateCostumeItem;
			CreateCostumeItem.ItemRefID = OutList[number].PackageProductId;
			CreateCostumeItem.UpdateItemMasterDataOnMD();
			CreateCostumeItem.UpdatePreviewItemPrimaryPoint();
			CreateCostumeItem.UpdatePreviewItemPower();
			UIP_CostumeItemSlots[number]->UpdateCostumeItemSlot_FromFB2Item(CreateCostumeItem);
		}
	}
}
