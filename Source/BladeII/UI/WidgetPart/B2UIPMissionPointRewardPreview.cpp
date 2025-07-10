// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIPMissionPointRewardPreview.h"
#include "B2UIManager.h"
#include "B2UIRewardEventSet.h"
#include "Retailer.h"


void UB2UIPMissionPointRewardPreview::BindDelegates()
{
	Super::BindDelegates();

	if (BTN_GetReward.IsValid())
	{
		BTN_GetReward->OnClicked.RemoveAll(this);
		BIND_CLICK_FUNC_TO_BTN(BTN_GetReward, &UB2UIPMissionPointRewardPreview::OnClickGetRewardBtn)
	}
}

void UB2UIPMissionPointRewardPreview::DestroySelf(class UB2UIManager* InUIManager)
{
	ClearDynamicIcons(InUIManager);
	Super::DestroySelf(InUIManager);
}

void UB2UIPMissionPointRewardPreview::OpenPreview(int32 InRewardBoxIndex, bool bInCanGetReward, EMissionType MissionType, int32 RequireMissionPoint)
{
	ClearDynamicIcons(UB2UIManager::GetInstance());

	RequestMissionType = MissionType;
	bCanGetReward = bInCanGetReward;
	RequestRewardIndex = InRewardBoxIndex;

	if (BTN_GetReward.IsValid())
		BTN_GetReward->SetIsEnabled(bCanGetReward);

	if (TB_RewardStarCount.IsValid())
	{
		FText FormatText = FText::FromString(TEXT("{0} / {1}"));
		TB_RewardStarCount->SetText(FText::AsNumber(RequireMissionPoint));
	}

	OnPopupBP();
}

void UB2UIPMissionPointRewardPreview::CreateRewardIconCommon(const TArray<b2network::B2RewardPtr>& RewardInfos)
{
	for (auto RewardPtr : RewardInfos)
	{
		if (RewardPtr && RewardPtr->raw_reward->id != 0)
		{
			UB2UIRewardIcon* CreatedIcon = Cast<UB2UIRewardIcon>(DynCreateInHorizontalBox(RewardTemplate, this, X_Rewards.Get()));
			if (CreatedIcon)
			{
				CreatedIcon->Init();
				CreatedIcon->UpdateRewardIcon(RewardPtr);
				CreatedIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

				CreatedRewardIcons.Add(CreatedIcon);
			}
		}
	}
}

void UB2UIPMissionPointRewardPreview::ClearDynamicIcons(class UB2UIManager* InUIManager)
{
	for (UB2UIRewardIcon* RewardIcon : CreatedRewardIcons)
	{
		if (RewardIcon != nullptr)
			RewardIcon->DestroySelf(InUIManager);
	}

	CreatedRewardIcons.Empty();
}

void UB2UIPMissionPointRewardPreview::OnClickGetRewardBtn()
{
	if (bCanGetReward)
	{
		int32 ServerMissionType = ConvertMissionType(RequestMissionType);
		data_trader::Retailer::GetInstance().RequestReceiveCompleteMissionReward(ServerMissionType, RequestRewardIndex);
		CloseMe();
	}
}

