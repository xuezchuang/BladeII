#include "B2UIGuildMain.h"
#include "B2UIManager.h"
#include "B2UIHeader.h"
#include "B2UIGuildMark.h"
#include "Retailer.h"
#include "B2UIGuildMemberSlot.h"
#include "B2UIDocHelper.h"
#include "B2UIGuildMatchPopup.h"
#include "Animation/WidgetAnimation.h"
#include "B2UIGuildSeasonReward.h"
#include "B2UIGuildInvitePopup.h"
#include "B2UIGuildRewardBoxProgress.h"
#include "B2UIMailRewardPopUp.h"
#include "B2UIGuildSchedule.h"


#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"

void UB2UIGuildMain::Init()
{
	Super::Init();

	GuildMainPageUpdate();
}

void UB2UIGuildMain::OnSceneOpen(EUIScene InOpenedScene)
{
	Super::OnSceneOpen(InOpenedScene);

	// UIHeader 와 같이 Scene 으로 구성하는 경우 Init 시점에 하면 Scene 에 포함된 header 가 생성이 안된 상황일 수 있음.
	SetLobbyUIHeaderTitleByGeneralTextTableKey(TEXT("Guild_Guild"));
	OpenPlayAnimationEvent();

}

void UB2UIGuildMain::OnOpen(bool RightNow /*= false*/)
{
	//Super::OnOpen(RightNow);
	//DeliveryInviteTicket = DeliveryGetGuildMemberCandidatesClass<FB2ResponseGetGuildMemberCandidatesPtr>::GetInstance().Subscribe([](const FB2ResponseGetGuildMemberCandidatesPtr& Invite){
	//	auto* GuildInviteUI = UB2UIManager::GetInstance()->OpenUI<UB2UIGuildInvitePopup>(UIFName::GuildInvite);
	//	GuildInviteUI->SetInviteList(Invite);
	//});

	//DeliveryGuildNoticeTicket = DeliveryUpdateGuildNoticeClass<FB2ResponseUpdateGuildNoticePtr>::GetInstance().Subscribe([this](const FB2ResponseUpdateGuildNoticePtr& Notice){
	//	BladeIIGameImpl::GetClientDataStore().GetMyGuildInfo().GuildInfo->notice = Notice->notice;
	//	this->WriteNoticeState();
	//});

	//DeliveryUpdateGuildTicket = DeliveryUpdateGuildClass<FB2ResponseUpdateGuildPtr>::GetInstance().Subscribe([this](const FB2ResponseUpdateGuildPtr& GuildInfo){
	//	ReceivedGuildUpdate(GuildInfo);
	//});

	//DeliveryGuildBattleTurnRewardTicket = DeliveryGuildBattleTurnRewardClass<FB2ResponseGuildBattleTurnRewardPtr>::GetInstance().Subscribe([this](const FB2ResponseGuildBattleTurnRewardPtr& HistoryData)
	//{
	//	auto* RewardPopup = UB2UIManager::GetInstance()->OpenUI<UB2UIMailRewardPopUp>(UIFName::RewardMailPopup);


	//	auto GuildDoc = UB2UIDocHelper::GetDocGuild();

	//	if (!GuildDoc)
	//		return;

	//	GuildDoc->SetTurnReward(false);
	//	this->SetRewardComplete(true);
	//});



	//WriteNoticeState();
}

void UB2UIGuildMain::OnHistoryOpen(bool IsHistory)
{
	//Init();
}

void UB2UIGuildMain::OnClose(bool RightNow /*= false*/)
{
	Super::OnClose(RightNow);


	//DeliveryGetGuildMemberCandidatesClass<FB2ResponseGetGuildMemberCandidatesPtr>::GetInstance().Unsubscribe(DeliveryInviteTicket);
	//DeliveryUpdateGuildNoticeClass<FB2ResponseUpdateGuildNoticePtr>::GetInstance().Unsubscribe(DeliveryGuildNoticeTicket);
	//DeliveryUpdateGuildClass<FB2ResponseUpdateGuildPtr>::GetInstance().Unsubscribe(DeliveryUpdateGuildTicket);
	//DeliveryGuildBattleTurnRewardClass<FB2ResponseGuildBattleTurnRewardPtr>::GetInstance().Unsubscribe(DeliveryGuildBattleTurnRewardTicket);

}


void UB2UIGuildMain::GuildMainPageUpdate()
{
	auto GuildDoc = UB2UIDocHelper::GetDocGuild();

	check(GuildDoc)

		SetGuildMain(static_cast<EGuildInfoType>(GuildDoc->GetGuildMainType()), static_cast<EGuildInfoType>(GuildDoc->GetGuildMainType()) == EGuildInfoType::MyGuild ?
			BladeIIGameImpl::GetClientDataStore().GetMyGuildInfo() : BladeIIGameImpl::GetClientDataStore().GetOtherGuildInfo());

	if (BladeIIGameImpl::GetClientDataStore().GetMyGuildInfo().GuildInfo)
	{
		SetGuildBattleCurrentState();
	}

	SetGuildSchedule();
	SetRewardBoxProgress();
}

void UB2UIGuildMain::ReceivedGuildUpdate(const FB2ResponseUpdateGuildPtr& GuildInfo)
{
	DevShowNotReadyYetMessage(TEXT("Guild_EditComplete"));
	UB2UIManager::GetInstance()->CloseUI(UIFName::GuildEdit);

	FB2ResponseGuildInfoPtr GuildInfoPtr = BladeIIGameImpl::GetClientDataStore().GetMyGuildInfo().GuildInfo;
	const FUserGuildInfo& GuildInfoUser = BladeIIGameImpl::GetClientDataStore().GetUserGuildInfo();

	GuildInfoPtr->mark_index = GuildInfo->mark_index;
	GuildInfoPtr->mark_color = GuildInfo->mark_color;
	GuildInfoPtr->mark_bg_index = GuildInfo->mark_bg_index;
	GuildInfoPtr->mark_bg_color = GuildInfo->mark_bg_color;
	GuildInfoPtr->join_option = GuildInfo->join_option;


	BladeIIGameImpl::GetClientDataStore().SetUserGuildInfo(FUserGuildInfo(GuildInfoUser.GuildName, GuildInfo->mark_index, 
		GuildInfo->mark_color, GuildInfo->mark_bg_index, GuildInfo->mark_bg_color, GuildInfoUser.GuildRank));

	BladeIIGameImpl::GetClientDataStore().SetGoldAmount(GuildInfo->current_gold);

	if (UIP_GuildMark.IsValid())
		UIP_GuildMark->SetGuildMark(GuildInfo->mark_index, GuildInfo->mark_color, GuildInfo->mark_bg_index, GuildInfo->mark_bg_color);
}

void UB2UIGuildMain::SetGuildMain(EGuildInfoType type, const FGuildInfo& GuildInfo)
{
	auto GuildDoc = UB2UIDocHelper::GetDocGuild();

	check(GuildDoc)


	GuildDoc->SetGuildMainType(static_cast<int32>(type));
	
	if (type == EGuildInfoType::MyGuild)
	{
		CP_MyGuildMenu->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		//CP_MyGuildBtn->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		CP_GuildJoin->SetVisibility(ESlateVisibility::Hidden);
		CP_OtherGuildMenu->SetVisibility(ESlateVisibility::Hidden);
		UIP_GuildRewardBoxProgress->SetVisibility(ESlateVisibility::Visible);
		X_NoticePanel->SetVisibility(ESlateVisibility::Visible);
		if (BladeIIGameImpl::GetClientDataStore().GetUserGuildInfo().GuildRank == b2network::B2GuildMemberRank::NORMAL)
		{
			BTN_Invite->SetIsEnabled(false);
			BTN_GuildEdit->SetIsEnabled(false);
		}
		else
		{
			BTN_Invite->SetIsEnabled(true);
			BTN_GuildEdit->SetIsEnabled(true);
		}

	}
	else
	{
		CP_MyGuildMenu->SetVisibility(ESlateVisibility::Hidden);
		//CP_MyGuildBtn->SetVisibility(ESlateVisibility::Hidden);
		CP_GuildJoin->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		X_NoticePanel->SetVisibility(ESlateVisibility::Hidden);

		if (GuildInfo.GuildInfo->join_option == b2network::B2GuildJoinOption::PUBLIC || GuildDoc->GetGuildAcceptState())
			BTN_GuildJoin->SetIsEnabled(true);
		else if (GuildInfo.GuildInfo->join_option == b2network::B2GuildJoinOption::PRIVATE)
			BTN_GuildJoin->SetIsEnabled(false);

		CP_OtherGuildMenu->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		UIP_GuildRewardBoxProgress->SetVisibility(ESlateVisibility::Hidden);
		SetOtherGuildMemberList(GuildInfo.GuildMemberInfo);
	}


	//UIP_GuildRewardBoxProgress->SetVisibility(ESlateVisibility::Hidden);//CBT 임시 막아놓기woong

	if (BTN_GuildRank.IsValid() && OV_GuildMemberInfoImage.IsValid())
	{
		BTN_GuildRank->SetIsEnabled(EGuildInfoType::MyGuild == type ? true : false);
	}

	if (BTN_GuildListInfo.IsValid() && OV_GuildRankingImage.IsValid())
	{
		BTN_GuildListInfo->SetIsEnabled(EGuildInfoType::MyGuild == type ? true : false);
	}

	if (BTN_GuildWarInfo.IsValid() && OV_HistoryImage.IsValid())
	{
		BTN_GuildWarInfo->SetIsEnabled(EGuildInfoType::MyGuild == type ? true : false);
	}
	if (BTN_GuildWar.IsValid())
	{
		BTN_GuildWar->SetVisibility(EGuildInfoType::MyGuild == type ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}

	//CBT 임시
	BTN_GuildWar->SetIsEnabled(false);
	//BTN_GuildWarInfo->SetIsEnabled(false);
	//BTN_GuildWarReady->SetIsEnabled(false);
	//BTN_GuildRank->SetIsEnabled(false);

	//길드전 될때 풀어야함
	BTN_Mercenary->SetIsEnabled(true);
	BTN_GuildWarReady->SetIsEnabled(true);
	BTN_GuildWar->SetIsEnabled(true);


	GuildID = GuildInfo.GuildInfo->guild_id;

	SetGuildInfo(GuildInfo.GuildInfo);
}

void UB2UIGuildMain::SetGuildInfo(const FB2ResponseGuildInfoPtr GuildInfo)
{
	FText None = FText::FromString(TEXT("-"));

	if (UIP_GuildMark.IsValid())
		UIP_GuildMark->SetGuildMark(GuildInfo->mark_index, GuildInfo->mark_color, GuildInfo->mark_bg_index, GuildInfo->mark_bg_color);

	if (RTB_GuildName.IsValid())
		RTB_GuildName->SetText(FText::FromString(GuildInfo->guild_name));

	if (RTB_GuildNumber.IsValid())
		RTB_GuildNumber->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_GuildNumber")), FText::AsNumber(GuildInfo->cur_member_count), FText::AsNumber(GuildInfo->max_member_count)));

	if (RTB_GuildScore.IsValid())
		RTB_GuildScore->SetText(GuildInfo->season_obtain_medal ? FText::AsNumber(GuildInfo->season_obtain_medal) : None);

	if (RTB_GuildRank.IsValid())
	{
		float PerCent = INDEX_NONE;
		FText PerCentText;
		if (!GuildInfo->season_ranking || !GuildInfo->season_ranking_total)
			PerCent = INDEX_NONE;
		else
			PerCent = (float(GuildInfo->season_ranking) / float(GuildInfo->season_ranking_total)) * 100;

		RTB_GuildRank->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_GuildRankNumber")), 
			GuildInfo->season_ranking ? FText::AsNumber(GuildInfo->season_ranking) : None,
			PerCent != INDEX_NONE ? FText::FromString(FString::Printf(TEXT("%.1f"), PerCent)): None));
	}

	if (RTB_WinCount.IsValid())
	{
		int32 WinCount = GuildInfo->total_result / b2network::B2GuildBattleResultType::BGBRT_WIN;
		FText WinCountText = WinCount ? FText::AsNumber(WinCount) : None;

		RTB_WinCount->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_BattleStatistics_WinCount")), WinCountText));			//추후에 서버 완성되면 다시 적용
	}

	if (TB_GuildCombat.IsValid())
		TB_GuildCombat->SetText(GuildInfo->members_avg_power ? FText::AsNumber(GuildInfo->members_avg_power) : None);		// jwyim power


	if (RTB_GuildNotice.IsValid())
		RTB_GuildNotice->SetText(FText::FromString(GuildInfo->notice));

	if (TB_GuildLevel.IsValid())
		TB_GuildLevel->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_Level_Abbrev")), FText::AsNumber(GuildInfo->level)));

	//길드레벨 퍼센테이지 구하기
	int32 GuildMaxExp = BladeIIGameImpl::GetClientDataStore().GetGuildExp(GuildInfo->level);
	int32 GuildMaxLv = BladeIIGameImpl::GetClientDataStore().GetGuildMaxLev();
	float GuildExpPercent = GuildInfo->exp ? float(GuildInfo->exp) / float(GuildMaxExp) : 0;

	if (PB_GuildProgress.IsValid() && TB_LevelPercent.IsValid() && IMG_LevelMax.IsValid())
	{
		if (GuildMaxLv == GuildInfo->level)
		{
			PB_GuildProgress->SetVisibility(ESlateVisibility::Hidden);
			TB_LevelPercent->SetVisibility(ESlateVisibility::Collapsed);
			IMG_LevelMax->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			PB_GuildProgress->SetVisibility(ESlateVisibility::Visible);
			TB_LevelPercent->SetVisibility(ESlateVisibility::Visible);
			IMG_LevelMax->SetVisibility(ESlateVisibility::Hidden);

	
			PB_GuildProgress->SetPercent(GuildExpPercent);
			TB_LevelPercent->SetText(FText::FromString(FString::Printf(TEXT("%d%%"), int32(GuildExpPercent * 100))));
		}
	}

}

void UB2UIGuildMain::SetGuildBattleCurrentState()
{
	auto GuildDoc = UB2UIDocHelper::GetDocGuild();

	if (!GuildDoc)
		return;

	auto BattleState = GuildDoc->GetGuildbattleStateNew();

	GuildBattleCurrentState = BattleState.current_state;
	bool JoinState = BladeIIGameImpl::GetClientDataStore().GetMyGuildInfo().GuildInfo->join_guild_battle;

	//TB_GuildBattleState->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ChallengeMod_GuildMatch")));

	if (TB_GuildBattleState.IsValid())									//woong
	{
		switch (GuildBattleCurrentState)
		{
		case b2network::B2GuildBattleStateType::NONE:					//유저가 휴식상태로 알고있는상태
			TB_GuildBattleState->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Battlebreaktime")));
			break;
		case b2network::B2GuildBattleStateType::INIT:
		case b2network::B2GuildBattleStateType::INITDONE:
			TB_GuildBattleState->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_BattleRequestReadyState")));
			break;		
		case b2network::B2GuildBattleStateType::READY:					//길드신청 상태
			TB_GuildBattleState->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, JoinState ? TEXT("Guild_BattleRequestCompleteState") : TEXT("Guild_BattleRequestState")));
			break;
		case b2network::B2GuildBattleStateType::MATCHING:				//매칭중일때
		case b2network::B2GuildBattleStateType::MATCHINGDONE:
			TB_GuildBattleState->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_BattleReadyState")));
			break;
		case b2network::B2GuildBattleStateType::BATTLE:					//길드전 기간
			TB_GuildBattleState->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_BattleState")));
			break;
		case b2network::B2GuildBattleStateType::SETTLEMENT:				//정산기간
		case b2network::B2GuildBattleStateType::SETTLEMENTDONE:
			TB_GuildBattleState->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_BattleEndReadyState")));
			break;
		case b2network::B2GuildBattleStateType::RESULT:					//보상받을수 있는상태
			TB_GuildBattleState->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_CalculateMessage")));
			break;
		}
	}
}

void UB2UIGuildMain::SetOtherGuildMemberList(const TArray<b2network::B2GuildMemberPtr>& RecommendGuildArr)
{
	check(VB_MemberList.IsValid())
	VB_MemberList->ClearChildren();

	int32 Rank = 1;
	for (auto Ele : RecommendGuildArr)
	{
		auto MemberSlot = CreateWidget<UB2UIGuildMemberSlot>(GetOwningPlayer(), UIP_GuildMemberInfoSlot);
		VB_MemberList->AddChild(MemberSlot);
		MemberSlot->Init();
		MemberSlot->SetGuildMember(Ele, Ele->contribution == 0 ? 0 : Rank);
		++Rank;
	}
}

void UB2UIGuildMain::SetRewardComplete(bool IsReward)
{
	UIP_GuildRewardBoxProgress->SetRewardComplete(IsReward);
}

void UB2UIGuildMain::SetGuildSchedule()
{
	if (UIP_GuildSchedule.IsValid())
		UIP_GuildSchedule->SetSchedule(BladeIIGameImpl ::GetClientDataStore().GetGuildBattleScheduleInfo());
}

void UB2UIGuildMain::WriteNoticeState()
{
	/*if (ETB_GuildNotice.IsValid())
		ETB_GuildNotice->SetVisibility(ESlateVisibility::Visible);*/

	if(BTN_GuildNoticePanel.IsValid())
		BTN_GuildNoticePanel->SetVisibility(ESlateVisibility::Visible);

	if (BTN_WriteNotice.IsValid())
		BTN_WriteNotice->SetVisibility(ESlateVisibility::HitTestInvisible);

	if (BTN_SaveNotice.IsValid())
		BTN_SaveNotice->SetVisibility(ESlateVisibility::Hidden);
}

void UB2UIGuildMain::SaveNoticeState()
{
	/*if (ETB_GuildNotice.IsValid())
		ETB_GuildNotice->SetVisibility(ESlateVisibility::Hidden);*/

	if (BTN_GuildNoticePanel.IsValid())
		BTN_GuildNoticePanel->SetVisibility(ESlateVisibility::Hidden);

	if (BTN_WriteNotice.IsValid())
		BTN_WriteNotice->SetVisibility(ESlateVisibility::Hidden);

	if (BTN_SaveNotice.IsValid())
		BTN_SaveNotice->SetVisibility(ESlateVisibility::Visible);
}

void UB2UIGuildMain::SetRewardBoxProgress()
{

	auto GuildDoc = UB2UIDocHelper::GetDocGuild();

	if (!GuildDoc)
		return;

	auto RewardInfoPtr = BladeIIGameImpl::GetClientDataStore().GetGuildBattleResultReward(GuildDoc->GetObtainMedal());


	/*b2network::B2mdGuildBattleResultRewardPtr NextRewardInfoPtr = nullptr;
	if(RewardInfoPtr)
		NextRewardInfoPtr = BladeIIGameImpl::GetClientDataStore().GetGuildBattleResultRewardGrade(RewardInfoPtr->reward_grade + 1);*/

	int32 NextMedal = 0;

	if (RewardInfoPtr)
	{
		auto NexRewardInfoPtr = BladeIIGameImpl::GetClientDataStore().GetGuildBattleResultReward(RewardInfoPtr->req_count_max + 1);

		if(NexRewardInfoPtr != RewardInfoPtr)
			NextMedal = RewardInfoPtr->req_count_max + 1;
	}

	if (UIP_GuildRewardBoxProgress.IsValid())
	{
		UIP_GuildRewardBoxProgress->SetGuildReward(RewardInfoPtr->reward_grade, GuildDoc->GetObtainMedal(), RewardInfoPtr->req_count_max, NextMedal);

		//길드보상이 첫번째상자가 아니고 보상을 받을수 있는 상태가 아니고 보상받을수있는 길드상태일때 받은걸로 표시 아 너무 복잡하구나 ㅠㅠ
		UIP_GuildRewardBoxProgress->SetRewardComplete((RewardInfoPtr->req_count_min && GuildDoc->GetTurnReward() == false && GuildDoc->GetGuildbattleStateNew().current_state >= b2network::B2GuildBattleStateType::RESULT) ? true : false);

		if (GuildDoc->GetJoinGuildBattlememeber() == false)
		{
			UIP_GuildRewardBoxProgress->SetGuildRewardText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Battle_NextReward")));
		}
	}
}

void UB2UIGuildMain::NotApplyCationPopup()
{
	UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_BattleNotRequestPopupMessage")),
		0.f,
		true,
		true);
	
}

void UB2UIGuildMain::OnClickGuildListInfo()
{
	UB2UIManager::GetInstance()->OpenUI(UIFName::MyGuildMemeberInfo);
}

void UB2UIGuildMain::OnClickGuildDropOut()
{
	FMsgPopupOnClick MsgPopup;
	MsgPopup.BindUObject(this, &UB2UIGuildMain::SendDropOut);

	UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_MessageHeader_PublicNoti")), FText::Format(
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_RejectMsg")),FText::FromString(BladeIIGameImpl::GetClientDataStore().GetMyGuildInfo().GuildInfo->guild_name)),0,true,true,EUIMsgPopupButtonGroup::YesOrNo
		, MsgPopup);
}

void UB2UIGuildMain::OnClickJoinGuild()
{
	FMsgPopupOnClick MsgPopup;
	MsgPopup.BindUObject(this, &UB2UIGuildMain::SendJoinGuild);
	UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_MessageHeader_PublicNoti")),FText::Format(
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_JoinMsg")), FText::FromString(BladeIIGameImpl::GetClientDataStore().GetOtherGuildInfo().GuildInfo->guild_name)), 0, true, true, EUIMsgPopupButtonGroup::YesOrNo
		, MsgPopup);

}

void UB2UIGuildMain::OnClickGuildWarReady()
{

	if (GuildBattleCurrentState != b2network::B2GuildBattleStateType::READY)
	{
		DevShowNotReadyYetMessage(TEXT("Guild_BattleReadyInfo"));
		return;			//레디아니면 안되게
	}

	//LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::GuildBattleReady);
}

void UB2UIGuildMain::OnClickGuildWar()
{
	auto GuildDoc = UB2UIDocHelper::GetDocGuild();

	if (!GuildDoc)
		return;

	int32 currentState = GuildDoc->GetGuildbattleStateNew().current_state;
	bool JoinState = BladeIIGameImpl::GetClientDataStore().GetMyGuildInfo().GuildInfo->join_guild_battle;

	if (!JoinState &&currentState > b2network::B2GuildBattleStateType::READY)
	{
		NotApplyCationPopup();
		return;
	}

	if (currentState == b2network::B2GuildBattleStateType::BATTLE)
	{
		data_trader::Retailer::GetInstance().RequestGetGuildBattleInfo();
	}
	else if (currentState == b2network::B2GuildBattleStateType::RESULT)
	{
		data_trader::Retailer::GetInstance().RequestGuildBattleTurnResult();
		return;
	}
	else
	{
		/*if (currentState == b2network::B2GuildBattleStateType::READY && JoinState)
		{
			UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple, BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
				BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_JoinError")));
			return;
		}*/

		//FGuildMatchPopup PopupDelegate;
		if (currentState == b2network::B2GuildBattleStateType::MATCHING)
		{
			/*if (!JoinState)
			{
				NotApplyCationPopup();
				data_trader::Retailer::GetInstance().RequestGetGuildBattleInfo();
				return;
			}*/

			UI_GuildMatchPopup->SetDelegate(FGuildMatchPopup::CreateLambda([this](){ this->UI_GuildMatchPopup->SetVisibility(ESlateVisibility::Hidden); }));

		}
		else
			UI_GuildMatchPopup->SetDelegate(FGuildMatchPopup::CreateLambda([this](){data_trader::Retailer::GetInstance().RequestGuildJoinBattle(); }));

		//UI_GuildMatchPopup->SetDelegate(PopupDelegate);
		UI_GuildMatchPopup->SetVisibilityPopup(true);
		UI_GuildMatchPopup->OpenPopup(GuildDoc->GetGuildbattleStateNew().current_state, BladeIIGameImpl::GetClientDataStore().GetUserGuildInfo().GuildRank);
	}


}

void UB2UIGuildMain::OnClickSeasonInfo()
{
	UB2UIManager::GetInstance()->OpenUI(UIFName::GuildSkill);
}

void UB2UIGuildMain::SendDropOut()
{
	data_trader::Retailer::GetInstance().RequestLeaveGuild();
}


void UB2UIGuildMain::SendJoinGuild()
{
	auto GuildDoc = UB2UIDocHelper::GetDocGuild();

	check(GuildDoc)

	FDateTime Time = FDateTime::UtcNow();

	int64 GuildLeaveRemainTime = GuildDoc->GetLeaveGuildTime() + (BladeIIGameImpl::GetClientDataStore().GetGuildRejoinTimeMin() * 60 * 1000);

	int64 RemainTime = (GuildLeaveRemainTime) - (Time.ToUnixTimestamp() * 1000);

	FTimespan RemainTime2 = FDateTime::FromUnixTimestamp(GuildLeaveRemainTime / 1000) - FDateTime::UtcNow();

	int32 RemainDay = RemainTime2.GetDays();
	int32 RemainHour = RemainTime2.GetHours();
	int32 RemainMin = RemainTime2.GetMinutes();

	bool IsKickedGuildID = false;

	if (GuildDoc->GetKickedGuildId() == GuildID)
		IsKickedGuildID = true;

	if (RemainTime > 0)
	{

		FText GuildNoticeText = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, (IsKickedGuildID ? TEXT("Guild_GuildJoinAfterVanishment"):TEXT("Guild_GuildJoinAfterReject"))), FText::AsNumber(RemainHour), FText::AsNumber(RemainMin));

		UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_MessageHeader_PublicNoti")), GuildNoticeText, 0, true, true, EUIMsgPopupButtonGroup::Confirm
		);

		return;
	}//추방시간 개선



	if (GuildDoc->GetGuildAcceptState() == false)
		data_trader::Retailer::GetInstance().RequestGuildAskJoinGuild(GuildID);
	else
		data_trader::Retailer::GetInstance().ReqeustAcceptInviteGuild(GuildID);

}

void UB2UIGuildMain::OnClickAni()
{
	if (BladeIIGameImpl::GetClientDataStore().GetUserGuildInfo().GuildRank == b2network::B2GuildMemberRank::NORMAL)
	{
		if (AniSlotHide)
		{
			SetPlayAnimationEvent(2);
			AniSlotHide = false;
		}
		else
		{
			SetPlayAnimationEvent(1);
			AniSlotHide = true;
		}
	}
	else
	{
		if (AniSlotHide)
		{
			SetPlayAnimationEvent(4);
			AniSlotHide = false;
		}
		else
		{
			SetPlayAnimationEvent(3);
			AniSlotHide = true;
		}
	}
}

void UB2UIGuildMain::OnClickInvite()
{	
	data_trader::Retailer::GetInstance().RequestGetGuildMemberCandidates(true);
}

void UB2UIGuildMain::OnClickGuildRanking()
{
	//임시 서버 받아서 랭킹창 열도록 해야함

	UB2UIManager::GetInstance()->OpenUI(UIFName::GuildRanking);
}

void UB2UIGuildMain::OnClickGuildWarInfo()
{
	data_trader::Retailer::GetInstance().RequestGuildBattleHistory();
}

void UB2UIGuildMain::OnNoticeTextChange(const FText& GuildNotice,ETextCommit::Type CommitMethod)
{
	if(GuildNotice.BuildSourceString().Len() > 128)
	{
		UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Notification")), FText::Format(
				BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_NoticeEditWarning")), FText::FromString(BladeIIGameImpl::GetClientDataStore().GetMyGuildInfo().GuildInfo->guild_name)), 0, true, true, EUIMsgPopupButtonGroup::Confirm);

		RTB_GuildNotice->SetText(FText::FromString(BladeIIGameImpl::GetClientDataStore().GetMyGuildInfo().GuildInfo->notice));
		return;
	}

	
	RTB_GuildNotice->SetText(GuildNotice);
	SaveNoticeState();
}

void UB2UIGuildMain::OnClickSaveNotice()
{

	data_trader::Retailer::GetInstance().RequestUpdateGuildNotice(RTB_GuildNotice->GetText().BuildSourceString());
}

void UB2UIGuildMain::OnClickGuildEdit()
{
	UB2UIManager::GetInstance()->OpenUI(UIFName::GuildEdit);
}

void UB2UIGuildMain::OnClickBTN_GuildBattleProgess()
{
	if (OV_Schedule.IsValid())
	{
		OV_Schedule->SetVisibility(OV_Schedule->GetVisibility() == ESlateVisibility::Visible ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
	}
}

void UB2UIGuildMain::OnClickBTN_Mercenary()
{
	//LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::GuildMercenary);
}

void UB2UIGuildMain::SetHideAni(UWidgetAnimation* Ani)
{
	AniSlotHide = false;
	PlayAnimation(Ani, Ani->GetEndTime());
}


void UB2UIGuildMain::CacheAssets()
{
	GET_SLOT(UB2Button, BTN_GuildListInfo)
	GET_SLOT(UB2Button, BTN_GuildRank)
	GET_SLOT(UB2Button, BTN_GuildWarInfo)
	GET_SLOT(UB2Button, BTN_GuildWarReady)
	GET_SLOT(UB2Button, BTN_GuildWar)
	GET_SLOT(UB2Button, BTN_Ani)
	GET_SLOT(UB2Button, BTN_GuildBuff)
	GET_SLOT(UB2Button, BTN_Mercenary)


	GET_SLOT(UB2UIGuildMark, UIP_GuildMark)
	if (UIP_GuildMark.IsValid())
		UIP_GuildMark->Init();

	GET_SLOT(UB2RichTextBlock, RTB_GuildName)
	GET_SLOT(UB2RichTextBlock, RTB_GuildNumber)
	GET_SLOT(UB2RichTextBlock, RTB_GuildScore)
	GET_SLOT(UB2RichTextBlock, RTB_GuildRank)
	GET_SLOT(UB2RichTextBlock, RTB_GuildNotice)

	GET_SLOT(UImage , IMG_LevelMax)

	GET_SLOT(UTextBlock, TB_GuildCombat)

	GET_SLOT(UB2Button, BTN_Invite)
	GET_SLOT(UB2Button, BTN_GuildNotice)
	GET_SLOT(UB2Button, BTM_DropOut)
	GET_SLOT(UB2Button, BTN_GuildJoin)
	GET_SLOT(UB2Button, BTN_GuildEdit)

	GET_SLOT(UCanvasPanel, CP_MyGuildMenu)
	GET_SLOT(UCanvasPanel, CP_OtherGuildMenu)
	GET_SLOT(UCanvasPanel, CP_MyGuildBtn)
	GET_SLOT(UCanvasPanel, CP_GuildJoin)

	GET_SLOT(UTextBlock , TB_GuildLevel)
	GET_SLOT(UProgressBar, PB_GuildProgress)
	GET_SLOT(UTextBlock, TB_LevelPercent)

	GET_SLOT(UPanelWidget, OV_HistoryImage)
	GET_SLOT(UPanelWidget, OV_GuildRankingImage)
	GET_SLOT(UPanelWidget, OV_GuildMemberInfoImage)


	GET_SLOT(UVerticalBox, VB_MemberList)


	GET_SLOT(UB2UIGuildMatchPopup, UI_GuildMatchPopup)
	if (UI_GuildMatchPopup.IsValid())
		UI_GuildMatchPopup->Init();

	GET_SLOT(UB2UIGuildSeasonReward, UI_GuildSeasonReward)
	if (UI_GuildSeasonReward.IsValid())
		UI_GuildSeasonReward->Init();

	GET_SLOT(UB2Button, BTN_GuildNoticePanel)
	GET_SLOT(UEditableTextBox, ETB_GuildNotice)
	GET_SLOT(UB2Button, BTN_WriteNotice)
	GET_SLOT(UB2Button, BTN_SaveNotice)

	GET_SLOT(UB2Button, BTN_GuildBattleProgess)
	GET_SLOT(UB2RichTextBlock, TB_GuildBattleState)

	GET_SLOT(UPanelWidget, OV_Schedule);

	GET_SLOT(UPanelWidget, X_NoticePanel);

	GET_SLOT(UB2UIGuildRewardBoxProgress, UIP_GuildRewardBoxProgress)
	if(UIP_GuildRewardBoxProgress.IsValid())
		UIP_GuildRewardBoxProgress->Init();

	GET_SLOT(UB2UIGuildSchedule, UIP_GuildSchedule)
	if (UIP_GuildSchedule.IsValid())
		UIP_GuildSchedule->Init();

	GET_SLOT(UB2RichTextBlock, RTB_WinCount)

	//Static Text
	GET_SLOT(UB2RichTextBlock, RTB_GuildMemberInfo)
	GET_SLOT(UTextBlock, TB_GuildJoin)

	GET_SLOT(UB2RichTextBlock, RTB_MainGuildInfo)
	GET_SLOT(UB2RichTextBlock, RTB_MainGuildRank)
	GET_SLOT(UB2RichTextBlock, RTB_MainWarInfo)

	GET_SLOT(UB2RichTextBlock, RTB_MainGuildScore)
	GET_SLOT(UB2RichTextBlock, RTB_MainGuildCombat)

	GET_SLOT(UB2RichTextBlock, RTB_MainGuildNotice)

	GET_SLOT(UTextBlock, TB_Save)
	GET_SLOT(UTextBlock, TB_Write)

	GET_SLOT(UB2RichTextBlock, RTB_GuildBuff)
	GET_SLOT(UB2RichTextBlock, RTB_GuildWarReady)

	GET_SLOT(UB2RichTextBlock, RTB_Leave)
	GET_SLOT(UB2RichTextBlock, RTB_Edit)
	GET_SLOT(UB2RichTextBlock, RTB_Invite)


}

void UB2UIGuildMain::UpdateStaticText()
{
	if (RTB_GuildMemberInfo.IsValid())
		RTB_GuildMemberInfo->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Member")));

	if (TB_GuildJoin.IsValid())
		TB_GuildJoin->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Join")));

	if (RTB_MainGuildInfo.IsValid())
		RTB_MainGuildInfo->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Member")));

	if (RTB_MainGuildRank.IsValid())
		RTB_MainGuildRank->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_GuildRank")));

	if (RTB_MainWarInfo.IsValid())
		RTB_MainWarInfo->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_BattleStatistics")));

	if (RTB_MainGuildScore.IsValid())
		RTB_MainGuildScore->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_GuildScore")));

	if (RTB_MainGuildCombat.IsValid())
		RTB_MainGuildCombat->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_CombatPower")));

	if (RTB_MainGuildNotice.IsValid())
		RTB_MainGuildNotice->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Notice")));

	if (TB_Save.IsValid())
		TB_Save->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_SaveNotice")));

	if (TB_Write.IsValid())
		TB_Write->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_WriteNotice")));

	if (RTB_GuildBuff.IsValid())
		RTB_GuildBuff->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Buff")));

	if (RTB_GuildWarReady.IsValid())
		RTB_GuildWarReady->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_BattleReady")));

	if (RTB_Leave.IsValid())
		RTB_Leave->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Leave")));

	if (RTB_Edit.IsValid())
		RTB_Edit->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Edit")));

	if (RTB_Invite.IsValid())
		RTB_Invite->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Invite")));
}

void UB2UIGuildMain::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_GuildListInfo, &UB2UIGuildMain::OnClickGuildListInfo)
	BIND_CLICK_FUNC_TO_BTN(BTM_DropOut, &UB2UIGuildMain::OnClickGuildDropOut)
	BIND_CLICK_FUNC_TO_BTN(BTN_GuildJoin, &UB2UIGuildMain::OnClickJoinGuild)
	BIND_CLICK_FUNC_TO_BTN(BTN_GuildWarReady, &UB2UIGuildMain::OnClickGuildWarReady)
	BIND_CLICK_FUNC_TO_BTN(BTN_GuildWar, &UB2UIGuildMain::OnClickGuildWar)
	BIND_CLICK_FUNC_TO_BTN(BTN_Ani, &UB2UIGuildMain::OnClickAni)
	BIND_CLICK_FUNC_TO_BTN(BTN_GuildBuff, &UB2UIGuildMain::OnClickSeasonInfo)
	BIND_CLICK_FUNC_TO_BTN(BTN_Invite, &UB2UIGuildMain::OnClickInvite)
	BIND_CLICK_FUNC_TO_BTN(BTN_GuildRank, &UB2UIGuildMain::OnClickGuildRanking)
	BIND_CLICK_FUNC_TO_BTN(BTN_GuildWarInfo, &UB2UIGuildMain::OnClickGuildWarInfo)
	BIND_CLICK_FUNC_TO_BTN(BTN_SaveNotice, &UB2UIGuildMain::OnClickSaveNotice)
	BIND_CLICK_FUNC_TO_BTN(BTN_GuildEdit, &UB2UIGuildMain::OnClickGuildEdit)
	BIND_CLICK_FUNC_TO_BTN(BTN_GuildBattleProgess, &UB2UIGuildMain::OnClickBTN_GuildBattleProgess)
	BIND_CLICK_FUNC_TO_BTN(BTN_Mercenary, &UB2UIGuildMain::OnClickBTN_Mercenary)

	ETB_GuildNotice->OnTextCommitted.AddDynamic(this,&UB2UIGuildMain::OnNoticeTextChange);
}
