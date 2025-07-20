// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIEventAttendancePakgeSlot.h"
#include "B2UIStore.h"
#include "B2UIDocHelper.h"
#include "../BladeII/BladeIILocText.h"
#include "Event.h"
void UB2UIEventAttendancePakgeSlot::Init()
{
	Super::Init();

	InitSlotState();
}

void UB2UIEventAttendancePakgeSlot::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2RichTextBlock, RTB_PackageName);
	GET_SLOT(UImage, IMG_PackageIcon);
	GET_SLOT(UWidgetSwitcher, WS_StateBuy);
	GET_SLOT(UB2Button, BTN_GotoPackageShop);

	GET_SLOT(UB2RichTextBlock, RTB_PossibleBuy);

	GET_SLOT(UB2RichTextBlock, RTB_CompleteBuy);
	GET_SLOT(UImage, IMG_CompleteBuyEffect);
}

void UB2UIEventAttendancePakgeSlot::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_GotoPackageShop, &UB2UIEventAttendancePakgeSlot::OnClickBtnGotoPackageShop);
}

void UB2UIEventAttendancePakgeSlot::UpdateStaticText()
{
	if (RTB_PossibleBuy.IsValid())	//备概 啊瓷
		RTB_PossibleBuy->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("Attendance_AvailableForPurchase")));

	if (RTB_CompleteBuy.IsValid())	//利侩 吝
		RTB_CompleteBuy->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("Attendance_Active")));
}

void UB2UIEventAttendancePakgeSlot::InitSlotState()
{
	auto* pStoreProductData = StoreProductData->GetDefaultObject<UB2UIStoreProductIconData>();
	if (pStoreProductData == nullptr)
	{
		return;
	}

	CachedPackageID = pStoreProductData->GetProductId(PackageType);

	UpdateFromPackageID(CachedPackageID);
}

void UB2UIEventAttendancePakgeSlot::UpdateFromPackageID(int32 PackageID)
{
	auto* pStoreProductData = StoreProductData->GetDefaultObject<UB2UIStoreProductIconData>();
	if (pStoreProductData == nullptr)
	{
		return;
	}

	FText PackageName = pStoreProductData->GetProductName(PackageID, PackageID);
	UpdatePackageNameAndIcon(PackageName, PackageIcon);
}

void UB2UIEventAttendancePakgeSlot::UpdatePackageNameAndIcon(FText ftPackageName, UMaterialInterface* pPackageIcon)
{
	if (RTB_PackageName.IsValid())
	{
		RTB_PackageName->SetText(ftPackageName);
	}

	if (IMG_PackageIcon.IsValid() && pPackageIcon)
	{
		IMG_PackageIcon->SetBrushFromMaterial(pPackageIcon);
	}
}

void UB2UIEventAttendancePakgeSlot::UpdateSlotState(bool IsBought)
{
	int32 WidgetSwitcherIndex = (int32)IsBought;
	if (WS_StateBuy.IsValid())
	{
		WS_StateBuy->SetActiveWidgetIndex(WidgetSwitcherIndex);
	}
}

void UB2UIEventAttendancePakgeSlot::OnClickBtnGotoPackageShop()
{
	if (auto* DocStore = UB2UIDocHelper::GetDocStore())
	{
		const int32 FlatratePackageTabIndex = 7;
		DocStore->SetPackageTabToOpen(FlatratePackageTabIndex);
		EnterShopClass<int32>::GetInstance().Signal(static_cast<int32>(EStorePageWhere::PackageStore));
	}
}