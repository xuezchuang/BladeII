// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIAttendanceParts.generated.h"


UCLASS()
class BLADEII_API UB2UIAttendanceParts : public UB2UIWidget
{
	GENERATED_BODY()


public:
	virtual void Init() override;
	
	void SetAttendanceInfo(FAttendanceDayData InAttendanceDayData);
	void ReceiveReward(b2network::B2RewardPtr RewardItem);
	void StartRewardAnimation();

	UFUNCTION(BlueprintCallable, Category = Animations)
		void FinishRecieveAnimation();

	UFUNCTION(BlueprintImplementableEvent)
		void OnStartRewardAnimation_BP();

protected:
	virtual void CacheAssets() override;

private:
	TWeakObjectPtr<UImage>           IMG_ITEM;
	TWeakObjectPtr<UImage>			 IMG_RewardDone;
	TWeakObjectPtr<UTextBlock>       TXT_Day;
	TWeakObjectPtr<UB2RichTextBlock>       TXT_ItemName;
	TWeakObjectPtr<UTextBlock>       TXT_ItemCount;
	TWeakObjectPtr<UPanelWidget>	 OV_RewardDone;
	TWeakObjectPtr<UPanelWidget>	 P_BackGroundSpecial;
	TWeakObjectPtr<UWidgetAnimation> Anim_Check;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool IsSpecialDay = false;
};
