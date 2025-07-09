// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidgetBase.h"
#include "B2UIEtherFilteringSlot.generated.h"

DECLARE_DELEGATE_OneParam(FOnSlotTouchDelegate, const int32);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIEtherFilteringSlot : public UB2UIWidgetBase
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	void SetIcon(int32 InType);
	void SetCount(int32 InType);
	void SetReceiver(class UB2UIEther* InReceiver);

protected:
	virtual void CacheAssets() override;
	virtual void OnClickedSenderBTNArea() override;
	
private:
	FOnSlotTouchDelegate								OnSlotTouch;
	int32												SlotType;
	TWeakObjectPtr<UTextBlock>							X_TB_Count;
	TWeakObjectPtr<UTextBlock>							X_TB_SetName;
	TWeakObjectPtr<UImage>								X_IMG_Type;
	TWeakObjectPtr<UImage>								X_IMG_OffenseBG;
	TWeakObjectPtr<UImage>								X_iMG_DefenseBG;
};
