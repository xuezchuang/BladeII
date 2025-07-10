// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "UI/B2UIWidget.h"
#include "B2UIMailRewardPopUp.generated.h"

/**
*
*/
DECLARE_DELEGATE(FMsgPopupOnClick)

UCLASS()
class BLADEII_API UB2UIMailRewardPopUp : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

	UFUNCTION()
		void OnClickConfirm();

public:
	virtual void OnOpen(bool RightNow = false) override;
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetConfirmCallback(const FMsgPopupOnClick& Handler);

	UFUNCTION(BlueprintCallable, Category = "animations")
		void FinishAnimation();

public:

	bool  bPlayAnimation;
	float PlayTime;
	float AccumulatedPlayTime;

	FMsgPopupOnClick				ConfirmDelegate;

	TWeakObjectPtr<UButton>			BTN_Confirm;
	TWeakObjectPtr<UTextBlock>		TB_Reward;

	TWeakObjectPtr<UWidgetAnimation> Anim_MailPopUp;

};
