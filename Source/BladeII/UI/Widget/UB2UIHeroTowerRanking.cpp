// Fill out your copyright notice in the Description page of Project Settings.

#include "UB2UIHeroTowerRanking.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"
#include "B2UIHeroTowerRankerInfo.h"
#include "B2UIDocHeroTower.h"
#include "Retailer.h"
#include "BladeIIGameImpl.h"

void UUB2UIHeroTowerRanking::CacheAssets()
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

	GET_SLOT(UTextBlock, TB_HeroRanking);

	if (VB_RankingList.IsValid())
		VB_RankingList->ClearChildren();
}

void UUB2UIHeroTowerRanking::Init()
{
	Super::Init();
}

void UUB2UIHeroTowerRanking::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UUB2UIHeroTowerRanking::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UUB2UIHeroTowerRanking::OnClickBtnClose);
	BIND_CLICK_FUNC_TO_BTN(BTN_AllRank, &UUB2UIHeroTowerRanking::OnClick_AllRanking);
	BIND_CLICK_FUNC_TO_BTN(BTN_FriendRank, &UUB2UIHeroTowerRanking::OnClick_FriendRanking);
}

void UUB2UIHeroTowerRanking::OnOpen(bool RightNow /*= false*/)
{
	Super::OnOpen(RightNow);
	
	CAPTURE_UOBJECT(UUB2UIHeroTowerRanking);

	DeliverytGetFriendRankingListTicket = DeliveryGetFriendRankingClass<FB2ResponseGetFriendRankingsPtr>::GetInstance().Subscribe(
	 USE_CAPTURE_OBJECT_AND_TICKET(DeliverytGetFriendRankingList, const FB2ResponseGetFriendRankingsPtr& msgPtr)
		Capture->SetFriendList(msgPtr);
	END_CAPTURE_OBJECT()
	);

	OnClick_AllRanking();
}

void UUB2UIHeroTowerRanking::OnClose(bool RightNow /*= false*/)
{
	Super::OnClose();

	DeliveryGetFriendRankingClass<FB2ResponseGetFriendRankingsPtr>::GetInstance().Unsubscribe(DeliverytGetFriendRankingListTicket);
}

void UUB2UIHeroTowerRanking::OnClick_AllRanking()
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

void UUB2UIHeroTowerRanking::OnClick_FriendRanking()
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

	data_trader::Retailer::GetInstance().RequestGetFriendRankings(b2network::B2MatchInfoType::BMIT_HEROTOWER);
}

void UUB2UIHeroTowerRanking::CloseWidgetDelegate()
{
	OnClickBtnClose();
}

void UUB2UIHeroTowerRanking::OnClickBtnClose()
{
	UB2UIManager::GetInstance()->CloseUI(GetMyUIFName());
}

void UUB2UIHeroTowerRanking::SetFriendList(const FB2ResponseGetFriendRankingsPtr& FriendRank)
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
		RankerCliInfo.ClearTimeSec = RankerSvrInfo->clear_time;
		RankerCliInfo.BestFloor = RankerSvrInfo->best_floor;
		RankerCliInfo.AccountID = RankerSvrInfo->account_id;

		for (auto EntryInfo : RankerSvrInfo->entries)
		{
			RankerCliInfo.Entry.Add(SvrToCliPCClassType(EntryInfo.character_type));
			RankerCliInfo.EntryLevels.Add(EntryInfo.level);
		}

		ToSet.Add(RankerCliInfo);
	}

	UpdateList();
}

void UUB2UIHeroTowerRanking::UpdateList()
{
	if (VB_RankingList.IsValid())
	{
		VB_RankingList->ClearChildren();
		const TArray<FModRankerInfo>* ModRankerList;

		if (bShowingAllRanker)
		{
			auto* HeroTowerDoc = UB2UIDocHelper::GetDocHeroTower();

			if (!HeroTowerDoc) return;
			
			ModRankerList = HeroTowerDoc->GetRankerList();

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
			auto* CreatedWidget = CreateWidget<UB2UIHeroTowerRankerInfo>(GetOwningPlayer(), RankerInfoClass);
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

void UUB2UIHeroTowerRanking::UpdateStaticText()
{
	if (STB_Title.IsValid()) // ¿µ¿õÀÇÅ¾ ·©Å·
		STB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("HeroTower_RankingTitle")));
	if (STB_AllRank.IsValid()) //ÀüÃ¼ ·©Å·
		STB_AllRank->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PVP1on1_TotalRanking")));
	if (STB_AllRank2.IsValid())
		STB_AllRank2->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PVP1on1_TotalRanking")));
	if (STB_FriendRank.IsValid()) // Ä£±¸ ·©Å·
		STB_FriendRank->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PVP1on1_FriendRanking")));
	if (STB_FriendRank2.IsValid())
		STB_FriendRank2->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PVP1on1_FriendRanking")));

	if (TB_HeroRanking.IsValid())
	{
		TB_HeroRanking->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("HeroTower_RankNotice")), FText::AsNumber(BladeIIGameImpl::GetClientDataStore().GetMinFloorforHeroRanking())));
	}
}