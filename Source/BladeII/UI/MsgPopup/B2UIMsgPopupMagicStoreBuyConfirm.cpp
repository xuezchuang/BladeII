// Fill out your copyright notice in the Description page of Project Settings.
#include "BladeII.h"
#include "B2UIManager.h"
#include "B2AndroidBackManager.h"
#include "B2UIStoreProductCost.h"
#include "BladeIIGameImpl.h"
#include "B2ItemInfo.h"
#include "B2UIMsgPopupMagicStoreBuyConfirm.h"


void UB2UIMsgPopupMagicStoreBuyConfirm::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_Title);

	GET_SLOT(UImage, IMG_ItemIcon);
	GET_SLOT(UB2RichTextBlock, TB_Content);
	GET_SLOT(UB2UIStoreProductCost, UIP_Cost);
	if (UIP_Cost.IsValid())
		UIP_Cost->Init();

	GET_SLOT(UTextBlock, TB_Buy);
}

void UB2UIMsgPopupMagicStoreBuyConfirm::UpdateStaticText()
{
	if (TB_Title.IsValid())
		TB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Notification")));
	if (TB_Buy.IsValid())
		TB_Buy->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Confirm")));
}

void UB2UIMsgPopupMagicStoreBuyConfirm::BindDelegates()
{
	Super::BindDelegates();
}

void UB2UIMsgPopupMagicStoreBuyConfirm::CloseWidgetDelegate()
{
	ClosePopup();
}

void UB2UIMsgPopupMagicStoreBuyConfirm::Init()
{
	Super::Init();

	this->SetVisibility(ESlateVisibility::Collapsed);
}

void UB2UIMsgPopupMagicStoreBuyConfirm::DestroySelf(UB2UIManager* InUIManager)
{
	if (UIP_Cost.IsValid())
		UIP_Cost->DestroySelf(InUIManager);

	Super::DestroySelf(InUIManager);
}

void UB2UIMsgPopupMagicStoreBuyConfirm::OpenPopup()
{
	if (this->GetVisibility() != ESlateVisibility::SelfHitTestInvisible)
	{
		this->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		B2AndroidBackManager::GetInstance()->AddBackWidget(this);
	}
}

bool UB2UIMsgPopupMagicStoreBuyConfirm::IsOpened()
{
	return this->GetVisibility() == ESlateVisibility::SelfHitTestInvisible ? true : false;
}

void UB2UIMsgPopupMagicStoreBuyConfirm::ClosePopup()
{
	if (this->GetVisibility() != ESlateVisibility::Collapsed)
	{
		this->SetVisibility(ESlateVisibility::Collapsed);
		B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);
	}
}

void UB2UIMsgPopupMagicStoreBuyConfirm::SetBuyConfirmPopupData(int32 ProductID)
{
	const FMagicShopProduct* MagicProductItem = BladeIIGameImpl::GetClientDataStore().GetMagicShopProductData(ProductID);
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

		SetItemIcon(ItemRefID);

		if (TB_Content.IsValid())
			TB_Content->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_MagicStore_BuyItem")), FText::AsNumber(MagicProductItem->ProductCount)));

		if (UIP_Cost.IsValid())
			UIP_Cost->SetCost(static_cast<EStoreItemCost>(MagicProductItem->PriceType), MagicProductItem->Price, 0);
	}
}

void UB2UIMsgPopupMagicStoreBuyConfirm::SetItemIcon(int32 ItemRefID)
{
	if (ItemRefID == 0)
		return;

	auto* ItemInfoTable = StaticFindItemInfo();
	if (!ItemInfoTable)
		return;

	if (IMG_ItemIcon.IsValid())
		IMG_ItemIcon->SetBrushFromMaterial(StaticFindItemInfo()->GetItemIcon(ItemRefID));
}
