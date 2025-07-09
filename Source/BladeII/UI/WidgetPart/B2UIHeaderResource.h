// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidgetBase.h"
#include "B2UIHeaderResource.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIHeaderResource : public UB2UIWidgetBase
{
	GENERATED_BODY()
	
public:
	UB2UIHeaderResource(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;

	void SetAmount(int32 NewValueAmount, int32 MaxValueAmoun = -1, bool bFromInit = false);
	void SetRemainTime(FText txtRemainTime);

	void SetAmountText(int32 NewValueAmount, int32 MaxValueAmoun = -1);

	void SetVisibilityOfPlus(bool showing);
	void SetTextColorByAmount(int32 currentHave, int32 requireedAmount);

	UFUNCTION()
	void OnPlusBtnClick();
	UFUNCTION()
	void OnToolTipBtnClick();

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void PlayAnimationEvent_AddAmount();

protected:
	virtual void CacheAssets() override;

	void UpdateValueAmount();

protected:
	TWeakObjectPtr<UB2Button>			BTN_ToolTip;
	TWeakObjectPtr<UTextBlock>			TB_Amount;
	TWeakObjectPtr<UTextBlock>			TB_RemainTime;	
	TWeakObjectPtr<UB2Button>			BTN_Plus;
	TWeakObjectPtr<UB2ButtonGoodInfoToolTip>	BTN_Goods;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ResourceType")
	EUIHeaderResourceType ResourceUIType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ValueAction")
	float ValueActionTime;

private:
	void OpenReadyPopUp();
	void SetBladePointAmountText(const int32 NewValueAmount, const int32 MaxValueAmoun = -1);

private:
	int32 m_nOrgValueAmount;
	int32 m_nCurValueAmount;
	int32 m_nGoalValueAmount;
	int32 m_nMaxValueAmount;
	int32 m_nBladePointShowMaxValue;	// const value
	float m_fValueChangeStartTime;

	int32 m_maxBladeAmount;

	bool isRequirement;
};
