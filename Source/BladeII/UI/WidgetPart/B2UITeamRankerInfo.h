#pragma once
//#include "BladeII.h"
#include "B2UIWidgetBase.h"
#include "../../DataStore/B2ClientDataStore.h"
#include "B2UITeamRankerInfo.generated.h"

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UITeamRankerInfo : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	void SetRankerInfo(const FModRankerInfo& RankerInfo, bool bNotResult = false);

	UFUNCTION()
		void OnClickSelect();

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

private:
	TWeakObjectPtr<UTextBlock> TB_Ranking;
	TWeakObjectPtr<class UB2UIGradeRankAndStar> UIP_GradeIcon;
	TWeakObjectPtr<class UB2UITeamEntryCharacter> UIP_Team_Entry_Characters1;
	TWeakObjectPtr<class UB2UITeamEntryCharacter> UIP_Team_Entry_Characters2;
	TWeakObjectPtr<class UB2UITeamEntryCharacter> UIP_Team_Entry_Characters3;

	TWeakObjectPtr<UTextBlock> TB_TotalAttackPoint;
	TWeakObjectPtr<UTextBlock> TB_TotalDefencePoint;
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

	TWeakObjectPtr<UB2Button>		BTN_Select;

	int64 AccountId;
	EPCClass PCClass1;
	EPCClass PCClass2;
	EPCClass PCClass3;
};