// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UICostumeEnhancePopup.generated.h"


UCLASS()
class BLADEII_API UB2UICostumeEnhancePopup : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;
	virtual void CloseWidgetDelegate() override;

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void UnsubscribeEvent();
	void SubscribeEvent();

	void UpdatePopupInfo(const FB2Item& InItem);
	void OnFailEnhance();
	void InitCostumeItemInfoPopup(const FVector2D& v2PopupPosition, const float& fPopupSizeX);

	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayAnimation_IMP();

	UFUNCTION(BlueprintCallable)
	void OnEndAnimation_BP();

protected:
	UFUNCTION()
	void OnClickBTN_Close();
	UFUNCTION()
	void OnClickBTN_EnhanceCommit();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UIHeaderBezel")
	float fTopBezel_BP;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	TSubclassOf<class UB2UICostumeDecompositionItem>	MaterialSlot;

private:
	TWeakObjectPtr<UCanvasPanel>					CP_PopupMain;
	
	TWeakObjectPtr<UB2Button>						BTN_Close;
	TWeakObjectPtr<UB2Button>						BTN_EnhanceCommit;

	TWeakObjectPtr<UTextBlock>						STB_EnhanceTitle;
	TWeakObjectPtr<UTextBlock>						STB_Enhance;
	TWeakObjectPtr<UTextBlock>						STB_Power;
	TWeakObjectPtr<UTextBlock>						STB_EnhanceIngredCount;
	TWeakObjectPtr<UTextBlock>						STB_EnhanceCommitBtn;
	
	TWeakObjectPtr<UTextBlock>						TB_CurEnhance;
	TWeakObjectPtr<UTextBlock>						TB_CurPower;
	TWeakObjectPtr<UTextBlock>						TB_AfterEnhance;
	TWeakObjectPtr<UTextBlock>						TB_AfterPower;
	TWeakObjectPtr<UTextBlock>						TB_AfterPowerPlus;
	TWeakObjectPtr<UTextBlock>						TB_EnhanceCostNum;

	TWeakObjectPtr<UPanelWidget>					List_NeedEnhance;
	
	TWeakObjectPtr<UImage>							IMG_EnhanceCostGoldIcon;
	TWeakObjectPtr<class UB2UICostumeItemSlot>		UIP_CostumeItemSlot;

	TArray<std::shared_ptr<class issue>> Issues;

	int32 CurrentItemID;
	bool bNotEnoughMoney;
	bool bRequest;

	EItemInvenType CurrentItemInvenType;

};
