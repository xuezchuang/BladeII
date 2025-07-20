// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidgetBase.h"
#include "B2UIStoreMileage.h"
#include "B2UIStoreMileageRewardPopupSlot.generated.h"


UCLASS()
class BLADEII_API UB2UIStoreMileageRewardPopupSlot : public UB2UIWidgetBase
{
	GENERATED_BODY()

protected:

	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

public:

	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void CreateRewardBox(int32 MileageType, int32 BoxGrade);
	void SetProgressBar(int32 BoxGrade);
	void SetBoxText(int32 BoxGrade);
	void SetRewardState(EUIStoreMileageRewardState RewardState, bool EnableState);

protected:

	UFUNCTION()
		void OnClickBTN_GetReward();

	UPROPERTY(EditAnywhere, Category = "BladeII")
		TSubclassOf<class UB2UIRewardIcon> RewardIconClass;

private:

	TWeakObjectPtr<UWidgetSwitcher> WS_ContentSelect;
	TWeakObjectPtr<UB2Button> BTN_GetReward;
	TWeakObjectPtr<UTextBlock> TB_GetReward;
	TWeakObjectPtr<UHorizontalBox> HB_Rewards;
	TWeakObjectPtr<UOverlay> P_ReceiveComplete;
	TWeakObjectPtr<UTextBlock> TB_Complete;
	TWeakObjectPtr<UTextBlock> TB_Title;
	TWeakObjectPtr<UProgressBar> PB_Mileage;

	int32 CurrentMileageType;
};
