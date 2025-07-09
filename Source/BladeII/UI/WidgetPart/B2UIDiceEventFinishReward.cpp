// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UIDiceEventFinishReward.h"

#include "B2RewardInfo.h"
#include "BladeIIUtil.h"

void UB2UIDiceEventFinishReward::InitDiceEventFinishReward(const b2network::B2mdDiceEventFinishReward& FinishReward)
{
	CachedDiceEventFinishReward = FinishReward;

	this->Init();

	UpdateIMG_RewardIcon();
}

void UB2UIDiceEventFinishReward::UpdateState(int32 iFinishCount)
{
	if (WS_State.IsValid())
	{
		bool IsComplete = iFinishCount >= CachedDiceEventFinishReward.number ? true : false;

		WS_State->SetActiveWidgetIndex((int32)(IsComplete));
	}
}

void UB2UIDiceEventFinishReward::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UWidgetSwitcher, WS_State);

	GET_SLOT(UB2Image, IMG_RewardIcon);

	GET_SLOT(UTextBlock, TB_FinishCount);
	GET_SLOT(UTextBlock, TB_RewardCount);

}

void UB2UIDiceEventFinishReward::UpdateStaticText()
{
	if (TB_FinishCount.IsValid())
	{
		FText ftFinishCount = FText::Format(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("DiceEvent_FinishCount")), FText::AsNumber(CachedDiceEventFinishReward.number));

		TB_FinishCount->SetText(ftFinishCount);
	}

	if (TB_RewardCount.IsValid())
	{
		FText ftRewardCount = FText::FromString(FString::Printf(TEXT("x %d"), CachedDiceEventFinishReward.reward_count));
		TB_RewardCount->SetText(ftRewardCount);
	}
}

void UB2UIDiceEventFinishReward::UpdateIMG_RewardIcon()
{
	if (IMG_RewardIcon.IsValid())
	{
		UMaterialInterface* pMaterial = StaticFindRewardInfo() ? StaticFindRewardInfo()->GetRewardIconMaterial(CachedDiceEventFinishReward.reward_id) : nullptr;
		if (pMaterial)
		{
			IMG_RewardIcon->SetBrushFromMaterial(pMaterial);
		}
	}
}