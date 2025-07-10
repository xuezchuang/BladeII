// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UICostumeStoreItemSlot.h"
#include "B2UIStoreProductCost.h"
#include "B2UIStoreEventLabel.h"
#include "B2UIDocHelper.h"
//#include "B2IAPGenericPlatform.h"
#include "B2GameInstance.h"
#include "B2ItemInfo.h"
#include "BladeIIGameImpl.h"



void UB2UICostumeStoreItemSlot::Init()
{
	Super::Init();
	StoreProductId = INDEX_NONE;
}

void UB2UICostumeStoreItemSlot::CacheAssets()
{
	GET_SLOT(UTextBlock, TB_HaveItem);
	GET_SLOT(UTextBlock, TB_Limited);

	GET_SLOT(UOverlay, OV_SlotWidget);
	GET_SLOT(UOverlay, O_Limited);
	GET_SLOT(UOverlay, O_HaveItem);
	
	GET_SLOT(UImage, IMG_ItemIcon);
	GET_SLOT(UImage, IMG_ItemGrade);
	GET_SLOT(UB2Button, BTN_ItemDetail);
	GET_SLOT(UB2Button, BTN_ItemToBuy);
	GET_SLOT(UB2UIStoreProductCost, UIP_Cost);
	if (UIP_Cost.IsValid())
		UIP_Cost->Init();
	GET_SLOT(UTextBlock, TB_ItemNameAndCount);
	GET_SLOT(UTextBlock, TB_RestrictComplete);

	GET_SLOT(UB2UIStoreEventLabel, UIP_Store_Event_Label);
	if (UIP_Store_Event_Label.IsValid())
		UIP_Store_Event_Label->Init();
}

void UB2UICostumeStoreItemSlot::UpdateStaticText()
{
	if (TB_HaveItem.IsValid())
		TB_HaveItem->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_STORE), FString(TEXT("Store_Possession"))));

	if (TB_Limited.IsValid())
		TB_Limited->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_STORE), FString(TEXT("Store_Limited"))));
}

void UB2UICostumeStoreItemSlot::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_ItemToBuy, &UB2UICostumeStoreItemSlot::OnClickedSlotButton);
	BIND_CLICK_FUNC_TO_BTN(BTN_ItemDetail, &UB2UICostumeStoreItemSlot::OnClickedSlotButton);
}


void UB2UICostumeStoreItemSlot::SetIcon(UMaterialInstance * ProductIcon)
{
	if (IMG_ItemIcon.IsValid())
		IMG_ItemIcon->SetBrushFromMaterial(ProductIcon);
}

void UB2UICostumeStoreItemSlot::SetCostumPackageData(const FStoreProductData & InStoreData, const FText& ProductName)
{
	StoreProductId = InStoreData.StoreProductId;
	StoreData = InStoreData;

	SetPackageProductMark(StoreProductId);
	SetRestrictData(StoreProductId, InStoreData.Product.ProductId);

	if (UB2ItemMiscInfo* pAllItemMiscInfo = StaticFindItemMiscInfo())
	{
		TArray<FPackageProductData> OutList;
		BladeIIGameImpl::GetClientDataStore().GetPackageProductList(InStoreData.Product.ProductId, OutList);

		int32 StarGrade = -1;
		if (OutList.Num() > 0)
		{
			FB2Item CreateCostumeItem;
			CreateCostumeItem.ItemRefID = OutList[0].PackageProductId;
			CreateCostumeItem.UpdateItemMasterDataOnMD();
			StarGrade = CreateCostumeItem.StarGrade;
		}

		if (TB_ItemNameAndCount.IsValid())
		{
			TB_ItemNameAndCount->SetText(ProductName);
			if(StarGrade >= 0)
				TB_ItemNameAndCount->SetColorAndOpacity(pAllItemMiscInfo->GetItemNameTextColor(StarGrade).GetSpecifiedColor());
		}
		if(IMG_ItemGrade.IsValid() && StarGrade >= 0)
		{
			UMaterialInterface* ItemIconBGMtrl = pAllItemMiscInfo->GetCostumeIconBGMtrl(StarGrade);
			if (ItemIconBGMtrl)
				IMG_ItemGrade->SetBrushFromMaterial(ItemIconBGMtrl);
		}
	}
	
	if (static_cast<EStoreItemCost>(StoreData.CostType) == EStoreItemCost::Cash)
	{
		//B2InAppPurchase::FB2CashProductInfo* ProductInfo = B2InAppPurchase::IB2IAPGenericPlatform::GetInstance()->GetMarketProduct(StoreProductId);
		//if (ProductInfo)
		//	UIP_Cost->SetCashCost(ProductInfo->Price, ProductInfo->CurrencyCode);
	}
	else
	{
		UIP_Cost->SetCost(static_cast<EStoreItemCost>(StoreData.CostType), StoreData.Cost, StoreData.CalculatedPrice);
	}
}

void UB2UICostumeStoreItemSlot::SetRestrictData(int32 ProductID, int32 PackageID)
{
	if (UB2UIDocStore* DocStore = UB2UIDocHelper::GetDocStore())
	{
		bool IsLimitedPackage = DocStore->IsPeriodicPackage(PackageID);
		int64 EndTime = DocStore->GetReceivedPeriodicPackageLeftTime(PackageID);
		FTimespan RemainTime = FDateTime::FromUnixTimestamp(EndTime / 1000) - UB2GameInstance::GetUTCNow();

		int32 UsedCount = DocStore->GetReceivedRestrictCount(ProductID);
		bool HaveItem = StoreData.RestrictCount == 0 ? false : (StoreData.RestrictCount - UsedCount <= 0);

		if (O_HaveItem.IsValid())
			O_HaveItem->SetVisibility(HaveItem ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);

		if (O_Limited.IsValid())
			O_Limited->SetVisibility(IsLimitedPackage ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);

		if (BTN_ItemToBuy.IsValid())
			BTN_ItemToBuy->SetIsEnabled((!IsLimitedPackage || RemainTime.GetSeconds() > 0) && !HaveItem);

		if (UIP_Cost.IsValid())
			UIP_Cost->SetVisibility(HaveItem ? ESlateVisibility::Hidden : ESlateVisibility::HitTestInvisible);

		if (TB_RestrictComplete.IsValid())
		{
			TB_RestrictComplete->SetVisibility(HaveItem ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
			TB_RestrictComplete->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Restrict_Complete")));
		}
	}
}

void UB2UICostumeStoreItemSlot::SetPackageProductMark(int32 ProductId)
{
	if (UB2UIDocStore* DocStore = UB2UIDocHelper::GetDocStore())
	{
		
		int32 MarkType = b2network::B2ShopMarkType::MARK_NONE;
		MarkType = DocStore->GetProductMark(ProductId);

		if (UIP_Store_Event_Label.IsValid())
			UIP_Store_Event_Label->SetStoreEventLabel(MarkType);
	}
}

void UB2UICostumeStoreItemSlot::OnClickedSlotButton()
{
	//UI Popup + 코스튬 미리보기
	SelectCostumePackageClass<int32>::GetInstance().Signal(StoreProductId);
	
}

