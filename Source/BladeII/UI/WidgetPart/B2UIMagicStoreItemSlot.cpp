// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIMagicStoreItemSlot.h"
#include "B2UIManager.h"
#include "B2UIStoreProductCost.h"
#include "BladeIIGameImpl.h"
#include "B2ItemInfo.h"



void UB2UIMagicStoreItemSlot::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_ItemName);
	GET_SLOT(UImage, IMG_ItemIcon);
	GET_SLOT(UImage, IMG_BuyItemIcon);
	GET_SLOT(UTextBlock, TB_ItemCount);
	GET_SLOT(UWidgetSwitcher, WS_IsPurchased);

	GET_SLOT(UB2UIStoreProductCost, UIP_Cost);
	if (UIP_Cost.IsValid())
		UIP_Cost->Init();
	GET_SLOT(UTextBlock, TB_BuyComplete);

	GET_SLOT(UB2Button, BTN_BuyItem);
	GET_SLOT(UB2Button, BTN_Buy);

	GET_SLOT(UWidgetAnimation, Anim_Change);
}

void UB2UIMagicStoreItemSlot::UpdateStaticText()
{
	if (TB_BuyComplete.IsValid())
		TB_BuyComplete->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Package_Complete")));
}

void UB2UIMagicStoreItemSlot::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_BuyItem, &UB2UIMagicStoreItemSlot::OnClickBTN_Buy);
	BIND_CLICK_FUNC_TO_BTN(BTN_Buy, &UB2UIMagicStoreItemSlot::OnClickBTN_Buy);
}

void UB2UIMagicStoreItemSlot::Init()
{
	Super::Init();
}

void UB2UIMagicStoreItemSlot::DestroySelf(UB2UIManager* InUIManager)
{
	if (UIP_Cost.IsValid())
		UIP_Cost->DestroySelf(InUIManager);

	UnbindDelegate();

	Super::DestroySelf(InUIManager);
}

void UB2UIMagicStoreItemSlot::SetData(int32 MagicStoreItemID, bool IsPurchased)
{
	if (MagicStoreItemID == 0)
		return;

	const FMagicShopProduct* MagicProductItem = BladeIIGameImpl::GetClientDataStore().GetMagicShopProductData(MagicStoreItemID);
	if (MagicProductItem)
	{
		int32 ItemRefID = 0;

		if (MagicProductItem->ProductType == EStoreProductType::ITEM)							// 아이템
		{
			ItemRefID = MagicProductItem->ProductDetail;
		}
		else if (MagicProductItem->ProductType == EStoreProductType::GENERAL_LOTTERY)			// 일반 뽑기권
		{
			ItemRefID = BladeIIGameImpl::GetClientDataStore().GetGeneralLotteryItemTemplateId(MagicProductItem->ProductDetail);
		}
		else if (MagicProductItem->ProductType == EStoreProductType::SELECTIVE_LOTTERY)			// 선택 뽑기권
		{
			ItemRefID = BladeIIGameImpl::GetClientDataStore().GetSelectiveLotteryItemTemplateId(MagicProductItem->ProductDetail);
		}
		else if (MagicProductItem->ProductType == EStoreProductType::SELECTIVE_AETHER_LOTTERY)	// 에테르 뽑기권
		{
			ItemRefID = BladeIIGameImpl::GetClientDataStore().GetSelectiveEtherLotteryItemTemplateId(MagicProductItem->ProductDetail);
		}

		SetItemIconAndName(ItemRefID);

		if (WS_IsPurchased.IsValid())
			WS_IsPurchased->SetActiveWidgetIndex(IsPurchased ? 0 : 1);

		if (TB_ItemCount.IsValid())
		{
			FSlateColor ItemCountColor = IsPurchased ? UB2UIManager::GetInstance()->TextColor_CannotBuyItem : UB2UIManager::GetInstance()->TextColor_CanBuyItem;
			TB_ItemCount->SetColorAndOpacity(ItemCountColor);
			TB_ItemCount->SetText(FText::AsNumber(MagicProductItem->ProductCount));
		}

		if (UIP_Cost.IsValid())
		{
			UIP_Cost->SetVisibility(IsPurchased ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
			UIP_Cost->SetCost(static_cast<EStoreItemCost>(MagicProductItem->PriceType), MagicProductItem->Price, 0);
		}

		if (TB_BuyComplete.IsValid())
			TB_BuyComplete->SetVisibility(IsPurchased ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

		if (BTN_BuyItem.IsValid())
			BTN_BuyItem->SetIsEnabled(!IsPurchased);

		if (BTN_Buy.IsValid())
			BTN_Buy->SetIsEnabled(!IsPurchased);
	}
}

void UB2UIMagicStoreItemSlot::SetVisibilityItem(ESlateVisibility bVislble)
{
	this->SetVisibility(bVislble);
}

void UB2UIMagicStoreItemSlot::StartAnimation(int32 SlotIndex)
{
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().SetTimer(AniTimeHandle, this,
		&UB2UIMagicStoreItemSlot::PlaySlotIndexAnimation, AniStartTime + (SlotIndex * AniSpeed), false);
}

void UB2UIMagicStoreItemSlot::PlaySlotIndexAnimation()
{
	SetVisibilityItem(ESlateVisibility::SelfHitTestInvisible);

	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(AniTimeHandle);

	if (Anim_Change.IsValid())
		PlayAnimation(Anim_Change.Get());
}

void UB2UIMagicStoreItemSlot::UnbindDelegate()
{
	ButtonDelegate.Unbind();
}

void UB2UIMagicStoreItemSlot::SetItemIconAndName(int32 ItemRefID)
{
	if (ItemRefID == 0)
		return;

	auto* ItemInfoTable = StaticFindItemInfo();
	if (!ItemInfoTable)
		return;

	UMaterialInstance* IconMtrl = nullptr;
	FText ItemName;
	StaticFindItemInfo()->GetItemIconAndName(ItemRefID, IconMtrl, ItemName);

	if (IMG_ItemIcon.IsValid())
		IMG_ItemIcon->SetBrushFromMaterial(IconMtrl);
	if (IMG_BuyItemIcon.IsValid())
		IMG_BuyItemIcon->SetBrushFromMaterial(IconMtrl);

	if (TB_ItemName.IsValid())
		TB_ItemName->SetText(ItemName);
}

void UB2UIMagicStoreItemSlot::OnClickBTN_Buy()
{
	ButtonDelegate.ExecuteIfBound();
}
