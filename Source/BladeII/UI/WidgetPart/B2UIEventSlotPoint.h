// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetPart/B2UIEventSlot.h"
#include "../B2RichTextBlock.h"
#include "B2UIEventSlotPoint.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIEventSlotPoint : public UB2UIEventSlot
{
	GENERATED_BODY()
	
protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

private:
	UFUNCTION()
		void OnClickBTN_GetReward();

public:
	void InitEventPointPart(const b2network::B2mdPointEventReward& EventRewardInfo);
	virtual void UpdateVisibleState() override;
	int32 GetNeedPoint() { return CachedEventRewardInfo.need_point; }

	UFUNCTION(BlueprintImplementableEvent, Category = "UB2UIEventSlotPoint")
		void OnGetReward_BP();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
		void GetRewardAnimEnd();

private:
	TWeakObjectPtr<class UB2UIRewardIcon>	UIP_RewardIcon;

	TWeakObjectPtr<UB2Button>				BTN_GetReward;

	TWeakObjectPtr<UB2RichTextBlock>		RTB_ReceiveBtn;

	TWeakObjectPtr<UB2RichTextBlock>		RTB_ItemName;

	TWeakObjectPtr<UOverlay>				O_Complete;

	TWeakObjectPtr<UImage>					IMG_GetRewardEffect;

private:
	b2network::B2mdPointEventReward			CachedEventRewardInfo;
};
