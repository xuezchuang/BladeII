// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UIGuildRewardInfo.generated.h"

UCLASS()
class BLADEII_API UB2UIGuildRewardSlotRow : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;

public:
	void SetRewardInfo(TArray<b2network::B2RewardPtr> RewardInfo);

protected:
	TArray<TWeakObjectPtr<class UB2UIRankingRewardPart>>	GuildRewardSlotArr;
	TWeakObjectPtr<class UB2UIRankingRewardPart>	UIP_GuildRewardSlotRowSlot;
	TWeakObjectPtr<class UB2UIRankingRewardPart>	UIP_GuildRewardSlotRowSlot1;
	TWeakObjectPtr<class UB2UIRankingRewardPart>	UIP_GuildRewardSlotRowSlot2;
	TWeakObjectPtr<class UB2UIRankingRewardPart>	UIP_GuildRewardSlotRowSlot3;
};


UCLASS()
class BLADEII_API UB2UIGuildRewardSlot : public UB2UIWidget
{
	GENERATED_BODY()
protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;

	void SetRewardInfo(b2network::B2mdGuildBattleResultRewardPtr RewardInfo);

	UFUNCTION(BlueprintImplementableEvent)
	void SetBoxGrade(int32 Grade);

	UFUNCTION()
		void OnClickDetail();

protected:
	//TWeakObjectPtr<class UB2UIGuildRewardSlotRow>		UIP_GuildRewardSlotRow;
	TWeakObjectPtr<UTextBlock>							TB_MedalCount;
	TWeakObjectPtr<UB2Button>							BTN_Detail;

	b2network::B2mdGuildBattleResultRewardPtr			RewardInfo;

	int32 GuildGrade;
};

UCLASS()
class BLADEII_API UB2UIGuildRewardInfo : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SetRewardInfo(const TMap<int32, b2network::B2mdGuildBattleResultRewardPtr>& InRewardInfo, int32 CurrentMedal);

	UFUNCTION()
		void OnClickClose();

protected:
	TWeakObjectPtr<UTextBlock>		TB_MedalCount;
	TWeakObjectPtr<UB2Button>		BTN_Close;
	TWeakObjectPtr<UHorizontalBox>	X_RewardSlotVB;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
		TSubclassOf<class UB2UIGuildRewardSlot>		GuildRewardSlot;
};
