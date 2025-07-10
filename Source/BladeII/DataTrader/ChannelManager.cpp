
#include "ChannelManager.h"

#include "Channel.h"
#include "InternalEvents.h"
#include "Event.h"

#include "Serializer.h"

#include "BladeIIUtil.h"

#include <algorithm>

namespace data_trader
{
	std::unique_ptr<ChannelManager> ChannelManager::Instance = nullptr;

	ChannelManager& ChannelManager::GetInstance()
	{
		if (Instance == nullptr)
		{
			Instance = std::unique_ptr<ChannelManager>(new ChannelManager);
		}

		return *Instance;
	}

	ChannelManager::ChannelManager() 
		: bReplayMode(false)
	{
		Issues.Add(data_trader::RoomParticipantJoinNotityClass<const FString&, b2network::B2RoomParticipantPtr>::GetInstance().Subscribe2(
			[this](const FString& Id, b2network::B2RoomParticipantPtr participant)
		{
			this->SubscribeChannel(Id, participant);
		}
		));
		Issues.Add(data_trader::RoomParticipantLeaveNotityClass<const FString&, b2network::B2RoomParticipantPtr>::GetInstance().Subscribe2(
			[this](FString const& Id, b2network::B2RoomParticipantPtr participant)
		{
			this->UnsubscribeChannel(Id, participant);
		}
		));
		Issues.Add(data_trader::RoomMessageNotifyClass<const FString&, b2network::B2RoomParticipantPtr, const FString&>::GetInstance().Subscribe2(
			[this](const FString& Id, b2network::B2RoomParticipantPtr participant, const FString& payload)
		{
			this->BroadcastMessage(Id, participant->id, payload);
		}
		));
		Issues.Add(data_trader::RoomHostChangedNotifyClass<const FString&, int32, int32>::GetInstance().Subscribe2(
			[this](const FString& Id, int32 RoomType, int32 HostId)
		{
			this->ChangeHost(Id, RoomType, HostId);
		}
		));
		Issues.Add(data_trader::RaidInvitationNotifyClass<const FString&, const FString&, b2network::B2RoomLocationPtr, int32, int32>::GetInstance().Subscribe2(
			[this](const FString& Inviter, const FString& InviterKey, b2network::B2RoomLocationPtr RoomLocationPtr, int32 mapId, int32 difficulty)
		{
			this->NotificationInviteMessage(Inviter, InviterKey, RoomLocationPtr, mapId, difficulty);
		}
		));
		Issues.Add(data_trader::RoomPropertyUpdatedNotifyClass<const FString&, const TArray<b2network::B2RoomPropertyPtr>&>::GetInstance().Subscribe2(
			[this](const FString& RoomId, const TArray<b2network::B2RoomPropertyPtr>& PropertyList)
		{
			this->NotififyUpdateProperty(RoomId, PropertyList);
		}
		));
	}

	ChannelManager::~ChannelManager()
	{
		Issues.Empty();
		ShutdownChannel();
	}

	void ChannelManager::ReleaseInstance()
	{
		if (Instance != nullptr)
			Instance.reset(nullptr);
	}

	void ChannelManager::CreateChannel(FB2RoomJoin& application)
	{
		/*auto Id = std::get<FB2RoomJoinResponse::room_id_index>(application);
		auto& participants = std::get<FB2RoomJoinResponse::participants_index>(application);

		UE_LOG(LogBladeII, Log, TEXT("ChannelManager::CreateChannel - Chennel ID = %s"), *Id);

		if (IsThisChannel(Id) == false)	
		{
			ChannelData.Empty();
			ChannelData = ChannelInfo(Id, ChannelPtr(new Channel(Id)));
		}

		SuccessToMatchmakeClass<ChannelPtr, uint32, const TArray<FB2RoomParticipantPtr>&>::GetInstance().Signal(ChannelData.pChannel, !participants.Num()?1:2, participants);*/
	}

	void ChannelManager::ShutdownChannel(TArray<FString> const& IdList)
	{
		for (auto Id : IdList)
		{
			if (IsThisChannel(Id))
			{
				ShutdownChannel();
				break;
			}
		}
	}

	void ChannelManager::ShutdownChannel()
	{
		ChannelData.Empty();
	}

	void ChannelManager::SubscribeChannel(FString const& Id, b2network::B2RoomParticipantPtr Participant)
	{
		if (!IsThisChannel(Id))
			return;

		auto ChannelPtr = ChannelData.pChannel;

		if (!ChannelPtr->HowMany())
		{
			ReceivedMyNetIdClass<uint32, int64, const FB2FindAccountInfoPtr&>::GetInstance().Signal(Participant->id, Participant->account_id, Participant->account_info);
		}
		else
		{
			ReceivedOtherNetIdClass<uint32, int64, const FB2FindAccountInfoPtr&>::GetInstance().Signal(Participant->id, Participant->account_id, Participant->account_info);
		}

		ChannelPtr->SubscribeChannel(Participant->id);
	}

	void ChannelManager::UnsubscribeChannel(FString const& Id, b2network::B2RoomParticipantPtr Participant)
	{
		if (!IsThisChannel(Id))
			return;

		auto ChannelPtr = ChannelData.pChannel;

		if (ChannelPtr.get() != nullptr)
			ChannelPtr->UnsubscribeChannel(Participant->id);

		ReceivedNotifyLeaveClass<uint32>::GetInstance().Signal(Participant->id);
	}

	void ChannelManager::SendMessage(int32 id, FString const& Body)
	{
		//// 채널에 안붙고 보내려는 메세지 본인한테 바로 돌아오는기능
		//if (IsActiveFakeChannelMessage())
		//{
		//	// 첫인자는 보낸이로 추정되는데. 이벤트 받는곳에서 본인으로 바꿔준다.
		//	BroadcastRoomMessageClass<uint32, const FString&, bool>::GetInstance().Signal(0, Body, true);
		//	return;
		//}

		//if (ChannelData.IsEmpty())
		//{
		//	UE_LOG(LogBladeII, Warning, TEXT("You don't paticipant any channel... so you're not able to send a message..."));
		//	return;
		//}

		//// now we assume there is only one channel... 
		//// so you must change this code to support multi channels...
		////BII_CHECK(channel_info != Channels.end());
		//SendRoomMessageClass<const FString&, int32, const FString&>::GetInstance().Signal(ChannelData.Id, id, Body);
	}

	void ChannelManager::BroadcastMessage(FString const& Id, uint32 SenderId, FString const& payload)
	{
		if (!IsThisChannel(Id))
		{
			UE_LOG(LogBladeII, Warning, TEXT("You haven't paticipanted the channel(id : %s), so you're not able to broadcast a message..."), *Id);
			return;
		}

		BroadcastRoomMessageClass<uint32, const FString&, bool>::GetInstance().Signal(SenderId, payload, false);
	}

	void ChannelManager::ChangeHost(FString const& Id, int32 RoomType, int32 HostId)
	{
		if (!IsThisChannel(Id))
		{
			UE_LOG(LogBladeII, Warning, TEXT("You haven't paticipanted the channel(id : %s), so you're not able to broadcast a message..."), *Id);
			return;
		}

		ReceivedNotifyChangeHostClass<int32, int32>::GetInstance().Signal(RoomType, HostId);
	}

	void ChannelManager::NotificationInviteMessage(const FString& Inviter, const FString& InviterKey, b2network::B2RoomLocationPtr RoomLocationPtr, int32 mapId, int32 difficulty)
	{
		if (RoomLocationPtr == nullptr)
		{
			UE_LOG(LogBladeII, Warning, TEXT("NotificationInviteMessage Error : RoomLocation is Null"));
			return;
		}

		ReceivedNotifyInviteMessageClass<const FString&, const FString&, const FString&, int32, int32>::GetInstance().Signal(Inviter, InviterKey, RoomLocationPtr->room_id, mapId, difficulty);		
	}

	void ChannelManager::NotififyUpdateProperty(const FString& RoomId, const TArray<b2network::B2RoomPropertyPtr>& PropertyList)
	{
		ReceivedNotifyUpdateRoomPropertyClass<const FString&, const TArray<b2network::B2RoomPropertyPtr>&>::GetInstance().Signal(RoomId, PropertyList);
	}

	//b2::protocol::commondata::Room_ACCESS ChannelManager::B2RoomAccessToPbRoomAccess(B2RoomAccess RoomAccess)
	//{
	//	b2::protocol::commondata::Room_ACCESS RoomType = b2::protocol::commondata::Room_ACCESS::Room_ACCESS_PUBLIC;

	//	switch (RoomAccess)
	//	{
	//	case B2RoomAccess::Public:
	//		RoomType = b2::protocol::commondata::Room_ACCESS::Room_ACCESS_PUBLIC;
	//		break;
	//	case B2RoomAccess::Private:
	//		RoomType = b2::protocol::commondata::Room_ACCESS::Room_ACCESS_PRIVATE;
	//		break;
	//	}

	//	return RoomType;
	////}

	//ChannelManager::B2RoomAccess ChannelManager::PbRoomAccessToB2RoomAccess(b2::protocol::commondata::Room_ACCESS RoomAccess)
	//{
	//	B2RoomAccess RoomType = B2RoomAccess::Public;

	//	switch (RoomAccess)
	//	{
	//	case b2::protocol::commondata::Room_ACCESS::Room_ACCESS_PUBLIC:
	//		RoomType = B2RoomAccess::Public;
	//		break;
	//	case b2::protocol::commondata::Room_ACCESS::Room_ACCESS_PRIVATE:
	//		RoomType = B2RoomAccess::Private;
	//		break;
	//	}

	//	return RoomType;
	//}
}