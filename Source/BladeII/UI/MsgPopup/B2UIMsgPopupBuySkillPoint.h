// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIMsgPopupBase.h"
#include "B2UIMsgPopupBuySkillPoint.generated.h"

/*
* The base class of messaged popup. it has two buttons.
*/

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIMsgPopupBuySkillPoint : public UB2UIMsgPopupBase
{
	GENERATED_BODY()

public:
	UB2UIMsgPopupBuySkillPoint(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	
	void SetButtonText(const FText& InText);
	void SetGemCostUnit(int32 InCost);
	void SetGemCostTotal(int32 InCostTotal);

	void InitializeBuySkillPoint(EPCClass);

	void UpdateGemCostTotal();
	void UpdateSelectPoint();

protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;

	UFUNCTION(BlueprintCallable, Category = "BladeII BattleSkill")
		void OnClickBtnConfirm();

	UFUNCTION(BlueprintCallable, Category = "BladeII BattleSkill")
		void OnClickBtnClose();

	UPROPERTY(EditAnywhere, Category = "Character Portrait")
		TArray<UMaterialInterface*> CharacterPortraits;
	UPROPERTY(EditAnywhere, Category = "BladeII BattleSkill")
	FSlateColor ButtonNameColorBase;

protected:
	TWeakObjectPtr<UButton>		BTN_Confirm;
	TWeakObjectPtr<UButton>		BTN_Close;
	TWeakObjectPtr<UTextBlock>	TB_BtnConfirm;
	TWeakObjectPtr<UTextBlock>	TB_GemPerSP;
	TWeakObjectPtr<UTextBlock>	TB_Title;
	TWeakObjectPtr<class UB2UISlotItem> UIP_ItemCost;

	TWeakObjectPtr<UImage>	IMG_SelectedChar;
	TWeakObjectPtr<UTextBlock> TB_TotalBoughtPointText;
	TWeakObjectPtr<UTextBlock> TB_TotalBoughtPoint;
	TWeakObjectPtr<UTextBlock> TB_SelectPointText;
	TWeakObjectPtr<UTextBlock> TB_SelectPoint;
	TWeakObjectPtr<UTextBlock> TB_SoldOut;
	TWeakObjectPtr<class UB2UIBuySkillPointBar> UIP_BuySkillPointBar;

	TWeakObjectPtr<UHorizontalBox>		HB_SelectPointBox;
	
private:
	FMsgPopupOnClick ClickHandler;
	uint32 ChangeBuySkillPointTicket;
	bool ButtonState;

	EPCClass SelectedCharacter;
};