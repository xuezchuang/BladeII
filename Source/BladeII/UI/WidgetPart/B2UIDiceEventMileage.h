// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "B2UIDiceEventMileage.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIDiceEventMileage : public UB2UIWidget
{
	GENERATED_BODY()
	
public:
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

public:
	void InitMasterData_Mileage(const int32 iEventID);
	void InitMileage();
	void InitMileageSlotList();

public:
	void UpdateMileage(int32 iRolledDiceCount, int32 MileageRewardedValue);

private:
	UFUNCTION()
	void OnClickBTN_DiceEventMileage();

	void OpenDiceEventMileageRewardPopup();
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UB2UIDiceEventMileageSlot>		UIDiceEventMileageSlotClass;

private:
	TWeakObjectPtr<UB2Button>		BTN_DiceEventMileage;
	TWeakObjectPtr<UHorizontalBox>	HB_MileageSlotList;

	TWeakObjectPtr<class UB2UIDiceEventMileageRewardPopup>	pDiceEventMileageRewardPopup;

	TArray<TWeakObjectPtr<class UB2UIDiceEventMileageSlot>>	ArrayMileageSlot;
		
private:
	TArray<b2network::B2mdDiceEventMileage>	CachedArrayMileageInfo;
	int32									CachedRolledDiceCount;
	int32									CachedMileageRewardValue;
};
