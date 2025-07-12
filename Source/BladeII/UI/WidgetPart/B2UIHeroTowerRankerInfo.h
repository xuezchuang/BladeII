// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "../../DataStore/B2ClientDataStore.h"
#include "B2UIHeroTowerRankerInfo.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIHeroTowerRankerInfo : public UB2UIWidgetBase
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	virtual void UpdateStaticText() override;
	void SetRankerInfo(const FModRankerInfo& RankerInfo);

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

public:
	UFUNCTION()
		void OnClickSelect();

private:
	TWeakObjectPtr<UTextBlock> TB_Ranking;
	TWeakObjectPtr<class UB2UIGradeRankAndStar> UIP_GradeIcon;
	TWeakObjectPtr<class UB2UIPVPEntryCharacters> UIP_EntryCharacters;
	TWeakObjectPtr<UButton> BTN_ShowPlayerInfo;

	TWeakObjectPtr<UTextBlock> TB_TotalAttackPoint;
	TWeakObjectPtr<UTextBlock> TB_TotalDefencePoint;
	TWeakObjectPtr<UTextBlock> TB_PlayerLevel;
	TWeakObjectPtr<UTextBlock> TB_PlayerName;
	TWeakObjectPtr<UTextBlock> TB_ClearTime;
	TWeakObjectPtr<UTextBlock> TB_ClearTower;

	TWeakObjectPtr<UTextBlock> STB_CombatPower;
	TWeakObjectPtr<UTextBlock> STB_HighScore;
	
	TWeakObjectPtr<UImage> IMG_RankNumber_1;
	TWeakObjectPtr<UImage> IMG_RankNumber_2;
	TWeakObjectPtr<UImage> IMG_RankNumber_3;
	TWeakObjectPtr<UImage> IMG_RankBG_1;
	TWeakObjectPtr<UImage> IMG_RankBG_2;
	TWeakObjectPtr<UImage> IMG_RankBG_3;

	TWeakObjectPtr<UB2Button> BTN_Select;
	
private:
	int64			AccountInfo;
	EPCClass		SubClass;
	EPCClass		MainClass;
	
};
