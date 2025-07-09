// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "UI/B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UIUnitySkillRewardPopup.generated.h"


UCLASS()
class BLADEII_API UB2UIUnitySkillRewardPopup : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void CloseWidgetDelegate() override;

	void SetUnitySkillImage(int32 nUnitySkillIndex);
protected:

	UFUNCTION(BlueprintCallable, Category = "animations")
	void FinishAnimation();

	UFUNCTION()
	void OnClickBTN_Modal();

private:
	TWeakObjectPtr<UB2Button> BTN_Confirm;
	TWeakObjectPtr<UTextBlock> STB_UnityRewardSuccess;
	TWeakObjectPtr<UTextBlock> TB_RewardMessage;
	TWeakObjectPtr<UB2Image> IMG_SkillIcon;
	TWeakObjectPtr<UTextBlock> TB_SkillName;

	TWeakObjectPtr<UWidgetAnimation> Anim_MailPopUp;
};
