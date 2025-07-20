// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIWidgetBase.h"
#include "../Widget/B2ButtonGoodInfoToolTip.h"
#include "Components/TextBlock.h"
#include "B2UIRewardEventSet.generated.h"

DECLARE_DELEGATE(FRepeatBattleCountDownCB)

UENUM(BlueprintType)
enum class EUIRewardEventSetType : uint8
{
	REST_ChapterStarReward_Gold,
	REST_ChapterStarReward_Gem,

	REST_RepeatMissionReward,

	REST_MailReceived,
	REST_ModDailyReward,

	REST_End
};

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIRewardIcon : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:
	UB2UIRewardIcon(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;
	virtual void CacheAssets() override;

	UMaterialInterface* GetIconMaterial(int32 RewardID);
	UMaterialInterface* GetIconMaterial(ERewardType Type);
	FText GetRewardText(int32 RewardID);

	void SetIconAndText(class UMaterialInterface* Image, const FText& Text);
	void SetIcon(class UMaterialInterface* Image);
	void SetDetailText(const FText& Text);
	void SetAmount(const FText& AmountText);
	void SetNeedCount(int32 nCount, int32 nNeedCount);
	void SetGrade(int32 nGrade);
	void SetCountColor(FSlateColor ApplyColor);

	void UpdateRewardIconAndName(int32 RewardID);
	void UpdateRewardIconByItemIndex(int32 ItemIndex, const FText& Text);
	void UpdateRewardIcon(int32 RewardID, const FText& Text);
	void UpdateRewardIcon(ERewardType Type, const FText& Text);
	void UpdateRewardIcon(b2network::B2RewardPtr RewardInfoPtr);

	FORCEINLINE void UpdateRewardIcon(int32 RewardID, int32 Count)
	{
		UpdateRewardIcon(RewardID, FText::AsNumber(Count));
	}
	FORCEINLINE void UpdateRewardIcon(ERewardType Type, int32 Count)
	{
		UpdateRewardIcon(Type, FText::AsNumber(Count));
	}

protected:
	void ActiveGoodsButton(int32 RewardID);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Reward)
		int32 RewardIdFromBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Reward)
		int32 RewardAmountFromBP;

protected:
	TWeakObjectPtr<UImage>						IMG_RewardMainIcon;
	TWeakObjectPtr<UTextBlock>					TB_RewardAmount;
	TWeakObjectPtr<UWidgetSwitcher>				TextSwitcher;
	TWeakObjectPtr<UTextBlock>					TB_HaveCount;
	TWeakObjectPtr<UTextBlock>					TB_NeedCount;
	TWeakObjectPtr<UB2ButtonGoodInfoToolTip>	BTN_Goods;
	TWeakObjectPtr<UWidgetSwitcher>				WS_GradeBG;

	int32 TranslateToGoodsInfo(int32 RewardID);
};
