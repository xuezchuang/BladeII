#pragma once

#include "B2UIWidgetBase.h"
#include "B2UIRankerInfo.generated.h"

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIRankerInfo : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	void SetRankerInfo(const FModRankerInfo& RankerInfo,bool bNotResult = false);

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

public:
	UFUNCTION()
		void OnClickSelect();

    UFUNCTION()
        void OnClickPVPReplay();

private:
	TWeakObjectPtr<UTextBlock> TB_Ranking;
	TWeakObjectPtr<class UB2UIGradeRankAndStar> UIP_GradeIcon;
	TWeakObjectPtr<class UB2UIPVPEntryCharacters> UIP_EntryCharacters;
	TWeakObjectPtr<UButton> BTN_ShowPlayerInfo;

	TWeakObjectPtr<UTextBlock> TB_CombatPower;
	TWeakObjectPtr<UTextBlock> TB_PlayerLevel;
	TWeakObjectPtr<UTextBlock> TB_PlayerName;

	TWeakObjectPtr<UImage> IMG_RankNumber_1;
	TWeakObjectPtr<UImage> IMG_RankNumber_2;
	TWeakObjectPtr<UImage> IMG_RankNumber_3;
	TWeakObjectPtr<UImage> IMG_RankBG_1;
	TWeakObjectPtr<UImage> IMG_RankBG_2;
	TWeakObjectPtr<UImage> IMG_RankBG_3;
	TWeakObjectPtr<UImage> IMG_BestBG;
	TWeakObjectPtr<UImage> IMG_Line;
	TWeakObjectPtr<UB2Button>	BTN_Select;

    TWeakObjectPtr<UB2Button> BTN_PVPReplay;
    TWeakObjectPtr<UTextBlock> TB_PVPReplay;

private:
	int64			AccountInfo;
	EPCClass		SubClass;
	EPCClass		MainClass;

    FModReplayInfo ReplayInfo;
};
