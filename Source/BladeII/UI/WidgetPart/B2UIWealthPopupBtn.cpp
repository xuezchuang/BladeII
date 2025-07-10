// Fill out your copyright notice in the Description page of Project Settings.


#include "B2UIWealthPopupBtn.h"

#include "B2UIWealthPopup.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Slate/SceneViewport.h"
#include "B2UIManager.h"

void UB2UIWealthPopupBtn::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2Button, BTN_WealthPopupOpen);
}

void UB2UIWealthPopupBtn::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_WealthPopupOpen, &UB2UIWealthPopupBtn::OnClickBTN_WealthPopupOpen);
}

void UB2UIWealthPopupBtn::OnClickBTN_WealthPopupOpen()
{
	if (auto* pWealthPopup = CreateWidget<UB2UIWealthPopup>(GetWorld(), UIWealthPopupClass))
	{
		float fConvertScale = 1.f / UWidgetLayoutLibrary::GetViewportScale(this);
		FGeometry fBtnGeometry = this->GetCachedGeometry();
		
		FVector2D v2WealthPopupPostion = fBtnGeometry.GetAbsolutePosition();
		FVector2D ViewPortPosition;

		if (UGameViewportClient* ViewportClient = this->GetWorld()->GetGameViewport())
		{
			if (FSceneViewport* SceneViewport = ViewportClient->GetGameViewport())
			{
				ViewPortPosition = SceneViewport->GetCachedGeometry().GetAbsolutePosition();
			}
		}

		v2WealthPopupPostion -= ViewPortPosition;
		v2WealthPopupPostion.Y += fBtnGeometry.GetAbsoluteSize().Y;
		v2WealthPopupPostion *= fConvertScale;
		
		pWealthPopup->InitWealthPopup(v2WealthPopupPostion);
		pWealthPopup->AddToViewport(WealthPopupZOrder);
	}
}

void UB2UIWealthPopupBtn::InitWealthPopupBtn_BP()
{
	this->Init();

	if (IsVisibleWealthPopupBtn())
	{
		this->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		this->SetVisibility(ESlateVisibility::Hidden);
	}
}

bool UB2UIWealthPopupBtn::IsVisibleWealthPopupBtn()
{
	bool IsVisible = false;

	EUIScene CurrUIScene = UB2UIManager::GetInstance()->GetCurrUIScene();

	const int32 ArrayMax = ArrayIsVisibleUIScene.Num();
	for (int32 i = 0; i < ArrayMax; ++i)
	{
		if (ArrayIsVisibleUIScene.IsValidIndex(i) == false)
		{
			continue;
		}

		if (CurrUIScene == ArrayIsVisibleUIScene[i])
		{
			IsVisible = true;
			break;
		}
	}

	return IsVisible;
}