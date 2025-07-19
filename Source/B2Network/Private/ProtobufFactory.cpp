#include "ProtobufFactory.h"
#include "B2NetworkPlugin.h"

#include "Config.h"
#include "Exception.h"
#include "LogManager.h"
#include "StringHash.h"


using namespace b2network;

#if B2_BUILD_LEVEL < B2_BUILD_LEVEL_QA // !B2_BUILD_LEVEL_LIVE, !!B2_BUILD_LEVEL_QA
static auto logger = LogManager::GetLogger("ProtobufFactory");
#endif

//google::protobuf::DynamicMessageFactory ProtobufFactory::s_factory;
std::unordered_map<uint32_t, ProtobufFactory::Metadata> ProtobufFactory::s_metadataMap;
std::unordered_map<uint32_t, uint32_t> ProtobufFactory::s_requestToResponseMap;
std::unordered_map<uint32_t, uint32_t> ProtobufFactory::s_responseToRequestMap;

std::string ProtobufFactory::s_masterDataHash;

void ProtobufFactory::Initialize()
{
    B2NETWORK_LOG_INFO(logger, "Initializing...");

#define REGISTER_REQUEST(server_type, category_name, protocol_name)		\
    ProtobufFactory::Register(ProtobufFactory::server_type,				\
        new b2::protocol::category_name::protocol_name##Request(),		\
        new b2::protocol::category_name::protocol_name##Response());

#define REGISTER_COMMAND(server_type, category_name, protocol_name)		\
    ProtobufFactory::Register(ProtobufFactory::server_type,				\
        ProtobufFactory::Command, new b2::protocol::category_name::protocol_name##Command());

#define REGISTER_NOTIFY(server_type, category_name, protocol_name)		\
    ProtobufFactory::Register(ProtobufFactory::server_type,				\
        ProtobufFactory::Notify, new b2::protocol::category_name::protocol_name##Notify());

	s_masterDataHash = "";

    // session server
    //

    REGISTER_REQUEST(Session, session, Ping);
	REGISTER_REQUEST(Session, session, GetServerVersion);
    REGISTER_REQUEST(Session, session, SignInSession);
    REGISTER_REQUEST(Session, session, GetAccount);
	REGISTER_REQUEST(Session, session, FindAccount);
	REGISTER_REQUEST(Session, session, GetAccountItem);
	REGISTER_REQUEST(Session, session, GetMasterData);
	REGISTER_REQUEST(Session, session, GetLatestEventData);
	REGISTER_REQUEST(Session, session, DismantleItems);
    REGISTER_REQUEST(Session, session, SellItems);
	REGISTER_REQUEST(Session, session, SellConsumableItem);
	REGISTER_REQUEST(Session, session, CheckExChangeCount);
	REGISTER_REQUEST(Session, session, GetItemRandomQuality);
	REGISTER_REQUEST(Session, session, GenerateItemRandomQuality);
	REGISTER_REQUEST(Session, session, ApplyItemRandomQuality);
	REGISTER_REQUEST(Session, session, DiscardItemRandomQuality);
	REGISTER_REQUEST(Session, session, GetFactory);
	REGISTER_REQUEST(Session, session, MakeFactoryItem);
	REGISTER_REQUEST(Session, session, UnsealBox);
	REGISTER_REQUEST(Session, session, ExChangeSystem);
    REGISTER_REQUEST(Session, session, EquipItems);
	REGISTER_REQUEST(Session, session, UnequipItems);
    REGISTER_REQUEST(Session, session, LockItems);
    REGISTER_REQUEST(Session, session, UnlockItems);
    REGISTER_REQUEST(Session, session, ExtendInventory);
    REGISTER_REQUEST(Session, session, LevelupItem);
	REGISTER_REQUEST(Session, session, EnhanceItem);
	REGISTER_REQUEST(Session, session, ExchangeItemEnhanceLevel);
	REGISTER_REQUEST(Session, session, OpenSealSlotItem);
	REGISTER_REQUEST(Session, session, CarveSealItem);
	REGISTER_REQUEST(Session, session, SurpassItem);
	REGISTER_REQUEST(Session, session, UpgradeItem);
    REGISTER_REQUEST(Session, session, ComposeItem);
	REGISTER_REQUEST(Session, session, ChangeItemCharacterType);
	REGISTER_REQUEST(Session, session, GetChangeItemCharacterTypeCost);
    REGISTER_REQUEST(Session, session, StartStage);
    REGISTER_REQUEST(Session, session, ClearStage);
	REGISTER_REQUEST(Session, session, FailStage);
	REGISTER_REQUEST(Session, session, SweepStage);
    REGISTER_REQUEST(Session, session, Resurrection);
    REGISTER_REQUEST(Session, session, CheckBladePointCharge);
	REGISTER_REQUEST(Session, session, GetItemSurpassCost);
    REGISTER_REQUEST(Session, session, GetItemUpgradeCost);
    REGISTER_REQUEST(Session, session, GetItemComposeCost);
    REGISTER_REQUEST(Session, session, GetActInfo);
    REGISTER_REQUEST(Session, session, GetStageInfo);
	REGISTER_REQUEST(Session, session, GetMatchStatus);
    REGISTER_REQUEST(Session, session, Matchmaking);
    REGISTER_REQUEST(Session, session, CancelMatchmaking);
	REGISTER_REQUEST(Session, session, RaidMatchmaking);
	REGISTER_REQUEST(Session, session, MatchResult);
	REGISTER_REQUEST(Session, session, LevelUpSkill);
	REGISTER_REQUEST(Session, session, ResetSkill);
	REGISTER_REQUEST(Session, session, SetUsingSkill);
	REGISTER_REQUEST(Session, session, BuySkillPoint);
    REGISTER_REQUEST(Session, session, GiveNickname);
	REGISTER_REQUEST(Session, session, UpdateNickname);
    REGISTER_REQUEST(Session, session, GiveCharacterName);
    REGISTER_REQUEST(Session, session, Invitation);
    REGISTER_REQUEST(Session, session, AcceptInvitation);
    REGISTER_REQUEST(Session, session, DeclineInvitation);
	REGISTER_REQUEST(Session, session, ActClearReward);
	REGISTER_REQUEST(Session, session, GetTeamBattleStatus);
	REGISTER_REQUEST(Session, session, UpdateTeamBattleEntry);
	REGISTER_REQUEST(Session, session, EnhanceTeamBattleFormation);
	REGISTER_REQUEST(Session, session, TeamMatchmaking);
	REGISTER_REQUEST(Session, session, TeamMatchResult);
	REGISTER_REQUEST(Session, session, EnhanceRankNode);
	REGISTER_REQUEST(Session, session, GetMailList);
	REGISTER_REQUEST(Session, session, OpenMail);
	REGISTER_REQUEST(Session, session, OpenAllMail);
	REGISTER_REQUEST(Session, session, GetNewMailCount);
	REGISTER_REQUEST(Session, session, AskFriend);
	REGISTER_REQUEST(Session, session, AcceptAskFriend);
	REGISTER_REQUEST(Session, session, RejectAskFriend); 
	REGISTER_REQUEST(Session, session, GetFriendList);
	REGISTER_REQUEST(Session, session, CancelAskFriend);
	REGISTER_REQUEST(Session, session, DeleteFriend);
	REGISTER_REQUEST(Session, session, GetAllSendAskFriend);
	REGISTER_REQUEST(Session, session, GetAllReceiveAskFriend);
	REGISTER_REQUEST(Session, session, SendSocialPoint);
	REGISTER_REQUEST(Session, session, ConfirmNewFriend);
	REGISTER_REQUEST(Session, session, GetRecommendFriend);
	REGISTER_REQUEST(Session, session, PlatformFriendInviteInfo);
	REGISTER_REQUEST(Session, session, PlatformFriendInviteCountUp);
	REGISTER_REQUEST(Session, session, PlatformFriendJoinerCountUp);
	REGISTER_REQUEST(Session, session, PlatformFriendInviteReward);
	REGISTER_REQUEST(Session, session, FindUser);
	REGISTER_REQUEST(Session, session, GetQuest);
	REGISTER_REQUEST(Session, session, AcceptQuest);
	REGISTER_REQUEST(Session, session, ReceiveQuestReward);
	REGISTER_REQUEST(Session, session, GetCounterDungeon);
	REGISTER_REQUEST(Session, session, StartCounterDungeon);
	REGISTER_REQUEST(Session, session, FinishCounterDungeon);
	REGISTER_REQUEST(Session, session, SweepCounterDungeon);
	REGISTER_REQUEST(Session, session, BuyCounterDungeonTicket);
	REGISTER_REQUEST(Session, session, CounterDungeonWeeklyReward);
	REGISTER_REQUEST(Session, session, GetDimension);
	REGISTER_REQUEST(Session, session, StartDimension);
	REGISTER_REQUEST(Session, session, FinishDimension);
	REGISTER_REQUEST(Session, session, CreateGuild);
	REGISTER_REQUEST(Session, session, AskJoinGuild);
	REGISTER_REQUEST(Session, session, LeaveGuild);
	REGISTER_REQUEST(Session, session, GetGuildDetailInfo);
	REGISTER_REQUEST(Session, session, GetGuildMember);
	REGISTER_REQUEST(Session, session, DeleteGuild);
	REGISTER_REQUEST(Session, session, GetRecommendGuild);
	REGISTER_REQUEST(Session, session, AppointSubGuildMaster);
	REGISTER_REQUEST(Session, session, DismissSubGuildMaster);
	REGISTER_REQUEST(Session, session, KickGuildMember);
	REGISTER_REQUEST(Session, session, UpdateGuild);
	REGISTER_REQUEST(Session, session, UpdateGuildNotice);
	REGISTER_REQUEST(Session, session, GetGuildMemberCandidates);
	REGISTER_REQUEST(Session, session, InviteGuild);
	REGISTER_REQUEST(Session, session, GetGuildInviteList);
	REGISTER_REQUEST(Session, session, RejectInviteGuild);
	REGISTER_REQUEST(Session, session, AcceptInviteGuild);
	REGISTER_REQUEST(Session, session, FindGuild);
	REGISTER_REQUEST(Session, session, ObtainGuildMaster);
	REGISTER_REQUEST(Session, session, HandOverGuildMaster);
	REGISTER_REQUEST(Session, session, GetGuildRankingList);
	REGISTER_REQUEST(Session, session, LevelUpGuildSkill);
	REGISTER_REQUEST(Session, session, BuyGuildSkill);
	REGISTER_REQUEST(Session, session, GetMission);
	REGISTER_REQUEST(Session, session, GetFeaturedMission);
	REGISTER_REQUEST(Session, session, ReceiveMissionReward);
	REGISTER_REQUEST(Session, session, ReceiveCompleteMissionReward);
	REGISTER_REQUEST(Session, session, GetWing);
	REGISTER_REQUEST(Session, session, EnhanceWing);
	REGISTER_REQUEST(Session, session, UpgradeWing);
	REGISTER_REQUEST(Session, session, EnhanceWingOption);
	REGISTER_REQUEST(Session, session, GetDonationEvent);
	REGISTER_REQUEST(Session, session, MakeDonation);
	REGISTER_REQUEST(Session, session, DonationPointReward);
	REGISTER_REQUEST(Session, session, GetFairy);
	REGISTER_REQUEST(Session, session, LevelupFairy);
	REGISTER_REQUEST(Session, session, BlessFairy);
	REGISTER_REQUEST(Session, session, ReceiveFairyGift);
	REGISTER_REQUEST(Session, session, GetHeroTower);
	REGISTER_REQUEST(Session, session, StartHeroTower);
	REGISTER_REQUEST(Session, session, FinishHeroTower);
	REGISTER_REQUEST(Session, session, AbandonHeroTower);
	REGISTER_REQUEST(Session, session, EnhanceRelic);
	REGISTER_REQUEST(Session, session, PromotionRelic);
	REGISTER_REQUEST(Session, session, GetAccountRelic);
	REGISTER_REQUEST(Session, session, SweepHeroTower);
	REGISTER_REQUEST(Session, session, BuyHeroTowerTicket);
	REGISTER_REQUEST(Session, session, DrawShopLottery);
	REGISTER_REQUEST(Session, session, RewardShopMileage);
	REGISTER_REQUEST(Session, session, GetLotteryShop);
	REGISTER_REQUEST(Session, session, BuyShopProduct);
	REGISTER_REQUEST(Session, session, GetGeneralShop);
	REGISTER_REQUEST(Session, session, GetMagicShop);
	REGISTER_REQUEST(Session, session, RenewalMagicShop);
	REGISTER_REQUEST(Session, session, PurchaseMagicShop);
	REGISTER_REQUEST(Session, session, GetCollectionItem);
	REGISTER_REQUEST(Session, session, RewardCollectionItem);
	REGISTER_REQUEST(Session, session, RewardCollectionSetItem);
	REGISTER_REQUEST(Session, session, GetMarketProduct);
	REGISTER_REQUEST(Session, session, PreparePurchase);
	REGISTER_REQUEST(Session, session, VerifyPurchase);
	REGISTER_REQUEST(Session, session, CancelPurchase);
	REGISTER_REQUEST(Session, session, FinishPurchase);
	REGISTER_REQUEST(Session, session, RaidResult);
	REGISTER_REQUEST(Session, session, DailyAttendance);
	REGISTER_REQUEST(Session, session, JoinGuildBattle);
	REGISTER_REQUEST(Session, session, GetGuildBattle);
	REGISTER_REQUEST(Session, session, StartGuildBattle);
	REGISTER_REQUEST(Session, session, FinishGuildBattle);
	REGISTER_REQUEST(Session, session, SetGuildBattleEntry);
	REGISTER_REQUEST(Session, session, GuildBattleTurnResult);
	REGISTER_REQUEST(Session, session, GuildBattleTurnReward);
	REGISTER_REQUEST(Session, session, GuildBattleSeasonReward);
	REGISTER_REQUEST(Session, session, GuildBattleHistory);
	REGISTER_REQUEST(Session, session, SetWingVisible);
	REGISTER_REQUEST(Session, session, SetHelmetVisible);
	REGISTER_REQUEST(Session, session, GetRaid);
	REGISTER_REQUEST(Session, session, RaidResurrection);
	REGISTER_REQUEST(Session, session, AssaultMatchmaking);
	REGISTER_REQUEST(Session, session, CancelAssaultMatchmaking);
	REGISTER_REQUEST(Session, session, AssaultResult);
	REGISTER_REQUEST(Session, session, AssaultResurrection);
	REGISTER_REQUEST(Session, session, GetAssaultBattleStatus);
	REGISTER_REQUEST(Session, session, GetFriendRankings);
	REGISTER_REQUEST(Session, session, GetItemRandomOption);
	REGISTER_REQUEST(Session, session, GenerateItemRandomOption);
	REGISTER_REQUEST(Session, session, ApplyItemRandomOption);
	REGISTER_REQUEST(Session, session, DiscardItemRandomOption);
	REGISTER_REQUEST(Session, session, CheckRedDot);
	REGISTER_REQUEST(Session, session, CheckChargePoints);
	REGISTER_REQUEST(Session, session, CheckPvPMatchPointCharge);
	REGISTER_REQUEST(Session, session, CheckTeamMatchPointCharge);
	REGISTER_REQUEST(Session, session, ReceiveAssaultMvpReward);
	
	REGISTER_REQUEST(Session, session, GetAccountAether);
	REGISTER_REQUEST(Session, session, DismantleAethers);
	REGISTER_REQUEST(Session, session, MountAethers);
	REGISTER_REQUEST(Session, session, UnmountAethers);
	REGISTER_REQUEST(Session, session, LockAethers);
	REGISTER_REQUEST(Session, session, UnlockAethers);
	REGISTER_REQUEST(Session, session, ExtendAetherInventory);
	REGISTER_REQUEST(Session, session, EnhanceAether);
	REGISTER_REQUEST(Session, session, ModifyAetherNewFlag);
	REGISTER_REQUEST(Session, session, SelectAetherDefenseOption);
	REGISTER_REQUEST(Session, session, SelectAetherUnitySkillOption);
	REGISTER_REQUEST(Session, session, SellAethers);

	REGISTER_REQUEST(Session, session, ModifyItemRandomOption);
	REGISTER_REQUEST(Session, session, ModifyItemNewFlag);
	REGISTER_REQUEST(Session, session, ModifyCostumeNewFlag);
	REGISTER_REQUEST(Session, session, StartRaid);
	REGISTER_REQUEST(Session, session, GetContentsOpenStatus);
	REGISTER_REQUEST(Session, session, GetDuelRewardBoxInfo);
	REGISTER_REQUEST(Session, session, GetDuelRewardBoxGainableList);
	REGISTER_REQUEST(Session, session, OpenDuelRewardBox);
	REGISTER_REQUEST(Session, session, EventAttendance);
	REGISTER_REQUEST(Session, session, BeginnerAttendance);
	REGISTER_REQUEST(Session, session, UpdateDailyPlayTime);
	REGISTER_REQUEST(Session, session, ReceiveDailyPlayTimeReward);
	REGISTER_REQUEST(Session, session, GetUnitySkillMission);
	REGISTER_REQUEST(Session, session, ReceiveUnitySkillReward);
	REGISTER_REQUEST(Session, session, AwakenUnitySkillMission);
	REGISTER_REQUEST(Session, session, AwakenUnitySkill);
	REGISTER_REQUEST(Session, session, GetDuelModeInfo);
	REGISTER_REQUEST(Session, session, GetChallengeModeInfo);
	REGISTER_REQUEST(Session, session, ReceiveRestReward);
	REGISTER_REQUEST(Session, session, GetHallOfFame);
	REGISTER_REQUEST(Session, session, PraiseTargetRanker);
	REGISTER_REQUEST(Session, session, GetTutorialProgress);
	REGISTER_REQUEST(Session, session, SetTutorialProgress);
	REGISTER_REQUEST(Session, session, UpdateTutorialProgress);
	REGISTER_REQUEST(Session, session, SkipTutorialProgress);
	REGISTER_REQUEST(Session, session, DrawTutorialItem);
	REGISTER_REQUEST(Session, session, PrepareTutorial);
	REGISTER_REQUEST(Session, session, GiveTutorialReward);
	REGISTER_REQUEST(Session, session, HelloSession);
	REGISTER_REQUEST(Session, session, CheckModeOpen);
	REGISTER_REQUEST(Session, session, GuildDonation);
	REGISTER_REQUEST(Session, session, CheckReachPower);
	REGISTER_REQUEST(Session, session, TrackingPowerDetail);

	REGISTER_REQUEST(Session, session, KakaoMessageProcessing);
	REGISTER_REQUEST(Session, session, KakaoCouponUse);
	REGISTER_REQUEST(Session, session, KakaoAccountConversionPrepare);
	REGISTER_REQUEST(Session, session, KakaoAccountConversionComplete);

	REGISTER_REQUEST(Session, session, GetEvent);
	REGISTER_REQUEST(Session, session, ReceivePointEventReward);
	REGISTER_REQUEST(Session, session, ReceiveLevelUpEventReward);
	REGISTER_REQUEST(Session, session, ReceiveStageClearEventReward);
	REGISTER_REQUEST(Session, session, GetSeasonMission);
	REGISTER_REQUEST(Session, session, RewardSeasonMission);
	REGISTER_REQUEST(Session, session, AttendSeasonMission);
	REGISTER_REQUEST(Session, session, BuyHotTimeBuff);
	REGISTER_REQUEST(Session, session, RewardFlatRatePackageBonus);
	REGISTER_REQUEST(Session, session, GetPackageShop);
	REGISTER_REQUEST(Session, session, GetPeriodPackageState);
	REGISTER_REQUEST(Session, session, RewardLevelUpPackage);
	REGISTER_REQUEST(Session, session, RewardReachLevelPackage);
	REGISTER_REQUEST(Session, session, GiveUpQuest);
	REGISTER_REQUEST(Session, session, GetContentsModeState);
	REGISTER_REQUEST(Session, session, RewardPointShopEvent);
	REGISTER_REQUEST(Session, session, ReportUser);
	REGISTER_REQUEST(Session, session, BlockChat);
	REGISTER_REQUEST(Session, session, UnblockChat);
	REGISTER_REQUEST(Session, session, GetBlockChatList);
	REGISTER_REQUEST(Session, session, ClientStateReport);
	REGISTER_REQUEST(Session, session, RewardTenLotteryEvent);
	REGISTER_REQUEST(Session, session, RewardSpendGemEvent);
	REGISTER_REQUEST(Session, session, ReceiveGemPurchaseEventReward);
	REGISTER_REQUEST(Session, session, RewardTenLotterySavingsEvent);
	REGISTER_REQUEST(Session, session, RollDiceEvent);
	REGISTER_REQUEST(Session, session, BuyDiceEventPoint);
	REGISTER_REQUEST(Session, session, RewardDiceEventMileage);

	REGISTER_REQUEST(Session, session, GetAccountCostume);
	REGISTER_REQUEST(Session, session, EquipCostumes);
	REGISTER_REQUEST(Session, session, UnequipCostumes);
	REGISTER_REQUEST(Session, session, EnhanceCostume);
	REGISTER_REQUEST(Session, session, DismantleCostumes);
	REGISTER_REQUEST(Session, session, LockCostumes);
	REGISTER_REQUEST(Session, session, UnlockCostumes);
	REGISTER_REQUEST(Session, session, GetCostumeShop);
	REGISTER_REQUEST(Session, session, PurchaseCostume);
	REGISTER_REQUEST(Session, session, SetCostumeVisible);

	// totem
	REGISTER_REQUEST(Session, session, GetAccountTotem);
	REGISTER_REQUEST(Session, session, EquipTotems);
	REGISTER_REQUEST(Session, session, UnequipTotems);
	REGISTER_REQUEST(Session, session, LockTotems);
	REGISTER_REQUEST(Session, session, UnlockTotems);
	REGISTER_REQUEST(Session, session, SellTotems);
	REGISTER_REQUEST(Session, session, DismantleTotems);
	REGISTER_REQUEST(Session, session, RefineTotem);
	REGISTER_REQUEST(Session, session, ModifyTotemNewFlag);

	// REPLACEMENT POINT: FACTORY CPP REQUEST
	// NEVER DELETE LINE ABOVE

	// for console command
	REGISTER_REQUEST(Session, session, IncreaseDarkCrystalPaid);
	REGISTER_REQUEST(Session, session, DeleteAccount);
    REGISTER_REQUEST(Session, session, AddItemToInventory);
	REGISTER_REQUEST(Session, session, AddCostumeToInventory);
	REGISTER_REQUEST(Session, session, AddTotemToInventory);
    REGISTER_REQUEST(Session, session, IncreaseCashPaid);
    REGISTER_REQUEST(Session, session, IncreaseMoney);
    REGISTER_REQUEST(Session, session, SetItemLevelup);
    REGISTER_REQUEST(Session, session, SetCharacterLevelExp);
	REGISTER_REQUEST(Session, session, SetCharacterRankInfo);
	REGISTER_REQUEST(Session, session, SetCharacterRelic);
    REGISTER_REQUEST(Session, session, SetBladePoint);
	REGISTER_REQUEST(Session, session, AddMail);
	REGISTER_REQUEST(Session, session, MakeRanking);
	REGISTER_REQUEST(Session, session, ForceClearStage);
	REGISTER_REQUEST(Session, session, SetQuestState);
	REGISTER_REQUEST(Session, session, IncreaseHeroPiece);
	REGISTER_REQUEST(Session, session, ForceClearHeroTower);
	REGISTER_REQUEST(Session, session, ForceStartCounterDungeon);
	REGISTER_REQUEST(Session, session, IncreaseSocialPoint);
	REGISTER_REQUEST(Session, session, PrepareSweepHeroTower);
	REGISTER_REQUEST(Session, session, PrepareGuild);
	REGISTER_REQUEST(Session, session, SetGuildBattleState);
	REGISTER_REQUEST(Session, session, ResetAttendanceTime);
	REGISTER_REQUEST(Session, session, ResetEventAttendance);
	REGISTER_REQUEST(Session, session, ResetDailyPlayTimeRecord);
	REGISTER_REQUEST(Session, session, ResetTeamMatchPoint);
	REGISTER_REQUEST(Session, session, ChangeQuestId);
	REGISTER_REQUEST(Session, session, ChangeQuestCount);
	REGISTER_REQUEST(Session, session, IncreaseRankPromotionPoint);
	REGISTER_REQUEST(Session, session, PrepareFriend);
	REGISTER_REQUEST(Session, session, AddAetherToInventory);
	REGISTER_REQUEST(Session, session, ForceUpdateAether);
	REGISTER_REQUEST(Session, session, SetGuildLevelExp);
	REGISTER_REQUEST(Session, session, SetMatchSeasonReward);
	REGISTER_REQUEST(Session, session, SetStageBoostCount);
	REGISTER_REQUEST(Session, session, SkipMatchTutorial);
	REGISTER_REQUEST(Session, session, SetDebugMode);
	REGISTER_REQUEST(Session, session, ServerCmd);
	REGISTER_REQUEST(Session, session, Trace);

	
	// for chat
	REGISTER_REQUEST(Session, session, GetChatChannel);
	REGISTER_REQUEST(Session, session, ChangeChatChannel);

    REGISTER_COMMAND(Session, session, Chat);
	REGISTER_REQUEST(Session, session, GuildChat);
	REGISTER_REQUEST(Session, session, GetGuildChat);
	REGISTER_REQUEST(Session, session, AskGuildSupport);
	REGISTER_REQUEST(Session, session, GuildSupport);

    REGISTER_NOTIFY(Session, session, SessionClosed);
    REGISTER_NOTIFY(Session, session, Chat);
	REGISTER_NOTIFY(Session, session, GuildChat);
    REGISTER_NOTIFY(Session, session, Invitation);
    REGISTER_NOTIFY(Session, session, AcceptInvitation);
    REGISTER_NOTIFY(Session, session, DeclineInvitation);
    REGISTER_NOTIFY(Session, session, RaidInvitation);
	REGISTER_NOTIFY(Session, session, QuestAlarm);
	REGISTER_NOTIFY(Session, session, MissionAlarm);
	REGISTER_NOTIFY(Session, session, AdminMessage);
	REGISTER_NOTIFY(Session, session, ContentsModeState);

    // channel server
    //
	REGISTER_REQUEST(Channel, channel, HelloChannel)
    REGISTER_REQUEST(Channel, channel, SignInChannel);
    REGISTER_REQUEST(Channel, channel, RoomJoin);
    REGISTER_REQUEST(Channel, channel, RoomLeave);
    REGISTER_REQUEST(Channel, channel, RoomKick);
    REGISTER_REQUEST(Channel, channel, RoomSetProperty);
    REGISTER_REQUEST(Channel, channel, RoomInvite);
	REGISTER_REQUEST(Channel, channel, RoomClose);
	REGISTER_REQUEST(Channel, channel, RoomLeaveResult);
	REGISTER_REQUEST(Channel, channel, RoomAccessControl);
    REGISTER_REQUEST(Channel, channel, RoomHeartbeat);
	REGISTER_REQUEST(Channel, channel, SetRoomDebugMode);
    REGISTER_COMMAND(Channel, channel, RoomMessage);
    REGISTER_COMMAND(Channel, channel, RoomHeartbeat);

    REGISTER_NOTIFY(Channel, channel, RoomCreate);
    REGISTER_NOTIFY(Channel, channel, RoomDelete);
    REGISTER_NOTIFY(Channel, channel, RoomParticipantJoin);
    REGISTER_NOTIFY(Channel, channel, RoomParticipantLeave);
    REGISTER_NOTIFY(Channel, channel, RoomHostChanged);
    REGISTER_NOTIFY(Channel, channel, RoomMessage);
    REGISTER_NOTIFY(Channel, channel, RoomPropertyUpdated);
	REGISTER_NOTIFY(Channel, channel, RoomHeartbeat);

	REGISTER_REQUEST(Channel, channel, ReportUserViaChannel);

	// for channelWorld
	REGISTER_REQUEST(Channel, channel, WorldJoin);
	REGISTER_REQUEST(Channel, channel, WorldLeave);
}

void ProtobufFactory::Finalize()
{
    B2NETWORK_LOG_INFO(logger, "Finalizing...");

    std::for_each(s_metadataMap.begin(), s_metadataMap.end(),
        [](std::pair<uint32_t, Metadata> entry)
        {
            delete entry.second.instance;
        });

    s_metadataMap.clear();
}

uint32_t ProtobufFactory::Register(const ServerType serverType, const MessageType messageType, const Message* instance)
{
    const auto descriptor = instance->GetDescriptor();
    const auto hash = StringHash::HashCode(descriptor->full_name());

    // TODO must check hash collision
    s_metadataMap.emplace(std::make_pair(hash,
        Metadata(instance, serverType, messageType, descriptor)));

    B2NETWORK_LOG_INFO(logger, "Register %s server %d message %d hash %d",
        descriptor->full_name().c_str(), serverType, messageType, hash);

    return hash;
}

void ProtobufFactory::Register(const ServerType serverType, const Message* request, const Message* response)
{
    auto requestHash = Register(serverType, Request, request);
    auto responseHash = Register(serverType, Response, response);

    // TODO must check hash collision
    s_requestToResponseMap[requestHash] = responseHash;
    s_responseToRequestMap[responseHash] = requestHash;
}

ProtobufFactory::Metadata ProtobufFactory::GetMetadata(const uint32_t hash)
{
    const auto it = s_metadataMap.find(hash);
    if (it == s_metadataMap.end())
    {
        B2NETWORK_LOG_ERROR(logger, "Metadata not found for protocol(hash=%s)", hash);

#if defined(B2NETWORK_PLATFORM_WINDOWS)
        throw Exception("Protocol Not Registered");
#elif defined(B2NETWORK_PLATFORM_ANDROID)
        // TODO fix undefined behavior
        return Metadata{ nullptr, ServerType::Session, MessageType::Request, nullptr };
#elif defined(B2NETWORK_PLATFORM_IOS)
        // TODO fix undefined behavior
        return Metadata{ nullptr, ServerType::Session, MessageType::Request, nullptr };
#else
#error Platform not supported
#endif
    }
    else
    {
        return it->second;
    }
}

ProtobufFactory::Metadata ProtobufFactory::GetMetadata(const Message& message)
{
    const auto protocol = message.GetDescriptor()->full_name();
    const auto hash = StringHash::HashCode(protocol);

    const auto it = s_metadataMap.find(hash);
    if (it == s_metadataMap.end())
    {
        B2NETWORK_LOG_ERROR(logger, "Metadata not found for protocol(%s)", protocol.c_str());

#if defined(B2NETWORK_PLATFORM_WINDOWS)
        throw Exception("Protocol Not Registered");
#elif defined(B2NETWORK_PLATFORM_ANDROID)
        // TODO fix undefined behavior
        return Metadata{ nullptr, ServerType::Session, MessageType::Request, nullptr };
#elif defined(B2NETWORK_PLATFORM_IOS)
        // TODO fix undefined behavior
        return Metadata{ nullptr, ServerType::Session, MessageType::Request, nullptr };
#else
#error Platform not supported
#endif
    }
    else
    {
        return it->second;
    }
}

ProtobufFactory::Metadata ProtobufFactory::GetMetadata(const MessagePtr message)
{
    return GetMetadata(*message);
}

ProtobufFactory::MessagePtr ProtobufFactory::CreateMessage(uint32_t type)
{
    const auto it = s_metadataMap.find(type);
    if (it == s_metadataMap.end())
    {
        B2NETWORK_LOG_INFO(logger, "Could not find message type=%u", type);

#if defined(B2NETWORK_PLATFORM_WINDOWS)
        throw Exception("Unknown Protobuf Message");
#elif defined(B2NETWORK_PLATFORM_ANDROID)
        B2NETWORK_LOG_ERROR(logger, "Unknown Protobuf Message Hash %s", type);
        return nullptr;
#elif defined(B2NETWORK_PLATFORM_IOS)
        B2NETWORK_LOG_ERROR(logger, "Unknown Protobuf Message Hash %s", type);
        return nullptr;
#else
#error Platform not supported
#endif
    }

    auto metadata = it->second;
    return std::shared_ptr<Message>(metadata.instance->New());
}

bool ProtobufFactory::IsRequest(const MessagePtr message)
{
    return (GetMetadata(message).messageType == Request);
}

bool ProtobufFactory::IsResponse(const MessagePtr message)
{
    return (GetMetadata(message).messageType == Response);
}

bool ProtobufFactory::IsNotify(const MessagePtr message)
{
    return (GetMetadata(message).messageType == Notify);
}

bool ProtobufFactory::IsTypeOf(const MessagePtr message, const Message& type)
{
    const auto messageHash = StringHash::HashCode(message->GetDescriptor()->full_name());
    const auto typeHash = StringHash::HashCode(type.GetDescriptor()->full_name());
    return messageHash == typeHash;
}

bool ProtobufFactory::IsResponseOf(const MessagePtr request, const MessagePtr response)
{
    if (!IsRequest(request))
    {
        return false;
    }

    if (!IsResponse(response))
    {
        return false;
    }

    auto responseHash = StringHash::HashCode(response->GetDescriptor()->full_name());
    const auto it = s_responseToRequestMap.find(responseHash);
    if (it == s_responseToRequestMap.end())
    {
        return false;
    }
    else
    {
        auto requestHash = StringHash::HashCode(request->GetDescriptor()->full_name());
        return (requestHash == it->second);
    }
}

uint32_t ProtobufFactory::GetResponseType(const MessagePtr request)
{
    auto requestHash = StringHash::HashCode(request->GetDescriptor()->full_name());
    const auto it = s_requestToResponseMap.find(requestHash);
    if (it == s_requestToResponseMap.end())
    {
        return 0;
    }
    else
    {
        return it->second;
    }
}

void b2network::ProtobufFactory::SetMasterDataHash(const std::string& hashStr)
{
	s_masterDataHash = hashStr;
}

std::string b2network::ProtobufFactory::GetMasterDataHash()
{
	return s_masterDataHash;
}
