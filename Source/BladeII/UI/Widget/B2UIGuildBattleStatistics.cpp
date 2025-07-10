#include "B2UIGuildBattleStatistics.h"
#include "B2UIGuildMark.h"
#include "B2UITeamEntryCharacter.h"
#include "B2UIGuildInfoPart.h"
#include "B2UIRewardEventSet.h"
#include "B2UIManager.h"

void UB2UIGuildStatistics_BattleResult::SetOpponentUserName(const FText& ToSet)
{
	if (TB_OpponentUserName.IsValid())
		TB_OpponentUserName->SetText(ToSet);
}

void UB2UIGuildStatistics_BattleResult::SetBattleNum(FText InBattleNumber)
{
	if (TB_BattleNum.IsValid())
		TB_BattleNum->SetText(InBattleNumber);
}

void UB2UIGuildStatistics_BattleResult::SetNumAcquiredMedals(int32 ToSet)
{
	for (int32 i = 0; i < AcquiredMedals.Num(); ++i)
	{
		if (AcquiredMedals[i].IsValid())
		{
			//IMG_AcquiredMedals[i]->SetVisibility(i < ToSet ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);

			i < ToSet ? AcquiredMedals[i]->OnMedal() : AcquiredMedals[i]->OffMedal();
		}
	}
}

void UB2UIGuildStatistics_BattleResult::SetAllyInfoAndPowerIndex(bool InAlly, int32 InPowerIndex)
{
	if (O_AllyMark.IsValid())
		O_AllyMark->SetVisibility(InAlly ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);

	if (O_EnemyIndex.IsValid())
		O_EnemyIndex->SetVisibility(InAlly ? ESlateVisibility::Hidden : ESlateVisibility::HitTestInvisible);

	if (TB_PowerIndex.IsValid())
		TB_PowerIndex->SetText(FText::AsNumber(InPowerIndex));
}

void UB2UIGuildStatistics_BattleResult::SetAttackEmpty(bool InIsEmpty)
{
	if(O_UserInfo.IsValid())
		O_UserInfo->SetVisibility(InIsEmpty ? ESlateVisibility::Hidden : ESlateVisibility::HitTestInvisible);
	if (O_Medal.IsValid())
		O_Medal->SetVisibility(InIsEmpty ? ESlateVisibility::Hidden : ESlateVisibility::HitTestInvisible);
	if(O_AttackEmpty.IsValid())
		O_AttackEmpty->SetVisibility(InIsEmpty ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
}

void UB2UIGuildStatistics_BattleResult::CacheAssets()
{
	GET_SLOT(UTextBlock, TB_BattleNum);
	GET_SLOT(UTextBlock, TB_OpponentUserName);
	/*IMG_AcquiredMedals.AddDefaulted(4);
	GET_SLOT_BYNAME(UImage, IMG_AcquiredMedals[0], IMG_AcquiredMedals01);
	GET_SLOT_BYNAME(UImage, IMG_AcquiredMedals[1], IMG_AcquiredMedals02);
	GET_SLOT_BYNAME(UImage, IMG_AcquiredMedals[2], IMG_AcquiredMedals03);
	GET_SLOT_BYNAME(UImage, IMG_AcquiredMedals[3], IMG_AcquiredMedals04);*/

	AcquiredMedals.AddDefaulted(4);

	GET_SLOT_BYNAME(UB2UIGuildInfoMedal, AcquiredMedals[0], UIP_GuildMedal01)
	GET_SLOT_BYNAME(UB2UIGuildInfoMedal, AcquiredMedals[1], UIP_GuildMedal02)
	GET_SLOT_BYNAME(UB2UIGuildInfoMedal, AcquiredMedals[2], UIP_GuildMedal03)
	GET_SLOT_BYNAME(UB2UIGuildInfoMedal, AcquiredMedals[3], UIP_GuildMedal04)


	for (auto& El : AcquiredMedals)
	{
		El->Init();
	}


	GET_SLOT(UPanelWidget, O_AllyMark)
	GET_SLOT(UPanelWidget, O_EnemyIndex)
	GET_SLOT(UTextBlock, TB_PowerIndex)


	GET_SLOT(UPanelWidget, O_UserInfo)
	GET_SLOT(UPanelWidget, O_Medal)
	GET_SLOT(UPanelWidget, O_AttackEmpty)

}

void UB2UIGuildStatistics_UserRecords::SetUserName(const FText& ToSet)
{
	if (TB_UserName.IsValid())
		TB_UserName->SetText(ToSet);
}


void UB2UIGuildStatistics_UserRecords::SetAllyInfoAndPowerIndex(bool InAlly, int32 InPowerIndex)
{

	if (O_AllyMark.IsValid())
		O_AllyMark->SetVisibility(InAlly ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);

	if (O_EnemyMark.IsValid())
		O_EnemyMark->SetVisibility(InAlly ? ESlateVisibility::Hidden : ESlateVisibility::HitTestInvisible);

	if (TB_PowerIndex.IsValid())
		TB_PowerIndex->SetText(FText::AsNumber(InPowerIndex));
}

void UB2UIGuildStatistics_UserRecords::AddBattleResult(bool InbAlly, int32 InPowerIndex,const FText& UserName, int32 AcquiredMedalsFromUser)
{
	if (AddedBattleResult >= UIP_BattleResults.Num())
		return;

	if (UIP_BattleResults[AddedBattleResult].IsValid())
	{
		UIP_BattleResults[AddedBattleResult]->SetAttackEmpty(false);
		UIP_BattleResults[AddedBattleResult]->SetOpponentUserName(UserName);
		UIP_BattleResults[AddedBattleResult]->SetNumAcquiredMedals(AcquiredMedalsFromUser);
		//UIP_BattleResults[AddedBattleResult]->SetBattleNum(AddedBattleResult + 1);
		UIP_BattleResults[AddedBattleResult]->SetAllyInfoAndPowerIndex(!InbAlly, InPowerIndex);			//반대로 넣어주자
		++AddedBattleResult;
	}
}

void UB2UIGuildStatistics_UserRecords::CacheAssets()
{
	GET_SLOT(UTextBlock, TB_UserName);

	UIP_BattleResults.AddDefaulted(2);

	GET_SLOT_BYNAME(UB2UIGuildStatistics_BattleResult, UIP_BattleResults[0], UIP_BattleResult01);
	GET_SLOT_BYNAME(UB2UIGuildStatistics_BattleResult, UIP_BattleResults[1], UIP_BattleResult02);
	int32 i = 1;
	for (auto& UIP : UIP_BattleResults)
	{
		if (UIP.IsValid())
		{
			UIP->Init();
			UIP->SetAttackEmpty(true);
			if (i == 1)
				UIP->SetBattleNum(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Map_Info_Attack1")));
			else
				UIP->SetBattleNum(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Map_Info_Attack2")));
		}

		++i;
	}	


	GET_SLOT(UTextBlock, TB_PowerIndex)
	GET_SLOT(UPanelWidget, O_AllyMark)
	GET_SLOT(UPanelWidget, O_EnemyMark)
}



void UB2UIGuildStatistics_UserRecordsList::SetGuildMark(bool bLeftGuild, int32 Pattern, int32 PatternColor, int32 BG, int32 BGColor)
{
	GuildMarkData& GuildMark = bLeftGuild ? LeftGuildMark : RightGuildMark;

	GuildMark.BG = BG;
	GuildMark.BGColor = BGColor;
	GuildMark.Pattern = Pattern;
	GuildMark.PatternColor = PatternColor;
}

UB2UIGuildStatistics_UserRecords* UB2UIGuildStatistics_UserRecordsList::AddUserRecords(bool bLeftGuild)
{
	if (!UserRecordsDefaultUI)
		return nullptr;

	UVerticalBox* List = bLeftGuild ? VB_LeftGuild_UserList.Get() : VB_RightGuild_UserList.Get();
	if (List)
	{
		auto* CreatedWidget = CreateWidget<UB2UIGuildStatistics_UserRecords>(GetWorld(), UserRecordsDefaultUI);
		if (CreatedWidget)
		{
			List->AddChild(CreatedWidget);
			CreatedWidget->Init();
			/*GuildMarkData& GuildMark = bLeftGuild ? LeftGuildMark : RightGuildMark;
			CreatedWidget->SetGuildMark(GuildMark.Pattern, GuildMark.PatternColor, GuildMark.BG, GuildMark.BGColor);*/

			return CreatedWidget;
		}
	}

	return nullptr;
}

void UB2UIGuildStatistics_UserRecordsList::SetData(bool bLeftGuild, const TArray<b2network::B2GuildBattleMemberPtr>& MemberData, const TArray<b2network::B2GuildBattleAttackPtr>& AttackData)
{
	TMap<int64, UB2UIGuildStatistics_UserRecords*> MembersRecordsCachingMap;

	for (auto& Member : MemberData)
	{
		auto* UserRecord = AddUserRecords(bLeftGuild);
		check(UserRecord);

		UserRecord->SetUserName(FText::FromString(Member->nickname));
		UserRecord->SetAllyInfoAndPowerIndex(bLeftGuild, Member->power_index);
		
		MembersRecordsCachingMap.Add(Member->account_id, UserRecord);
	}	

	for (auto& Attack : AttackData)
	{
		auto** UserRecordUI = MembersRecordsCachingMap.Find(Attack->account_id);
		if (UserRecordUI && *UserRecordUI)
		{
			(*UserRecordUI)->AddBattleResult(bLeftGuild , Attack->target_power_index, FText::FromString(Attack->target_nickname), Attack->obtain_medal);
		}
	}
}

void UB2UIGuildStatistics_UserRecordsList::CacheAssets()
{
	GET_SLOT(UVerticalBox, VB_LeftGuild_UserList);
	GET_SLOT(UVerticalBox, VB_RightGuild_UserList);

	if (VB_LeftGuild_UserList.IsValid())
		VB_LeftGuild_UserList->ClearChildren();

	if (VB_RightGuild_UserList.IsValid())
		VB_RightGuild_UserList->ClearChildren();
}

void UB2UIGuildStatistics_Statistics::SetData(const FB2ResponseGuildBattleTurnResultPtr& DataPtr)
{
	SetNumOfAttacks(true, DataPtr->my_guild->attacks.Num());
	SetNumOfAttacks(false, DataPtr->target_guild->attacks.Num());

	TArray<b2network::B2GuildBattleMemberPtr> TotalMemberPtr = DataPtr->my_guild->members;
	TArray<b2network::B2GuildBattleAttackPtr> TotalAttackPtr = DataPtr->my_guild->attacks;
	TotalAttackPtr.Append(DataPtr->target_guild->attacks);
	TotalMemberPtr.Append(DataPtr->target_guild->members);

	for (auto& Member : TotalMemberPtr)
	{
		if (Member->account_id == DataPtr->mvp_account_id)
		{
			SetMVPUser(FText::FromString(Member->nickname), Member->total_power, Member->total_power, Member->entry);	// jwyim power
			break;
		}
	}

	int32 MyGuildVictoryCount = 0;
	for (auto& AttackInfo : TotalAttackPtr)
	{
		if (AttackInfo->account_id == DataPtr->mvp_account_id)
		{
			AddMVPBattleResult(FText::FromString(AttackInfo->target_nickname), AttackInfo->obtain_medal);
		}

		if (AttackInfo->obtain_medal > 0)
			++MyGuildVictoryCount;
	}

	int32 OpponentGuildVictoryCount = 0;
	for (auto& AttackInfo : DataPtr->target_guild->attacks)
	{
		if (AttackInfo->obtain_medal > 0)
			++OpponentGuildVictoryCount;
	}

	SetNumOfVictories(true, MyGuildVictoryCount);
	SetNumOfVictories(false, OpponentGuildVictoryCount);
}

void UB2UIGuildStatistics_Statistics::SetNumOfAttacks(bool bLeft, int32 ToSet)
{
	TWeakObjectPtr<UTextBlock>& TB = bLeft ? TB_LeftGuild_NumOfAttack : TB_RightGuild_NumOfAttack;

	if (TB.IsValid())		
		TB->SetText(FText::AsNumber(ToSet));
}

void UB2UIGuildStatistics_Statistics::SetNumOfVictories(bool bLeft, int32 ToSet)
{
	TWeakObjectPtr<UTextBlock>& TB = bLeft ? TB_LeftGuild_NumOfVictories : TB_RightGuild_NumOfVictories;

	if (TB.IsValid())
		TB->SetText(FText::AsNumber(ToSet));
}

void UB2UIGuildStatistics_Statistics::SetMVPUser(const FText& UserName, int32 TotalAttack, int32 TotalDefense, const b2network::B2GuildBattleEntryPtr& EntryDataPtr)
{
	if (TB_MVPUserName.IsValid())
		TB_MVPUserName->SetText(UserName);

	if (TB_TotalAttack_Value.IsValid())
		TB_TotalAttack_Value->SetText(FText::AsNumber(TotalAttack));

	if (TB_TotalDefense_Value.IsValid())
		TB_TotalDefense_Value->SetText(FText::AsNumber(TotalDefense));

	// jwyim GuildEntry
	//check(UIP_MVP_Entry_Charaters.Num() == EntryDataPtr->character_slots.Num());
	//for (int32 CharIdx = 0; CharIdx < EntryDataPtr->character_slots.Num(); ++CharIdx)
	//{
	//	if (UIP_MVP_Entry_Charaters[CharIdx].IsValid())
	//	{
	//		UIP_MVP_Entry_Charaters[CharIdx]->SetEntryCharacter(SvrToCliPCClassType(EntryDataPtr->character_slots[CharIdx]->character_type), EntryDataPtr->character_slots[CharIdx]->character_level);
	//	}
	//}
}

void UB2UIGuildStatistics_Statistics::AddMVPBattleResult(const FText& UserName, int32 AcquiredMedalsFromUser)
{
	if (AddedBattleResult >= UIP_BattleResults.Num())
		return;

	if (UIP_BattleResults[AddedBattleResult].IsValid())
	{
		UIP_BattleResults[AddedBattleResult]->SetAttackEmpty(false);
		UIP_BattleResults[AddedBattleResult]->SetOpponentUserName(UserName);
		UIP_BattleResults[AddedBattleResult]->SetNumAcquiredMedals(AcquiredMedalsFromUser);
		//UIP_BattleResults[AddedBattleResult]->SetBattleNum(AddedBattleResult + 1);
		++AddedBattleResult;
	}
}

void UB2UIGuildStatistics_Statistics::CacheAssets()
{
	GET_SLOT(UTextBlock, TB_LeftGuild_NumOfAttack);
	GET_SLOT(UTextBlock, TB_LeftGuild_NumOfVictories);
	GET_SLOT(UTextBlock, TB_RightGuild_NumOfAttack);
	GET_SLOT(UTextBlock, TB_RightGuild_NumOfVictories);

	GET_SLOT(UTextBlock, TB_MVPUserName);
	GET_SLOT(UTextBlock, TB_TotalAttack_Value);
	GET_SLOT(UTextBlock, TB_TotalDefense_Value);

	UIP_MVP_Entry_Charaters.AddDefaulted(4);
	GET_SLOT_BYNAME(UB2UITeamEntryCharacter, UIP_MVP_Entry_Charaters[0], UIP_Entry_Char01);
	GET_SLOT_BYNAME(UB2UITeamEntryCharacter, UIP_MVP_Entry_Charaters[1], UIP_Entry_Char02);
	GET_SLOT_BYNAME(UB2UITeamEntryCharacter, UIP_MVP_Entry_Charaters[2], UIP_Entry_Char03);
	GET_SLOT_BYNAME(UB2UITeamEntryCharacter, UIP_MVP_Entry_Charaters[3], UIP_Entry_Char04);
	for (auto& UIP : UIP_MVP_Entry_Charaters)
	{
		if (UIP.IsValid())
			UIP->Init();
	}

	UIP_BattleResults.AddDefaulted(2);
	GET_SLOT_BYNAME(UB2UIGuildStatistics_BattleResult, UIP_BattleResults[0], UIP_BattleResult01);
	GET_SLOT_BYNAME(UB2UIGuildStatistics_BattleResult, UIP_BattleResults[1], UIP_BattleResult02);
	for (auto& UIP : UIP_BattleResults)
	{
		if (UIP.IsValid())
		{
			UIP->Init();
			UIP->SetVisibility(ESlateVisibility::Hidden);
		}
	}
};


void UB2UIGuildStatistics_GuildInfos::SetGuildInfo(bool bLeftGuild
	, const FText& GuildName,int32 Point,int32 GuildExp
	, int32 Pattern, int32 PatternColor, int32 BG, int32 BGColor)
{
	TWeakObjectPtr<UB2UIGuildInfoPart5>& UIP = bLeftGuild ? UIP_LeftGuildInfo : UIP_RightGuildInfo;

	if (UIP.IsValid())
	{
		UIP->SetGuildInfo(GuildName,  Point, GuildExp);			//추후에 경험치 추가 woong
		UIP->SetGuildMark(Pattern, PatternColor, BG, BGColor);
	}
}

void UB2UIGuildStatistics_GuildInfos::SetGuildBattleResult(int32 InResult)
{

	O_Victory->SetVisibility(ESlateVisibility::Hidden);
	O_Lose->SetVisibility(ESlateVisibility::Hidden);
	O_Draw->SetVisibility(ESlateVisibility::Hidden);

	if (InResult == b2network::B2GuildBattleResultType::BGBRT_WIN)
	{
		O_Victory->SetVisibility(ESlateVisibility::Visible);
	}
	else if (InResult == b2network::B2GuildBattleResultType::BGBRT_LOSE)
	{
		O_Lose->SetVisibility(ESlateVisibility::Visible);
	}
	else if (InResult == b2network::B2GuildBattleResultType::BGBRT_DRAW)
	{
		O_Draw->SetVisibility(ESlateVisibility::Visible);
	}
}

void UB2UIGuildStatistics_GuildInfos::CacheAssets()
{
	GET_SLOT(UB2UIGuildInfoPart5, UIP_LeftGuildInfo);
	if (UIP_LeftGuildInfo.IsValid())
		UIP_LeftGuildInfo->Init();

	GET_SLOT(UB2UIGuildInfoPart5, UIP_RightGuildInfo);
	if (UIP_RightGuildInfo.IsValid())
		UIP_RightGuildInfo->Init();

	GET_SLOT(UPanelWidget, O_Victory);
	GET_SLOT(UPanelWidget, O_Lose);
	GET_SLOT(UPanelWidget, O_Draw);
}

void UB2UIGuildStatistics_Rewards::SetRewardItems(const TArray<b2network::B2RewardPtr>& RewardList)
{
	if (!RewardIconDefaultUI)
		return;

	if (HB_RewardList.IsValid())
	{
		for (auto& Reward : RewardList)
		{
			auto* CreatedWidget = CreateWidget<UB2UIRewardIcon>(GetWorld(), RewardIconDefaultUI);
			if (CreatedWidget)
			{
				HB_RewardList->AddChild(CreatedWidget);
				CreatedWidget->UpdateRewardIcon(Reward);
			}
		}		
	}
}

void UB2UIGuildStatistics_Rewards::CacheAssets()
{
	GET_SLOT(UHorizontalBox, HB_RewardList);
}

void UB2UIGuildStatistics_Main::Init()
{
	Super::Init();

	SetViewState(EGuildStatisticsStates::BattleRecordView);
}

void UB2UIGuildStatistics_Main::SetViewState(UB2UIGuildStatistics_Main::EGuildStatisticsStates ToSet)
{
	ViewStates = ToSet;

	if (UIP_UserRecordsList.IsValid())
		UIP_UserRecordsList->SetVisibility(ToSet == UB2UIGuildStatistics_Main::EGuildStatisticsStates::BattleRecordView ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);


	if (VB_Statistics.IsValid())
		VB_Statistics->SetVisibility(ToSet != UB2UIGuildStatistics_Main::EGuildStatisticsStates::RewardView ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

void UB2UIGuildStatistics_Main::CacheAssets()
{
	GET_SLOT(UB2Button, BTN_Confirm);

	GET_SLOT(UVerticalBox, VB_Statistics);


	GET_SLOT(UB2UIGuildStatistics_GuildInfos, UIP_GuildInfos);
	if (UIP_GuildInfos.IsValid())
		UIP_GuildInfos->Init();
	GET_SLOT(UB2UIGuildStatistics_UserRecordsList, UIP_UserRecordsList);
	if (UIP_UserRecordsList.IsValid())
		UIP_UserRecordsList->Init();

}

void UB2UIGuildStatistics_Main::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Confirm, &UB2UIGuildStatistics_Main::OnClickedConfirmBTN);

}

void UB2UIGuildStatistics_Main::CloseWidgetDelegate()
{
	OnClickedConfirmBTN();
}

void UB2UIGuildStatistics_Main::OnClickedConfirmBTN()
{
	auto* UIManager = UB2UIManager::GetInstance();
	if (UIManager)
		UIManager->CloseUI(UIFName::GuildBattleStatistics);
}

void UB2UIGuildStatistics_Main::OnClickedStatisticsBTN()
{
	SetViewState(EGuildStatisticsStates::StatisticsView);
}

void UB2UIGuildStatistics_Main::OnClickedRecordsBTN()
{
	SetViewState(EGuildStatisticsStates::BattleRecordView);
}

void UB2UIGuildStatistics_Main::SetData(const FB2ResponseGuildBattleTurnResultPtr& ResultPtr)
{
	if (UIP_GuildInfos.IsValid())
	{
		UIP_GuildInfos->SetGuildInfo(
			true, FText::FromString(ResultPtr->my_guild->guild_name),  ResultPtr->my_guild->obtain_medal, 0,
			ResultPtr->my_guild->mark_index, ResultPtr->my_guild->mark_color, ResultPtr->my_guild->mark_bg_index, ResultPtr->my_guild->mark_bg_color);



		UIP_GuildInfos->SetGuildInfo(
			false, FText::FromString(ResultPtr->target_guild->guild_name),  ResultPtr->target_guild->obtain_medal, 0,
			ResultPtr->target_guild->mark_index, ResultPtr->target_guild->mark_color, ResultPtr->target_guild->mark_bg_index, ResultPtr->target_guild->mark_bg_color);

		UIP_GuildInfos->SetGuildBattleResult(ResultPtr->battle_result);
	}

	if (UIP_UserRecordsList.IsValid())
	{
		UIP_UserRecordsList->SetGuildMark(true, ResultPtr->my_guild->mark_index, ResultPtr->my_guild->mark_color, ResultPtr->my_guild->mark_bg_index, ResultPtr->my_guild->mark_bg_color);
		UIP_UserRecordsList->SetData(true, ResultPtr->my_guild->members, ResultPtr->my_guild->attacks);

		UIP_UserRecordsList->SetGuildMark(false, ResultPtr->target_guild->mark_index, ResultPtr->target_guild->mark_color, ResultPtr->target_guild->mark_bg_index, ResultPtr->target_guild->mark_bg_color);
		UIP_UserRecordsList->SetData(false, ResultPtr->target_guild->members, ResultPtr->target_guild->attacks);
	}

}