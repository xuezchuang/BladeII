// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidgetBase.h"
#include "B2UIDocDonation.h"
#include "B2UIDonationPointStepReward.generated.h"




/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIDonationPointStepReward : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	void UpdateInfo();

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;

private:
	UFUNCTION()
		void OnClickBack();

#if WITH_EDITOR
	void EditorOnPostLoad();
#endif
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DonationPointStepReward")
		int32 StepReward = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DonationPointStepReward")
		TSoftObjectPtr<class UMaterialInterface> CompletionIcon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DonationPointStepReward")
		TSoftObjectPtr<class UMaterialInterface> IncompletionIcon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DonationPointStepReward")
		FSlateColor CompletionColor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DonationPointStepReward")
		FSlateColor IncompletionColor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DonationPointStepReward")
		FText StepName;

	TWeakObjectPtr<UB2Image> IMG_Icon;
	TWeakObjectPtr<UTextBlock> TB_DonationPoint;
	TWeakObjectPtr<UB2Button> BTN_Back;
	TWeakObjectPtr<UImage> FX_Complete1;
	TWeakObjectPtr<UWidgetAnimation> ANIM_Complete;
	TWeakObjectPtr<UTextBlock> TB_StepName;
	TWeakObjectPtr<UB2UIWidgetBase> UIP_RedDot;
	TWeakObjectPtr<UPanelWidget> CP_Completion;
};