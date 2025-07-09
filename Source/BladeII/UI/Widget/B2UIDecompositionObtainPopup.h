// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UIDecompositionObtainPopup.generated.h"


UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIDecompositionObtainPopup : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;

public:
	void SetEtherPieceInfo(int32 ObtainAmount);
	void SetEquipItemInfo(TArray<int32> ObtainItemType, TArray<int32> ObtainAmount);

private:
	UFUNCTION()
		void OnClickClose();

	TWeakObjectPtr<UTextBlock>					X_TB_Title;
	TWeakObjectPtr<UTextBlock>					X_TB_ObtainDesc;

	TArray<TWeakObjectPtr<UVerticalBox>>		X_VB_Obtain;
	TArray<TWeakObjectPtr<UImage>>				X_IMG_Obtain;
	TArray<TWeakObjectPtr<UTextBlock>>			X_TB_ObtainCount;

	TWeakObjectPtr<UTextBlock>					X_TB_Btn;
	TWeakObjectPtr<UB2Button>					X_BTN_Yes;
};
