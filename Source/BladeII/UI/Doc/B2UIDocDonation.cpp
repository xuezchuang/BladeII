// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIDocDonation.h"
#include "B2GameInstance.h"
#include "BladeIIUtil.h"
#include "BladeIIGameImpl.h"
#include "Global.h"

void UB2UIDocDonation::SetDonationEventData(const FB2GetDonationEventPtr& InData)
{
	DonationID = InData->donation_id;
	FreeDonationCount = InData->free_donation_count;
	DonationPoint = InData->point;
	MaxDonationPointStep = InData->whole_step;

	DonationPointRewardStates.Empty();
	for (const b2network::B2DonationPointRewardStatePtr& Elem : InData->point_reward_state)
	{
		DonationPointRewardStates.Add(Elem->index, Elem->rewarded);
	}

	Settled = InData->settled;
	Rankers.Empty();
	for (int i = 0; i < InData->rankers.Num(); i++)
	{//배열 순서대로 순위
		Rankers.Add(FRankerData(InData->rankers[i]->account_id ,i + 1, InData->rankers[i]->nickname, InData->rankers[i]->point));
	}

	FromRanking = InData->from_ranking;
	ScrollTotalRankerCount = InData->total_ranker_count;
	MyRanking = InData->ranking->ranking;
	TotalRankerCount = InData->ranking->total_ranker_count;
}

void UB2UIDocDonation::SetDonationEventBaseData(const TArray<b2network::B2mdDonationEventInfoPtr>& InData)
{
	DonationEventInfos.Empty();
	for (const b2network::B2mdDonationEventInfoPtr& Elem : InData)
	{
		DonationEventInfos.Add(FDonationEventInfo(Elem));
	}
	
	SetCurrentDonationEvent();
}

void UB2UIDocDonation::SetMakeDonationData(const FB2MakeDonationPtr& InData)
{
	IsFreeDonation = InData->is_free_donation;
	FreeDonationCount = InData->free_donation_count;
	DonationPresents.Empty();
	for (const b2network::B2DonationPresentPtr& Elem : InData->donation_presents)
	{
		DonationPresents.Add(FDonationPresent(Elem->item_template_id, Elem->amount, Elem->donation_consumable_type));
	}
	DecreaseGem = InData->decrease_gem;
	if (DecreaseGem != 0)
	{
		BladeIIGameImpl::GetClientDataStore().ReplaceUserDataWithDoc(EDocUserDataType::Gem, InData->current_gem);
	}
	
	DecreaseGold = InData->decrease_gold;
	if (DecreaseGold != 0)
	{
		BladeIIGameImpl::GetClientDataStore().ReplaceUserDataWithDoc(EDocUserDataType::Gold, InData->current_gold);
	}
	
	DonationPoint = InData->current_point;
	LastDonationKey = InData->last_donation_key;
	BundleFixedReward = FBundleFixedReward(InData->bundled_fixed_reward->id, InData->bundled_fixed_reward->count);
	
	DonationRewardItem.Empty();
	TU::CopyVectorHelper<FB2Item, b2network::B2ItemServerInfoPtr> SvrToCliItemConvertHelper(&DonationRewardItem);
	SvrToCliItemConvertHelper = InData->items;

	BladeIIGameImpl::GetClientDataStore().AddNewAcquiredItems(DonationRewardItem);
	PresentGem = InData->present_gem;
	PresentBladePoint = InData->present_blade_point;
}

void UB2UIDocDonation::SetDonationPointRewardData(const FB2DonationPointRewardPtr& InData)
{
	for (const b2network::B2DonationPointRewardStatePtr& Elem : InData->point_reward_state)
	{
		if (DonationPointRewardStates.Contains(Elem->index))
		{
			DonationPointRewardStates[Elem->index] = Elem->rewarded;
		}
		else
		{
			DonationPointRewardStates.Add(Elem->index, Elem->rewarded);
		} 
	}
}

void UB2UIDocDonation::GetDonationRanker(OUT TArray<FRankerData>& OutRankers)
{
	for (int i = 0; i < Rankers.Num(); i++)
	{
		OutRankers.Add(Rankers[i]);
	}
}

int32 UB2UIDocDonation::GetMyRanking()
{
	return MyRanking;
}

int32 UB2UIDocDonation::GetDonationPoint()
{
	return DonationPoint;
}

FDateTime UB2UIDocDonation::GetEndTime()
{
	return CurrentDonationEventInfo.EventEndTime;
}

FDateTime UB2UIDocDonation::GetStartTime()
{
	return CurrentDonationEventInfo.EventStartTime;
}

FDateTime UB2UIDocDonation::GetExpireTime()
{
	return CurrentDonationEventInfo.EventExpireTime;
}

int32 UB2UIDocDonation::GetMinimumDonationPoint()
{
	return CurrentDonationEventInfo.MinimumDonationPoint;
}

int32 UB2UIDocDonation::GetFreeDonationCount()
{
	return FreeDonationCount;
}

int32 UB2UIDocDonation::GetOneCost()
{
	return CurrentDonationEventInfo.OneCost;
}

int32 UB2UIDocDonation::GetTenCost()
{
	return CurrentDonationEventInfo.TenCost;
}

int32 UB2UIDocDonation::GetOneMoreOneCost()
{
	return CurrentDonationEventInfo.OneMoreOneCost;
}

int32 UB2UIDocDonation::GetOneMoreTenCost()
{
	return CurrentDonationEventInfo.OneMoreTenCost;
}

int32 UB2UIDocDonation::GetDonationID()
{
	return DonationID;
}

int32 UB2UIDocDonation::GetTemplateID()
{
	return TemplateID;
}

void UB2UIDocDonation::GetDonationRewardItem(OUT TArray<FB2Item>& OutRankers)
{
	for (int i = 0; i < DonationRewardItem.Num(); i++)
	{
		OutRankers.Add(DonationRewardItem[i]);
	}
}

void UB2UIDocDonation::SetIsResultPopup(bool InValue)
{
	IsResultPopup = InValue;
}

bool UB2UIDocDonation::GetIsResultPopup()
{
	return IsResultPopup;
}

FString UB2UIDocDonation::GetLastDonationKey()
{
	return LastDonationKey;
}

bool UB2UIDocDonation::GetDonationPointStepIsReward(int32 InStep)
{
	bool Result = false;
	if (DonationPointRewardStates.Contains(InStep))
	{
		Result = DonationPointRewardStates[InStep];
	}
	return Result;
}

int32 UB2UIDocDonation::GetLobbyUIDonationID()
{
	TArray<FDonationTimeGap> TimeGaps;
	FDateTime DateNow = UB2GameInstance::GetLocalNow();

	for (const FDonationEventInfo& Elem : DonationEventInfos)
	{
		FDateTime StartTime = Elem.EventStartTime;
		FDateTime ExpireTime = Elem.EventExpireTime;

		FDonationTimeGap TimeGap;

		if (DateNow > StartTime && DateNow < ExpireTime)
		{
			TimeGap.DonationID = Elem.DonationID;
			TimeGap.Gap = 0;
			TimeGaps.Add(TimeGap);
		}
		else if (DateNow < ExpireTime && DateNow < StartTime)
		{
			TimeGap.DonationID = Elem.DonationID;
			TimeGap.Gap = StartTime - DateNow;
			TimeGaps.Add(TimeGap);
		}
	}

	TimeGaps.Sort([](const FDonationTimeGap& A, const FDonationTimeGap& B) {return A.Gap < B.Gap; });

	int32 Result = -1;
	if (TimeGaps.IsValidIndex(0))
	{
		Result = TimeGaps[0].DonationID;
	}

	return Result;
}

EDonationPointStepState UB2UIDocDonation::GetDonationPointStepState(int32 InStep)
{
	EDonationPointStepState Result = EDonationPointStepState::Completion;

	if (GetDonationPointStepIsReward(InStep))
	{
		Result = EDonationPointStepState::Completion;
	}
	else
	{
		Result = EDonationPointStepState::Incompletion;

		FDonationPointStepRewardInfo StepInfo = GetDonationPointStepRewardInfo(InStep);
		if (DonationPoint >= StepInfo.RewardPoint)
		{
			Result = EDonationPointStepState::Reward;
		}
	}

	return Result;
}

int32 UB2UIDocDonation::GetDonationPointStepRewardPoint(int32 InStep)
{
	FDonationPointStepRewardInfo RewardInfo = GetDonationPointStepRewardInfo(InStep);
	return RewardInfo.RewardPoint;
}

FDonationPointStepRewardInfo UB2UIDocDonation::GetDonationPointStepRewardInfo(int32 InStep)
{
	FDonationPointStepRewardInfo Result;

	for (FDonationPointStepRewardInfo Elem : DonationPointStepRewardInfos)
	{
		if (Elem.TemplateID == GetTemplateID() && Elem.Step == InStep)
		{
			Result = Elem;
		}
	}

	return Result;
}

void UB2UIDocDonation::GetDonationPointRankingRewardInfos(OUT TArray<FDonationPointRankingRewardInfo>& OutRewardInfo)
{
	for (int i =0; i<DonationPointRankingRewardInfos.Num(); i++)
	{
		if (DonationPointRankingRewardInfos[i].TemplateID == GetTemplateID())
		{
			OutRewardInfo.Add(DonationPointRankingRewardInfos[i]);
		}
	}
}

FDonationPointRankingRewardInfo UB2UIDocDonation::GetDonationPointRankingRewardInfo(int32 InRanking)
{
	for (int i = 0; i < DonationPointRankingRewardInfos.Num(); i++)
	{
		if (DonationPointRankingRewardInfos[i].Rank == InRanking && DonationPointRankingRewardInfos[i].TemplateID == GetTemplateID())
		{
			return DonationPointRankingRewardInfos[i];
		}
	}

	return FDonationPointRankingRewardInfo();
}

void UB2UIDocDonation::GetDonationPresents(OUT TArray<FDonationPresent>& OutPresent)
{
	for (int i = 0; i < DonationPresents.Num(); i++)
	{
		OutPresent.Add(DonationPresents[i]);
	}
}

void UB2UIDocDonation::GetDonationBundleFixedRewardInfos(OUT TArray<FDonationBundleFixedRewardInfo>& OutFixedRewardInfo)
{
	for (int i = 0; i < DonationBundleFixedRewardInfos.Num(); i++)
	{
		if (DonationBundleFixedRewardInfos[i].TemplateID == GetTemplateID())
		{
			OutFixedRewardInfo.Add(DonationBundleFixedRewardInfos[i]);
		}
	}
}

FBundleFixedReward UB2UIDocDonation::GetBundleFixedReward()
{
	return BundleFixedReward;
}

int32 UB2UIDocDonation::GetCostType()
{
	return CurrentDonationEventInfo.CostType;
}

EDonationEventState UB2UIDocDonation::GetDonationEventState()
{
	FDateTime DateNow = UB2GameInstance::GetLocalNow();
	EDonationEventState DonationState = EDonationEventState::Expiration;

	if (DateNow > GetStartTime() && DateNow < GetEndTime())
	{
		//이 유저는 이벤트에 참여 했다
		if (GConfig)
		{
			GConfig->SetBool(TEXT("DonationEvent"), TEXT("Participation"), true, GB2UserSavedStateIni);
			GConfig->Flush(false, GB2UserSavedStateIni);
		}

		DonationState = EDonationEventState::Ongoing;
	}
	else if (DateNow > GetEndTime() && DateNow < GetExpireTime())
	{
		DonationState = EDonationEventState::NotOngoing;
	}
	else if (SetCurrentDonationEvent())
	{
		DonationState = EDonationEventState::Expiration;
	}
	else
	{
		DonationState = EDonationEventState::None;
	}

	return DonationState;
}

bool UB2UIDocDonation::GetSettled()
{
	return Settled;
}
 
int32 UB2UIDocDonation::GetScrollTotalRankerCount()
{
	return ScrollTotalRankerCount;
}

bool UB2UIDocDonation::SetCurrentDonationEvent()
{
	bool Result = false;
	int32 LobbyUIDonationID = GetLobbyUIDonationID();
	TemplateID = -1;

	for (const FDonationEventInfo& Elem : DonationEventInfos)
	{
		if (Elem.DonationID == LobbyUIDonationID)
		{
			if (CurrentDonationEventInfo.DonationID != Elem.DonationID)
			{
				CurrentDonationEventInfo = FDonationEventInfo(Elem);
			}

			TemplateID = CurrentDonationEventInfo.TemplateID;

			Result = true;
			break;
		}
	}

	return Result;
}

void UB2UIDocDonation::SetMDDonationPointRankingRewardInfo(const TArray<b2network::B2mdDonationPointRankingRewardInfoPtr>& InData)
{
	DonationPointRankingRewardInfos.Empty();
	for (const b2network::B2mdDonationPointRankingRewardInfoPtr Elem : InData)
	{
		DonationPointRankingRewardInfos.Add(FDonationPointRankingRewardInfo(Elem));
	}
}

void UB2UIDocDonation::SetMDDonationBundleFixedRewardInfo(const TArray<b2network::B2mdDonationBundleFixedRewardInfoPtr>& InData)
{
	DonationBundleFixedRewardInfos.Empty();
	for (const b2network::B2mdDonationBundleFixedRewardInfoPtr& Elem : InData)
	{
		DonationBundleFixedRewardInfos.Add(FDonationBundleFixedRewardInfo(Elem));
	}
}

void UB2UIDocDonation::SetMDDonationPointStepRewardInfo(const TArray<b2network::B2mdDonationPointStepRewardInfoPtr>& InData)
{
	DonationPointStepRewardInfos.Empty();
	for (const b2network::B2mdDonationPointStepRewardInfoPtr& Elem : InData)
	{
		DonationPointStepRewardInfos.Add(FDonationPointStepRewardInfo(Elem));
	}
}

FDonationEventInfo::FDonationEventInfo(const b2network::B2mdDonationEventInfoPtr& InData)
{
	DonationID = InData->donation_id;
	CostType = InData->cost_type;
	OneCost = InData->cost;
	OneMoreOneCost = InData->extra_cost;
	MinimumDonationPoint = InData->cut_off_point;
	ObtainPoint = InData->obtain_point;
	TenCost = InData->bundle_cost;
	OneMoreTenCost = InData->extra_bundle_cost;
	OneMoreTenObtainPoint = InData->bundle_obtain_point;
	SerialDonationCount = InData->serial_donation_count;
	EventStartTime = UB2GameInstance::GetUtcToLocalTime(GetServerTimeToDateTime(InData->start_time));;
	EventEndTime = UB2GameInstance::GetUtcToLocalTime(GetServerTimeToDateTime(InData->end_time));
	EventExpireTime = UB2GameInstance::GetUtcToLocalTime(GetServerTimeToDateTime(InData->expire_time));;
	EventTitle = InData->title;
	EventDescription = InData->description;
	BannerURL = InData->banner_url;
	LinkText = InData->link_text;
	LinkURL = InData->link_url;
	TemplateID = InData->donation_template_id;
}

FDonationPointRankingRewardInfo::FDonationPointRankingRewardInfo(const b2network::B2mdDonationPointRankingRewardInfoPtr& InData)
{
	TemplateID = InData->donation_id;
	Rank = InData->rank;
	RewardID1 = InData->reward_id1;
	RewardCount1 = InData->reward_count1;
	RewardID2 = InData->reward_id2;
	RewardCount2 = InData->reward_count2;
	RewardID3 = InData->reward_id3;
	RewardCount3 = InData->reward_count3;
	RewardID4 = InData->reward_id4;
	RewardCount4 = InData->reward_count4;
	RewardID5 = InData->reward_id5;
	RewardCount5 = InData->reward_count5;
}

FDonationPointStepRewardInfo::FDonationPointStepRewardInfo(const b2network::B2mdDonationPointStepRewardInfoPtr& InData)
{
	TemplateID = InData->donation_id;
	Step = InData->step;
	RewardPoint = InData->point;
	RewardID1 = InData->reward_id1;
	RewardCount1 = InData->reward_count1;
	RewardID2 = InData->reward_id2;
	RewardCount2 = InData->reward_count2;
	RewardID3 = InData->reward_id3;
	RewardCount3 = InData->reward_count3;
}
