// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UIControlRanking.h"
#include "B2UIManager.h"
#include "B2UIRankerInfo.h"
#include "Retailer.h"
#include "B2UIDocHelper.h"
#include "B2UIDocControl.h"



void UB2UIControlRanking::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2Button, BTN_Close);
	GET_SLOT(UOverlay, OV_AllRanking);
	GET_SLOT(UOverlay, OV_AllRanking2);
	GET_SLOT(UOverlay, OV_FriendRanking);
	GET_SLOT(UOverlay, OV_FriendRanking2);

	GET_SLOT(UB2Button, BTN_AllRank);
	GET_SLOT(UB2Button, BTN_FriendRank);

	GET_SLOT(UCanvasPanel, CP_Ranking);

	GET_SLOT(UVerticalBox, VB_RankingList);

	GET_SLOT(UTextBlock, STB_Title);
	GET_SLOT(UTextBlock, STB_AllRank);
	GET_SLOT(UTextBlock, STB_AllRank2);
	GET_SLOT(UTextBlock, STB_FriendRank);
	GET_SLOT(UTextBlock, STB_FriendRank2);

	if (VB_RankingList.IsValid())
		VB_RankingList->ClearChildren();
}

void UB2UIControlRanking::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIControlRanking::OnClickBtnClose);
	BIND_CLICK_FUNC_TO_BTN(BTN_AllRank, &UB2UIControlRanking::OnClick_AllRanking);
	BIND_CLICK_FUNC_TO_BTN(BTN_FriendRank, &UB2UIControlRanking::OnClick_FriendRanking);
}

void UB2UIControlRanking::CloseWidgetDelegate()
{
	OnClickBtnClose();
}

void UB2UIControlRanking::OnClickBtnClose()
{
	UB2UIManager::GetInstance()->CloseUI(GetMyUIFName());
}

void UB2UIControlRanking::Init()
{
	Super::Init();
}

void UB2UIControlRanking::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIControlRanking::SetFriendList(const FB2ResponseGetFriendRankingsPtr& FriendRank)
{
	if (bShowingAllRanker) return;

	TArray<FModRankerInfo>& ToSet = FriendRankingList;
	
	ToSet.Empty();
	int32 BeginRank = 0;
	for (auto RankerSvrInfo : FriendRank->rankers)
	{
		FModRankerInfo RankerCliInfo;
		RankerCliInfo.UserName = RankerSvrInfo->nickname;
		RankerCliInfo.Grade = RankerSvrInfo->battle_grade;
		RankerCliInfo.GradeStar = RankerSvrInfo->battle_star_count;
		RankerCliInfo.Rank = ++BeginRank;
		RankerCliInfo.TotalCombatPower = RankerSvrInfo->total_power;			// jwyim power

		for (auto EntryInfo : RankerSvrInfo->entries)
		{
			RankerCliInfo.Entry.Add(SvrToCliPCClassType(EntryInfo.character_type));
			RankerCliInfo.EntryLevels.Add(EntryInfo.level);
		}

		ToSet.Add(RankerCliInfo);
	}

	UpdateList();
}

void UB2UIControlRanking::UpdateList()
{
	if (VB_RankingList.IsValid())
	{
		VB_RankingList->ClearChildren();
		const TArray<FModRankerInfo>* ModRankerList;

		if (bShowingAllRanker)
		{
			auto* ControlDoc = UB2UIDocHelper::GetDocControl();

			if (!ControlDoc) return;

			ModRankerList = ControlDoc->GetRankerList();

			if (!ModRankerList)
				return;

		}
		else
		{
			ModRankerList = &FriendRankingList;
			if (!ModRankerList)
				return;
		}

		for (auto Ranker : *ModRankerList)
		{
			auto* CreatedWidget = CreateWidget<UB2UIRankerInfo>(GetOwningPlayer(), RankerInfoClass);
			if (CreatedWidget != NULL)
			{
				auto* AddedSlot = Cast<UVerticalBoxSlot>(VB_RankingList->AddChild(CreatedWidget));
				check(AddedSlot);
				RankerList.Add(CreatedWidget);
				CreatedWidget->Init();
				CreatedWidget->SetRankerInfo(Ranker);
			}
		}
	}
}

void UB2UIControlRanking::OnClose(bool RightNow /*= false*/)
{
	Super::OnClose();

	DeliveryGetFriendRankingClass<FB2ResponseGetFriendRankingsPtr>::GetInstance().Unsubscribe(DeliverytGetFriendRankingListTicket);
}

void UB2UIControlRanking::UpdateStaticText()
{
	if (STB_Title.IsValid()) // 명예의 전당
		STB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Control_RankingTitle")));
	if (STB_AllRank.IsValid()) //전체 랭킹
		STB_AllRank->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PVP1on1_TotalRanking")));
	if (STB_AllRank2.IsValid())
		STB_AllRank2->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PVP1on1_TotalRanking")));
	if (STB_FriendRank.IsValid()) // 친구 랭킹
		STB_FriendRank->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PVP1on1_FriendRanking")));
	if (STB_FriendRank2.IsValid())
		STB_FriendRank2->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PVP1on1_FriendRanking")));
}

void UB2UIControlRanking::OnOpen(bool RightNow /*= false*/)
{
	Super::OnOpen(RightNow);

	OnClick_AllRanking();

	CAPTURE_UOBJECT(UB2UIControlRanking);
	DeliverytGetFriendRankingListTicket = DeliveryGetFriendRankingClass<FB2ResponseGetFriendRankingsPtr>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliverytGetFriendRankingList, const FB2ResponseGetFriendRankingsPtr& msgPtr)
		Capture->SetFriendList(msgPtr);
	END_CAPTURE_OBJECT()
		);
}

void UB2UIControlRanking::OnClick_AllRanking()
{
	if (OV_AllRanking.IsValid())
		OV_AllRanking->SetVisibility(ESlateVisibility::Hidden);
	if (OV_AllRanking2.IsValid())
		OV_AllRanking2->SetVisibility(ESlateVisibility::Visible);
	if (OV_FriendRanking.IsValid())
		OV_FriendRanking->SetVisibility(ESlateVisibility::Visible);
	if (OV_FriendRanking2.IsValid())
		OV_FriendRanking2->SetVisibility(ESlateVisibility::Hidden);

	if (VB_RankingList.IsValid())
		VB_RankingList->ClearChildren();

	bShowingAllRanker = true;

	UpdateList();
}

void UB2UIControlRanking::OnClick_FriendRanking()
{
	if (OV_AllRanking.IsValid())
		OV_AllRanking->SetVisibility(ESlateVisibility::Visible);
	if (OV_AllRanking2.IsValid())
		OV_AllRanking2->SetVisibility(ESlateVisibility::Hidden);
	if (OV_FriendRanking.IsValid())
		OV_FriendRanking->SetVisibility(ESlateVisibility::Hidden);
	if (OV_FriendRanking2.IsValid())
		OV_FriendRanking2->SetVisibility(ESlateVisibility::Visible);

	if (VB_RankingList.IsValid())
		VB_RankingList->ClearChildren();

	bShowingAllRanker = false;

	data_trader::Retailer::GetInstance().RequestGetFriendRankings(b2network::B2MatchInfoType::BMIT_ASSAULT);
}
