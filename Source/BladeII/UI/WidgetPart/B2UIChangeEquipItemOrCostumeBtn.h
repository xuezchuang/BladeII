// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "B2UIChangeEquipItemOrCostumeBtn.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIChangeEquipItemOrCostumeBtn : public UB2UIWidget
{
	GENERATED_BODY()
	
protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	void InitInputDelay();
	void UpdateInputDelay(const float& InDeltaTime);
	bool IsEnableInputDelay();
	void InitEquipSlotVisibleBroadCast();

protected:
	UFUNCTION()
	void OnChangeEquipItemOrCostumeBtnPressed(bool IsCheck);

protected:
	UFUNCTION(BlueprintCallable, Category = "B2UIChangeEquipItemOrCostumeBtn")
	void InitChangeEquipItemOrCostumeBtn_BP();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float fMaxInputDelay;
	
private:
	TWeakObjectPtr<UCheckBox> CB_ChangeEquipItemOrCostume;

private:
	float fCurrentInputDelay;
};
