#include "B2UIStoreProductCost.h"
#include "B2UISummonItemSlotItem.h"
#include "B2UIDocHelper.h"
#include "BladeIILocText.h"

#include "BladeIIGameImpl.h"
void UB2UIStoreProductCost::Init()
{
	Super::Init();
}

void UB2UIStoreProductCost::CacheAssets()
{
	GET_SLOT(UTextBlock, TB_Desc);
	GET_SLOT(UImage, IMG_Icon);
	GET_SLOT(UTextBlock, TB_Cost);

	// Funs Sale
	GET_SLOT(UOverlay, OV_PreCost);
	GET_SLOT(UB2RichTextBlock, RTB_PreCost);
	GET_SLOT(UImage, IMG_Arrow);
}

// 살 수 없는 조건 변경시 B2UIUtil::IsSummonItemBuy도 같이 변경 필요
void UB2UIStoreProductCost::SetCost(EStoreItemCost InCostType, int32 InCost, int32 InCalculatedCost)
{
	if (OV_PreCost.IsValid())
		OV_PreCost->SetVisibility(InCalculatedCost > 0 ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	if (IMG_Arrow.IsValid())
		IMG_Arrow->SetVisibility(InCalculatedCost > 0 ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	if (TB_Cost.IsValid())
	{
		TB_Cost->SetColorAndOpacity(FLinearColor::White);
	}

	if (InCost == 0)
	{
		if (IMG_Icon.IsValid())
			IMG_Icon->SetVisibility(ESlateVisibility::Collapsed);

		if (TB_Cost.IsValid())
		{
			TB_Cost->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_SummonItem_FreeToSummon")));

			auto* HBSlot = Cast<UHorizontalBoxSlot>(TB_Cost->Slot);
			if (HBSlot)
				HBSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
		}
	}
	else
	{
		int32 UserCost = 0;
		if (auto UserInfo = UB2UIDocHelper::GetDocUser())
		{
			switch (InCostType)
			{
			case EStoreItemCost::Gem:
				UserCost = UserInfo->GetGem();
				break;

			case EStoreItemCost::DarkCrystal:
				UserCost = UserInfo->GetDarkCrystal();
				break;

			case EStoreItemCost::FriendshipPoint:
				UserCost = UserInfo->GetSocialPoint();
				break;

			case EStoreItemCost::Gold:
				UserCost = UserInfo->GetGold();
				break;

			case EStoreItemCost::FamePoint:
				UserCost = UserInfo->GetFamePoint();
				break;

			case EStoreItemCost::TenLotteryTicket:
				UserCost = BladeIIGameImpl::GetClientDataStore().GetShopTenLotteryTicket();
				break;

			default:
				break;
			}
		}

		if (InCalculatedCost > 0)
		{
			if (RTB_PreCost.IsValid())
				RTB_PreCost->SetText(FText::AsNumber(InCost));

			if (TB_Cost.IsValid())
			{
				TB_Cost->SetText(FText::AsNumber(InCalculatedCost));
				TB_Cost->SetColorAndOpacity(InCalculatedCost <= UserCost ? FLinearColor::White : FLinearColor::Red);

				auto* HBSlot = Cast<UHorizontalBoxSlot>(TB_Cost->Slot);
				if (HBSlot)
					HBSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Left);
			}
		}
		else
		{
			if (TB_Cost.IsValid())
			{
				if (InCostType == EStoreItemCost::End)
				{
					TB_Cost->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Restrict_Complete")));
					TB_Cost->SetColorAndOpacity(FLinearColor::White);

					auto* HBSlot = Cast<UHorizontalBoxSlot>(TB_Cost->Slot);
					if (HBSlot)
						HBSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
				}
				else if (InCostType == EStoreItemCost::TenLotteryTicket)
				{
					TB_Cost->SetText(FText::AsNumber(UserCost)); // 10회 뽑기권은 가지고 있는 갯수 출력
					TB_Cost->SetColorAndOpacity(FLinearColor::White);
					auto* HBSlot = Cast<UHorizontalBoxSlot>(TB_Cost->Slot);
					if (HBSlot)
						HBSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Left);
				}
				else
				{
					TB_Cost->SetText(FText::AsNumber(InCost));
					TB_Cost->SetColorAndOpacity(InCost <= UserCost ? FLinearColor::White : FLinearColor::Red);

					auto* HBSlot = Cast<UHorizontalBoxSlot>(TB_Cost->Slot);
					if (HBSlot)
						HBSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Left);
				}
			}
		}

		if (!IMG_Icon.IsValid())
			return;

		if (InCostType == EStoreItemCost::End)
		{
			IMG_Icon->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			IMG_Icon->SetVisibility(ESlateVisibility::HitTestInvisible);

			for (auto& Data : IconDataInfos)
			{
				if (Data.Type == InCostType)
				{
					IMG_Icon->SetBrushFromMaterial(Data.Icon);
					break;
				}
			}
		}
	}
}

void UB2UIStoreProductCost::SetCashCost(float InCost, FString InCurrencyCode)
{
	if (IMG_Icon.IsValid())
		IMG_Icon->SetVisibility(ESlateVisibility::Collapsed);

	if (TB_Cost.IsValid())
	{
		FNumberFormattingOptions FormttingOption;
		FormttingOption.MaximumFractionalDigits = 2;

		FText CostText = FText::AsNumber(InCost, &FormttingOption);
		TB_Cost->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Currency_Cost")), FText::FromString(InCurrencyCode), CostText));
		TB_Cost->SetColorAndOpacity(FLinearColor::White);

		auto* HBSlot = Cast<UHorizontalBoxSlot>(TB_Cost->Slot);
		if (HBSlot)
			HBSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
	}
}

void UB2UIStoreProductCost::SetSummonDesc(int32 ButtonType)
{
	if (TB_Desc.IsValid() && ButtonType == static_cast<ESummonButtonType>(SummonButtonType_Ten))
		TB_Desc->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_SummonItem_TenSummon")));

	if (TB_Desc.IsValid() && ButtonType == static_cast<ESummonButtonType>(SummonButtonType_One))
		TB_Desc->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_SummonItem_OneSummon")));
}

void UB2UIStoreProductCost::SetSealCosnt(ESealCostType InCostType, int32 InCost)
{
	EStoreItemCost StoreCost = EStoreItemCost::End;

	switch (InCostType)
	{
	case ESealCostType::Gem:
		StoreCost = EStoreItemCost::Gem;
		break;
	case ESealCostType::Gold:
		StoreCost = EStoreItemCost::Gold;
		break;
	}

	SetCost(StoreCost, InCost,0);
}
