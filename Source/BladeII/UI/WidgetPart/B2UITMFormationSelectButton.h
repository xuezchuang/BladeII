// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
//#include "BladeII.h"
#include "UI/B2UIWidgetBase.h"
#include "B2UITMFormationSelectButton.generated.h"

DECLARE_DELEGATE_OneParam(FFormationAnimationDelegate, ETMFormation);
DECLARE_DELEGATE_TwoParams(FFormationClickDelegate, ETMFormation, int32);

UCLASS()
class BLADEII_API UB2UITMFormationSelectButton : public UB2UIWidgetBase
{
	GENERATED_BODY()

private:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;

	UFUNCTION()
	void OnClickSelect();
	UFUNCTION()
	void OnClickEnhance();
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	UFUNCTION(BlueprintImplementableEvent)
		void OnClickState();

	UFUNCTION(BlueprintImplementableEvent)
		void OffClickState();
	
	void SetDelegate(FFormationAnimationDelegate AnimaionDelegate, FFormationClickDelegate ClickDelegate);
	void SetFormation(ETMFormation Formation, int32 Level, TArray<float>& AttackBonus, TArray<float>& DefenseBonus);
	void SetOnSlotState();
	void SetOffSlotState();
private:
	TWeakObjectPtr<class UB2UIFormationSelectButtonSlot> UIP_SlotInfo;
	TWeakObjectPtr<UButton> BTN_Enhance;
	TWeakObjectPtr<UButton> BTNX_Select;
	TWeakObjectPtr<UTextBlock> TB_Enhance;
	TArray<TWeakObjectPtr<UB2RichTextBlock>> RTB_Ability;
	TArray<TWeakObjectPtr<UOverlay>> O_Select;
	TArray<TWeakObjectPtr<UOverlay>> O_Default;
	ETMFormation FormationType;
	int32 FormationLevel;
	FFormationClickDelegate ButtonClickDelegate;
	FFormationAnimationDelegate SelectDelegate;
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
		TArray<UMaterialInterface*>		FormationOnIcon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
		TArray<UMaterialInterface*>		FormationOffIcon;
};
