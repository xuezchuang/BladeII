// Fill out your copyright notice in the Description page of Project Settings.
#include "BladeII.h"
#include "B2UIManager.h"
#include "B2UIGuildRewardInfo.h"
#include "B2UIRankingRewardPart.h"
#include "B2UIModRewardBoxDetailInfo.h"

void UB2UIGuildRewardSlotRow::CacheAssets()
{
	Super::CacheAssets();

	GuildRewardSlotArr.SetNum(4);

	GET_SLOT_BYNAME(UB2UIRankingRewardPart, GuildRewardSlotArr[0], UIP_GuildRewardSlotRowSlot)
	GET_SLOT_BYNAME(UB2UIRankingRewardPart, GuildRewardSlotArr[1], UIP_GuildRewardSlotRowSlot1)
	GET_SLOT_BYNAME(UB2UIRankingRewardPart, GuildRewardSlotArr[2], UIP_GuildRewardSlotRowSlot2)
	GET_SLOT_BYNAME(UB2UIRankingRewardPart, GuildRewardSlotArr[3], UIP_GuildRewardSlotRowSlot3)


	for (auto RankingRewardPar : GuildRewardSlotArr)
	{
		RankingRewardPar->Init();
	}


}

void UB2UIGuildRewardSlotRow::BindDelegates()
{
	Super::BindDelegates();
}

void UB2UIGuildRewardSlotRow::Init()
{
	Super::Init();
}


void UB2UIGuildRewardSlotRow::SetRewardInfo(TArray<b2network::B2RewardPtr> RewardInfo)
{

	for (int32 i = 0; i < GuildRewardSlotArr.Num(); ++i)
	{
		if (i < RewardInfo.Num() && RewardInfo[i]->raw_reward->id != 0)
		{
			GuildRewardSlotArr[i]->SetVisibility(ESlateVisibility::Visible);
			GuildRewardSlotArr[i]->SetItemInfo(RewardInfo[i]);
		}
		else
		{
			GuildRewardSlotArr[i]->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UB2UIGuildRewardSlot::CacheAssets()
{
	Super::CacheAssets();


	/*GET_SLOT(UB2UIGuildRewardSlotRow ,UIP_GuildRewardSlotRow);
	if (UIP_GuildRewardSlotRow.IsValid())
		UIP_GuildRewardSlotRow->Init();*/
	GET_SLOT(UTextBlock, TB_MedalCount);
	GET_SLOT(UB2Button, BTN_Detail);
}

void UB2UIGuildRewardSlot::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_Detail, &UB2UIGuildRewardSlot::OnClickDetail);
}

void UB2UIGuildRewardSlot::Init()
{
	Super::Init();

	RewardInfo = nullptr;
}


void UB2UIGuildRewardSlot::SetRewardInfo(b2network::B2mdGuildBattleResultRewardPtr InRewardInfo)
{
	/*if (UIP_GuildRewardSlotRow.IsValid())
		UIP_GuildRewardSlotRow->SetRewardInfo(RewardInfo->rewards);*/

	RewardInfo = InRewardInfo;

	if (TB_MedalCount.IsValid())
		TB_MedalCount->SetText(FText::AsNumber(RewardInfo->req_count_min));

	GuildGrade = RewardInfo->reward_grade;
	SetBoxGrade(RewardInfo->reward_grade);
}

void UB2UIGuildRewardSlot::OnClickDetail()
{
	BII_CHECK(RewardInfo);

	auto* UIManager = UB2UIManager::GetInstance();
	if (UIManager)
	{
		auto BoxGradeUI = UIManager->OpenUI<UB2UIModRewardBoxDetailInfo>(UIFName::ModRewardBoxDetail);

		if (BoxGradeUI)
		{
			BoxGradeUI->SetPanelClear();
			BoxGradeUI->SetRewardCount(RewardInfo->rewards.Num());
			BoxGradeUI->GuildGradeEvent(GuildGrade);
			for (int32 i = 0; i < RewardInfo->rewards.Num(); ++i)
			{
				BoxGradeUI->SetRewardIcon(RewardInfo->rewards[i]->raw_reward->id, RewardInfo->rewards[i]->raw_reward->count, RewardInfo->rewards[i]->raw_reward->count, i);
			}
		}
	}

}

void UB2UIGuildRewardInfo::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_MedalCount)

	GET_SLOT(UB2Button, BTN_Close)
	GET_SLOT(UHorizontalBox, X_RewardSlotVB)
}

void UB2UIGuildRewardInfo::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIGuildRewardInfo::OnClickClose);
}

void UB2UIGuildRewardInfo::CloseWidgetDelegate()
{
	OnClickClose();
}

void UB2UIGuildRewardInfo::Init()
{
	Super::Init();

}

void UB2UIGuildRewardInfo::DestroySelf(UB2UIManager* InUIManager)
{

	Super::DestroySelf(InUIManager);
}


void UB2UIGuildRewardInfo::SetRewardInfo(const TMap<int32, b2network::B2mdGuildBattleResultRewardPtr>& RewardInfo, int32 CurrentMedal)
{
	if (TB_MedalCount.IsValid())
		TB_MedalCount->SetText(FText::AsNumber(CurrentMedal));

	X_RewardSlotVB->ClearChildren();

	for (auto RewardInfoSlot : RewardInfo)
	{
		if (RewardInfoSlot.Value->req_count_min == 0)
			continue;

		auto* CreateSlot = CreateWidget<UB2UIGuildRewardSlot>(GetWorld(), GuildRewardSlot);
		if (!CreateSlot)
			return;

		auto SlotPanel = Cast<UHorizontalBoxSlot>(X_RewardSlotVB->AddChild(CreateSlot));

		BII_CHECK(SlotPanel);

		SlotPanel->SetSize(ESlateSizeRule::Fill);
		SlotPanel->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
		SlotPanel->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);

		CreateSlot->Init();

		CreateSlot->SetRewardInfo(RewardInfoSlot.Value);

	}
}

void UB2UIGuildRewardInfo::OnClickClose()
{
	UB2UIManager::GetInstance()->CloseUI(UIFName::GuildBattleRewardInfoList);
}
