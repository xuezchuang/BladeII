// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "B2UISeasonEventTab.generated.h"

enum class ESEASONEVENT_TAB_STATE : int32
{
	Disable = 0,
	UnSelected,
	Selected,
	Max,
};

UCLASS()
class BLADEII_API UB2UISeasonEventTab : public UB2UIWidget
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void RegisterUIMarkForRedDot() final;
	
protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;

protected:
	UFUNCTION()
	void OnClickBTN_SeasonEventTab();

	UFUNCTION()
	bool HasRedDot_SeasonEventTab();

public:
	void SetSeasonEventDay(int32 iDay) { iSeasonEventDay = iDay; }
	void SetSeasonEventProgressDay(int32 iDay) { iSeasonEventProgressDay = iDay; }
	void SetCheckReddot(bool bCheck) { bCheckRedDot = bCheck; }
	void UpdateSeasonEventTab(int32 iSelectSeasonEventDay);

private:
	TWeakObjectPtr<UWidgetSwitcher>	WS_IMG_EventTab;

	TArray<TWeakObjectPtr<UTextBlock>>	ArrayTB_SeasonEventTab;

	TWeakObjectPtr<UB2Button>		BTN_SeasonEventTab;

	int32	iSeasonEventDay;

	int32	iSeasonEventProgressDay;

	bool	bCheckRedDot;
};
