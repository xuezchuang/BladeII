// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UIPointShopLimitedEventPopup.generated.h"

UCLASS()
class BLADEII_API UB2PointShopEventPopupSlotVB : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;

public:
	void AddChildWidget(UWidget* Content);

private:
	TWeakObjectPtr<UVerticalBox>	VB;
};


UCLASS()
class BLADEII_API UB2PointShopEventPopupSlot : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;

public:
	void InitSlot(FText ftSlotText, bool IsTitleSlot = false);

private:
	TWeakObjectPtr<UWidgetSwitcher>		WS_Type;
	TWeakObjectPtr<UB2RichTextBlock>	RTB_Text;
};


typedef int32 iEVENT_TYPE;
typedef int32 iEVENT_POINT;

UCLASS()
class BLADEII_API UB2UIPointShopLimitedEventPopup : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()
	
protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;

public:
	void OpenPopup();
	void ClosePopup();

	void InitPointShopLimitedEventPopup(const int32 iEventID);

private:
	void InitStarGradeColumn();
	void InitEventPointColumn();

private:
	int32 GetStarGradeColumnMax();
	
private:
	UFUNCTION()
	void OnClickBTN_Close();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UB2PointShopEventPopupSlotVB>	PointShopEventPopupSlotVBClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UB2PointShopEventPopupSlot>	PointShopEventPopupSlotClass;

private:
	TWeakObjectPtr<UB2Button>	BTN_Close;

	TWeakObjectPtr<UB2RichTextBlock>			RTB_PopupTitle;
	TWeakObjectPtr<UB2RichTextBlock>			RTB_PopupExplain;

	TWeakObjectPtr<UHorizontalBox>				HB_Main;

private:
	TMap<iEVENT_TYPE, TArray<iEVENT_POINT>>		TMap_EventPoint;
};
