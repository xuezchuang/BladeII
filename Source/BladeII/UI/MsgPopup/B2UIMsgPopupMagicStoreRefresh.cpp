// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIMsgPopupMagicStoreRefresh.h"
#include "B2UIManager.h"
#include "B2AndroidBackManager.h"
#include "B2UIDocHelper.h"
#include "BladeIIGameImpl.h"



void UB2UIMsgPopupMagicStoreRefresh::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_Title);

	GET_SLOT(UWidgetSwitcher, WS_RemainCount);
	GET_SLOT(UTextBlock, TB_RefreshDesc);
	GET_SLOT(UTextBlock, TB_RefreshCount);
	GET_SLOT(UTextBlock, TB_Cost);
	GET_SLOT(UTextBlock, TB_EndRefresh);

	GET_SLOT(UTextBlock, TB_Buy);
}

void UB2UIMsgPopupMagicStoreRefresh::UpdateStaticText()
{
	if (TB_Title.IsValid())
		TB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Notification")));
	if (TB_RefreshDesc.IsValid())
		TB_RefreshDesc->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_MagicStore_RefreshDesc")));
	if (TB_EndRefresh.IsValid())
		TB_EndRefresh->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_MagicStore_EndRefresh")));

	if (TB_Buy.IsValid())
		TB_Buy->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Confirm")));
}

void UB2UIMsgPopupMagicStoreRefresh::BindDelegates()
{
	Super::BindDelegates();

}

void UB2UIMsgPopupMagicStoreRefresh::CloseWidgetDelegate()
{
	ClosePopup();
}

void UB2UIMsgPopupMagicStoreRefresh::Init()
{
	Super::Init();

	this->SetVisibility(ESlateVisibility::Collapsed);
}

void UB2UIMsgPopupMagicStoreRefresh::DestroySelf(UB2UIManager* InUIManager)
{

	Super::DestroySelf(InUIManager);
}

void UB2UIMsgPopupMagicStoreRefresh::OpenPopup()
{
	if (this->GetVisibility() != ESlateVisibility::SelfHitTestInvisible)
	{
		this->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		B2AndroidBackManager::GetInstance()->AddBackWidget(this);
	}

	SetRenewalCount();
}

bool UB2UIMsgPopupMagicStoreRefresh::IsOpened()
{
	return this->GetVisibility() == ESlateVisibility::SelfHitTestInvisible ? true : false;
}

void UB2UIMsgPopupMagicStoreRefresh::ClosePopup()
{
	if (this->GetVisibility() != ESlateVisibility::Collapsed)
	{
		this->SetVisibility(ESlateVisibility::Collapsed);
		B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);
	}
}

void UB2UIMsgPopupMagicStoreRefresh::SetRenewalCount()
{
	if (auto* DocStore = UB2UIDocHelper::GetDocStore())
	{
		int32 RenewalCount = DocStore->GetRenewalCount();
		int32 RenewalMaxCount = BladeIIGameImpl::GetClientDataStore().GetMagicShopRenewalCostDataCount();
		const FMagicShopRenewalCost* RenewalCost = BladeIIGameImpl::GetClientDataStore().GetMagicShopRenewalCostData(RenewalCount);
		int32 MyGem = BladeIIGameImpl::GetClientDataStore().GetGemAmount();

		if (RenewalCount < RenewalMaxCount)
		{
			if (WS_RemainCount.IsValid())
				WS_RemainCount->SetActiveWidgetIndex(0);

			if (TB_RefreshCount.IsValid())
			{
				TB_RefreshCount->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_MagicStore_RefreshCount")),
					FText::AsNumber(RenewalMaxCount - RenewalCount), FText::AsNumber(RenewalMaxCount)));
			}

			if (RenewalCost)
			{
				if (TB_Cost.IsValid())
				{
					TB_Cost->SetText(FText::AsNumber(RenewalCost->GemCost));
					TB_Cost->SetColorAndOpacity(MyGem >= RenewalCost->GemCost ? FLinearColor::White : FLinearColor::Red);
				}
			}

			OnPlaySuccessSound(MyGem >= RenewalCost->GemCost ? true : false);
		}
		else
		{
			if (WS_RemainCount.IsValid())
				WS_RemainCount->SetActiveWidgetIndex(1);

			OnPlaySuccessSound(false);
		}
	}
}
