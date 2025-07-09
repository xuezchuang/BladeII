// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UIFormationPopup.generated.h"


struct IncreaseData
{
	float AttackIncrease;
	float DefenceIncrease;

	IncreaseData()
	{
		AttackIncrease = 0;
		DefenceIncrease = 0;
	}
};

struct FAbilitySlot
{
	TWeakObjectPtr<UHorizontalBox>										HB_Box;
	TWeakObjectPtr<UB2RichTextBlock>									RTB_CurAbil;
	TWeakObjectPtr<UTextBlock>											TB_NextAbil;
	TWeakObjectPtr<UImage>												IMG_Arrow;

	IncreaseData SetInfo(ETMFormation InFormation, int32 InLevel, int32 InIndex);
	void MaxLevelSetting();
};

UCLASS()
class BLADEII_API UB2UIFormationPopup : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

public:
	virtual	void Init() override;
	void SetData(ETMFormation InFormation, int32 InLevel);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
		TArray<UMaterialInterface*> FormationIconArr;
	UFUNCTION(BlueprintCallable, Category = BladeII)
		void EndSuccessAnim_BP();

protected:
	virtual void DestroySelf(UB2UIManager* InUIManager) override;
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;
	virtual void CloseWidgetDelegate() override;

private:
	void PlaySuccessAnim();
	void SetAbility(ETMFormation InFormation, int32 InLevel);
	UFUNCTION()
		void OnClickedConfirm();
	
	const int32 MAX_SLOT = 3;
	
	TWeakObjectPtr<UButton> BTN_Cancle;
	TWeakObjectPtr<UB2Button> BTN_Confirm;
	TWeakObjectPtr<UTextBlock> TB_EnhanceCost;
	TWeakObjectPtr<UTextBlock> TB_Lv;
	TWeakObjectPtr<UTextBlock> TB_Formation;
	TWeakObjectPtr<UTextBlock> TB_Title;
	TWeakObjectPtr<UTextBlock> TB_Descrition;
	TWeakObjectPtr<UTextBlock> TB_Confirm;
	TWeakObjectPtr<UTextBlock> TB_Cancle;
	TWeakObjectPtr<UTextBlock> TB_MaxLevel;
	TWeakObjectPtr<UImage> IMG_Formation;
	TWeakObjectPtr<class UB2UISlotItem> UIP_EnhanceCost;
	TArray<FAbilitySlot> AbilitySlots;
	b2network::B2BattleFormationPtr NextFormationInfo;
	uint32 EnhanceFormationEventID;
	ETMFormation FormationType;
	TArray<IncreaseData> IncreaseDatas;
	UWidgetAnimation* Anim_Enhance;
	UWidgetAnimation* Anim_Background;
};

