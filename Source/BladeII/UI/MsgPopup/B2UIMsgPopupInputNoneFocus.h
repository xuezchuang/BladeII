// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2UIMsgPopupSimple.h"
#include "B2UIMsgPopupInputNoneFocus.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIMsgPopupInputNoneFocus : public UB2UIMsgPopupSimple
{
	GENERATED_BODY()
public:
	UB2UIMsgPopupInputNoneFocus(const FObjectInitializer& ObjectInitializer);
	virtual void OnOpen(bool RightNow = false) override;

	void SetInputText(const FText& InText);
	FText GetInputText() const;

	void SetInputTextIsPassword(bool IsPassword);
protected:
	virtual void CacheAssets() override;

protected:
	TWeakObjectPtr<UEditableText>	ETB_Input;
};
