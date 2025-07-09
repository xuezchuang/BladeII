// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "B2UIEventTabPointShopLimited.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIEventTabPointShopLimited : public UB2UIWidget
{
	GENERATED_BODY()
	
protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
	
private:
	UFUNCTION()
	void OnClickBTN_EventTap();

public:
	void InitEventTab(const int32 iTapGroupID, const int32 iTapGroupRewarID);

public:
	void UpdateEventTabPointShopLimited(bool IsSelected);

public:
	const int32 GetTapGroupID() { return CachedTapGroupID; }

private:
	TWeakObjectPtr<UB2Button>			BTN_EventTap;
	TWeakObjectPtr<UWidgetSwitcher>		WS_State;
	TWeakObjectPtr<UB2RichTextBlock>	RTB_TapGroupNameSelected;
	TWeakObjectPtr<UB2RichTextBlock>	RTB_TapGroupNameUnSelected;
	
private:
	int32 CachedTapGroupID;
	int32 CachedTapGroupRewardID;
};
