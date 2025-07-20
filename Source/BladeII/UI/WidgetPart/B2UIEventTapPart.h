// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "B2UIEventTapPart.generated.h"

UCLASS()
class BLADEII_API UB2UIEventTapPart : public UB2UIWidget
{
	GENERATED_BODY()
	
protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void RegisterUIMarkForRedDot() final;

public:
	void InitEventTap(const b2network::B2mdEventInfo& fEventInfo);
	void SetSelected(bool bSelected);
	int32 GetEventID() { return CachedEventInfo.event_id; }
	int32 GetTapOrder() { return CachedEventInfo.tab_order; }
	int32 GetEventCategory() { return CachedEventInfo.event_category; }
	int32 GetEventSubCategory() { return CachedEventInfo.event_sub_category; }
	bool HasRedDot_EventTap();

private:
	UFUNCTION()
	void OnClickBTN_EventTap();

private:
	TWeakObjectPtr<UTextBlock>	TB_Selected;
	TWeakObjectPtr<UTextBlock>	TB_UnSelected;

	TWeakObjectPtr<UOverlay>	O_Selected;
	TWeakObjectPtr<UOverlay>	O_Unselected;
	
	TWeakObjectPtr<UB2Button>	BTN_EventTap;

private:
	b2network::B2mdEventInfo	CachedEventInfo;
};
