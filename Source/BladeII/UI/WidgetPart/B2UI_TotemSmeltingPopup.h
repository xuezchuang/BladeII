// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UI_TotemSmeltingPopup.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UI_TotemSmeltingPopup : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

protected:
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;

	virtual void CloseWidgetDelegate() override;

public:
	void InitTotemSmeltingPopup(const FB2Totem& InTotem);

private:
	void SubscribeEvents();
	void UnsubscribeEvents();

	void UpdateTotemSmeltingPopup(const FB2Totem& InTotem);
	void UpdateTabBtnState(const int32& iTabIndex);

	void UpdateTotemIcon(const FB2Totem& InTotem);
	void UpdateTotemName(const FB2Totem& InTotem);
	void UpdateSmeltingLevel(const FB2Totem& InTotem);
	void UpdateSmeltingSuccessChance(const FB2Totem& InTotem);

	void UpdateBySmeltingResult();

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void PlayAnimation_SmeltingSuccess();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayAnimation_SmeltingFail();

	UFUNCTION(BlueprintImplementableEvent)
	void PlaySound_SetSmeltingHoleSuccess();

	UFUNCTION(BlueprintImplementableEvent)
	void PlaySound_SetSmeltingHoleFail();

	UFUNCTION(BlueprintCallable, Category = "B2Totem")
	void OnFinishAnimationEvent_SmeltingSuccess();

	UFUNCTION(BlueprintCallable, Category = "B2Totem")
	void OnFinishAnimationEvent_SmeltingFail();

private:
	UFUNCTION()
		void OnClickBTN_Close();
	UFUNCTION()
		void OnClickBTN_TabSmeltingByGold();
	UFUNCTION()
		void OnClickBTN_TabSmeltingByPiece();

private:
	TWeakObjectPtr<UTextBlock>							TB_SmeltingTitle;
	TWeakObjectPtr<UB2Button>							BTN_Close;

	TWeakObjectPtr<UB2RichTextBlock>					RTB_TotemName;
	TWeakObjectPtr<UImage>								IMG_TotemIcon;
	TWeakObjectPtr<UB2RichTextBlock>					RTB_SuccessChance;
	TWeakObjectPtr<UTextBlock>							TB_SmeltingLevel;

	TWeakObjectPtr<UWidgetSwitcher>						WS_SmeltingByGold;
	TWeakObjectPtr<UTextBlock>							TB_UnSelectedSmeltingByGold;
	TWeakObjectPtr<UTextBlock>							TB_SelectedSmeltingByGold;
	TWeakObjectPtr<UB2Button>							BTN_TabSmeltingByGold;

	TWeakObjectPtr<UWidgetSwitcher>						WS_SmeltingByPiece;
	TWeakObjectPtr<UTextBlock>							TB_UnSelectedSmeltingByPiece;
	TWeakObjectPtr<UTextBlock>							TB_SelectedSmeltingByPiece;
	TWeakObjectPtr<UB2Button>							BTN_TabSmeltingByPiece;

	TWeakObjectPtr<class UB2UI_TotemSmeltingStatus>		UIP_TotemSmeltingStatusBP;

private:
	TArray<issue_ptr>									Issues;

	int32												CurrentSelectedTab;

	FB2Totem											CachedB2TotemInfo;
};
