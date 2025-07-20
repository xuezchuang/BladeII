
#include "B2Chatting.h"
//#include "B2UIChatting.h"
//#include "B2ClientDataStore.h"
#include "Event.h"
//#include "EventSubsystem.h"
//#include "B2UIManager.h"
//#include "Retailer.h"
//#include "InternalEvents.h"
//#include "B2UIDocHelper.h"
//#include "BladeIIGameMode.h"
//#include "B2LobbyGameMode.h"
//#include "Retailer.h"
//#include "B2ItemInfo.h"
//#include "B2ChattingFilteringInfo.h"
//#include "B2UIUtil.h"
//#include "B2UIBlockList.h"
//#include "B2UIMsgPopupCenter.h"

//#include "BladeIIGameImpl.h"

/*==================================================
FB2ChatTypeMessageList
==================================================*/
FB2ChatMessage* FB2ChatTypeMessageList::FindMessage(const int64 ChatID)
{
	// 0捞搁 八荤 阂啊
	if (ChatID < 1)
		return nullptr;

	for (auto& Each : Messages)
	{
		if (Each.ChatID == ChatID)
			return &Each;
	}

	return nullptr;
}

void FB2ChatTypeMessageList::RemoveFromChatID(const int64 ChatID)
{
	// 0捞搁 八荤 阂啊
	if (ChatID < 1)
		return;
	for (int32 i = 0; i < Messages.Num(); ++i)
	{
		if (Messages[i].ChatID == ChatID)
		{
			Messages.RemoveAt(i);
			return;
		}
	}
}

void FB2ChatTypeMessageList::RemoveFromUserName(const FString& Name, const EB2ChatNoticeType InNotice)
{
	if (Name.IsEmpty())
		return;

	for (int32 i = Messages.Num() - 1; i > -1; --i)
	{
		if (InNotice == Messages[i].NoticeType && Messages[i].SendUserNick.ToString() == Name)
		{
			Messages.RemoveAt(i);
		}
	}
}

/*==================================================
FB2ChatMessage
==================================================*/
void FB2ChatMessage::SetCurrentAsMessageTime()
{
	if (MessageType != EB2ChatType::GuildNotice && MessageType != EB2ChatType::Guild)
	{
		// 角力肺 皋矫瘤啊 辑滚俊辑 档馒茄 矫痢 趣篮 磊脚狼 皋矫瘤俊 措茄 铰牢脚龋啊 柯 矫痢俊 技泼登绢具 窃.
		MessageTime = FDateTime::UtcNow().ToUnixTimestamp();//static_cast<int64>(FPlatformTime::Seconds() * 1000);
	}
}

int32 FB2ChatMessage::GetMessageElapsedTime() const
{
	if (MessageType != EB2ChatType::GuildNotice && MessageType != EB2ChatType::Guild)
		return static_cast<int32>(FDateTime::UtcNow().ToUnixTimestamp() - MessageTime);//(FPlatformTime::Seconds() - (MessageTime * 0.001f)) ;
	else
	{
		int64 NextAskTime = MessageTime;
		FTimespan RemainTime2 = FDateTime::UtcNow() - FDateTime::FromUnixTimestamp(NextAskTime / 1000);
		int32 Seconds = RemainTime2.GetTotalSeconds();
		return Seconds;
	}
}

bool FB2ChatMessage::HasGuildInfo() const
{
	return GuildInfo.get() != nullptr && GuildInfo->GuildId > 0;
}

FText FB2ChatMessage::GetAreaName(int32 AreaNum)
{
	EB2ChatGetRouteType AreaType = SvrToCliGetRouteType(AreaNum);
	//switch (AreaType)
	//{
	//case EB2ChatGetRouteType::Shop:
	//	return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_PublicNoti_GetItemFromShop"));
	//case EB2ChatGetRouteType::Lottery:
	//	return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_PublicNoti_GetItemFromLottery"));
	//case EB2ChatGetRouteType::Compose:
	//	return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_PublicNoti_GetItemFromCompose"));
	//case EB2ChatGetRouteType::Upgrade:
	//	return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_PublicNoti_GetItemFromUpgrade"));
	//case EB2ChatGetRouteType::MakeDonation:
	//	return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_PublicNoti_GetItemFromDonation"));
	//case EB2ChatGetRouteType::Factory_Forge:
	//	return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_PublicNoti_GetItemFromFactory_Forge"));
	//case EB2ChatGetRouteType::Factory_Upgrade:
	//	return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_PublicNoti_GetItemFromFactory_Upgrade"));
	//}

	return FText::FromString(TEXT(""));
}

void FB2ChatMessage::SetChatInfo(b2network::FB2MessageInfoChatNotifyPtr InChatInfo)
{
	if (InChatInfo.get() == nullptr)
		return;

	MessageType = SvrToCliChatType(InChatInfo->type);
	bool IsNoticeType = MessageType == EB2ChatType::Notice || MessageType == EB2ChatType::GuildNotice;
	NoticeType = IsNoticeType ? SvrToCliChatNoticeType(InChatInfo->notice_type) : EB2ChatNoticeType::None;

	SendUserNick = FText::FromString(InChatInfo->from);
	UserAcccountID = InChatInfo->account_id;
	ChatID = 0;
	CharacterMaxLevel = InChatInfo->character_level_max;

	if (InChatInfo->guild.get() != nullptr && InChatInfo->guild->guild_id > 0)
	{
		GuildInfo = std::make_shared<FB2ChatGuildInfo>();
		GuildInfo->PatternIndex = InChatInfo->guild->mark_index;
		GuildInfo->PetternColor = InChatInfo->guild->mark_color;
		GuildInfo->BGImage = InChatInfo->guild->mark_bg_index;
		GuildInfo->BGColor = InChatInfo->guild->mark_bg_color;
		GuildInfo->GuildId = InChatInfo->guild->guild_id;
		GuildInfo->Name = FText::FromString(InChatInfo->guild->guild_name);
	}

	NoticeArgs = InChatInfo->notice_args;
	SetConvertMessage(InChatInfo->body);

	//bIsMyMessage = (BladeIIGameImpl::GetClientDataStore().GetUserNickName() == InChatInfo->from);
	SetCheckBroadCastMessage();
}

void FB2ChatMessage::SetChatInfo(b2network::B2GuildChatDataPtr InGuildChatInfo)
{
	if (InGuildChatInfo.get() == nullptr)
		return;

	MessageType = SvrToCliChatType(InGuildChatInfo->chat_type);
	bool IsNoticeType = (MessageType == EB2ChatType::GuildNotice);
	NoticeType = IsNoticeType ? SvrToCliChatNoticeType(InGuildChatInfo->chat_notice_type) : EB2ChatNoticeType::None;

	SendUserNick = FText::FromString(InGuildChatInfo->nickname);
	UserAcccountID = InGuildChatInfo->account_id;
	ChatID = InGuildChatInfo->id;
	CharacterMaxLevel = InGuildChatInfo->character_level_max;
	// 辨靛 瘤盔父 update_time阑 敬促.(瘤盔肯丰绰 绝绢瘤扁锭巩俊 救靖)
	MessageTime = (NoticeType == EB2ChatNoticeType::GuildSupport ? InGuildChatInfo->update_time : InGuildChatInfo->create_time);

	if (InGuildChatInfo->guild_id > 0)
	{
		GuildInfo = std::make_shared<FB2ChatGuildInfo>();
		GuildInfo->GuildId = InGuildChatInfo->guild_id;
		GuildInfo->SupportTempleteID = InGuildChatInfo->support_item_template_id;
		GuildInfo->SupportCount = InGuildChatInfo->support_count;
		GuildInfo->SupportMaxCount = InGuildChatInfo->support_max_count;
		GuildInfo->UpdateTime = InGuildChatInfo->update_time;
		GuildInfo->ExpireTime = InGuildChatInfo->expire_time;
	}

	NoticeArgs = InGuildChatInfo->notice_args;
	SetConvertMessage(InGuildChatInfo->message);

	//bIsMyMessage = (BladeIIGameImpl::GetClientDataStore().GetUserNickName() == InGuildChatInfo->nickname);
	SetCheckBroadCastMessage();
}

bool FB2ChatMessage::IsBroadcastMessage() const
{
	return bCanBroadcast;
}

void FB2ChatMessage::SetCheckBroadCastMessage()
{
	bCanBroadcast = false;
	bool IsNoticeType = MessageType == EB2ChatType::Notice || MessageType == EB2ChatType::GuildNotice;
	//if (IsNoticeType)
	//{
	//	if (NoticeType == EB2ChatNoticeType::GetItem)
	//	{
	//		if (NoticeArgs.Num() > 2)
	//		{
	//			int32 StarGrade = FItemRefIDHelper::ExtractItemStarGradeFromRefID(FCString::Atoi(*NoticeArgs[1]));

	//			// 傍瘤鸥涝吝 酒捞袍 裙垫 舅覆老 版快父 规价啊瓷窃. (6己何磐 啊瓷)
	//			const int32 LimitStarGrade = FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE - 1;
	//			bCanBroadcast = StarGrade >= LimitStarGrade;
	//		}
	//	}
	//	else if (NoticeType == EB2ChatNoticeType::EnhanceItem)
	//	{
	//		if (NoticeArgs.Num() > 2)
	//			bCanBroadcast = (BladeIIGameImpl::GetClientDataStore().GetUserNickName() != NoticeArgs[0]);
	//	}
	//}
}

void FB2ChatMessage::AddNoticeArgs(FString Arg)
{
	NoticeArgs.Add(Arg);
}

void FB2ChatMessage::SetSystemChatMessage(EB2ChatNoticeType InNoticeType)
{
	FText ResultText = FText::FromString(TEXT(""));
	//switch (InNoticeType)
	//{
	//case EB2ChatNoticeType::GuildJoin:
	//{
	//	FText UserName = NoticeArgs.Num() > 0 ? FText::FromString(NoticeArgs[0]) : FText::FromString(TEXT(""));
	//	ResultText = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_Notification_GuildJoin")), UserName);
	//}
	//break;
	//case EB2ChatNoticeType::GuildLeave:
	//{
	//	FText UserName = NoticeArgs.Num() > 0 ? FText::FromString(NoticeArgs[0]) : FText::FromString(TEXT(""));
	//	ResultText = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_Notification_GuildDropout")), UserName);
	//}
	//break;
	//case EB2ChatNoticeType::GuildKick:
	//{
	//	FText UserName = NoticeArgs.Num() > 0 ? FText::FromString(NoticeArgs[0]) : FText::FromString(TEXT(""));
	//	ResultText = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_Notification_GuildBanish")), UserName);
	//}
	//break;
	//case EB2ChatNoticeType::GuildStartBattle:
	//	ResultText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_Notification_GuildMatchStart"));
	//	break;
	//case EB2ChatNoticeType::GuildEndBattle:
	//	ResultText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_Notification_GuildMatchEnd"));
	//	break;
	//case EB2ChatNoticeType::GuildChangeNotice:
	//	ResultText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_Notification_GuildChangeNotice"));
	//	break;
	//case EB2ChatNoticeType::GetItem:
	//{
	//	if (NoticeArgs.Num() > 2)
	//	{
	//		int32 ItemRefID = FCString::Atoi(*NoticeArgs[1]);
	//		FText UserName = FText::FromString(NoticeArgs[0]);

	//		FText ItemName = StaticFindItemInfo()->GetItemName(ItemRefID);
	//		int32 StarGrade = FItemRefIDHelper::ExtractItemStarGradeFromRefID(ItemRefID);

	//		ItemName = FText::Format(FText::FromString(TEXT("{0} {1}"))
	//			, FText::FromString(FString::FromInt(StarGrade))
	//			, ItemName);

	//		FText ItemColor = FItemGradeInfo::GetItemGradeColor(StarGrade);
	//		int32 GetAreaNum = FCString::Atoi(*NoticeArgs[2]);
	//		FText AreaName = GetAreaName(GetAreaNum);
	//		ResultText = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_PublicNoti_GetItem"))
	//			, UserName
	//			, ItemColor
	//			, ItemName
	//			, AreaName);
	//	}
	//}
	//break;
	//case EB2ChatNoticeType::RaidStart:
	//	ResultText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_PublicNoti_RaidStart"));
	//	break;
	//case EB2ChatNoticeType::RaidEnd:
	//	ResultText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_PublicNoti_RaidEnd"));
	//	break;
	//case EB2ChatNoticeType::AssaultStart:
	//	ResultText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_PublicNoti_CaptureStart"));
	//	break;
	//case EB2ChatNoticeType::AssaultEnd:
	//	ResultText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_PublicNoti_CaptureEnd"));
	//	break;
	//case EB2ChatNoticeType::GuildSupport:
	//{
	//	// 			FB2Item CurItem;
	//	// 			BladeIIGameImpl::GetGuildDataStore().GetGuildSupportItem(GuildInfo->SupportTempleteID, CurItem);

	//	UB2ItemInfo* ItemInfoTable = StaticFindItemInfo();
	//	FSingleItemInfoData* ThisItemInfoData = HasGuildInfo() ? ItemInfoTable->GetInfoData(GuildInfo->SupportTempleteID) : NULL;

	//	ResultText = ThisItemInfoData->GetLocalizedName();
	//}
	//break;
	//case EB2ChatNoticeType::EnhanceItem:
	//{
	//	if (NoticeArgs.Num() >= 3)
	//	{
	//		FText UserName = FText::FromString(NoticeArgs[0]);
	//		int32 ItemRefID = FCString::Atoi(*NoticeArgs[1]);
	//		FText EnhanceCount = FText::FromString(NoticeArgs[2]);

	//		FText ItemName = StaticFindItemInfo()->GetItemName(ItemRefID);
	//		int32 StarGrade = FItemRefIDHelper::ExtractItemStarGradeFromRefID(ItemRefID);

	//		ItemName = FText::Format(FText::FromString(TEXT("{0} {1}"))
	//			, FText::FromString(FString::FromInt(StarGrade))
	//			, ItemName);

	//		FText ItemColor = FItemGradeInfo::GetItemGradeColor(StarGrade);

	//		ResultText = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_PublicNoti_EnhanceItem"))
	//			, UserName
	//			, ItemColor
	//			, ItemName
	//			, EnhanceCount);
	//	}
	//}
	//break;

	//case EB2ChatNoticeType::ChangeChannel:
	//{
	//	FText ChannelNum = NoticeArgs.Num() > 0 ? FText::FromString(NoticeArgs[0]) : FText::FromString(TEXT(""));
	//	ResultText = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_Notification_MoveChannel")), ChannelNum);
	//}
	//break;
	//case EB2ChatNoticeType::AdminMessage:
	//{
	//	ResultText = NoticeArgs.Num() > 0 ? FText::FromString(NoticeArgs[0]) : FText::FromString(TEXT(""));
	//}
	//break;
	//}

	MessageText = ResultText;
}

void FB2ChatMessage::SetConvertMessage(const FString& InMessage)
{
	FText ResultText = FText::FromString(TEXT(""));
	bool IsNoticeType = MessageType == EB2ChatType::Notice || MessageType == EB2ChatType::GuildNotice;
	bool HasNotice = NoticeType != EB2ChatNoticeType::None;
	if (HasNotice)
	{
		SetSystemChatMessage(NoticeType);
	}
	else
	{
		//UB2ChattingFilteringInfo* ChattingFilteringInfo = StaticFindChattingFilteringInfo();
		//if (ChattingFilteringInfo)
		//{
		//	ResultText = ChattingFilteringInfo->Filtering(InMessage);
		//}
		//else
		//{
		//	ResultText = FText::FromString(InMessage);
		//}

		MessageText = ResultText;
	}
}


/*==================================================
FB2ChatStore
==================================================*/

const int32 FB2ChatStore::MessageLengthLimit = 140; // 辑滚俊辑 罐酒具 茄促搁 const 钱绊 罐档废..
const int32 FB2ChatStore::MinAllowedChannel = 1;
const int32 FB2ChatStore::MaxAllowedChannel = 999;

FB2ChatStore::FB2ChatStore()
{
	CurrentChannel = 0;
	LastRequestedChannelNum = -1;
	CachedGuildID = 0;
	bInitGetGuildChat = false;
	MaxBlockList = 10;
	CachedChatID = 0;
}

void FB2ChatStore::OnModuleStartup()
{
	SubscribeEvent();

	for (int8 CCI = 0; CCI < static_cast<int8>(EB2ChatType::End); ++CCI)
	{
		EB2ChatType ChatType = static_cast<EB2ChatType>(CCI);
		int32 Capacity = 0;
		int32 Sort = 0;
		if (ChatType == EB2ChatType::GuildNotice)
		{
			Capacity = FB2ChatManagementRaw::Capacity::GuildSupport;
			Sort = FB2ChatManagementRaw::Sort::Id;
		}
		else
		{
			Capacity = FB2ChatManagementRaw::Capacity::Chat;
			Sort = FB2ChatManagementRaw::Sort::Time;
		}
		FB2ChatTypeMessageList NewList(Capacity, Sort);
		ChatData.Add(ChatType, NewList);
	}

}

void FB2ChatStore::OnModuleShutdown()
{
	UnsubscribeEvent();
}

void FB2ChatStore::SubscribeEvent()
{
	UnsubscribeEvent();

	//Issues.Add(data_trader::ChatMessageNotifyClass<b2network::FB2MessageInfoChatNotifyPtr>::GetInstance().Subscribe2(
	//	[this](b2network::FB2MessageInfoChatNotifyPtr InChatInfo)
	//	{
	//		this->OnServerChatMessageNotify(InChatInfo);
	//	}
	//));

	//Issues.Add(data_trader::GuildChatMessageNotifyClass<b2network::FB2MessageInfoGuildChatNotifyPtr>::GetInstance().Subscribe2(
	//	[this](b2network::FB2MessageInfoGuildChatNotifyPtr InGuildChatInfo)
	//	{
	//		this->OnServerGuildChatMessageNotify(InGuildChatInfo);
	//	}
	//));

	//Issues.Add(DeliveryGetChatChannelClass<FB2GetChatChannelResult>::GetInstance().Subscribe2(
	//	[this](const FB2GetChatChannelResult& InResponse)
	//	{
	//		this->ResponseGetChannelNum(InResponse);
	//	}
	//));

	//Issues.Add(DeliveryChangeChatChannelClass<>::GetInstance().Subscribe2(
	//	[this]()
	//	{
	//		this->ResponseChangeChannel();
	//	}
	//));

	//Issues.Add(DeliveryAskGuildSupportClass<FB2ResponseAskGuildSupportPtr>::GetInstance().Subscribe2(
	//	[this](const FB2ResponseAskGuildSupportPtr& InResponse)
	//	{
	//		this->ResponseAskGuildSupport(InResponse);
	//	}
	//));

	//Issues.Add(DeliveryGuildSupportClass<FB2ResponseGuildSupportPtr>::GetInstance().Subscribe2(
	//	[this](const FB2ResponseGuildSupportPtr& InResponse)
	//	{
	//		this->ResponseGuildSupport(InResponse);
	//	}
	//));

	//Issues.Add(DeliveryGetGuildChatClass<FB2ResponseGetGuildChatPtr>::GetInstance().Subscribe2(
	//	[this](const FB2ResponseGetGuildChatPtr& InResponse)
	//	{
	//		this->ResponseGetGuildChat(InResponse);
	//	}
	//));

	//Issues.Add(data_trader::AdminMessageNotifyClass<const FString&, int32>::GetInstance().Subscribe2(
	//	[this](const FString& InMessage, int32 InDisaplayTime)
	//	{
	//		this->ResponseAdminMessage(InMessage, InDisaplayTime);
	//	}
	//));

	//Issues.Add(DeliveryBlcokUserClass<FB2ResponseBlockChatPtr>::GetInstance().Subscribe2(
	//	[this](const FB2ResponseBlockChatPtr& InBlockUser)
	//	{
	//		this->ResponseBlockUser(InBlockUser);
	//	}
	//));

	//Issues.Add(DeliveryUnBlcokUserClass<>::GetInstance().Subscribe2(
	//	[this]()
	//	{
	//		this->ResponseUnBlockUser();
	//	}
	//));

	//Issues.Add(DeliveryGetBlockChatListClass<FB2ResponseGetBlockChatListPtr>::GetInstance().Subscribe2(
	//	[this](const FB2ResponseGetBlockChatListPtr& InBlockUser)
	//	{
	//		this->ResponseBlockUserList(InBlockUser);
	//	}
	//));

	RequestGetGuildChat();
}

void FB2ChatStore::UnsubscribeEvent()
{
	Issues.Empty();
}

void FB2ChatStore::OnServerChatMessageNotify(b2network::FB2MessageInfoChatNotifyPtr& InChatInfo)
{
	FB2ChatMessage CliChatMsgInfo;
	CliChatMsgInfo.SetChatInfo(InChatInfo);

	if (IsBlockUser(CliChatMsgInfo.UserAcccountID))
	{
		return;
	}

	AddChatMessage(CliChatMsgInfo.MessageType, CliChatMsgInfo);

	//UpdateChatUIClass<bool>::GetInstance().Signal(IsShowingChatCategory(CliChatMsgInfo.MessageType)); // UI 啊 凯妨 乐促搁 诀单捞飘
	// 盲泼 UI 啊 凯妨乐瘤 臼篮 版快甫 困秦 茄临 盲泼 矫弊澄档

	// 教臂盲泼扑诀篮 宏肺靛某胶飘 皋技瘤老版快父 捞亥飘 朝妨淋
	//if (CliChatMsgInfo.IsBroadcastMessage())
	//	UpdateChatSinglePopupClass<const FB2ChatMessage&>::GetInstance().Signal(CliChatMsgInfo);
}

void FB2ChatStore::OnServerGuildChatMessageNotify(b2network::FB2MessageInfoGuildChatNotifyPtr& GuildChatNotify)
{
	SetGuildChatBadge(GuildChatNotify);
	SetProcessGuildChatMessages(GuildChatNotify);
}

void FB2ChatStore::SetGuildChatBadge(b2network::FB2MessageInfoGuildChatNotifyPtr& GuildChatNotify)
{
	//UB2UIManager* UIMgrInst2 = UB2UIManager::GetInstance();
	//if (UIMgrInst2)
	//{
	//	auto* Popup = UIMgrInst2->GetUI<UB2UIChatting>(UIFName::Chatting);
	//	if (Popup == nullptr || Popup->GetCurChatCategory() != EB2UIChatCategory::UICHTY_Guild)
	//	{
	//		UB2UIDocGuild* DocGuild = UB2UIDocHelper::GetDocGuild();
	//		if (DocGuild)
	//		{
	//			const int32 NoticeCount = FMath::Clamp(DocGuild->GetGuildChatBadge() + GuildChatNotify->chat_list.Num(), 0, FB2ChatManagementRaw::Capacity::Chat);
	//			DocGuild->SetGuildChatBadge(NoticeCount);
	//		}
	//	}
	//	if (Popup)
	//		Popup->UpdateGuildBadge();
	//}
}

void FB2ChatStore::SetProcessGuildChatMessages(b2network::FB2MessageInfoGuildChatNotifyPtr& GuildChatNotify)
{
	bool bIsAutoScrollEnd = true;

	// 漂沥 皋技瘤老 版快 扁粮 皋技瘤俊辑 瘤款促..
	for (auto& guildChat : GuildChatNotify->chat_list)
	{
		EB2ChatNoticeType NoticeType = SvrToCliChatNoticeType(guildChat->chat_notice_type);
		if (IsSpecialGuildNotify(NoticeType))
		{
			EB2ChatType ChatType = SvrToCliChatType(guildChat->chat_type);
			FB2ChatTypeMessageList* ThisList = ChatData.Find(ChatType);
			if (ThisList)
			{
				TArray<FB2ChatMessage>& ThisMessages = ThisList->Messages;

				const FString SendName = guildChat->notice_args.Num() > 0 ? guildChat->notice_args[0] : FString(TEXT(""));
				const int64 ChatID = guildChat->id;

				switch (NoticeType)
				{
				case EB2ChatNoticeType::GuildKick:
				case EB2ChatNoticeType::GuildLeave:
				{
					GuildOutStateSetting(SendName, guildChat->create_time);
					ThisList->RemoveFromUserName(SendName, EB2ChatNoticeType::GuildSupport);
				}
				break;
				default:
					ThisList->RemoveFromChatID(ChatID);
					break;
				}
			}
		}
	}

	for (auto& guildChat : GuildChatNotify->chat_list)
	{
		FB2ChatMessage CliChatMsgInfo;
		CliChatMsgInfo.SetChatInfo(guildChat);

		if (IsSpecialGuildNotify(SvrToCliChatNoticeType(guildChat->chat_notice_type)))
		{
			if (IsShowingChatCategory(CliChatMsgInfo.MessageType))
			{
				CachedNewChat.Enqueue(CliChatMsgInfo);
			}
			continue;
		}

		bool bIsNewGuildSupport = CliChatMsgInfo.HasGuildInfo() ? CliChatMsgInfo.GuildInfo->SupportCount > 0 : false;
		if (bIsNewGuildSupport)
			bIsAutoScrollEnd = false;

		AddChatMessage(CliChatMsgInfo.MessageType, CliChatMsgInfo);
	}

	bIsAutoScrollEnd = bIsAutoScrollEnd ? IsShowingChatCategory(EB2ChatType::Guild) : bIsAutoScrollEnd;

	//UpdateChatUIClass<bool>::GetInstance().Signal(bIsAutoScrollEnd); // UI 啊 凯妨 乐促搁 诀单捞飘

	//UpdateMarkRedDotClass<const FName&>::GetInstance().Signal(UIFName::BattleMain);
}

bool FB2ChatStore::IsSpecialGuildNotify(EB2ChatNoticeType InNoticeType)
{
	TArray<EB2ChatNoticeType> SpecialList = {
		EB2ChatNoticeType::GuildSupportComplete,
		EB2ChatNoticeType::GuildKick,
		EB2ChatNoticeType::GuildLeave,
	};

	for (int32 i = 0; i < SpecialList.Num(); ++i)
	{
		if (SpecialList[i] == InNoticeType)
			return true;
	}

	return false;
}

bool FB2ChatStore::IsShowingChatCategory(EB2ChatType InType)
{
	//if (auto* Popup = UB2UIManager::GetInstance()->GetUI<UB2UIChatting>(UIFName::Chatting))
	//{
	//	switch (Popup->GetCurChatCategory())
	//	{
	//	case EB2UIChatCategory::UICHTY_All:
	//		return InType == EB2ChatType::Channel;
	//	case EB2UIChatCategory::UICHTY_Guild:
	//		return InType == EB2ChatType::Guild || InType == EB2ChatType::GuildNotice;
	//	case EB2UIChatCategory::UICHTY_Noti:
	//		return InType == EB2ChatType::Notice;
	//	default:
	//		return false;
	//	}
	//}

	return false;
}

void FB2ChatStore::GetChatMessages(const TArray<EB2ChatType>& WantedTypes, TArray<FB2ChatMessage>& OutData, bool bSortByTime)
{
	for (EB2ChatType ThisType : WantedTypes)
	{
		FB2ChatTypeMessageList* ThisList = ChatData.Find(ThisType);
		if (ThisList)
		{
			OutData.Append(ThisList->Messages);
		}
	}

	if (bSortByTime)
	{
		// 肚 促弗 沥纺 扁霖捞 鞘夸秦瘤搁 StableSort 肺
		OutData.Sort(FB2ChatMessageListTimeSorter());
	}
	else
	{
		OutData.Sort(FB2ChatMessageListIDSorter());
	}
}

bool FB2ChatStore::UpdateGuildChatMessage()
{
	TArray<EB2ChatType> RetArray;

	RetArray.Add(EB2ChatType::Guild);
	RetArray.Add(EB2ChatType::GuildNotice);

	TArray<FB2ChatMessage> AllMessages;
	GetChatMessages(RetArray, AllMessages, false);

	if (AllMessages.Num() < 1)
		return false;

	int32 OriginCount = AllMessages.Num();
	FDateTime CurrentTime = FDateTime::UtcNow();
	for (int32 i = AllMessages.Num() - 1; i >= 0; --i)
	{
		if (AllMessages[i].NoticeType != EB2ChatNoticeType::GuildSupport)
			continue;

		int64 NextAskTime = AllMessages[i].HasGuildInfo() ? AllMessages[i].GuildInfo->ExpireTime : 0;
		FTimespan RemainTime2 = FDateTime::FromUnixTimestamp(NextAskTime / 1000) - CurrentTime;
		int32 Seconds = RemainTime2.GetTotalSeconds();
		if (Seconds < 1)
		{
			AllMessages.RemoveAt(i);
		}
	}

	return !(OriginCount == AllMessages.Num());
}

void FB2ChatStore::AddChatMessage(EB2ChatType InType, const FB2ChatMessage& InMessageStruct)
{
	FB2ChatTypeMessageList* ThisList = ChatData.Find(InType);
	if (ThisList)
	{
		TArray<FB2ChatMessage>& ThisMessages = ThisList->Messages;

		// 扁粮俊 勒栏搁 绝沮.
		if (InMessageStruct.NoticeType == EB2ChatNoticeType::GuildSupport)
		{
			FB2ChatMessage* FindMessage = ThisList->FindMessage(InMessageStruct.ChatID);
			if (FindMessage)
			{
				ThisList->RemoveFromChatID(InMessageStruct.ChatID);
			}
		}

		int32 NewElemIndex = ThisMessages.Add(InMessageStruct);
		if (ThisMessages.IsValidIndex(NewElemIndex))
		{
			ThisMessages[NewElemIndex].MessageType = InType; // 鸥涝篮 咯扁辑 嘎苗淋.
			// 咯扁辑 MessageTime 捞 技泼登骨肺 盲泼 皋矫瘤啊 坷搁 官肺 捞吧 烹秦 Add 甫 秦具 窃.
			ThisMessages[NewElemIndex].SetCurrentAsMessageTime();
		}

		if (IsShowingChatCategory(InType))
		{
			CachedNewChat.Enqueue(ThisMessages[NewElemIndex]);
		}

		// 力茄 俺荐 逞板绰瘤 眉农窍绊 力芭
		if (ThisList->SortType == FB2ChatManagementRaw::Sort::Id)
			ThisMessages.Sort(FB2ChatMessageListIDSorter());
		else
			ThisMessages.Sort(FB2ChatMessageListTimeSorter());

		// 力茄 俺荐甫 逞板绰瘤 眉农
		if (ThisMessages.Num() > ThisList->ClientStoreLimit)
		{
			ThisMessages.RemoveAt(ThisList->ClientStoreLimit, ThisMessages.Num() - ThisList->ClientStoreLimit);
		}
	}
}

void FB2ChatStore::RequestChatCommand(EB2ChatType InType, const FText& InMessageText)
{
	//if (InType != EB2ChatType::End)
	//{
	//	// 焊郴绰 措惑篮 老措老 盲泼捞 绝促搁 鞘夸绝阑 巴..
	//	// TODO get max character level	//力老 臭篮 某腐磐 饭骇 荤侩
	//	int32 characterLevelMax = BladeIIGameImpl::GetLocalCharacterData().GetUserLevel();
	//	data_trader::SendChatMessageClass<int32, const FString&, int32, const FString&, int32>::GetInstance().Signal(
	//		CliToSvrChatType(InType), TEXT(""), 0, InMessageText.ToString(), characterLevelMax
	//	);
	//}
}

void FB2ChatStore::RequestGetGuildChat()
{
//	bool bChangeGuild = BladeIIGameImpl::GetClientDataStore().GetUserGuildID() != CachedGuildID;
//	if (bChangeGuild == false && bInitGetGuildChat)
//		return;
//
//	bInitGetGuildChat = true;
//	CachedGuildID = BladeIIGameImpl::GetClientDataStore().GetUserGuildID();
//	data_trader::Retailer::GetInstance().RequestGetGuildChat(0);
}

void FB2ChatStore::RequestBlockUser(const int64 InAccountId)
{
	//if (BlockUsers.Num() >= MaxBlockList)
	//{
	//	UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupSimple>(EUIMsgPopup::Simple,
	//		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Notification")),
	//		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("FullBlockList")),
	//		0.f,
	//		true,
	//		true,
	//		EUIMsgPopupButtonGroup::Confirm
	//	);

	//	return;
	//}

	//data_trader::RequestBlockChatClass<int64>::GetInstance().Signal(InAccountId);
}

void FB2ChatStore::ResponseBlockUser(const FB2ResponseBlockChatPtr& InBlockUse)
{
	//if (InBlockUse && InBlockUse->blocked_user)
	//{
	//	if (auto* UIChatting = UB2UIManager::GetInstance()->GetUI<UB2UIChatting>(UIFName::Chatting))
	//	{
	//		UIChatting->SetUserBlockState(true, InBlockUse->blocked_user->nickname);
	//	}

	//	SetBlockUser(InBlockUse->blocked_user->account_id, InBlockUse->blocked_user->nickname);
	//}
}

void FB2ChatStore::RequestUnBlockUser(const int64 InAccountId)
{
	//if (auto* UIChatting = UB2UIManager::GetInstance()->GetUI<UB2UIChatting>(UIFName::Chatting))
	//{
	//	bool IsBlockUser = BlockUsers.Contains(InAccountId);
	//	UIChatting->SetUserBlockState(false, IsBlockUser ? BlockUsers[InAccountId] : FString(""));
	//}

	//data_trader::RequestUnblockChatClass<int64>::GetInstance().Signal(InAccountId);
	RemoveBlockUser(InAccountId);
}

void FB2ChatStore::ResponseUnBlockUser()
{
}

void FB2ChatStore::RequestBlockUserList()
{
	//data_trader::RequestGetBlockChatListClass<>::GetInstance().Signal();
}

void FB2ChatStore::ResponseBlockUserList(const FB2ResponseGetBlockChatListPtr& InBlockUsers)
{
	if (InBlockUsers)
	{
		for (const auto& BlockUser : InBlockUsers->blocked_users)
		{
			BlockUsers.Add(BlockUser->account_id, BlockUser->nickname);
		}
	}
}

bool FB2ChatStore::IsPossibleSpamMessage(const FText& InText)
{

	return false;
}

void FB2ChatStore::SetBlockUser(int64 AccountId, const FString& NickName)
{
	if (!BlockUsers.Contains(AccountId))
	{
		BlockUsers.Add(AccountId, NickName);
	}
}

void FB2ChatStore::RemoveBlockUser(int64 AccountId)
{
	//if (auto* Popup = UB2UIManager::GetInstance()->GetUI<UB2UIBlockList>(UIFName::BlockUserList))
	//{
	//	Popup->RemoveBlockUser(AccountId);
	//}

	//if (BlockUsers.Contains(AccountId))
	//{
	//	BlockUsers.Remove(AccountId);
	//}
}

bool FB2ChatStore::IsBlockUser(int32 InAccountId) const
{
	return BlockUsers.Contains(InAccountId);
}

void FB2ChatStore::RequestGetChannelNum()
{
	//data_trader::Retailer::GetInstance().RequestGetChatChannel();
}

void FB2ChatStore::ResponseGetChannelNum(const FB2GetChatChannelResult& InChannelNumResponse)
{
	//SetChannelNum(GET_TUPLE_DATA(FB2ResponseGetChatChannel::channel_number_index, InChannelNumResponse));
}

void FB2ChatStore::RequestChangeChannel(int32 InReqNum)
{
	if (IsInAllowedChannelRange(InReqNum))
	{
		//data_trader::Retailer::GetInstance().RequestChangeChatChannel(InReqNum);
		LastRequestedChannelNum = InReqNum; // 夸没俊 措茄 览翠捞 坷搁 捞吧肺 技泼且 芭.
	}
}

void FB2ChatStore::ResponseChangeChannel()
{
	if (IsInAllowedChannelRange(LastRequestedChannelNum))
	{
		SetChannelNum(LastRequestedChannelNum);
	}

	TArray<FString> Args;
	Args.Add(FString::FromInt(LastRequestedChannelNum));
	CreateSystemNotice(EB2ChatType::Channel, EB2ChatNoticeType::ChangeChannel, Args);
	//UpdateChatUIClass<bool>::GetInstance().Signal(IsShowingChatCategory(EB2ChatType::Channel)); // UI 啊 凯妨 乐促搁 诀单捞飘

	LastRequestedChannelNum = -1;
}

void FB2ChatStore::SetChannelNum(int32 InChannelNum)
{
	CurrentChannel = InChannelNum;

	//UB2UIDocSome* DocSome = UB2UIDocHelper::GetDocSome();
	//if (DocSome)
	//{
	//	DocSome->SetCurrentChatChannel(CurrentChannel);
	//}
}

void FB2ChatStore::ResponseAskGuildSupport(const FB2ResponseAskGuildSupportPtr& InAskSuuportResponse)
{
	//UB2UIDocGuild* DocGuild = UB2UIDocHelper::GetDocGuild();
	//if (DocGuild)
	//{
	//	DocGuild->SetNextAskSupportTime(InAskSuuportResponse->next_ask_support_time);
	//}

	//auto* Popup = UB2UIManager::GetInstance()->GetUI<UB2UIChatting>(UIFName::Chatting);
	//if (Popup)
	//	Popup->UpdateControlInfo();

}

void FB2ChatStore::ResponseGuildSupport(const FB2ResponseGuildSupportPtr& InResponse)
{
	//BladeIIGameImpl::GetLocalCharacterData().GetUserAllItems().RemoveItems(InResponse->deleted_item_ids);

	//UpdateChatUIClass<bool>::GetInstance().Signal(false); // UI 啊 凯妨 乐促搁 诀单捞飘
	//UpdateMarkRedDotClass<const FName&>::GetInstance().Signal(UIFName::BattleMain);
}

void FB2ChatStore::ResponseGetGuildChat(const FB2ResponseGetGuildChatPtr& InResponse)
{
	//UB2UIDocGuild* DocGuild = UB2UIDocHelper::GetDocGuild();
	//if (DocGuild)
	//{
	//	DocGuild->SetGuildChatBadge(0);
	//	DocGuild->SetNextAskSupportTime(InResponse->next_ask_support_time);
	//}

	//auto* Popup = UB2UIManager::GetInstance()->GetUI<UB2UIChatting>(UIFName::Chatting);
	//if (Popup)
	//	Popup->UpdateGuildBadge();

	FB2ChatTypeMessageList* ThisList = ChatData.Find(EB2ChatType::GuildNotice);
	if (ThisList)
	{
		ThisList->Messages.Empty();
	}
	ThisList = ChatData.Find(EB2ChatType::Guild);
	if (ThisList)
	{
		ThisList->Messages.Empty();
	}

	InResponse->chat_list.Sort([](b2network::B2GuildChatDataPtr A, b2network::B2GuildChatDataPtr B)->bool
		{
			return A->id < B->id; // Newer one first
		});

	for (auto guildChat : InResponse->chat_list)
	{
		FB2ChatMessage CliChatMsgInfo;
		CliChatMsgInfo.SetChatInfo(guildChat);

		AddChatMessage(CliChatMsgInfo.MessageType, CliChatMsgInfo);
	}

	//UpdateChatUIClass<bool>::GetInstance().Signal(IsShowingChatCategory(EB2ChatType::Guild));
	//UpdateMarkRedDotClass<const FName&>::GetInstance().Signal(UIFName::BattleMain);
}

void FB2ChatStore::ResponseAdminMessage(const FString& InMessage, const int32 DisplayTime)
{
	//UpdateAdminMessageClass<const FString&, const int32>::GetInstance().Signal(InMessage, DisplayTime);

	// 矫胶袍 舅覆俊 眠啊
	TArray<FString> Args;
	Args.Add(InMessage);
	CreateSystemNotice(EB2ChatType::Notice, EB2ChatNoticeType::AdminMessage, Args);
	//UpdateChatUIClass<bool>::GetInstance().Signal(IsShowingChatCategory(EB2ChatType::Notice));
}

void FB2ChatStore::GuildOutStateSetting(const FString& SendName, int64 InUpdateTime)
{
	//if (BladeIIGameImpl::GetClientDataStore().GetUserNickName() == SendName)
	//{
	//	auto GuildDoc = UB2UIDocHelper::GetDocGuild();

	//	check(GuildDoc)



	//		GuildDoc->SetKickedGuildId(BladeIIGameImpl::GetClientDataStore().GetUserGuildID());
	//	BladeIIGameImpl::GetClientDataStore().SetGuildAccountID(0);			//磊扁啊 眠规寸沁促搁 辨靛酒捞叼甫 0栏肺 父甸绢霖促;

	//	GuildDoc->SetLeaveGuildTime(InUpdateTime/* + (BladeIIGameImpl::GetClientDataStore().GetGuildRejoinTimeMin() * 60 * 1000)*/);

	//	if (UB2UIManager::GetInstance()->GetCurrUIScene() == EUIScene::GuildMain)
	//	{
	//		//瘤陛 辨靛 皋牢 其捞瘤版快父 蝶肺 贸府
	//		//辨靛俊辑 眠规寸窍继嚼聪促

	//		FMsgPopupOnClick	Delegate;
	//		Delegate.BindLambda([]()
	//			{
	//				UB2UIManager::GetInstance()->ClearUISceneHistory();
	//				LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::LobbyMain);
	//			});
	//		UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
	//			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_MessageHeader_PublicNoti")), FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_DeportationMessage")),
	//				FText::FromString(BladeIIGameImpl::GetClientDataStore().GetMyGuildInfo().GuildInfo->guild_name)),
	//			0, true, true, EUIMsgPopupButtonGroup::Confirm
	//			, Delegate);
	//	}
	//	else
	//	{
	//		GuildDoc->SetGuildOutState(true);
	//	}
	//}
}

void FB2ChatStore::CreateSystemNotice(EB2ChatType InType, EB2ChatNoticeType InNoticeType, const TArray<FString>& InArgs)
{
	// 盲澄 捞悼俊 蝶弗 皋技瘤 眠啊 
	FB2ChatMessage CliChatMsgInfo;
	CliChatMsgInfo.MessageType = InType;
	CliChatMsgInfo.NoticeType = InNoticeType;

	for (auto& Each : InArgs)
	{
		CliChatMsgInfo.AddNoticeArgs(Each);
	}

	CliChatMsgInfo.SetSystemChatMessage(InNoticeType);

	AddChatMessage(CliChatMsgInfo.MessageType, CliChatMsgInfo);

	// 教臂盲泼扑诀篮 宏肺靛某胶飘 皋技瘤老版快父 捞亥飘 朝妨淋
	//if (CliChatMsgInfo.IsBroadcastMessage())
	//	UpdateChatSinglePopupClass<const FB2ChatMessage&>::GetInstance().Signal(CliChatMsgInfo);
}

bool FB2ChatStore::TryGetNewChatMessage(FB2ChatMessage& OutData)
{
	if (CachedNewChat.IsEmpty())
		return false;
	return CachedNewChat.Dequeue(OutData);
}

void FB2ChatStore::ClearNewChatMessages()
{
	if (CachedNewChat.IsEmpty() == false)
		CachedNewChat.Empty();
}