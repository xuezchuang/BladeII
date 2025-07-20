#pragma once
//#include "BladeII.h"
#include "EngineMinimal.h"
#include "CommonStruct.h"
//#include "B2UIEnum.h"

UENUM(BlueprintType)
enum class EB2ChatType : uint8
{
	Notice,			// 傍瘤
	Channel,		// 盲澄 盲泼
	GuildNotice,	// 辨靛 傍瘤
	Guild,			// 辨靛 盲泼
	OneToOne,		// 庇加富
	End
};

UENUM(BlueprintType)
enum class EB2ChatNoticeType : uint8
{
	None,
	GuildJoin,
	GuildLeave,
	GuildKick,
	GuildStartBattle,
	GuildEndBattle,
	GuildChangeNotice,
	GetItem,
	RaidStart,
	RaidEnd,
	AssaultStart,
	AssaultEnd,
	GuildSupport,
	GuildSupportComplete,
	EnhanceItem,

	// 肺拿俊辑父 静绰 舅覆
	ChangeChannel = 100,
	AdminMessage,

	End,
};

UENUM(BlueprintType)
enum class EB2ChatGetRouteType : uint8
{
	None,
	Shop,
	Lottery,
	Upgrade,
	Compose,
	MakeDonation,
	Factory_Forge,
	Factory_Upgrade,
	End
};

inline EB2ChatType SvrToCliChatType(int32 InServerType)
{
	switch (InServerType)
	{
	case b2network::B2ChatType::NOTICE:				return EB2ChatType::Notice;
	case b2network::B2ChatType::CHANNEL:			return EB2ChatType::Channel;
	case b2network::B2ChatType::GUILD_NOTICE:		return EB2ChatType::GuildNotice;
	case b2network::B2ChatType::GUILD:				return EB2ChatType::Guild;
	case b2network::B2ChatType::ONE_TO_ONE:			return EB2ChatType::OneToOne;
	}
	return EB2ChatType::Channel;
}

inline int32 CliToSvrChatType(EB2ChatType InCliType)
{
	//switch (InCliType)
	//{
	//case EB2ChatType::Notice:						return b2network::B2ChatType::NOTICE;
	//case EB2ChatType::Channel:						return b2network::B2ChatType::CHANNEL;
	//case EB2ChatType::GuildNotice:					return b2network::B2ChatType::GUILD_NOTICE;
	//case EB2ChatType::Guild:						return b2network::B2ChatType::GUILD;
	//case EB2ChatType::OneToOne:						return b2network::B2ChatType::ONE_TO_ONE;
	//// To be added and/or modified..
	//}
	//return b2::protocol::session::ChatType::NOTICE;
	return 0;
}

inline EB2ChatNoticeType SvrToCliChatNoticeType(int32 InServerType)
{
	switch (InServerType)
	{
	case b2network::B2ChatNotifyType::GUILD_JOIN:				return EB2ChatNoticeType::GuildJoin;
	case b2network::B2ChatNotifyType::GUILD_LEAVE:				return EB2ChatNoticeType::GuildLeave;
	case b2network::B2ChatNotifyType::GUILD_KICK:				return EB2ChatNoticeType::GuildKick;
	case b2network::B2ChatNotifyType::GUILD_START_BATTLE:		return EB2ChatNoticeType::GuildStartBattle;
	case b2network::B2ChatNotifyType::GUILD_END_BATTLE:			return EB2ChatNoticeType::GuildEndBattle;
	case b2network::B2ChatNotifyType::GUILD_CHANGE_NOTICE:		return EB2ChatNoticeType::GuildChangeNotice;
	case b2network::B2ChatNotifyType::GET_ITEM:					return EB2ChatNoticeType::GetItem;
	case b2network::B2ChatNotifyType::RAID_START:				return EB2ChatNoticeType::RaidStart;
	case b2network::B2ChatNotifyType::RAID_END:					return EB2ChatNoticeType::RaidEnd;
	case b2network::B2ChatNotifyType::ASSAULT_START:			return EB2ChatNoticeType::AssaultStart;
	case b2network::B2ChatNotifyType::ASSAULT_END:				return EB2ChatNoticeType::AssaultEnd;
	case b2network::B2ChatNotifyType::GUILD_SUPPORT:			return EB2ChatNoticeType::GuildSupport;
	case b2network::B2ChatNotifyType::GUILD_SUPPORT_COMPLETE:	return EB2ChatNoticeType::GuildSupportComplete;
	case b2network::B2ChatNotifyType::ENHANCE_ITEM:				return EB2ChatNoticeType::EnhanceItem;
	}
	return EB2ChatNoticeType::None;
}

inline int32 CliToSvrChatNoticeType(EB2ChatNoticeType InCliType)
{
	switch (InCliType)
	{
	case EB2ChatNoticeType::GuildJoin:						return b2network::B2ChatNotifyType::GUILD_JOIN;
	case EB2ChatNoticeType::GuildLeave:						return b2network::B2ChatNotifyType::GUILD_LEAVE;
	case EB2ChatNoticeType::GuildKick:						return b2network::B2ChatNotifyType::GUILD_KICK;
	case EB2ChatNoticeType::GuildStartBattle:				return b2network::B2ChatNotifyType::GUILD_START_BATTLE;
	case EB2ChatNoticeType::GuildEndBattle:					return b2network::B2ChatNotifyType::GUILD_END_BATTLE;
	case EB2ChatNoticeType::GuildChangeNotice:				return b2network::B2ChatNotifyType::GUILD_CHANGE_NOTICE;
	case EB2ChatNoticeType::GetItem:						return b2network::B2ChatNotifyType::GET_ITEM;
	case EB2ChatNoticeType::RaidStart:						return b2network::B2ChatNotifyType::RAID_START;
	case EB2ChatNoticeType::RaidEnd:						return b2network::B2ChatNotifyType::RAID_END;
	case EB2ChatNoticeType::AssaultStart:					return b2network::B2ChatNotifyType::ASSAULT_START;
	case EB2ChatNoticeType::AssaultEnd:						return b2network::B2ChatNotifyType::ASSAULT_END;
	case EB2ChatNoticeType::GuildSupport:					return b2network::B2ChatNotifyType::GUILD_SUPPORT;
	case EB2ChatNoticeType::GuildSupportComplete:			return b2network::B2ChatNotifyType::GUILD_SUPPORT_COMPLETE;
	case EB2ChatNoticeType::EnhanceItem:					return b2network::B2ChatNotifyType::ENHANCE_ITEM;
	}

	//return b2::protocol::session::ChatNoticeType::GUILD_JOIN;
	return 0;
}

inline EB2ChatGetRouteType SvrToCliGetRouteType(int32 InServerType)
{
	switch (InServerType)
	{
	case b2network::B2ItemGetRouteType::BUY_SHOP:			return EB2ChatGetRouteType::Shop;
	case b2network::B2ItemGetRouteType::LOTTERY:			return EB2ChatGetRouteType::Lottery;
	case b2network::B2ItemGetRouteType::UPGRADE_ITEM:		return EB2ChatGetRouteType::Upgrade;
	case b2network::B2ItemGetRouteType::COMPOSE_ITEM:		return EB2ChatGetRouteType::Compose;
	case b2network::B2ItemGetRouteType::MAKE_DONATION:		return EB2ChatGetRouteType::MakeDonation;
	case b2network::B2ItemGetRouteType::FACTORY_MAKE:		return EB2ChatGetRouteType::Factory_Forge;
	case b2network::B2ItemGetRouteType::FACTORY_UPGRADE:	return EB2ChatGetRouteType::Factory_Upgrade;
	}
	return EB2ChatGetRouteType::None;
}

//
// TODO Chat : Might need some sophisticated time format
// 

struct FB2ChatGuildInfo
{
	int64 GuildId;
	int32 PatternIndex;
	int32 PetternColor; 
	int32 BGImage;
	int32 BGColor;

	int32 SupportTempleteID;
	int32 SupportCount;
	int32 SupportMaxCount;
	int64 UpdateTime;
	int64 ExpireTime;

	FText Name;
};
using FB2ChatGuildInfoPtr = std::shared_ptr<FB2ChatGuildInfo>;

/*==================================================
 FB2ChatMessage.. and some related
==================================================*/

/** Represent single chatting message */
struct FB2ChatMessage
{
	FB2ChatMessage()
	{
		MessageType = EB2ChatType::End;
		NoticeType = EB2ChatNoticeType::End;
		bIsMyMessage = false;
		MessageTime = 0;
		UserAcccountID = 0;
		bCanBroadcast = false;
	}

	EB2ChatType MessageType;
	EB2ChatNoticeType NoticeType;

	bool bCanBroadcast;
	uint32 bIsMyMessage : 1; // If this message is originated from this local machine, instead of receiving via network.

	int64 ChatID;
	int64 UserAcccountID;
	int64 MessageTime; // Simply platform time set from client.

	FText SendUserNick;
	FText MessageText; // The main content here.

	FB2ChatGuildInfoPtr GuildInfo;

	int32 CharacterMaxLevel;
private:
	TArray<FString> NoticeArgs;
public:
	void SetChatInfo(b2network::FB2MessageInfoChatNotifyPtr InChatInfo);
	void SetChatInfo(b2network::B2GuildChatDataPtr InGuildChatInfo);

	void SetCurrentAsMessageTime(); // To be called at some proper timing, when this chatting message has arrived.
	
	void SetSystemChatMessage(EB2ChatNoticeType InNoticeType);
	void AddNoticeArgs(FString Arg);

	FText GetAreaName(int32 AreaNum);

	int32 GetMessageElapsedTime() const;

	bool HasGuildInfo() const;
	bool IsBroadcastMessage() const;

	friend struct FB2ChatMessageListTimeSorter;

private:
	void SetConvertMessage(const FString &InMessage);
	void SetCheckBroadCastMessage();
};

struct FB2ChatMessageListTimeSorter
{
	bool operator()(const FB2ChatMessage& A, const FB2ChatMessage& B) const
	{
		return A.MessageTime > B.MessageTime; // Newer one first
	}
};

struct FB2ChatMessageListIDSorter
{
	bool operator()(const FB2ChatMessage& A, const FB2ChatMessage& B) const
	{
		return A.ChatID > B.ChatID; // Newer one first
	}
};

namespace FB2ChatManagementRaw
{
	namespace Capacity
	{
		const int32 GuildSupport	= 50;
		const int32 Chat			= 100;
	}

	namespace Sort
	{
		const int32 Time			= 1;
		const int32 Id				= 2;
	}

}

/** Message list per a B2ChatType */
struct FB2ChatTypeMessageList
{
	FB2ChatTypeMessageList(const int32 Capacity, const int32 Sort)
		: ClientStoreLimit(Capacity)
		, SortType(Sort)
	{
	}

	FB2ChatMessage* FindMessage(const int64 ChatID);
	void RemoveFromChatID(const int64 ChatID);
	void RemoveFromUserName(const FString &Name, const EB2ChatNoticeType InNotice);

	int32 ClientStoreLimit;
	int32 SortType;

	TArray<FB2ChatMessage> Messages; // All elements are supposed to have the same type.
};


/*==================================================
 FB2ChatStore
==================================================*/

/** The main chatting management struct, almost consistent throughout the game module. */
struct FB2ChatStore
{
	const static int32 MessageLengthLimit;

private:

	TMap<EB2ChatType, FB2ChatTypeMessageList> ChatData;
	TMap<int32, FString> BlockUsers;

	TQueue<FB2ChatMessage> CachedNewChat;

	// Not probably need to get from server. Just fixed..
	const static int32 MinAllowedChannel;
	const static int32 MaxAllowedChannel;

	int32 MaxBlockList;
	int32 CurrentChannel;

	int32 LastRequestedChannelNum; // To be set on request, then applied to CurrentChannel on response.
	int64 CachedGuildID;
	int64 CachedChatID;
	bool bInitGetGuildChat;

	TArray<std::shared_ptr<class issue>> Issues;

public:
	FB2ChatStore();

	void OnModuleStartup();
	void OnModuleShutdown();

private:
	void SubscribeEvent();
	void UnsubscribeEvent();

	void OnServerChatMessageNotify(b2network::FB2MessageInfoChatNotifyPtr &ChatInfo);
	void OnServerGuildChatMessageNotify(b2network::FB2MessageInfoGuildChatNotifyPtr &GuildChatInfo);

	void SetGuildChatBadge(b2network::FB2MessageInfoGuildChatNotifyPtr &GuildChatNotify);
	void SetProcessGuildChatMessages(b2network::FB2MessageInfoGuildChatNotifyPtr &GuildChatNotify);

	bool IsSpecialGuildNotify(EB2ChatNoticeType InNoticeType);
	bool IsShowingChatCategory(EB2ChatType InType);

	void CreateSystemNotice(EB2ChatType InType, EB2ChatNoticeType InNoticeType, const TArray<FString>& InArgs);
public:
	/** Get messages of designated types, optionally sort them by time (newer one at front) */
	void GetChatMessages(const TArray<EB2ChatType>& WantedTypes, TArray<FB2ChatMessage>& OutData, bool bSortByTime = true);

	bool UpdateGuildChatMessage();

	void AddChatMessage(EB2ChatType InType, const FB2ChatMessage& InMessageStruct);

	/** Send chatting message from local user. */
	void RequestChatCommand(EB2ChatType InType, const FText& InMessageText);

	bool IsPossibleSpamMessage(const FText& InText);

	bool IsBlockUser(int32 InAccountId) const;
	void SetBlockUser(int64 AccountId, const FString& NickName);
	void RemoveBlockUser(int64 AccountId);

	void RequestGetChannelNum();
	void ResponseGetChannelNum(const FB2GetChatChannelResult& InChannelNumResponse);

	void RequestChangeChannel(int32 InReqNum);
	void ResponseChangeChannel();

	void RequestGetGuildChat();

	void RequestBlockUser(const int64 InAccountId);
	void ResponseBlockUser(const FB2ResponseBlockChatPtr& InBlockUser);

	void RequestUnBlockUser(const int64 InAccountId);
	void ResponseUnBlockUser();

	void RequestBlockUserList();
	void ResponseBlockUserList(const FB2ResponseGetBlockChatListPtr& InBlockUsers);

	bool TryGetNewChatMessage(FB2ChatMessage &OutData);
	void ClearNewChatMessages();
private:

	void ResponseAskGuildSupport(const FB2ResponseAskGuildSupportPtr &InAskSuuportResponse);
	void ResponseGuildSupport(const FB2ResponseGuildSupportPtr &InResponse);
	void ResponseGetGuildChat(const FB2ResponseGetGuildChatPtr &InResponse);
	void ResponseAdminMessage(const FString &InMessage, const int32 DisplayTime);

	void GuildOutStateSetting(const FString& SendName,int64 InUpdateTime);
	void SetChannelNum(int32 InChannelNum);
public:
	FORCEINLINE int32 GetCurrentUserBlockCount() const { return BlockUsers.Num(); }
	FORCEINLINE int32 GetMaxUserBlockCount() const { return MaxBlockList; }
	FORCEINLINE const TMap<int32, FString>& GetBlockUserList() const { return BlockUsers; }
	FORCEINLINE bool IsInAllowedChannelRange(int32 TestNum) { return (TestNum >= MinAllowedChannel && TestNum <= MaxAllowedChannel); }
	FORCEINLINE int32 GetMinAllowedChannel() { return MinAllowedChannel; }
	FORCEINLINE int32 GetMaxAllowedChannel() { return MaxAllowedChannel; }
	FORCEINLINE int32 GetCurrentChannel() { return CurrentChannel; }
};
