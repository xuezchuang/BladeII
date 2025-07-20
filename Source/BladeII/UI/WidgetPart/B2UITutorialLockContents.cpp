// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UITutorialLockContents.h"

#include "B2UIManager.h"

#include "TutorialManager.h"

UB2UITutorialLockContents::UB2UITutorialLockContents(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, TutorialID(0)
	, TutorialStep(0)
	, LockType(ETutorialLockType::EOverrideLock)
	, LockImageOffset(FVector2D::ZeroVector)
	, LockImageScale(FVector2D(1.0f, 1.0f))
	, IsVisibilityLockTitleText(false)
	, IsVisibilityLockDescText(false)
	, LockTextOffset(FVector2D::ZeroVector)
	, OverrideLockTitleColor(FLinearColor::Transparent)
	, OverrideLockDescColor(FLinearColor::Transparent)
{
}

void UB2UITutorialLockContents::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UButton, BTN_Click);
	GET_SLOT(UImage, IMG_BG);
	GET_SLOT(UImage, IMG_DefaultBG);
	GET_SLOT(UImage, IMG_Lock);
	GET_SLOT(UImage, IMG_DefaultLock);
	GET_SLOT(UOverlay, X_O_Padding);
	GET_SLOT(UOverlay, X_O_ConditionText);
	GET_SLOT(UTextBlock, TB_Condition1);
	GET_SLOT(UTextBlock, TB_Condition2);
}

void UB2UITutorialLockContents::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Click, &UB2UITutorialLockContents::OnBTNClickLockContents);	

	UnBindTutorialDoc();
	TutorialManager::GetInstance().OnCurrentTutorialIDChanged.AddUObject(this, &UB2UITutorialLockContents::OnChanagedTutorialId);
	TutorialManager::GetInstance().OnCurrentTutorialStepChanged.AddUObject(this, &UB2UITutorialLockContents::OnChanagedTutorialStepId);
}

void UB2UITutorialLockContents::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	bool IsCacheAsset = (X_O_Padding.IsValid() && X_O_ConditionText.IsValid() && BTN_Click.IsValid());
	if (IsCacheAsset == false)
	{
		CacheAssets();
		BindDelegates();
	}

	UpdateDynamicWidgets();
	CheckDestroySelfByOpenedTutorial();
}


void UB2UITutorialLockContents::Init()
{
	Super::Init();

	UpdateDynamicWidgets();
	CheckDestroySelfByOpenedTutorial();
}

#if WITH_EDITOR
void UB2UITutorialLockContents::PreEditChange(FProperty* PropertyAboutToChange)
{
	Super::PreEditChange(PropertyAboutToChange);
}

void UB2UITutorialLockContents::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	UpdateDynamicWidgets();
}
#endif // WITH_EDITOR

void UB2UITutorialLockContents::UpdateDynamicWidgets()
{
	UpdateTutorialLockText();
	UpdateLockImage();
}

void UB2UITutorialLockContents::UpdateTutorialLockText()
{
	if (TB_Condition1.IsValid())
	{
#if WITH_EDITOR
		if (FApp::IsGame())
			TB_Condition1->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("TutorialCondition_OpenCondition")), GetTutorialTitle(TutorialID)));
#else //WITH_EDITOR
		TB_Condition1->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("TutorialCondition_OpenCondition")), GetTutorialTitle(TutorialID)));
#endif //WITH_EDITOR

		TB_Condition1->SetVisibility(IsVisibilityLockTitleText ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);

		if (OverrideLockTitleColor != FSlateColor(FLinearColor::Transparent))
			TB_Condition1->SetColorAndOpacity(OverrideLockTitleColor);
	}

	if (TB_Condition2.IsValid())
	{
		FText ConditionText = GetTutorialDesc(TutorialID);
		if (ConditionText.IsEmpty() == false)
			TB_Condition2->SetText(ConditionText);
		TB_Condition2->SetVisibility(IsVisibilityLockDescText ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);

		if (OverrideLockDescColor != FSlateColor(FLinearColor::Transparent))
			TB_Condition2->SetColorAndOpacity(OverrideLockDescColor);
	}
}

void UB2UITutorialLockContents::UpdateLockImage()
{
	if (CustomBackGroundImageInfo.IsVisibleImage && IMG_BG.IsValid())
		IMG_BG->SetBrush(CustomBackGroundImageInfo.ImageBrush);
	if (CustomLockImageInfo.IsVisibleImage && IMG_Lock.IsValid())
		IMG_Lock->SetBrush(CustomLockImageInfo.ImageBrush);
	if (IMG_BG.IsValid())
		IMG_BG->SetVisibility(CustomBackGroundImageInfo.IsVisibleImage ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	if (IMG_DefaultBG.IsValid())
		IMG_DefaultBG->SetVisibility(CustomBackGroundImageInfo.IsVisibleImage == false ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	if (IMG_Lock.IsValid())
		IMG_Lock->SetVisibility(CustomLockImageInfo.IsVisibleImage ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	if (IMG_DefaultLock.IsValid())
		IMG_DefaultLock->SetVisibility(CustomLockImageInfo.IsVisibleImage == false ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);

	if (X_O_Padding.IsValid())
	{
		if (auto* VSlot = Cast<UVerticalBoxSlot>(X_O_Padding.Get()->Slot))
			VSlot->SetPadding(FMargin(LockImageOffset.X, LockImageOffset.Y, 0.f, 0.f));
	}

	if (X_O_ConditionText.IsValid())
	{
		if (auto* VSlot = Cast<UVerticalBoxSlot>(X_O_ConditionText.Get()->Slot))
			VSlot->SetPadding(FMargin(LockTextOffset.X, LockTextOffset.Y, 0.f, 0.f));
	}

	if (IMG_Lock.IsValid())
		IMG_Lock->SetRenderScale(LockImageScale);

	if (IMG_DefaultLock.IsValid())
		IMG_DefaultLock->SetRenderScale(LockImageScale);
}


void UB2UITutorialLockContents::CheckDestroySelfByOpenedTutorial()
{
	//[@AKI 171113, 俊叼磐老 版快绰 遏阑 救窍霸 秦具 瞪巴 鞍篮单 俊叼磐 葛靛俊辑绰 遏阑 亲惑 吧霸 登绢 乐绢 林籍 贸府窃.
/*
#if WITH_EDITOR
	if (FApp::IsGame() == false)
		return;
#endif //WITH_EDITOR
*/

	auto& TutorialManagerInstance = TutorialManager::GetInstance();

	switch (LockType)
	{
	case ETutorialLockType::EOverrideLock:
		{
			if (TutorialManagerInstance.IsLockTutorial(TutorialID, TutorialStep) == false)
			{
				UnBindTutorialDoc();
				SetVisibility(ESlateVisibility::Collapsed);
			}
			else
			{
				BindDelegates();
				SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
		}
		break;
	case ETutorialLockType::EParentHidden:
		{
			if (TutorialManagerInstance.IsLockTutorial(TutorialID, TutorialStep))
			{
				UnBindTutorialDoc();
				if (auto* CurParent = GetParent())
					CurParent->SetVisibility(ESlateVisibility::Hidden);
			}
			else
			{
				BindDelegates();
				SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
		}
		break;
	}
}

void UB2UITutorialLockContents::UnBindTutorialDoc()
{
	TutorialManager::GetInstance().OnCurrentTutorialIDChanged.RemoveAll(this);
	TutorialManager::GetInstance().OnCurrentTutorialStepChanged.RemoveAll(this);
}

void UB2UITutorialLockContents::OnBTNClickLockContents()
{
	TutorialManager::OpenLockContentPopup(TutorialID, TutorialStep);
}

void UB2UITutorialLockContents::OnLastTutorialId(class TutorialManager* Sender, int32 CurTutorialId, int32 PreTutorialId)
{
	CheckDestroySelfByOpenedTutorial();
}

void UB2UITutorialLockContents::OnChanagedTutorialId(class TutorialManager* Sender, int32 CurTutorialId, int32 PreTutorialId)
{
	CheckDestroySelfByOpenedTutorial();
}

void UB2UITutorialLockContents::OnChanagedTutorialStepId(class TutorialManager* Sender, int32 CurTutorialStepId, int32 PreTutorialStepId)
{
	CheckDestroySelfByOpenedTutorial();
}

