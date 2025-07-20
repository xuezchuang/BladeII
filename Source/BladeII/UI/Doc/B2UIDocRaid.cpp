#include "B2UIDocRaid.h"
#include "B2ClientDataStore.h"
#include "BladeIIGameImpl.h"


#define MAX_RAID_PLAYER_NUM 4

UB2UIDocRaid::UB2UIDocRaid(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Init();
}

UB2UIDocRaid::~UB2UIDocRaid()
{

}

void UB2UIDocRaid::Init()
{
	Super::Init();

	LastRaidStep = 1;
	LastRaidType = INDEX_NONE;
	OpenedRaidJoinType = ERaidJoinType::QuickStart;
	MySlotIdx = -1;
	IsReceiveInvite = false;
	RaidErrorCode = INDEX_NONE;
	RaidOpenByRaidType.Empty();

	SET_UIDOC_FIELD_FourField(JoinSlotState, ERaidJoinSlotState::Max)
	SET_UIDOC_FIELD_FourField(JoinSlotNetId, -1)
	SET_UIDOC_FIELD_FourField(JoinSlotCharName, FText::FromString(TEXT("")))
	SET_UIDOC_FIELD_FourField(JoinSlotCharLevel, 0)
	SET_UIDOC_FIELD_FourField(JoinSlotPCClass, EPCClass::EPC_End)

	SET_UIDOC_FIELD_FourField(JoinSlotCombatPoint, 0)
	SET_UIDOC_FIELD_FourField(AccountId, -1)
	SET_UIDOC_FIELD_FourField(NetId, -1)
	SET_UIDOC_FIELD_FourField(CharName, FText::FromString(TEXT("")))
	SET_UIDOC_FIELD_FourField(CharLevel, 0)
	SET_UIDOC_FIELD_FourField(PCClass, EPCClass::EPC_End)
	SET_UIDOC_FIELD_FourField(PCLevel, 0)
	SET_UIDOC_FIELD_FourField(PCHealth, 0.f)
	SET_UIDOC_FIELD_FourField(PCMaxHealth, 0.f)
	SET_UIDOC_FIELD_FourField(PCShield, 0.f)
	SET_UIDOC_FIELD_FourField(PCInCriticalCondition, false)
	SET_UIDOC_FIELD_FourField(RaidContribution, 0)
	SET_UIDOC_FIELD_FourField(PCExitMainGame, false)
	SET_UIDOC_FIELD_FourField(IsMyGuildMember, false)
}

void UB2UIDocRaid::Destroy()
{
	Super::Destroy();
}

bool UB2UIDocRaid::IsEmpty()
{
	return (MySlotIdx == -1);
}

void UB2UIDocRaid::ClearRaidJoinData()
{	
	MySlotIdx = -1;
	IsOpenedInviteUI = false;
	RaidResultReward = nullptr;
	RaidBonusPlayerNetID = -1;
	IsReceiveInvite = false;
	
	SET_UIDOC_FIELD_FourField(JoinSlotState, ERaidJoinSlotState::Max);
	SET_UIDOC_FIELD_FourField(JoinSlotCharName, FText::FromString(TEXT("")));
	SET_UIDOC_FIELD_FourField(JoinSlotCharLevel, 0);
	SET_UIDOC_FIELD_FourField(JoinSlotNetId, -1);
	SET_UIDOC_FIELD_FourField(JoinSlotPCClass,EPCClass::EPC_End);
	SET_UIDOC_FIELD_FourField(JoinSlotIsMyGuildMember, false);
	SET_UIDOC_FIELD_FourField(IsMyGuildMember,		 false);
	/*SET_UIDOC_FIELD_FourField(JoinSlotAttackPoint, 0);
	SET_UIDOC_FIELD_FourField(JoinSlotDefensePoint,0);*/
}

void UB2UIDocRaid::ClearRaidBattleData()
{
	SET_UIDOC_FIELD_FourField(NetId, -1);
	SET_UIDOC_FIELD_FourField(CharName,				 FText::FromString(TEXT("")));
	SET_UIDOC_FIELD_FourField(CharLevel,			 0);
	SET_UIDOC_FIELD_FourField(PCClass,				 EPCClass::EPC_End);
	SET_UIDOC_FIELD_FourField(PCLevel,				 0);
	SET_UIDOC_FIELD_FourField(PCHealth,				 0.f);
	SET_UIDOC_FIELD_FourField(PCMaxHealth,			 0.f);
	SET_UIDOC_FIELD_FourField(PCShield,				 0.f);
	SET_UIDOC_FIELD_FourField(PCInCriticalCondition, false);
	SET_UIDOC_FIELD_FourField(RaidContribution,		 0);
	SET_UIDOC_FIELD_FourField(PCExitMainGame,		 false);
}

void UB2UIDocRaid::SetJoinSlotData(int32 SlotIndex, int32 InJoinSlotNetId, FText InCharName, int32 InCharLevel, EPCClass InPCClass, int32 InCombatPower, int32 InGuildIndex)
{
	//SetJoinSlotNetIdByIndex(SlotIndex, InJoinSlotNetId);
	//SetJoinSlotCharNameByIndex(SlotIndex, InCharName);
	//SetJoinSlotCharLevelByIndex(SlotIndex, InCharLevel);
	//SetJoinSlotPCClassByIndex(SlotIndex, InPCClass);
	//SetJoinSlotCombatPointByIndex(SlotIndex, InCombatPower);

	//int32 MyGuildID = BladeIIGameImpl::GetClientDataStore().GetUserGuildID();
	//bool IsMyGuild = MyGuildID > 0 ? (MyGuildID==InGuildIndex) : false;
	//SetJoinSlotIsMyGuildMemberByIndex(SlotIndex, IsMyGuild);
}

void UB2UIDocRaid::ResetJoinSlotData(int32 SlotIndex)
{
	SetJoinSlotStateByIndex(SlotIndex, ERaidJoinSlotState::Max);
	SetJoinSlotCharNameByIndex(SlotIndex, FText::FromString(TEXT("")));
	SetJoinSlotCharLevelByIndex(SlotIndex, 0);
	SetJoinSlotNetIdByIndex(SlotIndex, -1);
	SetJoinSlotPCClassByIndex(SlotIndex, EPCClass::EPC_End);
	SetJoinSlotCombatPointByIndex(SlotIndex, 0);
	SetIsMyGuildMemberByIndex(SlotIndex, false);
	SetJoinSlotIsMyGuildMemberByIndex(SlotIndex, false);
}

bool UB2UIDocRaid::IsAllEmptyJoinSlot()
{
	// 磊扁磊脚阑 力寇窍绊, 浇吩捞 厚绢乐绰瘤 眉农
	for (int32 i = 0; i < MAX_RAID_PLAYER_NUM; ++i)
	{
		if (GetMySlotIdx() == i)
			continue;

		if (GetJoinSlotStateByIndex(i) == ERaidJoinSlotState::JoinCharacter)
			return false;
	}

	return true;
}

int32 UB2UIDocRaid::GetInvitedUserNum()
{
	int32 InvitedNum = 0;
	for (int32 i = 1; i < MAX_RAID_PLAYER_NUM; ++i)
	{
		if (GetJoinSlotStateByIndex(i) == ERaidJoinSlotState::JoinCharacter)
			++InvitedNum;
	}

	return InvitedNum;

}

void UB2UIDocRaid::SetOpenedInviteUIAndChangeJoinSlotState(bool IsOpen)
{
	IsOpenedInviteUI = IsOpen;

	for (int32 i = 0; i < MAX_RAID_PLAYER_NUM; ++i)
	{
		if (IsOpen)
		{
			if (GetJoinSlotStateByIndex(i) == ERaidJoinSlotState::Invitable)
				SetJoinSlotStateByIndex(i, ERaidJoinSlotState::OpenInviteUI);
		}
		else
		{
			// 檬措甫 己傍窍瘤 臼疽促搁 惑怕 函版
			if (GetJoinSlotStateByIndex(i) == ERaidJoinSlotState::OpenInviteUI)
				SetJoinSlotStateByIndex(i, GetDefaultSlotState(ERaidJoinType::MakeRoom));
		}
	}
}

bool UB2UIDocRaid::IsJoinedUser(const FText& CharName)
{
	for (int32 i = 0; i < MAX_RAID_PLAYER_NUM; ++i)
	{
		if (GetJoinSlotCharNameByIndex(i).CompareTo(CharName) == 0)
			return true;
	}

	return false;
}

ERaidJoinSlotState UB2UIDocRaid::GetDefaultSlotState(ERaidJoinType JoinType)
{
	return (JoinType == ERaidJoinType::MakeRoom) ? (IsOpenedInviteUI ? ERaidJoinSlotState::OpenInviteUI  : ERaidJoinSlotState::Invitable) : ERaidJoinSlotState::SearchingQuickJoin;
}

bool UB2UIDocRaid::GetRaidRewardInfo(int32 Ranking, EPCClass PcClass, int32& OutItemId, int32& OutItemCount, bool& UsedRewardID)
{
	if (RaidResultReward == nullptr)
		return false;

	for (auto EachItem : RaidResultReward->raid_rewards)
	{
		if (EachItem->ranking == Ranking)
		{
			if (EachItem->item_template_id > 0) // 袍敲复 酒捞叼啊 乐栏搁 厘厚幅 扼绰 舵.
			{
				int32 TempID = 1000000 + (PCClassToInt(PcClass)*100000);
				TempID = TempID + (EachItem->item_template_id % 100000);
				OutItemId = TempID;
				OutItemCount = EachItem->item_count;
				UsedRewardID = false;
				return true;
			}
		}
	}

	// 困俊辑 府畔登瘤 臼疽促搁 捞扒 厘厚幅啊 酒囱 老馆 焊惑.
	// 扁咯档啊 何练窍搁 焊惑 沥焊啊 绝栏骨肺, rewardptr 蔼阑 曼炼且 荐 绝澜.
	// 弊贰辑 id绰 付胶磐 单捞磐俊辑 茫酒坷档废窃. count蔼篮 rewardptr俊辑 弊措肺
	const TArray<FRaidMasterData>* RaidData = BladeIIGameImpl::GetClientDataStore().GetRaidMasterDataList(GetLastRaidType());
	if (RaidData && RaidData->IsValidIndex(GetLastRaidStep() - 1))
	{
		FRaidMasterData RewardInfo = (*RaidData)[GetLastRaidStep() - 1];
		OutItemId = RewardInfo.MainRewardID;
		OutItemCount = 0;
		UsedRewardID = true;

		b2network::B2RewardPtr MainRewardItem = RaidResultReward->main_reward;
		if (MainRewardItem && MainRewardItem->raw_reward)
		{
			if (MainRewardItem->raw_reward->id > 0)
			{
				OutItemCount = MainRewardItem->raw_reward->count;
			}
		}

		return true;
	}

	return false;
}

bool UB2UIDocRaid::GetRaidBonusRewardInfo(int32 BonusUserRank, EPCClass UserPCClass, int32& OutItemId, int32& OutItemCount, bool& UsedRewardID)
{
	if (RaidResultReward == nullptr)
		return false;

	for (auto EachItem : RaidResultReward->raid_rewards)
	{
		if (EachItem->ranking == BonusUserRank)
		{
			if (EachItem->item_template_id > 0) // 袍敲复 酒捞叼啊 乐栏搁 厘厚幅 扼绰 舵.
			{
				int32 TempID = 1000000 + (PCClassToInt(UserPCClass) * 100000);
				TempID = TempID + (EachItem->item_template_id % 100000);
				OutItemId = TempID;
				OutItemCount = EachItem->item_count;
				UsedRewardID = false;
				return true;
			}
		}
	}

	// 困俊辑 府畔登瘤 臼疽促搁 捞扒 厘厚幅啊 酒囱 老馆 焊惑.
	// 扁咯档啊 何练窍芭唱, 郴啊 焊呈胶 措惑磊啊 酒匆版快俊 焊惑 沥焊啊 绝栏骨肺, rewardptr 蔼阑 曼炼且 荐 绝澜.
	// 弊贰辑 id绰 付胶磐 单捞磐俊辑 茫酒坷档废窃. count蔼篮 rewardptr俊辑 弊措肺
	const TArray<FRaidMasterData>* RaidData = BladeIIGameImpl::GetClientDataStore().GetRaidMasterDataList(GetLastRaidType());
	if (RaidData && RaidData->IsValidIndex(GetLastRaidStep() - 1))
	{
		FRaidMasterData RewardInfo = (*RaidData)[GetLastRaidStep() - 1];
		OutItemId = RewardInfo.MainRewardID;
		OutItemCount = 0;
		UsedRewardID = true;

		b2network::B2RewardPtr BonusRewardItem = RaidResultReward->main_bonus_reward;
		if (BonusRewardItem && BonusRewardItem->raw_reward)
		{
			if (BonusRewardItem->raw_reward->id > 0)
			{
				OutItemCount = BonusRewardItem->raw_reward->count;
			}
		}

		return true;
	}

	return false;
}

int32 UB2UIDocRaid::GetBattleSlotIndex(uint32 NetID)
{
	for (int32 i = 0; i < MAX_RAID_PLAYER_NUM; ++i)
	{
		if (NetID == GetNetIdByIndex(i))
			return i;
	}
	
	return -1;
}

int32 UB2UIDocRaid::GetJoinSlotIndex(uint32 NetID)
{
	for (int32 i = 0; i < MAX_RAID_PLAYER_NUM; ++i)
	{
		if (NetID == GetJoinSlotNetIdByIndex(i))
			return i;
	}

	return -1;
}

void UB2UIDocRaid::SetGetRaidPtr(FB2ResponseGetRaidPtr ResponsePtr)
{
	m_ptrGetRaidPtr = ResponsePtr;
}

FB2ResponseGetRaidPtr UB2UIDocRaid::GetGetRaidPtr()
{
	return m_ptrGetRaidPtr;
}

bool UB2UIDocRaid::IsPanelty()
{
	if (m_ptrGetRaidPtr)
	{
		return GetPaneltyTime() > 0;
	}
	return false;
}

FTimespan UB2UIDocRaid::GetPaneltyTime()
{
	FTimespan RemainTime(0);
	if (m_ptrGetRaidPtr)
	{
		FDateTime PaneltyTime = FDateTime::FromUnixTimestamp(m_ptrGetRaidPtr->penalty_remove_time / 1000);
		FDateTime TimeNow = FDateTime::UtcNow();
		RemainTime = PaneltyTime - TimeNow;
	}
	return RemainTime;
}



void UB2UIDocRaid::SetRaidInvitationInfo(const FString& Inviter, const FString& RoomId, int32 MapId, int32 Step, const FDateTime& DateTime)
{
	RaidInvitationInfo.InviterName		= Inviter;
	RaidInvitationInfo.InvitedRoomId	= RoomId;
	RaidInvitationInfo.RaidType			= MapId;
	RaidInvitationInfo.RaidStep			= Step;
	RaidInvitationInfo.InviteTime		= DateTime;
	RaidInvitationInfo.RoomType			= b2network::B2RoomType::BRT_Raid;
}

void UB2UIDocRaid::ClearRaidInvitationInfo()
{
	RaidInvitationInfo.InviterName = TEXT("");
	RaidInvitationInfo.InvitedRoomId = TEXT("");
	RaidInvitationInfo.RaidType = INDEX_NONE;
	RaidInvitationInfo.RaidStep = INDEX_NONE;
	RaidInvitationInfo.InviteTime = FDateTime(0) ;
	RaidInvitationInfo.RoomType = b2network::B2RoomType::BRT_Raid;
}

void UB2UIDocRaid::SetPeriodInfos(TArray<b2network::B2MatchOpenPeriodInfoPtr>& PeriodInfo)
{
	RaidOpenPeriodInfo = PeriodInfo;

	// 坷锹咯何 汲沥
	TArray<b2network::B2MatchOpenPeriodInfoPtr> CurrentDayOpenTimes;
	bool bIsNoCheckTypeRaidOpen = BladeIIGameImpl::GetClientDataStore().GetCurrentModeOpenTimes(RaidOpenPeriodInfo, CurrentDayOpenTimes);
	SetRaidOpen(bIsNoCheckTypeRaidOpen);

	RaidOpenByRaidType.Empty();
	for (auto i = 0; i < static_cast<int32>(ERaidBossIndex::BOSS_MAX); i++)
	{
		TArray<b2network::B2MatchOpenPeriodInfoPtr> OpenTimes;
		bool bIsOpen = BladeIIGameImpl::GetClientDataStore().GetRaidModeOpenTimes(RaidOpenPeriodInfo, i, OpenTimes);
		RaidOpenByRaidType.Add(bIsOpen);
	}
}

bool UB2UIDocRaid::GetRaidOpenByType(int32 nType)
{
	if (RaidOpenByRaidType.IsValidIndex(nType))
	{
		return RaidOpenByRaidType[nType];
	}

	return false;
}

void UB2UIDocRaid::SetPartyRetryInfo(b2network::B2EndpointPtr EndPoint, const FString& _NextRoomId)
{
	ChannelEndpoint = EndPoint;
	NextRoomId = _NextRoomId;
}

void UB2UIDocRaid::ClearPartyRetryInfo()
{
	ChannelEndpoint = nullptr;
	NextRoomId = TEXT("");
}

bool UB2UIDocRaid::HasValidPartyRetryInfo()
{
	return ChannelEndpoint && NextRoomId.Len() > 0; 
}

#undef MAX_RAID_PLAYER_NUM