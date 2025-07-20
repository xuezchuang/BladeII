// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIMsgPopupBase.h"
#include "B2UIMsgPopupCenter.generated.h"

/*
* The center intensely floating message on the screen.
*/

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIMsgPopupCenter : public UB2UIMsgPopupBase
{
	GENERATED_BODY()

public:
	UB2UIMsgPopupCenter(const FObjectInitializer& ObjectInitializer);
	
protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

public:

	/** Better call it as soon as open it. Check out the BP widget asset for which animation is bound to which anim. */
	UFUNCTION(BlueprintImplementableEvent)
	void PlayOpenAnimation(int32 AnimIndex);
};
