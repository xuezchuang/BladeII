// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "EventSubsystem.h"
#include "B2UIDecompositionPopup.generated.h"


DECLARE_DELEGATE(FOnClickPositive)

UCLASS()
class BLADEII_API UB2UIDecompositionPopup : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

public:
	void SetEtherInfo(const int32 InCount, const FOnClickPositive& InPositiveCallBack);
	void SetEquipItemInfo(TArray<int32> InItemType, TArray<int32> InCount, const FOnClickPositive& InPositiveCallBack);
	void SetTotemInfo(const int32 InCount, const FOnClickPositive& InPositiveCallBack);

	UFUNCTION()
		void OnClickPositive();
	UFUNCTION()
		void OnClickNegative();

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void CloseWidgetDelegate() override;

private:
	void SubscribeEvents();
	void UnsubscribeEvents();

private:

	TWeakObjectPtr<UTextBlock>					X_TB_Title;
	TWeakObjectPtr<UTextBlock>					X_TB_Description;

	TArray<TWeakObjectPtr<UVerticalBox>>		X_VB_Decomposition;
	TArray<TWeakObjectPtr<UImage>>				X_IMG_Decomposition;
	TArray<TWeakObjectPtr<UTextBlock>>			X_TB_Count;
	
	TWeakObjectPtr<UB2Button>					X_BTN_Negative;
	TWeakObjectPtr<UB2Button>					X_BTN_Positive;
	TWeakObjectPtr<UTextBlock>					X_TB_BtnNegative;
	TWeakObjectPtr<UTextBlock>					X_TB_BtnPositive;
	
private:
	FOnClickPositive							PositiveCallBack;
	TArray<std::shared_ptr<issue>>				Issues;
};
