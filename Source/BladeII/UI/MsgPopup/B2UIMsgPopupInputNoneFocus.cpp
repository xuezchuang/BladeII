// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIMsgPopupInputNoneFocus.h"
#include "B2UIManager.h"


UB2UIMsgPopupInputNoneFocus::UB2UIMsgPopupInputNoneFocus(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UB2UIMsgPopupInputNoneFocus::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UEditableText, ETB_Input)
}

void UB2UIMsgPopupInputNoneFocus::OnOpen(bool RightNow)
{
	Super::OnOpen(RightNow);
}

void UB2UIMsgPopupInputNoneFocus::SetInputText(const FText& InText)
{
	if (ETB_Input.IsValid())
		ETB_Input->SetText(InText);
}

FText UB2UIMsgPopupInputNoneFocus::GetInputText() const
{
	if (ETB_Input.IsValid())
		return ETB_Input->GetText();

	return FText::FromString(TEXT(""));
}

void UB2UIMsgPopupInputNoneFocus::SetInputTextIsPassword(bool IsPassword)
{
	if (ETB_Input.IsValid())
		ETB_Input->SetIsPassword(IsPassword);
}





