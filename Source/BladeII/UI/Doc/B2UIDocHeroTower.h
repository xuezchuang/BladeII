// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "B2UIDocBase.h"
#include "CommonStruct.h"
#include "B2ClientDataStore.h"
#include "B2UIDocHeroTower.generated.h"

/**
 * 
 */

UCLASS()
class BLADEII_API UB2UIDocHeroTower : public UB2UIDocBase
{
	GENERATED_BODY()

public:
	UB2UIDocHeroTower(const FObjectInitializer& ObjectInitializer);
	~UB2UIDocHeroTower();
	
	virtual void Init() override;
	virtual void Destroy() override;

	int32 GetDailyTryCount();

	// GetBestRecordFloor() function need replace to GetStartFloor()
	FORCEINLINE int32 GetBestRecordFloor() { return 0; }

	//FORCEINLINE int32 GetBestRecordTime() { return 0; }
	//FORCEINLINE int32 GetFloorRecordTime(int32 nFloor) { return 0; }
	int32 GetStartFloor();
	int32 GetMaxClearFloor();
	int32 GetPossibleTryCount();
	
	// return clear_time_sec
	//FORCEINLINE int32 GetClearHistory(int32 nFloor) { return 0; }

	int32 GetNextFloorFromFinishInfo();
	FString GetPlayTokenFromFinishInfo();

	void SetRankerList(const TArray<b2network::B2MatchRankerPtr>& RankerList, int32 BeginRank);
	FORCEINLINE const TArray<FModRankerInfo>* GetRankerList() { return &RankingList; }

	FORCEINLINE bool IsClearHeroTower(){ return m_bIsClearHeroTower; }

	void SetGetHeroTowerPtr(FB2ResponseGetHeroTowerPtr ResponsePtr);
	void SetFinishHeroTowerPtr(FB2ResponseFinishHeroTowerPtr ResponsePtr);

	bool CanSweepToday();
	void ForcedSetSweepToday(bool bAlreadySweep);

	DECLARE_UIDOC_FIELD(EPCClass, PCClass);
	DECLARE_UIDOC_FIELD(EPCClass, Relic_SelectedPCClass);
	DECLARE_UIDOC_FIELD(FAncientRelic, Relic_SelectedRelicInfo);

	DECLARE_UIDOC_FIELD(int32, DailyLeftFreeTryCount);

public:
	FB2ResponseGetHeroTowerPtr m_ptrGetHeroTower;
	FB2ResponseFinishHeroTowerPtr m_ptrFinishHeroTower;

	int32 m_nCurrentFloorRecordTimeMilliSecForResult;

	bool m_bIsClearHeroTower;
private:
	TArray<FModRankerInfo> RankingList;
};
