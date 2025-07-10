// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIDailyNoticeSlot.h"
#include "B2UIManager.h"
#include "B2UIStore.h"
#include "BladeIIGameImpl.h"
#include "B2UIDocHelper.h"



void UB2UIDailyNoticeSlot::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UWidgetSwitcher, WS_Active);
	GET_SLOT(UB2Image, IMG_DailyNotice);
	GET_SLOT(UTextBlock, TB_DailyNoticeName);

	GET_SLOT(UWidgetSwitcher, WS_GotoShop);
	GET_SLOT(UB2Button, BTN_GotoShop);
	GET_SLOT(UTextBlock, TB_GotoShop);
	GET_SLOT(UTextBlock, TB_RemainTime);
}

void UB2UIDailyNoticeSlot::UpdateStaticText()
{
	if (TB_GotoShop.IsValid())
		TB_GotoShop->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("DailyNotice_GotoShop")));
}

void UB2UIDailyNoticeSlot::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_GotoShop, &UB2UIDailyNoticeSlot::OnClickBTN_GotoShop);
}

void UB2UIDailyNoticeSlot::Init()
{
	Super::Init();
}

void UB2UIDailyNoticeSlot::DestroySelf(UB2UIManager* InUIManager)
{

	Super::DestroySelf(InUIManager);
}

void UB2UIDailyNoticeSlot::SetDailyNoticeData(int32 PackageID)
{
	CurrentPackageID = PackageID;

	UMaterialInstance* ProductIcon = GetProductIcon(PackageID);
	if (IMG_DailyNotice.IsValid() && ProductIcon)
		IMG_DailyNotice->SetBrushFromMaterial(ProductIcon);

	if (TB_DailyNoticeName.IsValid())
		TB_DailyNoticeName->SetText(GetProductName(PackageID));
}

void UB2UIDailyNoticeSlot::SetMoveButton(bool bIsMove)
{
	if (WS_Active.IsValid())
	{
		WS_Active->SetActiveWidgetIndex(bIsMove ? 0 : 1);
	}

	if (WS_GotoShop.IsValid())
	{
		WS_GotoShop->SetActiveWidgetIndex(bIsMove ? 0 : 1);
	}
}

void UB2UIDailyNoticeSlot::SetPeriodicPackageLeftTime(int64 EndTime)
{
	FTimespan RemainTime = FDateTime::FromUnixTimestamp(EndTime / 1000) - FDateTime::UtcNow();

	int32 RemainDay = RemainTime.GetDays();
	int32 RemainHour = RemainTime.GetHours();
	int32 RemainMin = RemainTime.GetMinutes();
	int32 RemainSecond = RemainTime.GetSeconds();

	if (TB_RemainTime.IsValid())
	{
		if (RemainDay > 0)
		{
			TB_RemainTime->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_PackageMission")), RemainDay));
		}
		else
		{
			if (RemainHour > 0)
			{
				TB_RemainTime->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_PackageMission_Hour")), RemainHour, RemainMin));
			}
			else
			{
				if (RemainMin > 0)
				{
					TB_RemainTime->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_PackageMission_Minute")), RemainMin));
				}
				else
				{
					TB_RemainTime->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_PackageMission_Second")));
				}
			}
		}
	}
}

UMaterialInstance* UB2UIDailyNoticeSlot::GetProductIcon(int32 PackageID)
{
	auto* ProductData = ProductIconData->GetDefaultObject<UB2UIStoreProductIconData>();
	if (ProductData)
		return ProductData->GetProductIcon(PackageID, PackageID);

	return nullptr;
}

FText UB2UIDailyNoticeSlot::GetProductName(int32 PackageID)
{
	auto* ProductData = ProductIconData->GetDefaultObject<UB2UIStoreProductIconData>();
	if (ProductData)
		return ProductData->GetProductName(PackageID, PackageID);

	return FText::FromString(TEXT("Unknown Product"));
}

void UB2UIDailyNoticeSlot::OnClickBTN_GotoShop()
{
	const FPackageData* PackageData = BladeIIGameImpl::GetClientDataStore().GetPackageData(CurrentPackageID);
	int32 CategoryNumber = PackageData ? PackageData->PackageTabType : 1;

	if (auto* DocStore = UB2UIDocHelper::GetDocStore())
	{
		DocStore->SetPackageTabToOpen(CategoryNumber);
	}
	
	EnterShopClass<int32>::GetInstance().Signal(static_cast<int32>(EStorePageWhere::PackageStore));
}
