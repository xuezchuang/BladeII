// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIMsgPopupInputNoneFocus.h"
#include "B2UIMsgPopupInput.generated.h"


/*
* Inputable Popup. and it has two buttons.
*/

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIMsgPopupInput : public UB2UIMsgPopupInputNoneFocus
{
	GENERATED_BODY()

public:
	UB2UIMsgPopupInput(const FObjectInitializer& ObjectInitializer);
	virtual void OnOpen(bool RightNow = false) override;
};