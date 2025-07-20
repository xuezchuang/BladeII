// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIKakaoFriendRewardRow.h"
#include "B2UIFriendKakaoInvite.h"
#include "BladeIIGameImpl.h"
#include "B2UIManager.h"
#include "B2ItemInfo.h"
#include "BladeIIUtil.h"
#include "B2ClientDataStore.h"
#include "B2RewardInfo.h"
#include "B2UIDocHelper.h"
#include "Retailer.h"

UB2UIKakaoFriendRewardRow::UB2UIKakaoFriendRewardRow(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	OwnerUI = nullptr;
	CurRewardState = EFriendRewardUIState::None;
}

void UB2UIKakaoFriendRewardRow::Init()
{
	Super::Init();
}

void UB2UIKakaoFriendRewardRow::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_Title);
	GET_SLOT(UTextBlock, TB_RewardAmount);
	
	GET_SLOT(UImage, IMG_RewardItemIcon);

	GET_SLOT(UB2Button, BTN_Reward);
	GET_SLOT(UTextBlock, TB_GetReward);
	GET_SLOT(UTextBlock, TB_Progress);
	GET_SLOT(UTextBlock, TB_Complete);

	GET_SLOT(UPanelWidget, X_GetReward);

	SetRewardState(EFriendRewardUIState::Progesss);
}

void UB2UIKakaoFriendRewardRow::UpdateStaticText()
{
	if (TB_Complete.IsValid())
		TB_Complete->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("General_GetReward2")));
	if (TB_GetReward.IsValid())
		TB_GetReward->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("General_GetReward")));
	
}

void UB2UIKakaoFriendRewardRow::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_Reward, &UB2UIKakaoFriendRewardRow::OnClickReward);
}

void UB2UIKakaoFriendRewardRow::OnClickReward()
{
	if (CurRewardState == EFriendRewardUIState::GetReward)
	{
		UE_LOG(LogBladeII, Log, TEXT("@@@ OnClickReward"));
		if(OwnerUI != nullptr)
			OwnerUI->OnKakaoGetReward(RowIndex);
	}
}

void UB2UIKakaoFriendRewardRow::SetRewardInfoEveryJoin(const int32 RewardItemId, const int32 RwardItemAmount)
{
	if (IMG_RewardItemIcon.IsValid())
		IMG_RewardItemIcon->SetBrushFromMaterial(StaticFindRewardInfo() ? StaticFindRewardInfo()->GetRewardIconMaterial(RewardItemId) : nullptr);

	if (TB_Title.IsValid())
		TB_Title->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Friend_KakaoFriendInvite_Invite01"))));
	if(TB_RewardAmount.IsValid())
		TB_RewardAmount->SetText(FText::Format(
			BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("CounterDG_RewardNum"))), FText::AsNumber(RwardItemAmount)
		));
}

void UB2UIKakaoFriendRewardRow::SetRewardInfoEveryJoinTip(FText Message)
{
	SetRewardJoinTipState();
	if (TB_Progress.IsValid())
		TB_Progress->SetText(Message);
}

void UB2UIKakaoFriendRewardRow::SetRewardInfo(const b2network::B2mdKakaoFriendInviteRewardPtr &InviteRewardInfo, bool IsInviteMode)
{
	if (X_GetReward.IsValid())
		X_GetReward->SetVisibility(ESlateVisibility::Visible);

	if (TB_Progress.IsValid())
		TB_Progress->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("Quest_InProgress")));

 	if (IMG_RewardItemIcon.IsValid())
 		IMG_RewardItemIcon->SetBrushFromMaterial(StaticFindRewardInfo() ? StaticFindRewardInfo()->GetRewardIconMaterial(InviteRewardInfo->reward_id) : nullptr);
	if (TB_RewardAmount.IsValid())
		TB_RewardAmount->SetText(FText::Format(
			BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("CounterDG_RewardNum"))), FText::AsNumber(InviteRewardInfo->reward_count)
		));

	if (IsInviteMode)
	{
		FText InviteGoal;
		FString TextKey = FString::Printf(TEXT("Friend_KakaoFriendInvite_Invite%d"), InviteRewardInfo->goal_count);
		InviteGoal = BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TextKey);
		if (TB_Title.IsValid())
			TB_Title->SetText(InviteGoal);
	}
	else
	{
		FText JoinerGoal = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Friend_KakaoFriendInviteSuccessCount")), FText::AsNumber(InviteRewardInfo->goal_count));
		if (TB_Title.IsValid())
			TB_Title->SetText(JoinerGoal);
	}
}

void UB2UIKakaoFriendRewardRow::SetRewardState(const bool IsGetReward, const int32 CurCount, const int32 ConditionCount)
{
	EFriendRewardUIState UIState = EFriendRewardUIState::Progesss;
	if (IsGetReward)
	{
		UIState = EFriendRewardUIState::Complete;
	}
	else
	{
		if (ConditionCount <= CurCount)
			UIState = EFriendRewardUIState::GetReward;
		else 
			UIState = EFriendRewardUIState::Progesss;
	}
	
	SetRewardState(UIState);
}

void UB2UIKakaoFriendRewardRow::SetRewardState(EFriendRewardUIState State)
{
	CurRewardState = State;

	switch (CurRewardState)
	{
	case EFriendRewardUIState::Progesss:
		SetProgressState();
		break;
	case EFriendRewardUIState::GetReward:
		SetGetRewardState();
		break;
	case EFriendRewardUIState::Complete:
		SetCompleteState();
		break;
	default:
			return;
	}
}

void UB2UIKakaoFriendRewardRow::SetRewardJoinTipState()
{
	if (X_GetReward.IsValid())
		X_GetReward->SetVisibility(ESlateVisibility::HitTestInvisible);
	if (BTN_Reward.IsValid())
		BTN_Reward->SetVisibility(ESlateVisibility::Hidden);

	if (TB_Progress.IsValid())
		TB_Progress->SetVisibility(ESlateVisibility::HitTestInvisible);
	if (TB_Complete.IsValid())
		TB_Complete->SetVisibility(ESlateVisibility::Hidden);
	if (TB_GetReward.IsValid())
		TB_GetReward->SetVisibility(ESlateVisibility::Hidden);
}

void UB2UIKakaoFriendRewardRow::SetProgressState()
{
	if (BTN_Reward.IsValid())
		BTN_Reward->SetVisibility(ESlateVisibility::Hidden);

	if (TB_Progress.IsValid())
		TB_Progress->SetVisibility(ESlateVisibility::HitTestInvisible);
	if (TB_Complete.IsValid())
		TB_Complete->SetVisibility(ESlateVisibility::Hidden);
	if (TB_GetReward.IsValid())
		TB_GetReward->SetVisibility(ESlateVisibility::Hidden);
}

void UB2UIKakaoFriendRewardRow::SetGetRewardState()
{
	if (BTN_Reward.IsValid())
	{
		BTN_Reward->SetVisibility(ESlateVisibility::Visible);
		BTN_Reward->SetIsEnabled(true);
	}

	if (TB_Progress.IsValid())
		TB_Progress->SetVisibility(ESlateVisibility::Hidden);
	if (TB_Complete.IsValid())
		TB_Complete->SetVisibility(ESlateVisibility::Hidden);
	if (TB_GetReward.IsValid())
		TB_GetReward->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UB2UIKakaoFriendRewardRow::SetCompleteState()
{
	if (BTN_Reward.IsValid())
	{
		BTN_Reward->SetVisibility(ESlateVisibility::Hidden);
		BTN_Reward->SetIsEnabled(false);
	}

	if (TB_Progress.IsValid())
		TB_Progress->SetVisibility(ESlateVisibility::Hidden);
	if (TB_Complete.IsValid())
		TB_Complete->SetVisibility(ESlateVisibility::HitTestInvisible);
	if (TB_GetReward.IsValid())
		TB_GetReward->SetVisibility(ESlateVisibility::Hidden);
}
