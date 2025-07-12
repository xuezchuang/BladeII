#pragma once
#include "UObject/Object.h"
#include "B2UIDocBase.h"
#include "../../DataStore/B2ClientDataStore.h"
#include "B2UIDocDimension.generated.h"

/* This is the place where you can put "SOME" fucking data that you have no idea of where to put shits in. */

struct FDimensionReward
{
	int32 ItemIndex;
	int32 Count;
};

UCLASS()
class BLADEII_API UB2UIDocDimension : public UB2UIDocBase
{
	GENERATED_BODY()

public:
	UB2UIDocDimension(const FObjectInitializer& ObjectInitializer);
	~UB2UIDocDimension();

	virtual void Init() override;
	virtual void Destroy() override;
	
	bool GetDimensionOpen();

	void SetMapID(int32 Diff);

	void SetDimensionPtr(FB2ResponseGetDimensionPtr ResponsePtr);
	int32 GetWeeklyBestRecordDiff();
	int32 GetWeeklyBestRecordTime();
	int32 GetMaxClearDiff();
	FModRankerInfo GetMyRank();
	int64 GetSettlement_Time();
	int32 GetSettlement_PeriodHour();
	int32 GetDailyTryCount();
	int32 GetClearCount();
	void SetClearCount(int32 count);

	int32 GetMaxOpenDiff();
	int32 GetMinOpenDiff();
	int32 GetLastClearDiff();

	void SetMyRank();
	void SetRankerList(const TArray<b2network::B2MatchRankerPtr>& RankerList, int32 BeginRank);
	FORCEINLINE const TArray<FModRankerInfo>* GetRankerList() { return &RankingList; }

	b2network::B2DimensionRankingRewardPtr GetWeeklyReward();
	void ClearWeeklyReward();

	void SetFinishDimensionPtr(FB2ResponseFinishDimensionPtr ResponsePtr);
	bool GetIsNewRecord();
	TArray<FDimensionReward> GetClearRewards();


	DECLARE_UIDOC_FIELD(int32, MapIndex)
	DECLARE_UIDOC_FIELD(int32, Difficulty)
	DECLARE_UIDOC_FIELD(int32, MapLoadID)
	DECLARE_UIDOC_FIELD(int32, ClientStageID)

	DECLARE_UIDOC_FIELD(int32, DimensionTicket)

	DECLARE_UIDOC_FIELD(bool, FirstEnterDimension)
private:

	void GenerateMapLoadID();
	void GenerateClientStageID();

public:
	FB2ResponseGetDimensionPtr					m_ptrGetDimension;
	FB2ResponseFinishDimensionPtr				m_ptrFinishDimension;

	int32										m_nCurrentRecordTimeMilliSecForResult;
	EDimensionResult 							m_bIsClear;
private:
	TArray<FModRankerInfo>						RankingList;
	FModRankerInfo								MyRank;
};
