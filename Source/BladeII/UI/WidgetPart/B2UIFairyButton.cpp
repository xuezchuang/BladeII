// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UIFairyButton.h"
#include "FairyManager.h"
#include "B2UIDocHelper.h"
#include "B2UIManager.h"

void UB2UIFairyButton::Init()
{
	Super::Init();

	CachedButtonStatus = EFairyButtonStatus::Type::NONE;
}

void UB2UIFairyButton::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIFairyButton::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2Button, BTN_Fairy);
	GET_SLOT(UWidgetSwitcher, WS_Image);
	GET_SLOT(UTextBlock, TB_LockText);

	GET_SLOT(UImage, IMG_Lock);
	GET_SLOT(UImage, IMG_FairyLcok); 
	GET_SLOT(UImage, IMG_FairyUnlcok); 

	if (TB_LockText.IsValid())
		TB_LockText->SetVisibility(ESlateVisibility::Hidden);
	if (IMG_Lock.IsValid())
		IMG_Lock->SetVisibility(ESlateVisibility::Hidden);
	if (IMG_FairyLcok.IsValid())
		IMG_FairyLcok->SetVisibility(ESlateVisibility::Hidden);
	if (IMG_FairyUnlcok.IsValid())
		IMG_FairyUnlcok->SetVisibility(ESlateVisibility::Hidden);
	if (WS_Image.IsValid())
		WS_Image->SetVisibility(ESlateVisibility::Hidden);
}

void UB2UIFairyButton::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Fairy, &UB2UIFairyButton::OnClickBTN_Fairy);
}

void UB2UIFairyButton::UpdateStaticText()
{

}

void UB2UIFairyButton::OnClickBTN_Fairy()
{
	if (CachedButtonStatus == EFairyButtonStatus::Type::LOCK)
		return;
	OnClickBtnHandler.ExecuteIfBound(CurFairyType);
}

void UB2UIFairyButton::SetFairyBtnHandler(const FFairyButtonOnClick& InBtnHandler)
{
	OnClickBtnHandler = InBtnHandler;
}

void UB2UIFairyButton::SetFairyButtonStatus(EFairyButtonStatus::Type Status)
{
	if (CachedButtonStatus == EFairyButtonStatus::Type::LOCK)
		return;
	CachedButtonStatus = Status;
	ChangeButtonState(Status);
}

void UB2UIFairyButton::ChangeButtonState(EFairyButtonStatus::Type Status)
{
	CachedButtonStatus = Status;

	if (WS_Image.IsValid())
	{
		WS_Image->SetActiveWidgetIndex(Status == EFairyButtonStatus::Type::ENABLED ? 1 : 0);
	}
}

void UB2UIFairyButton::SetFairyInfo(FB2FairyStatusInfo &Status)
{
	SetFairyType(Status.FairyType);
	FairyManager &Instance = FairyManager::GetInstance();

	if (TB_LockText.IsValid())
		TB_LockText->SetVisibility(ESlateVisibility::HitTestInvisible);
	if (IMG_Lock.IsValid())
		IMG_Lock->SetVisibility(ESlateVisibility::HitTestInvisible);
	if (IMG_FairyLcok.IsValid())
		IMG_FairyLcok->SetVisibility(ESlateVisibility::HitTestInvisible);
	if (IMG_FairyUnlcok.IsValid())
		IMG_FairyUnlcok->SetVisibility(ESlateVisibility::HitTestInvisible);
	if (WS_Image.IsValid())
		WS_Image->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	if (Status.bClearStage == false)
	{
		if (TB_LockText.IsValid())
			TB_LockText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		if (IMG_Lock.IsValid())
			IMG_Lock->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		ChangeButtonState(EFairyButtonStatus::Type::LOCK);
	}
	else
	{
		if (TB_LockText.IsValid())
			TB_LockText->SetVisibility(ESlateVisibility::Collapsed);

		if (IMG_Lock.IsValid())
			IMG_Lock->SetVisibility(ESlateVisibility::Collapsed);

		ChangeButtonState(EFairyButtonStatus::Type::DISABLED);
	}

	if (TB_LockText.IsValid())
		TB_LockText->SetText(Instance.GetFairyOpenStageName(Status.FairyType));

	if (NPCImage.Contains(Status.FairyType))
	{
		TSoftObjectPtr<UMaterialInterface> MIPtr = NPCImage.FindRef(Status.FairyType);
		UMaterialInterface* Mat = MIPtr.LoadSynchronous();

		if (IMG_FairyLcok.IsValid())
			IMG_FairyLcok->SetBrushFromMaterial(Mat);
		if (IMG_FairyUnlcok.IsValid())
			IMG_FairyUnlcok->SetBrushFromMaterial(Mat);
	}
}