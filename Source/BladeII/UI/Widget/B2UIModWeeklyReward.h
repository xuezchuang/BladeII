#pragma once

#include "B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "B2UIBackWidget.h"
#include "../DataStore/B2ClientDataStore.h"
#include "B2UIModWeeklyReward.generated.h"

/*
* Lobby Main UI
*/

UCLASS()
class BLADEII_API UB2UIModWeeklyReward : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()
private:
	TWeakObjectPtr<UB2Button>						BTN_Close;

	TArray<TWeakObjectPtr<UCanvasPanel>>				GradeOverlayArr;
	TArray<TWeakObjectPtr<class UB2UIRankingRewardRow>> RankingRewardRow;

protected:
	UPROPERTY(EditAnywhere)
		TSoftClassPtr<class UB2UIGradeRankAndStar>			UIP_GradeRankAndStar;

private:
	void RewardDataSetting(const TArray<FModWeeklyReward>&	WeeklyData);
	virtual void CloseWidgetDelegate() override;

	TArray<class UB2UIGradeRankAndStar*>		GradeWidgetMap;

public:
	virtual void Init() override;
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

public:
	void SetOpenRewardListType(FName UIType);
	void GradeInfo();

	UFUNCTION()
		void OnClickClose();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GradeStar Overay Margin")
		FMargin GradeMargin;
};