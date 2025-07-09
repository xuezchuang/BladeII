// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Doc/B2UIDocBase.h"
#include "B2UIDocDonation.generated.h"

struct FRankerData
{
	FRankerData(int64 InAccountID, int32 InRanking, FString InNickName, int32 InDonationPoint)
		: AccountID(InAccountID), Ranking(InRanking), NickName(InNickName), DonationPoint(InDonationPoint)
	{
	}

	explicit FRankerData()
		: AccountID(-1), Ranking(-1), NickName(FString()), DonationPoint(-1)
	{
	}

	int64 AccountID;
	int32 Ranking;
	FString NickName;
	int32 DonationPoint;
};

struct FDonationPresent
{
	FDonationPresent(int32 InItemTemplateID, int32 InAmount, int32 InDonationConsumableType)
		: ItemTemplateID(InItemTemplateID), Amount(InAmount), DonationConsumableType(InDonationConsumableType)
	{
	}

	explicit FDonationPresent()
		: ItemTemplateID(-1), Amount(-1), DonationConsumableType(-1)
	{
	}

	int32 ItemTemplateID;
	int32 Amount;
	int32 DonationConsumableType;
};

struct FBundleFixedReward
{
	FBundleFixedReward(int32 InID, int32 InAmount)
		: ID(InID), Amount(InAmount)
	{
	}

	explicit FBundleFixedReward()
		: ID(-1), Amount(-1)
	{
	}
	int32 ID;
	int32 Amount;
};

struct FDonationPointStepRewardInfo
{
	FDonationPointStepRewardInfo(const b2network::B2mdDonationPointStepRewardInfoPtr& InData);

	explicit FDonationPointStepRewardInfo()
		: TemplateID(-1), Step(-1), RewardPoint(-1), RewardID1(-1), RewardCount1(-1), RewardID2(-1), RewardCount2(-1), RewardID3(-1), RewardCount3(-1)
	{
	}

	int32 TemplateID; // donationid가 아님
	int32 Step;
	int32 RewardPoint;
	int32 RewardID1;
	int32 RewardCount1;
	int32 RewardID2;
	int32 RewardCount2;
	int32 RewardID3;
	int32 RewardCount3;
};

struct FDonationPointRankingRewardInfo
{
	FDonationPointRankingRewardInfo(const b2network::B2mdDonationPointRankingRewardInfoPtr& InData);

	explicit FDonationPointRankingRewardInfo()
		: TemplateID(-1), Rank(-1), RewardID1(-1), RewardCount1(-1), RewardID2(-1), RewardCount2(-1), RewardID3(-1), RewardCount3(-1), RewardID4(-1), RewardCount4(-1), RewardID5(-1), RewardCount5(-1)
	{
	}

	int32 TemplateID; // donationid가 아님
	int32 Rank;
	int32 RewardID1;
	int32 RewardCount1;
	int32 RewardID2;
	int32 RewardCount2;
	int32 RewardID3;
	int32 RewardCount3;
	int32 RewardID4;
	int32 RewardCount4;
	int32 RewardID5;
	int32 RewardCount5;
};

struct FDonationBundleFixedRewardInfo
{
	FDonationBundleFixedRewardInfo(const b2network::B2mdDonationBundleFixedRewardInfoPtr& InData)
	{
		TemplateID = InData->donation_id;
		RewardID = InData->reward_id;
		MinAmount = InData->min_amount;
		MaxAmount = InData->max_amount;
	}

	explicit FDonationBundleFixedRewardInfo()
		: TemplateID(-1), RewardID(-1), MinAmount(-1), MaxAmount(-1)
	{
	}

	int32 TemplateID; // donationid가 아님
	int32 RewardID;
	int32 MinAmount;
	int32 MaxAmount;
};

struct FDonationEventInfo
{
	FDonationEventInfo(const b2network::B2mdDonationEventInfoPtr& InData);


	explicit FDonationEventInfo()
		: DonationID(-1),CostType(-1),  OneCost(-1), OneMoreOneCost(-1), MinimumDonationPoint(-1), ObtainPoint(-1), TenCost(-1), OneMoreTenCost(-1), OneMoreTenObtainPoint(-1), SerialDonationCount(-1),
		EventStartTime(FDateTime(0)), EventEndTime(FDateTime(0)), EventExpireTime(FDateTime(0)), EventTitle(FString(TEXT(""))), EventDescription(FString(TEXT(""))), BannerURL(FString(TEXT(""))),
		LinkText(FString(TEXT(""))), LinkURL(FString(TEXT(""))), TemplateID(-1)
	{
	}

	int32 DonationID;
	int32 CostType;
	int32 OneCost;
	int32 OneMoreOneCost;
	int32 MinimumDonationPoint;
	int32 ObtainPoint;
	int32 TenCost;
	int32 OneMoreTenCost;
	int32 OneMoreTenObtainPoint;
	int32 SerialDonationCount;
	FDateTime EventStartTime;
	FDateTime EventEndTime;
	FDateTime EventExpireTime;
	FString EventTitle;
	FString EventDescription;
	FString BannerURL;
	FString LinkText;
	FString LinkURL;
	int32 TemplateID;
};

enum class EDonationPointStepState : uint8
{
	Reward = 0,			//보상 있음
	Completion = 1,		// 완료
	Incompletion = 2			//  미완료
};

enum class EDonationEventState : uint8
{
	Ongoing = 0,			//진행중
	NotOngoing = 1,		//진행 ~ 만료 사이
	Expiration = 2,			// 만료 ~ 다음이벤트 사이
	None = 3				// 만료 ~ 아무것도 없음
};

/**
*
*/
UCLASS()
class BLADEII_API UB2UIDocDonation : public UB2UIDocBase
{
	GENERATED_BODY()

public:
	void SetDonationEventData(const FB2GetDonationEventPtr& InData);
	void SetDonationEventBaseData(const TArray<b2network::B2mdDonationEventInfoPtr>& InData);
	void SetMakeDonationData(const FB2MakeDonationPtr& InData);
	void SetDonationPointRewardData(const FB2DonationPointRewardPtr& InData);
	void GetDonationRanker(OUT TArray<FRankerData>& OutRankers);
	int32 GetMyRanking();
	int32 GetDonationPoint();
	FDateTime GetEndTime();
	FDateTime GetStartTime();
	FDateTime GetExpireTime();
	int32 GetMinimumDonationPoint();
	int32 GetFreeDonationCount();
	int32 GetOneCost();
	int32 GetTenCost();
	int32 GetOneMoreOneCost();
	int32 GetOneMoreTenCost();
	int32 GetDonationID();
	int32 GetTemplateID();

	void GetDonationRewardItem(OUT TArray<FB2Item>& OutRankers);
	void SetIsResultPopup(bool InValue);
	bool GetIsResultPopup();
	FString GetLastDonationKey();
	EDonationPointStepState GetDonationPointStepState(int32 InStep);
	int32 GetDonationPointStepRewardPoint(int32 InStep);
	FDonationPointStepRewardInfo GetDonationPointStepRewardInfo(int32 InStep);
	void GetDonationPointRankingRewardInfos(OUT TArray<FDonationPointRankingRewardInfo>& OutRewardInfo);
	FDonationPointRankingRewardInfo GetDonationPointRankingRewardInfo(int32 InRanking);
	void GetDonationPresents(OUT TArray<FDonationPresent>& OutPresent);
	void GetDonationBundleFixedRewardInfos(OUT TArray<FDonationBundleFixedRewardInfo>& OutFixedRewardInfo);
	FBundleFixedReward GetBundleFixedReward();
	int32 GetCostType();
	EDonationEventState GetDonationEventState();
	bool GetSettled();
	int32 GetScrollTotalRankerCount();
	bool SetCurrentDonationEvent();

	void SetMDDonationPointRankingRewardInfo(const TArray<b2network::B2mdDonationPointRankingRewardInfoPtr>& InData);
	void SetMDDonationBundleFixedRewardInfo(const TArray<b2network::B2mdDonationBundleFixedRewardInfoPtr>& InData);
	void SetMDDonationPointStepRewardInfo(const TArray<b2network::B2mdDonationPointStepRewardInfoPtr>& InData);

private:
	bool GetDonationPointStepIsReward(int32 InStep);
	int32 GetLobbyUIDonationID();

private:
	bool IsResultPopup;

	int32 DonationID;
	int32 TemplateID;
	int32 FreeDonationCount;
	int32 DonationPoint;
	int32 MaxDonationPointStep;
	TMap<int32, bool> DonationPointRewardStates;
	bool Settled;
	TArray<FRankerData> Rankers;
	int32 FromRanking;
	int32 ScrollTotalRankerCount;
	int32 MyRanking = 0;
	int32 TotalRankerCount;

	TArray<FDonationEventInfo> DonationEventInfos;
	FDonationEventInfo CurrentDonationEventInfo;

	bool IsFreeDonation;
	TArray<FDonationPresent> DonationPresents;
	int32 DecreaseGem;
	int32 DecreaseGold;
	FString LastDonationKey;
	FBundleFixedReward BundleFixedReward;
	TArray<FB2Item> DonationRewardItem;
	int32 PresentGem;
	int32 PresentBladePoint;

	TArray<FDonationPointStepRewardInfo> DonationPointStepRewardInfos;
	TArray<FDonationPointRankingRewardInfo> DonationPointRankingRewardInfos;
	TArray<FDonationBundleFixedRewardInfo> DonationBundleFixedRewardInfos;

	struct FDonationTimeGap
	{
		explicit FDonationTimeGap()
			: DonationID(-1), Gap(-1) {}
		int32 DonationID;
		FTimespan Gap;
	};
};
