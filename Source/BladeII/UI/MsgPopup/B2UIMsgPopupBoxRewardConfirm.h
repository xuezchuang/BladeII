// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/MsgPopup/B2UIMsgPopupSimple.h"
#include "B2UIMsgPopupBoxRewardConfirm.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIMsgPopupBoxRewardConfirm : public UB2UIMsgPopupSimple
{
	GENERATED_BODY()
	
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void ApplyMvpBox();

	UFUNCTION(BlueprintImplementableEvent)
	void ApplyRewardBox(int32 BoxGrade);

	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	
};
