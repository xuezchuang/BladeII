// Fill out your copyright notice in the Description page of Project Settings.
#include "BladeII.h"
#include "B2UIManager.h"
#include "B2UIGuildRewardBoxProgress.h"
#include "B2UIGuildRewardInfo.h"
#include "B2UIDocHelper.h"
#include "Retailer.h"
#include "B2UIMailRewardPopUp.h"


#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"

void UB2UIGuildRewardBoxProgress::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UProgressBar,	PB_MedalProgress);
	GET_SLOT(UB2Button,		BTN_RewardClick);
	GET_SLOT(UB2RichTextBlock,	TB_NextMedal);
	GET_SLOT(UB2RichTextBlock,	TB_MedalProgress);

	GET_SLOT(UPanelWidget, CP_ProgressReward);
	GET_SLOT(UPanelWidget, CP_GetReward);

	GET_SLOT(UB2UIWidgetBase, Reddot_GuilldReward);
}						   

void UB2UIGuildRewardBoxProgress::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_RewardClick, &UB2UIGuildRewardBoxProgress::OnClickBTN_RewardClick);
}

void UB2UIGuildRewardBoxProgress::Init()
{
	Super::Init();


}

void UB2UIGuildRewardBoxProgress::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);


}


void UB2UIGuildRewardBoxProgress::OnOpen(bool RightNow /*= false*/)
{
	Super::OnOpen(RightNow);

}


void UB2UIGuildRewardBoxProgress::OnClose(bool RightNow /*= false*/)
{
	Super::OnClose(RightNow);

	
}

void UB2UIGuildRewardBoxProgress::RegisterUIMarkForRedDot()
{
	REGISTER_REDDOT_CONDITION(Reddot_GuilldReward.Get(), &UB2UIGuildRewardBoxProgress::HasRedDot_GuildReward);
}

void UB2UIGuildRewardBoxProgress::SetGuildReward(int32 Grade, int32 CurrentMedal, int32 MaxMedal, int32 NextMedal)
{
	EventGradeBoxIcon(Grade);

	CurrentMedalCount = CurrentMedal;

	if (TB_NextMedal.IsValid())
	{
		TB_NextMedal->SetVisibility(NextMedal <= 0 ? ESlateVisibility::Hidden : ESlateVisibility::SelfHitTestInvisible);
		
		TB_NextMedal->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Box_NextGoal")), FText::AsNumber(NextMedal)));
		
	}

	if (TB_MedalProgress.IsValid())
	{
		TB_MedalProgress->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), CurrentMedal, MaxMedal)));
	}

	if (PB_MedalProgress.IsValid())
	{
		PB_MedalProgress->SetVisibility(ESlateVisibility::Visible);
		PB_MedalProgress->SetPercent(CurrentMedal == 0 ? 0 : float(CurrentMedal) / float(MaxMedal));
	}

	if (Grade == BladeIIGameImpl::GetClientDataStore().GetGuildBattleResultRewardMaxGrade())
	{
		PB_MedalProgress->SetVisibility(ESlateVisibility::Hidden);
		TB_NextMedal->SetVisibility(ESlateVisibility::Hidden);
		TB_MedalProgress->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Box_Complete")));
	}


}


void UB2UIGuildRewardBoxProgress::SetRewardComplete(bool IsReward)
{

	//RewardState = IsReward;
	auto GuildDoc = UB2UIDocHelper::GetDocGuild();
	DoMarkRedDot();
	if (!GuildDoc)
		return;

	if (GuildDoc->GetJoinGuildBattlememeber() == false)
	{
		SetGuildRewardText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Battle_NextReward")));
	}

	if (PB_MedalProgress.IsValid() && TB_NextMedal.IsValid() && IsReward&& GuildDoc->GetJoinGuildBattlememeber())
	{
		PB_MedalProgress->SetVisibility(ESlateVisibility::Hidden);
		TB_NextMedal->SetVisibility(ESlateVisibility::Hidden);
	}


	if(CP_GetReward.IsValid())
		CP_GetReward->SetVisibility(IsReward&& GuildDoc->GetJoinGuildBattlememeber() ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	if(IsReward&& GuildDoc->GetJoinGuildBattlememeber() && TB_MedalProgress.IsValid())
		TB_MedalProgress->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_GetReward2")));
}

void UB2UIGuildRewardBoxProgress::SetGuildRewardText(FText InText)
{
	if(TB_NextMedal.IsValid())
		TB_NextMedal->SetText(InText);
}

bool UB2UIGuildRewardBoxProgress::HasRedDot_GuildReward()
{
	auto GuildDoc = UB2UIDocHelper::GetDocGuild();

	if (!GuildDoc)
		return false;

	auto RewardInfoPtr = BladeIIGameImpl::GetClientDataStore().GetGuildBattleResultReward(GuildDoc->GetObtainMedal());

	if (!RewardInfoPtr)
		return false;

	return  GuildDoc->GetTurnReward()
		&& GuildDoc->GetGuildbattleStateNew().current_state >= b2network::B2GuildBattleStateType::RESULT && RewardInfoPtr->req_count_min
		&& GuildDoc->GetJoinGuildBattlememeber() == true;
}

void UB2UIGuildRewardBoxProgress::OnClickBTN_RewardClick()
{
	auto GuildDoc = UB2UIDocHelper::GetDocGuild();

	if (!GuildDoc)
		return;

	auto RewardInfoPtr = BladeIIGameImpl::GetClientDataStore().GetGuildBattleResultReward(GuildDoc->GetObtainMedal());

	if (GuildDoc->GetTurnReward()
		&& GuildDoc->GetGuildbattleStateNew().current_state >= b2network::B2GuildBattleStateType::RESULT && RewardInfoPtr->req_count_min
		&& GuildDoc->GetJoinGuildBattlememeber() == true)
	{
		data_trader::Retailer::GetInstance().RequestGuildBattleTurnReward();
		return;
	}

	auto* RewardInfoList = UB2UIManager::GetInstance()->OpenUI<UB2UIGuildRewardInfo>(UIFName::GuildBattleRewardInfoList);

	if (!RewardInfoList)
		return;

	RewardInfoList->SetRewardInfo(BladeIIGameImpl::GetClientDataStore().GetGuildBattleResultRewardArr(), CurrentMedalCount);
}


