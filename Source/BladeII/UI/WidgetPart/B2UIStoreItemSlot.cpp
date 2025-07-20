#include "B2UIStoreItemSlot.h"
#include "Event.h"

#include "BladeIILocText.h"
#include "B2UIStoreItemSlotIcon.h"
#include "B2UIDocHelper.h"
#include "B2UIStore.h"
#include "B2UIStoreEventLabel.h"
//#include "B2IAPGenericPlatform.h"


void UB2UIStoreItemSlot::Init()
{
	Super::Init();
}

void UB2UIStoreItemSlot::CacheAssets()
{
	GET_SLOT(UOverlay, OV_SlotWidget);
	GET_SLOT(UOverlay, OV_SlotWidget_Dark);
	GET_SLOT(UOverlay, OV_SlotWidget_Fame);
	GET_SLOT(UImage, IMG_ItemIcon);
	GET_SLOT(UB2Button, BTN_ItemToBuy);
	GET_SLOT(UB2Button, BTN_GetReward);
	GET_SLOT(UB2UIStoreProductCost, UIP_Cost);
	if (UIP_Cost.IsValid())
		UIP_Cost->Init();
	GET_SLOT(UTextBlock, TB_ItemNameAndCount);

	GET_SLOT(UOverlay, OV_Bonus);
	GET_SLOT(UImage, IMG_BonusItemIcon);
	GET_SLOT(UTextBlock, TB_BonusItemName);
	GET_SLOT(UTextBlock, TB_BonusItemCount);
	GET_SLOT(UTextBlock, TB_BonusText);

	GET_SLOT(UOverlay, OV_PreBonusItemCount);
	GET_SLOT(UB2RichTextBlock, RTB_PreBonusItemCount);
	GET_SLOT(UImage, IMG_Arrow);

	GET_SLOT(UTextBlock, TB_RestrictType);
	GET_SLOT(UTextBlock, TB_RestrictCount);
	GET_SLOT(UTextBlock, TB_RestrictComplete);

	GET_SLOT(UTextBlock, TB_LeftDay);

	GET_SLOT(UWidgetSwitcher, SW_Restrict);

	GET_SLOT(UOverlay, OV_DailyLimit);
	GET_SLOT(UOverlay, OV_SalePeriod);
	GET_SLOT(UTextBlock, TB_SalePeriod);

	GET_SLOT(UB2UIStoreEventLabel, UIP_Store_Event_Label);
	if (UIP_Store_Event_Label.IsValid())
		UIP_Store_Event_Label->Init();

	if (OV_SalePeriod.IsValid())
		OV_SalePeriod->SetVisibility(ESlateVisibility::Collapsed);
	if (OV_DailyLimit.IsValid())
		OV_DailyLimit->SetVisibility(ESlateVisibility::Collapsed);
}

void UB2UIStoreItemSlot::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_ItemToBuy, &UB2UIStoreItemSlot::OnClickedSlotButton);
	BIND_CLICK_FUNC_TO_BTN(BTN_GetReward, &UB2UIStoreItemSlot::OnClickedSlotButton);
}

void UB2UIStoreItemSlot::OnClickedSlotButton()
{
	SelectItemToBuyClass<int32>::GetInstance().Signal(StoreProductId);
}

void UB2UIStoreItemSlot::CreateSlotWidget(int32 SlotWidgetType, class UMaterialInstance* ProductIcon, int32 SlotIconEffectType)
{
	if (!StoreSlotWidgetArray.IsValidIndex(SlotWidgetType)) return;

	TSubclassOf<UB2UIStoreItemSlotIcon> StoreSlotUIClass = StoreSlotWidgetArray[SlotWidgetType].LoadSynchronous();
	auto* SlotIcon = StoreSlotUIClass ? CreateWidget<UB2UIStoreItemSlotIcon>(GetWorld(), StoreSlotUIClass) : nullptr;

	if (SlotIcon)
	{
		if (SlotWidgetType == static_cast<int32>(ESTORE_SLOT_WIDGET_TYPE::DARKCRYSTAL))
		{
			if (OV_SlotWidget_Dark.IsValid())
			{
				SlotIcon->Init();
				SlotIcon->SetIcon(ProductIcon);
				SlotIcon->SetEffect(SlotIconEffectType);

				OV_SlotWidget_Dark->AddChild(SlotIcon);
			}
		}
		else if (SlotWidgetType == static_cast<int32>(ESTORE_SLOT_WIDGET_TYPE::FAME_POINT))
		{
			if (OV_SlotWidget_Fame.IsValid())
			{
				SlotIcon->Init();
				SlotIcon->SetIcon(ProductIcon);
				SlotIcon->SetEffect(SlotIconEffectType);

				OV_SlotWidget_Fame->AddChild(SlotIcon);
			}
		}
		else
		{
			if (OV_SlotWidget.IsValid())
			{
				SlotIcon->Init();
				SlotIcon->SetIcon(ProductIcon);
				SlotIcon->SetEffect(SlotIconEffectType);

				OV_SlotWidget->AddChild(SlotIcon);
			}
		}
	}
}

void UB2UIStoreItemSlot::SetData(int32 InStoreProductId, class UMaterialInstance* ProductIcon, const FText& ProductName, int32 InItemCount, bool bPackage)
{
	StoreProductId = InStoreProductId;
	Count = InItemCount;

	if (TB_ItemNameAndCount.IsValid())
	{
		if (bPackage)
			TB_ItemNameAndCount->SetText(ProductName);
		else
			TB_ItemNameAndCount->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_ItemNameAndCountToBuy")), ProductName, FText::AsNumber(Count)));
	}

	if (IMG_ItemIcon.IsValid() && ProductIcon)
	{
		IMG_ItemIcon->SetBrushFromMaterial(ProductIcon);
	}

	if (SW_Restrict.IsValid())
	{
		SW_Restrict->SetActiveWidget(0);
	}
}

void UB2UIStoreItemSlot::SetBonusData(bool bShowBonus, class UMaterialInstance* BonusIcon, const FText& BonusName, int32 InItemCount, int32 InBonusCount)
{
	if (OV_PreBonusItemCount.IsValid())
		OV_PreBonusItemCount->SetVisibility(InBonusCount > 0 ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	if (IMG_Arrow.IsValid())
		IMG_Arrow->SetVisibility(InBonusCount > 0 ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	if (OV_Bonus.IsValid())
		OV_Bonus->SetVisibility(bShowBonus ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);

	if (IMG_BonusItemIcon.IsValid())
		IMG_BonusItemIcon->SetBrushFromMaterial(BonusIcon);

	if (TB_BonusItemName.IsValid())
		TB_BonusItemName->SetText(BonusName);

	if (TB_BonusText.IsValid())
		TB_BonusText->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Bonus_Product")), BonusName));

	if (InBonusCount > 0)
	{
		if (RTB_PreBonusItemCount.IsValid())
			RTB_PreBonusItemCount->SetText(FText::AsNumber(InItemCount));

		if (TB_BonusItemCount.IsValid())
		{
			TB_BonusItemCount->SetText(FText::AsNumber(InBonusCount));
			TB_BonusItemCount->SetColorAndOpacity(FLinearColor::Red);
		}
	}
	else
	{
		if (TB_BonusItemCount.IsValid())
			TB_BonusItemCount->SetText(FText::Format(FText::FromString(TEXT("x{0}")), FText::AsNumber(InItemCount)));
	}
}

void UB2UIStoreItemSlot::SetRestrictData(int32 ProductId, int32 RestrictType, int32 RestrictCount)
{
	if (TB_RestrictType.IsValid())
	{
		TB_RestrictType->SetVisibility(RestrictType == EStoreRestrictType::NONE ? ESlateVisibility::Hidden : ESlateVisibility::SelfHitTestInvisible);
		TB_RestrictType->SetText(GetRestrictTypeName(RestrictType));
	}

	if (OV_DailyLimit.IsValid())
	{
		OV_DailyLimit->SetVisibility(RestrictType == EStoreRestrictType::NONE ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	}

	if (TB_RestrictCount.IsValid())
	{
		if (RestrictType == EStoreRestrictType::NONE)
		{
			TB_RestrictCount->SetVisibility(ESlateVisibility::Hidden);
			TB_RestrictComplete->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			if (SW_Restrict.IsValid())
				SW_Restrict->SetActiveWidgetIndex(0);

			int32 RemainCount = 0;
			if (UB2UIDocStore* DocStore = UB2UIDocHelper::GetDocStore())
			{
				int32 UsedCount = DocStore->GetReceivedRestrictCount(ProductId);
				RemainCount = RestrictCount - UsedCount;

				TB_RestrictCount->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				TB_RestrictCount->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Restrict_Count")), RemainCount, RestrictCount));
			}

			if (BTN_ItemToBuy.IsValid())
				BTN_ItemToBuy->SetIsEnabled(RemainCount == 0 ? false : true);
			if (BTN_GetReward.IsValid())
				BTN_GetReward->SetIsEnabled(RemainCount == 0 ? false : true);

			if (UIP_Cost.IsValid())
				UIP_Cost->SetVisibility(RemainCount == 0 ? ESlateVisibility::Hidden : ESlateVisibility::SelfHitTestInvisible);
			if (TB_RestrictComplete.IsValid())
			{
				TB_RestrictComplete->SetVisibility(RemainCount == 0 ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
				TB_RestrictComplete->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Restrict_Complete")));
			}
		}
	}

}

void UB2UIStoreItemSlot::SetPeriodData(int32 ProductId)
{
	if (UB2UIDocStore* DocStore = UB2UIDocHelper::GetDocStore())
	{
		int64 EndTime = DocStore->GetReceivedPeriodicPackageLeftTime(ProductId);
		FTimespan RemainTime = FDateTime::FromUnixTimestamp(EndTime / 1000) - FDateTime::UtcNow();

		if (SW_Restrict.IsValid())
			SW_Restrict->SetActiveWidgetIndex(RemainTime.GetSeconds() > 0 ? 1 : 0);

		if (BTN_ItemToBuy.IsValid())
			BTN_ItemToBuy->SetIsEnabled(RemainTime.GetSeconds() > 0 ? false : true);
		if (BTN_GetReward.IsValid())
			BTN_GetReward->SetIsEnabled(RemainTime.GetSeconds() > 0 ? false : true);

		if (UIP_Cost.IsValid())
			UIP_Cost->SetVisibility(RemainTime.GetSeconds() > 0 ? ESlateVisibility::Hidden : ESlateVisibility::HitTestInvisible);
		if (TB_RestrictComplete.IsValid())
		{
			TB_RestrictComplete->SetVisibility(RemainTime.GetSeconds() > 0 ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
			TB_RestrictComplete->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Restrict_Complete")));
		}

		int32 RemainDay = RemainTime.GetDays();
		int32 RemainHour = RemainTime.GetHours();
		int32 RemainMin = RemainTime.GetMinutes();
		int32 RemainSecond = RemainTime.GetSeconds();

		if(TB_LeftDay.IsValid())
		{
			if (RemainDay > 0)
			{
				TB_LeftDay->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_PackagePeriod")), RemainDay));
			}
			else
			{
				if (RemainHour > 0)
				{
					TB_LeftDay->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_PackagePeriod_Hour")), RemainHour, RemainMin));
				}
				else
				{
					if (RemainMin > 0)
					{
						TB_LeftDay->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_PackagePeriod_Minute")), RemainMin));
					}
					else
					{
						TB_LeftDay->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_PackagePeriod_Second")));
					}
				}
			}
		}
	}
}

FText UB2UIStoreItemSlot::GetRestrictTypeName(int32 RestrictType)
{
	switch (RestrictType)
	{
	case EStoreRestrictType::DAILY:
		return BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Restrict_Daily"));

	case EStoreRestrictType::WEEKLY:
		return BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Restrict_Weekly"));

	case EStoreRestrictType::MONTHLY:
		return BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Restrict_Monthly"));

	case EStoreRestrictType::NUMBER:
		return BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Restrict_Number"));

	default:
		return FText::FromString(TEXT("Unknown Product"));
	}
}

void UB2UIStoreItemSlot::SetProductMark(int32 ProductId)
{
	if (UB2UIDocStore* DocStore = UB2UIDocHelper::GetDocStore())
	{
		int32 ProductMark = DocStore->GetProductMark(ProductId);

		if (UIP_Store_Event_Label.IsValid())
			UIP_Store_Event_Label->SetStoreEventLabel(ProductMark);
	}
}

void UB2UIStoreItemSlot::SetPackageProductMark(int32 ProductId)
{
	if (UB2UIDocStore* DocStore = UB2UIDocHelper::GetDocStore())
	{
		int32 PackageProductMark = DocStore->GetPackageProductMark(ProductId);

		if (UIP_Store_Event_Label.IsValid())
			UIP_Store_Event_Label->SetStoreEventLabel(PackageProductMark);
	}
}

void UB2UIStoreItemSlot::SetFundsSale(int32 DiscountRate)
{
	if (UIP_Store_Event_Label.IsValid())
		UIP_Store_Event_Label->SetFundsSale(DiscountRate);
}

void UB2UIStoreItemSlot::SetCashBonus(int32 IncreaseRate)
{
	if (UIP_Store_Event_Label.IsValid())
		UIP_Store_Event_Label->SetCashBonus(IncreaseRate);
}

void UB2UIStoreItemSlot::SetCost(EStoreItemCost InCostType, int32 InCost, int32 InCalculatedCost)
{
	//B2InAppPurchase::FB2CashProductInfo* ProductInfo = B2InAppPurchase::IB2IAPGenericPlatform::GetInstance()->GetMarketProduct(StoreProductId);
	//if (ProductInfo && InCostType == EStoreItemCost::Cash)
	//{
	//	if (UIP_Cost.IsValid())
	//	{
	//		UIP_Cost->SetCashCost(ProductInfo->Price, ProductInfo->CurrencyCode);
	//	}
	//}
	//else
	//{
	//	if (UIP_Cost.IsValid())
	//	{
	//		UIP_Cost->SetCost(InCostType, InCost, InCalculatedCost);
	//	}
	//}
}

void UB2UIStoreItemSlot::SetSalePeriodData(bool bIsLimited, int64 EndTime)
{
	if (OV_SalePeriod.IsValid())
		OV_SalePeriod->SetVisibility(bIsLimited ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	if (bIsLimited)
	{
		FDateTime EndDateTime = FDateTime::FromUnixTimestamp(EndTime / 1000);
		if (TB_SalePeriod.IsValid())
		{
			TB_SalePeriod->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("EventAttendance_Period2")),
				FText::FromString(FString::FromInt(EndDateTime.GetYear())), FText::FromString(FString::FromInt(EndDateTime.GetMonth())), FText::FromString(FString::FromInt(EndDateTime.GetDay()))
			));
		}
	}
}