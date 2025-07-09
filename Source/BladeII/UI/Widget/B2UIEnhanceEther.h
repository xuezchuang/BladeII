// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UIEnhanceEther.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIEnhanceEther : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

public:
	void SetInfo(const FB2Ether* EnhanceEther, EPCClass InSelectPCClass);
	virtual void Init() override;
	
	UFUNCTION(BlueprintCallable, Category = BladeII)
		void EndFailNotice_BP();
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
		void PlayFailAnim_BP();

	UPROPERTY(EditAnywhere)
		int MAXLEVEL = 9;

protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

private:
	virtual void CloseWidgetDelegate() override;

	UFUNCTION()
		void OnClickClose();
	UFUNCTION()
		void OnClickEnhance();
	UFUNCTION()
		void OnClickAutoEnhance();
	UFUNCTION()
		void CollectLightAnimationFinished();
	UFUNCTION()
		void EnhanceSuccessAnimationFinished();

	int GetChangeSubOptionIndex(const FB2Ether* EnhanceEther, int32 InSubOptionState) const;
	void SetEtherCost();
	void UpdateInfo();
	void SetIsAutoEnhance(bool Value);
	void SetSuccessEffectVisible(ESlateVisibility InValue);
	void ResponseEnhanceEther(const FB2EnhanceEtherPtr EnhanceEtherPtr);

	const int MAXSUBOPTION = 3;
	bool IsTouch;//max레벨 체크
	bool IsAnimantion;
	bool IsAutoEnhance;
	bool IsEnhanceSuccess;
	int64 EtherID;
	uint32 EnhanceEtherEventID;
	int32 EnhaceCost;
	int32 SubOptionState;
	EPCClass	PCClass;
	b2network::B2AetherServerInfoPtr	EnhanceEtherInfo;
	
	TWeakObjectPtr<UB2Button> X_BTN_Close;
	TWeakObjectPtr<UB2Button> X_BTN_Enhance;
	TWeakObjectPtr<UB2Button> X_BTN_AutoEnhance;
	TWeakObjectPtr<UB2Button> X_BTN_Confirm;

	TWeakObjectPtr<UB2RichTextBlock> X_RTB_UpMainOptionName;
	TWeakObjectPtr<UB2RichTextBlock> X_RTB_UpMainOptionValue;
	TWeakObjectPtr<UB2RichTextBlock> X_RTB_EnhanceRate;

	TWeakObjectPtr<UTextBlock> X_TB_TItle;
	TWeakObjectPtr<UTextBlock> X_TB_BaseTier;
	TWeakObjectPtr<UTextBlock> X_TB_UpTier;
	TWeakObjectPtr<UTextBlock> X_TB_CostText;
	TWeakObjectPtr<UTextBlock> X_TB_Cost;
	TWeakObjectPtr<UTextBlock> X_TB_EnhanceBTN;
	TWeakObjectPtr<UTextBlock> X_TB_AutoEnhanceBTN;
	TWeakObjectPtr<UTextBlock> X_TB_UpOptionDesc;
	TWeakObjectPtr<UTextBlock> X_TB_LeveUPText;
	TWeakObjectPtr<UTextBlock> X_TB_MaxDesc;
	TWeakObjectPtr<UTextBlock> X_TB_Confirm;

	TWeakObjectPtr<UOverlay> X_O_UpOptionDesc;
	
	TWeakObjectPtr<UImage> X_IMG_BaseTierBG;
	TWeakObjectPtr<UImage> X_IMG_UpTierBG;
	TWeakObjectPtr<UImage> X_IMG_Line;

	TWeakObjectPtr<UHorizontalBox> X_HB_Buttons;
	TWeakObjectPtr<UHorizontalBox> X_HB_Cost;
	
	TWeakObjectPtr<UPanelWidget> X_PW_MaxLevel;
	TWeakObjectPtr<UPanelWidget> X_PW_Tier;
	
	TWeakObjectPtr<UWidgetAnimation> ANIM_Enhance_Success;
	TWeakObjectPtr<UWidgetAnimation> ANIM_Bonus_01;
	TWeakObjectPtr<UWidgetAnimation> ANIM_Bonus_02;
	TWeakObjectPtr<UWidgetAnimation> Anim_Collect_light;
	
	TWeakObjectPtr<class UB2UIEtherScrollSlot> X_UIP_EtherScrollSlot;
	TWeakObjectPtr<class UB2UIEtherOptionSlot> X_UIP_BaseMainOption;
	
	TArray<TWeakObjectPtr<UB2RichTextBlock>> X_RTB_UPSubOptions;
	TArray<TWeakObjectPtr<UImage>> X_IMG_UPBGs;
	TArray<TWeakObjectPtr<class UB2UIEtherOptionSlot>> UIP_BaseEtherOptions;
	TArray<float> BeforeSubOptionValue;
};