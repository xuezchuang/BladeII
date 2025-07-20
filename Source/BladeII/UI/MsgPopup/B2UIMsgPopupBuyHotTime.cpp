// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIMsgPopupBuyHotTime.h"
#include "HotTimeBuffManager.h"
#include "BladeIIUtil.h"
#include "B2UIManager.h"
#include "CommonStruct.h"
#include "B2ItemInfo.h"


UB2UIMsgPopupBuyHotTime::UB2UIMsgPopupBuyHotTime(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	BuffType = EHotTimeBuffType::End;
}

void UB2UIMsgPopupBuyHotTime::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UButton, BTN_Close)
	GET_SLOT(UButton, BTN_Buy)
	GET_SLOT(UTextBlock, TB_Buy)
	GET_SLOT(UTextBlock, TB_Price)
	GET_SLOT(UB2Image, IMG_BuffImage)
	GET_SLOT(UB2Image, IMG_GoodsImage)
}

void UB2UIMsgPopupBuyHotTime::UpdateStaticText()
{
	
}

void UB2UIMsgPopupBuyHotTime::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIMsgPopupBuyHotTime::OnClickBtnClose)
	BIND_CLICK_FUNC_TO_BTN(BTN_Buy, &UB2UIMsgPopupBuyHotTime::OnClickBtnBuy)
}

void UB2UIMsgPopupBuyHotTime::OnClickBtnBuy()
{
	HotTimeBuffManager::GetInstance().RequestBuyProductBuff(BuffType);

	DestroySelf(UB2UIManager::GetInstance());
}

void UB2UIMsgPopupBuyHotTime::OnClickBtnClose()
{
	DestroySelf(UB2UIManager::GetInstance());
}

void UB2UIMsgPopupBuyHotTime::SetHotTimeImage(UTexture2D * IconImg, FVector2D ImgSize)
{
	//if (IMG_BuffImage.IsValid())
	//{
	//	IMG_BuffImage->SetBrushFromTexture(IconImg);
	//	IMG_BuffImage->GetBrush().ImageSize = ImgSize;
	//}
}

void UB2UIMsgPopupBuyHotTime::SetHotTimeBuffType(EHotTimeBuffType InBuffType)
{
	BuffType = InBuffType;
	FHotTimeProductBuffInfo ProductBuffInfo;
	HotTimeBuffManager::GetInstance().GetProductBuffInfo(BuffType, ProductBuffInfo);

	bool AvailTicket = HotTimeBuffManager::GetInstance().AvailablePurchaseBuffTicket(BuffType);
	int32 price = AvailTicket ? 1 : ProductBuffInfo.price;

	if (IMG_GoodsImage.IsValid()) 
	{
		if(AvailTicket)
			IMG_GoodsImage->SetBrushFromMaterial(StaticFindItemInfo()->GetItemIcon(HotTimeBuffManager::GetInstance().GetBuffTicketID(BuffType)));
		else
			IMG_GoodsImage->SetBrushFromMaterial(StaticFindItemInfo()->GetItemIcon(FItemRefIDHelper::GetGoodsID_Gem()));
	}	
	if (TB_Price.IsValid())
	{
		TB_Price->SetText(FText::AsNumber(price));
	}

	if (TB_Buy.IsValid())
	{
		if (AvailTicket)
			TB_Buy->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_EVENT), FString(TEXT("HotTime_BuyTicketUseItem"))));
		else
			TB_Buy->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Buy"))));
	}
}