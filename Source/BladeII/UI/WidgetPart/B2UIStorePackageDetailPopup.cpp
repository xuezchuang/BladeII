#include "B2UIStorePackageDetailPopup.h"
#include "B2AndroidBackManager.h"
#include "B2UIHeader.h"
#include "B2UIManager.h"
#include "B2ItemInfo.h"
#include "B2UIStore.h"
#include "B2UIStoreProductCost.h"
#include "B2RewardInfo.h"
#include "BladeIIGameImpl.h"
#include "B2PlatformAPI.h"
#include "../../DataStore/B2ClientDataStore.h"
#include "../B2RichTextBlock.h"
//#include "B2IAPGenericPlatform.h"


//Slot
void UB2UIStorePackageDetailPopupSlot::Init()
{
	Super::Init();
}

void UB2UIStorePackageDetailPopupSlot::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UImage, IMG_ItemIcon);

	GET_SLOT(UPanelWidget, P_Bonus);
	GET_SLOT(UPanelWidget, P_Daily);
	GET_SLOT(UTextBlock, TB_Daily);
	GET_SLOT(UTextBlock, TB_Count);
	GET_SLOT(UTextBlock, TB_Name);
	GET_SLOT(UTextBlock, TB_Bonus);
	GET_SLOT(UB2ButtonGoodInfoToolTip, BTN_Goods);
}

void UB2UIStorePackageDetailPopupSlot::SetInfo(int32 ItemID, UMaterialInterface* IconImage, int32 ItemCount, FText ItemName, int32 Period, bool IsDaily)
{
	ProductCount = ItemCount;
	PayPeriod = Period;

	if (IMG_ItemIcon.IsValid())
		IMG_ItemIcon->SetBrushFromMaterial(IconImage);

	if (TB_Count.IsValid())
		TB_Count->SetText(FText::AsNumber(ItemCount));

	if (TB_Name.IsValid())
		TB_Name->SetText(ItemName);

	if (P_Daily.IsValid())
		P_Daily->SetVisibility(IsDaily ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);

	if (BTN_Goods.IsValid())
		BTN_Goods->SetItemInfo(ItemID);

	if (TB_Daily.IsValid())
		TB_Daily->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_PackageText3")));

	if (P_Bonus.IsValid())
		P_Bonus->SetVisibility(IsDaily ? ESlateVisibility::Hidden : ESlateVisibility::Collapsed);
}

void UB2UIStorePackageDetailPopupSlot::SetRewardInfo(int32 RewardID, int32 Period, int32 RewardCount)
{
	ProductCount = RewardCount;
	PayPeriod = Period;

	FSingleRewardInfoData* RewardInfo = StaticFindRewardInfo() ? StaticFindRewardInfo()->GetInfoData(RewardID) : nullptr;
	UMaterialInterface* IconMat = StaticFindRewardInfo() ? StaticFindRewardInfo()->GetRewardIconMaterial(RewardID) : nullptr;

	if (!RewardInfo) return;

	if (IMG_ItemIcon.IsValid())
	{
		if (IconMat != nullptr)
			IMG_ItemIcon->SetBrushFromMaterial(IconMat);
	}

	if (TB_Count.IsValid())
		TB_Count->SetText(FText::AsNumber(RewardCount));

	if (TB_Name.IsValid())
		TB_Name->SetText(BladeIIGetLOCText(RewardInfo->TextCategory, RewardInfo->RewardNameForTextTable));

	if (P_Daily.IsValid())
		P_Daily->SetVisibility(ESlateVisibility::HitTestInvisible);

	if (BTN_Goods.IsValid())
		BTN_Goods->SetItemInfo(RewardInfo->ItemInfoIndex);

	if (TB_Daily.IsValid())
		TB_Daily->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_BoostPackage_BonusDesc")));

	if (P_Bonus.IsValid())
		P_Bonus->SetVisibility(ESlateVisibility::HitTestInvisible);
	if (TB_Bonus.IsValid())
		TB_Bonus->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_BonusGoods")));
}

// Popup
void UB2UIStorePackageDetailPopup::Init()
{
	Super::Init();
}

void UB2UIStorePackageDetailPopup::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UImage, IMG_Package);

	GET_SLOT(UB2Button, BTN_Buy);
	GET_SLOT(UB2Button, BTN_Detail);
	GET_SLOT(UB2Button, BTN_Close);

	GET_SLOT(UTextBlock, TB_Title);
	GET_SLOT(UTextBlock, TB_TotalPrice);
	GET_SLOT(UB2RichTextBlock, TB_BoostDesc);
	GET_SLOT(UTextBlock, TB_ItemName);

	GET_SLOT(UTextBlock, STB_PocketItem);
	GET_SLOT(UTextBlock, TB_UsedItem);
	GET_SLOT(UB2RichTextBlock, TB_WarningSummury);
	GET_SLOT(UB2RichTextBlock, RTB_BuyDesc);
	GET_SLOT(UTextBlock, STB_Detail);
	GET_SLOT(UTextBlock, STB_Buy);

	GET_SLOT(UWidgetSwitcher, WS_Content);

	GET_SLOT(UUniformGridPanel, GP_PackageList);

	GET_SLOT(UB2UIStoreProductCost, UIP_Cost);
	if (UIP_Cost.IsValid())
		UIP_Cost->Init();

	BoostItemWidgets.Empty();
	BoostItemWidgets.SetNum(2);
	GET_SLOT_BYNAME(UB2UIStorePackageDetailPopupSlot, BoostItemWidgets[0], UIP_BoostItem1);
	GET_SLOT_BYNAME(UB2UIStorePackageDetailPopupSlot, BoostItemWidgets[1], UIP_BoostItem2);

}

void UB2UIStorePackageDetailPopup::UpdateStaticText()
{
	Super::UpdateStaticText();

	if (TB_Title.IsValid())
		TB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_PackageText4")));
	if (STB_PocketItem.IsValid())
		STB_PocketItem->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_PackageText1")));
	if (RTB_BuyDesc.IsValid())
		RTB_BuyDesc->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Terms_Text")));
	if (STB_Detail.IsValid())
		STB_Detail->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_PackageText2")));
	if (STB_Buy.IsValid())
		STB_Buy->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Buy")));
}

void UB2UIStorePackageDetailPopup::BindDelegates()
{
	BIND_RELEASE_FUNC_TO_BTN(BTN_Buy, &UB2UIStorePackageDetailPopup::OnClickBTN_Buy);
	BIND_RELEASE_FUNC_TO_BTN(BTN_Detail, &UB2UIStorePackageDetailPopup::OnClickBTN_Detail);
	BIND_RELEASE_FUNC_TO_BTN(BTN_Close, &UB2UIStorePackageDetailPopup::OnClickBtnClose);
}

void UB2UIStorePackageDetailPopup::CloseWidgetDelegate()
{
	OnClickBtnClose();
}

void UB2UIStorePackageDetailPopup::OnClickBtnClose()
{
	CloseMe();
}

void UB2UIStorePackageDetailPopup::CloseMe()
{
	DestroyRewardIcons();
	SetPopupVisible(false);
}

void UB2UIStorePackageDetailPopup::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);

	DestroyRewardIcons();

	if (UIP_Cost.IsValid())
		UIP_Cost->DestroySelf(InUIManager);

	for (auto Item : BoostItemWidgets)
	{
		Item->DestroySelf(InUIManager);
	}
}

void UB2UIStorePackageDetailPopup::SetPopupVisible(bool IsVisible)
{
	if (IsVisible && this->GetVisibility() != ESlateVisibility::Visible)
	{
		this->SetVisibility(ESlateVisibility::Visible);
		B2AndroidBackManager::GetInstance()->AddBackWidget(this);

		UB2UIHeader* HeaderUI = UB2UIManager::GetInstance()->GetUI<UB2UIHeader>(UIFName::Header);
		if (HeaderUI != nullptr)
			HeaderUI->SetForcedBlockHeader(true);
	}
	else if (IsVisible == false && this->GetVisibility() != ESlateVisibility::Collapsed)
	{
		this->SetVisibility(ESlateVisibility::Collapsed); // 郴何 酒捞能甸篮 destroy 窍瘤父 磊脚篮 见扁扁父 窃.
		B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);

		UB2UIHeader* HeaderUI = UB2UIManager::GetInstance()->GetUI<UB2UIHeader>(UIFName::Header);
		if (HeaderUI != nullptr)
			HeaderUI->SetForcedBlockHeader(false);
	}
}

void UB2UIStorePackageDetailPopup::ShowMe()
{
	SetPopupVisible(true);
	//OnPopupBP();
}

void UB2UIStorePackageDetailPopup::SetPopupInfo(int32 ProuctID, int32 popupType)
{
	CurrentStoreID = ProuctID;
	auto* ProductData = BladeIIGameImpl::GetClientDataStore().GetStoreProductData(ProuctID);
	check(ProductData);
	if (ProductData->StoreCategory == 0 && ProductData->GoodsCategory == -1)
	{
		int32 PakageID = ProductData->Product.ProductId;

		SetMainPackageInfo(PakageID, ProductData->CostType, ProductData->Cost, ProductData->CalculatedPrice);

		if (popupType == static_cast<int32>(ESTORE_BUYPOPUP_WIDGET_TYPE::BOOST))
			SetBoostListInfo(ProductData);
		else
			SetBasicListInfo(ProductData);

		if (WS_Content.IsValid())
			WS_Content->SetActiveWidgetIndex(popupType);
	}
}

void UB2UIStorePackageDetailPopup::SetPackageTypeInfo(int32 PackageType)
{
	switch (static_cast<EPACKAGE_TYPE>(PackageType))
	{
	case EPACKAGE_TYPE::SevenDays_Summon:
		if (TB_UsedItem.IsValid())
		{
			TB_UsedItem->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			TB_UsedItem->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Package_Used_SummonItem")));
		}
		if (TB_WarningSummury.IsValid())
			TB_WarningSummury->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Package_WarningSummury")));
		break;

	case EPACKAGE_TYPE::Gold_Boost:
	case EPACKAGE_TYPE::Exp_Boost:
		if (TB_UsedItem.IsValid())
		{
			TB_UsedItem->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			TB_UsedItem->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Package_Used_HotTimeBuff")));
		}
		if (TB_WarningSummury.IsValid())
			TB_WarningSummury->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Package_WarningSummury")));
		break;

	case EPACKAGE_TYPE::BladePoint_Boost:
	case EPACKAGE_TYPE::Time_Boost:
		if (TB_UsedItem.IsValid())
			TB_UsedItem->SetVisibility(ESlateVisibility::Collapsed);
		if (TB_WarningSummury.IsValid())
			TB_WarningSummury->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Package_WarningSummury")));
		break;

	case EPACKAGE_TYPE::Normal_Package:
		if (TB_UsedItem.IsValid())
			TB_UsedItem->SetVisibility(ESlateVisibility::Collapsed);
		if (TB_WarningSummury.IsValid())
			TB_WarningSummury->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Package_Receive_Mail")));
		break;

	default:
		break;
	}
}

void UB2UIStorePackageDetailPopup::SetMainPackageInfo(int32 PackageID, int32 CostType, int32 Cost, int32 CalculatedCost)
{
	if (IMG_Package.IsValid())
	{
		IMG_Package->SetBrushFromMaterial(GetProductIcon(PackageID));
	}

	if (TB_ItemName.IsValid())
	{
		TB_ItemName->SetText(GetProductName(PackageID));
	}

	//if (UIP_Cost.IsValid())
	//{
	//	if (static_cast<EStoreItemCost>(CostType) == EStoreItemCost::Cash)
	//	{
	//		B2InAppPurchase::FB2CashProductInfo* ProductInfo = B2InAppPurchase::IB2IAPGenericPlatform::GetInstance()->GetMarketProduct(CurrentStoreID);
	//		if (ProductInfo)
	//			UIP_Cost->SetCashCost(ProductInfo->Price, ProductInfo->CurrencyCode);
	//	}
	//	else
	//	{
	//		UIP_Cost->SetCost(static_cast<EStoreItemCost>(CostType), Cost, CalculatedCost);
	//	}
	//}
}

void UB2UIStorePackageDetailPopup::SetBasicListInfo(const FStoreProductData* ProductData)
{
	DestroyRewardIcons();

	int32 ProductCnt = 0;
	TArray<FPackageProductData> ProductList;

	BladeIIGameImpl::GetClientDataStore().GetPackageProductList(ProductData->Product.ProductId, ProductList);
	UB2ItemInfo* AllItemInfo = StaticFindItemInfo();

	for (auto& Elem : ProductList)
	{
		int32 ItemID = BladeIIGameImpl::GetClientDataStore().GetItemTemplateIdFromMDProductData(Elem.PackageProductId, Elem.PackageProductType);
		FSingleItemInfoData* ThisItemInfo = AllItemInfo ? AllItemInfo->GetInfoData(ItemID) : NULL;

		if (ThisItemInfo != nullptr)
		{
			auto* CreatedWidget = CreateWidget<class UB2UIStorePackageDetailPopupSlot>(GetOwningPlayer(), PopupSlot);
			if (CreatedWidget != NULL)
			{
				CreatedWidget->Init();
				CreatedWidget->SetInfo(ItemID,
					ThisItemInfo->GetIconMaterial(AllItemInfo),
					Elem.PackageProductCount,
					ThisItemInfo->GetLocalizedName(),
					ProductData->PayPeriod,
					false);

				int32 nRow = (int32)(ProductCnt / 4);
				int32 nColum = ProductCnt % 4;

				UUniformGridSlot* GridWidgetSlot = GP_PackageList->AddChildToUniformGrid(CreatedWidget);

				if (GridWidgetSlot)
				{
					//GridWidgetSlot->SetPadding(5.0);
					GridWidgetSlot->SetRow(nRow);
					GridWidgetSlot->SetColumn(nColum);
				}
				CreatedRewardIcons.Add(CreatedWidget);
				ProductCnt++;
			}
		}
	}
}

void UB2UIStorePackageDetailPopup::SetBoostListInfo(const FStoreProductData* ProductData)
{
	TArray<FPackageProductData> ProductList;
	BladeIIGameImpl::GetClientDataStore().GetPackageProductList(ProductData->Product.ProductId, ProductList);
	const FPackageData* PackageData = BladeIIGameImpl::GetClientDataStore().GetPackageData(ProductData->Product.ProductId);
	UB2ItemInfo* AllItemInfo = StaticFindItemInfo();

	int32 SlotCount = 0;
	for (auto ProductItem : ProductList)
	{
		int32 ItemID = BladeIIGameImpl::GetClientDataStore().GetItemTemplateIdFromMDProductData(ProductItem.PackageProductId, ProductItem.PackageProductType);
		FSingleItemInfoData* ThisItemInfo = AllItemInfo ? AllItemInfo->GetInfoData(ItemID) : NULL;

		if (ThisItemInfo)
		{
			if (BoostItemWidgets[SlotCount].IsValid())
			{
				BoostItemWidgets[SlotCount]->Init();
				BoostItemWidgets[SlotCount]->SetInfo(ItemID,
					ThisItemInfo->GetIconMaterial(AllItemInfo),
					ProductItem.PackageProductCount,
					ThisItemInfo->GetLocalizedName(),
					ProductData->PayPeriod,
					true);
				SlotCount++;
			}
		}
	}

	// [180417_YJ] 何胶飘俊 7老 焊呈胶 焊惑 俺充捞 眠啊登菌澜.
	// 捞 版快绰 沥富 漂荐茄 版快咯辑 焊呈胶 = 焊惑狼 俺充 捞扼绊 窃.
	// 绊肺 老馆利牢 酒捞袍 鸥涝栏肺 酒捞袍沥焊甫 掘绢坷绰霸 酒聪扼.. RewardID 蔼栏肺 沥焊甫 技泼秦林绢具窃.
	if (SlotCount == 1 && PackageData)
	{
		if (PackageData->PackagePeriodBonusID != 0)
		{
			if (BoostItemWidgets[SlotCount].IsValid())
			{
				BoostItemWidgets[SlotCount]->Init();
				BoostItemWidgets[SlotCount]->SetRewardInfo(PackageData->PackagePeriodBonusID, PackageData->PackagePeriod, PackageData->PackagePeriodBonusCount);
			}
		}
	}

	if (TB_BoostDesc.IsValid())
	{
		FString TempString = TEXT("Store_BoostPackage_Desc") + FString::FormatAsNumber(ProductData->Product.ProductId); // 缔磊府绰 菩虐瘤 ID
		FText TempText = BladeIIGetLOCText(B2LOC_CAT_STORE, TempString);

		TempText = FText::Format(TempText, BoostItemWidgets[0]->GetTotalProductCount(), BoostItemWidgets[1]->GetTotalProductCount());

		TB_BoostDesc->SetText(TempText);
	}

}

void UB2UIStorePackageDetailPopup::CreateRewardIcons()
{
	DestroyRewardIcons();
}

void UB2UIStorePackageDetailPopup::DestroyRewardIcons()
{
	UB2UIManager* UIMgr = UB2UIManager::GetInstance();
	for (UB2UIStorePackageDetailPopupSlot* ThisIcon : CreatedRewardIcons)
	{
		if (ThisIcon)
		{
			ThisIcon->DestroySelf(UIMgr);
		}
	}
	CreatedRewardIcons.Empty();
	GP_PackageList->ClearChildren();
}

void UB2UIStorePackageDetailPopup::OnClickBTN_Buy()
{
	ButtonOneParamDelegate.ExecuteIfBound(CurrentStoreID);
}

void UB2UIStorePackageDetailPopup::OnClickBTN_Detail()
{
	B2P_ShowOpenURL(B2P_GetKakaoCustomData(TEXT("withdSubscUrl")));
}

UMaterialInstance* UB2UIStorePackageDetailPopup::GetProductIcon(int32 ProductId)
{
	auto* ProductData = ProductIconData->GetDefaultObject<UB2UIStoreProductIconData>();
	if (ProductData)
		return ProductData->GetProductIcon(ProductId, ProductId);

	return nullptr;
}

FText UB2UIStorePackageDetailPopup::GetProductName(int32 ProductId)
{
	auto* ProductData = ProductIconData->GetDefaultObject<UB2UIStoreProductIconData>();
	if (ProductData)
		return ProductData->GetProductName(ProductId, ProductId);

	return FText::FromString(TEXT("Unknown Product"));
}
