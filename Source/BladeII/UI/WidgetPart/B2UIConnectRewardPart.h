// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidgetBase.h"
#include "../B2RichTextBlock.h"
#include "B2UIConnectRewardPart.generated.h"


UCLASS()
class BLADEII_API UB2UIConnectRewardPart : public UB2UIWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void UpdateStaticText() override;

	bool  GetReward() const;
	int32 GetRewardItemIndex() const;
	float GetInterval() const;
	float GetRewardTime() const;
	int32 GetRewardOrder() const { return RewardOrder; }

	void SetCanReceiveEffect(bool Enable);
	void SetReward(const bool _Reward);
	void SetRewardState(int32 InItemIndex, int32 InRewardTime, float InInterval, int32 InRewardCount, bool InReward, int32 InRewardOrder);

	UFUNCTION(BlueprintCallable, Category = Animations)
	void FinishRecieveAnimation();
	
	UFUNCTION(BlueprintImplementableEvent, Category = BladeII)
	void OnStartRewardAnimation_BP();
private:
	bool   IsReward;
	int32  RewardItemIndex;
	int32  RewardTime;
	int32  RewardOrder;
	float  Interval;

	TWeakObjectPtr<UImage>			 IMG_Base;
	TWeakObjectPtr<UImage>			 IMG_RewardEnd;
	TWeakObjectPtr<UImage>			 IMG_RewardDone;
	TWeakObjectPtr<UB2Image>		 IMG_ITEM;
	TWeakObjectPtr<UB2Image>         IMG_ActiveInner;
	TWeakObjectPtr<UB2Image>		 IMG_ActiveOuter;

	TWeakObjectPtr<UTextBlock>		 TB_RewardCount;
	TWeakObjectPtr<UPanelWidget>	 P_ReceiveComplete;
	TWeakObjectPtr<UPanelWidget>	 P_CanReceive;
	TWeakObjectPtr<UB2RichTextBlock> TB_Minute;
	TWeakObjectPtr<UB2RichTextBlock> TB_ItemName;
	TWeakObjectPtr<UWidgetAnimation>	Anim_Receive;
	TWeakObjectPtr<UTextBlock>			TB_GetReward;
};
