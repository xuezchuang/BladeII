// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "../../DataStore/B2ClientDataStore.h"
#include "B2UIRankingRewardRow.generated.h"


UCLASS()
class BLADEII_API UB2UIRankingRewardRow : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SetRowInfo(const FModWeeklyReward& WeeklyReward);
	void SetRankingText(const FModWeeklyReward& WeeklyReward);

protected:
	UPROPERTY(EditAnywhere)
		TSoftClassPtr<class UB2UIRankingRewardPart>			UIP_RankingRewardPart;

private:
	//TWeakObjectPtr <class UB2UIRankingRewardPart> RankingRewardPart0;
	//TWeakObjectPtr<class UB2UIRankingRewardPart> RankingRewardPart1;
	//TWeakObjectPtr<class UB2UIRankingRewardPart> RankingRewardPart2;

	TWeakObjectPtr<UTextBlock>	TB_Ranking;

	TWeakObjectPtr<UVerticalBox>		X_VerticalBox;

	//TArray<TWeakObjectPtr<class UB2UIRankingRewardPart>> RankingRewardParts;

	UPROPERTY(Transient)
		class UB2UIRankingRewardPart* RankingRewardView;
};
