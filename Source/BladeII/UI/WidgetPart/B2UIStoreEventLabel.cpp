// Fill out your copyright notice in the Description page of Project Settings.
#include "BladeII.h"
#include "B2UIManager.h"
#include "B2UIStoreEventLabel.h"


void UB2UIStoreEventLabel::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UOverlay, OV_Event_Lobby);
	GET_SLOT(UTextBlock, TB_Event_Lobby);

	GET_SLOT(UOverlay, OV_Sale);
	GET_SLOT(UTextBlock, TB_Sale);
	GET_SLOT(UTextBlock, TB_SaleValue);

	GET_SLOT(UOverlay, OV_Bonus);
	GET_SLOT(UTextBlock, TB_Bonus);
	GET_SLOT(UTextBlock, TB_BonusValue);

	GET_SLOT(UOverlay, OV_Hit);
	GET_SLOT(UTextBlock, TB_Hit);

	GET_SLOT(UOverlay, OV_Hot);
	GET_SLOT(UTextBlock, TB_Hot);

	GET_SLOT(UOverlay, OV_New);
	GET_SLOT(UTextBlock, TB_New);

	GET_SLOT(UOverlay, OV_LimitLevel);
	GET_SLOT(UTextBlock, TB_LimitLevel);

	GET_SLOT(UOverlay, OV_SaleForSummon);
	GET_SLOT(UTextBlock, TB_SaleForSummon);

	GET_SLOT(UOverlay, OV_Limited);
	GET_SLOT(UTextBlock, TB_Limited);
}

void UB2UIStoreEventLabel::UpdateStaticText()
{
	if (TB_Event_Lobby.IsValid())
		TB_Event_Lobby->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_EventLabel_Event")));
	if (TB_Sale.IsValid())
		TB_Sale->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_EventLabel_Sale")));
	if (TB_Bonus.IsValid())
		TB_Bonus->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_EventLabel_Bonus")));
	if (TB_Hit.IsValid())
		TB_Hit->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_EventLabel_Hit")));
	if (TB_Hot.IsValid())
		TB_Hot->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_EventLabel_Hot")));
	if (TB_New.IsValid())
		TB_New->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_EventLabel_New")));
	if (TB_LimitLevel.IsValid())
		TB_LimitLevel->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_EventLabel_LimitLevel")));
	if (TB_SaleForSummon.IsValid())
		TB_SaleForSummon->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_EventLabel_SaleForSummon")));
	if (TB_Limited.IsValid())
		TB_Limited->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Limited")));
}

void UB2UIStoreEventLabel::BindDelegates()
{
	Super::BindDelegates();
}

void UB2UIStoreEventLabel::Init()
{
	Super::Init();
}

void UB2UIStoreEventLabel::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIStoreEventLabel::SetVisibleLabel(bool bIsVisible)
{
	if (OV_Event_Lobby.IsValid())
		OV_Event_Lobby->SetVisibility(bIsVisible ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	if (OV_Sale.IsValid())
		OV_Sale->SetVisibility(bIsVisible ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	if (OV_Bonus.IsValid())
		OV_Bonus->SetVisibility(bIsVisible ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	if (OV_Hit.IsValid())
		OV_Hit->SetVisibility(bIsVisible ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	if (OV_Hot.IsValid())
		OV_Hot->SetVisibility(bIsVisible ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	if (OV_New.IsValid())
		OV_New->SetVisibility(bIsVisible ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	if (OV_LimitLevel.IsValid())
		OV_LimitLevel->SetVisibility(bIsVisible ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	if (OV_SaleForSummon.IsValid())
		OV_SaleForSummon->SetVisibility(bIsVisible ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	if (OV_Limited.IsValid())
		OV_Limited->SetVisibility(bIsVisible ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
}

void UB2UIStoreEventLabel::SetLobbyEventLabel(bool bIsVisible)
{
	SetVisibleLabel(false);

	if (OV_Event_Lobby.IsValid())
		OV_Event_Lobby->SetVisibility(bIsVisible ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
}

void UB2UIStoreEventLabel::SetLimitLevel(bool bIsLimitLevel)
{
	SetVisibleLabel(false);

	if (OV_LimitLevel.IsValid())
		OV_LimitLevel->SetVisibility(bIsLimitLevel ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
}

void UB2UIStoreEventLabel::SetStoreEventLabel(int32 InMark)
{
	SetVisibleLabel(false);

	switch (InMark)
	{
	case EStoreMark::NONE:
		break;

	case EStoreMark::NEW:
		if (OV_New.IsValid())
			OV_New->SetVisibility(ESlateVisibility::HitTestInvisible);
		break;

	case EStoreMark::HOT:
		if (OV_Hot.IsValid())
			OV_Hot->SetVisibility(ESlateVisibility::HitTestInvisible);
		break;

	case EStoreMark::HIT:
		if (OV_Hit.IsValid())
			OV_Hit->SetVisibility(ESlateVisibility::HitTestInvisible);
		break;

	case EStoreMark::SALE:
		if (OV_Sale.IsValid())
			OV_Sale->SetVisibility(ESlateVisibility::HitTestInvisible);
		break;
	case EStoreMark::LIMIT:
		if (OV_Limited.IsValid())
			OV_Limited->SetVisibility(ESlateVisibility::HitTestInvisible);
		break;
	default:
		break;
	}
}

void UB2UIStoreEventLabel::SetFundsSale(int32 InDiscountRate)
{
	SetVisibleLabel(false);

	if (OV_Sale.IsValid())
		OV_Sale->SetVisibility(ESlateVisibility::HitTestInvisible);

	if (TB_SaleValue.IsValid())
		TB_SaleValue->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_EventLabel_DiscountRate")), FText::AsNumber(InDiscountRate)));
}

void UB2UIStoreEventLabel::SetFundsSaleForSummon()
{
	SetVisibleLabel(false);

	if (OV_SaleForSummon.IsValid())
		OV_SaleForSummon->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UB2UIStoreEventLabel::SetCashBonus(int32 InIncreaseRate)
{
	SetVisibleLabel(false);

	if (OV_Bonus.IsValid())
		OV_Bonus->SetVisibility(ESlateVisibility::HitTestInvisible);

	if (TB_BonusValue.IsValid())
		TB_BonusValue->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_EventLabel_IncreaseRate")), FText::AsNumber(InIncreaseRate)));
}
