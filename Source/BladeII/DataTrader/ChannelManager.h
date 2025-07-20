#pragma once
//#include "BladeII.h"
#include <map>
#include <memory>

#include "InternalEvents.h"
#include "MessageRecorder.h"

class issue;
using issue_ptr = std::shared_ptr<issue>;

namespace data_trader
{
	class Channel;

	struct ChannelInfo
	{
		//ChannelInfo(ChannelInfo const&) = delete;
		//ChannelInfo& operator=(ChannelInfo const&) = delete;

		ChannelInfo()
		{
			Empty();
		}
		ChannelInfo(const FString& InId, std::shared_ptr<Channel> InChannelPtr)
			: Id(InId), pChannel(InChannelPtr)
		{}

		FString						Id;
		std::shared_ptr<Channel>	pChannel;

		void Empty()
		{ 
			Id = TEXT("");  
			pChannel = nullptr;
		}
		bool IsEmpty()
		{
			return (Id == TEXT("") && pChannel == nullptr);
		}
	};

	class ChannelManager
	{
	public:
		static const int32 ROOM_All = 0;	// message type

		enum class B2RoomAccess : uint8
		{
			Public = 0,
			Private = 1
		};

	public:
		static std::unique_ptr<ChannelManager>	Instance;
		static ChannelManager& GetInstance();
		static void ReleaseInstance();

		ChannelManager(ChannelManager const&) = delete;
		ChannelManager& operator = (ChannelManager const&) = delete;

		~ChannelManager();

		void CreateChannel(FB2RoomJoin& application);
		void ShutdownChannel(TArray<FString> const&);
		void ShutdownChannel();

		void SubscribeChannel(FString const&, b2network::B2RoomParticipantPtr);
		void UnsubscribeChannel(FString const&, b2network::B2RoomParticipantPtr);

		void SendMessage(int32, FString const&);
		void BroadcastMessage(FString const&, uint32 SenderNetId, FString const&);

		void ChangeHost(FString const&, int32, int32);

		void NotificationInviteMessage(const FString&, const FString&, b2network::B2RoomLocationPtr, int32, int32);
		void NotififyUpdateProperty(const FString&, const TArray<b2network::B2RoomPropertyPtr>&);

		//static b2::protocol::commondata::Room_ACCESS	B2RoomAccessToPbRoomAccess(B2RoomAccess RoomAccess);
		//static B2RoomAccess				PbRoomAccessToB2RoomAccess(b2::protocol::commondata::Room_ACCESS RoomAccess);


		void SetReplayMode(bool bCurrentReplay) { bReplayMode = bCurrentReplay; }
		bool IsReplayMode() const { return bReplayMode; }

		MessageRecorder& GetRecorder() { return Recorder; }

	private:
		ChannelManager();

		bool IsThisChannel(FString const& Id) const
		{
			return ChannelData.Id == Id;
		}

		ChannelInfo		ChannelData;	
		MessageRecorder	Recorder;

		TArray<issue_ptr> Issues;

		bool			bReplayMode;
	};
}
