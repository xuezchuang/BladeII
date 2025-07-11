//// Fill out your copyright notice in the Description page of Project Settings.
//// Project BladeII, ACTION SQUARE
//
//
//#include "B2NetworkBridge.h"
//#include "Event.h"
////#include "InternalEvents.h"
//
////////////////////////////////////////////////////////////////////////////
////#include "Client.h"
////#include "Cryption.h"
//#include "Engine.h"
//#include "CredentialInfo.h"
//#include "Endpoint.h"
//#include "BladeIIUtil.h"
////
////#include "Global.h"
////#include "FakeConfigure.h"
////#include "BladeIIBlockToSyncNetwork.h"
////#include "ChannelManager.h"
//
////#include "B2Version.h"
//#include "BladeIIGameImpl.h"
//#include "B2ClientDataStore.h"
//
//class FB2NetworkDriver
//{
////	std::shared_ptr<b2network::Client> m_client = nullptr;
////	FDelegateHandle	OnWorldTickStartDelegateHandle;
////
//public:
//	FB2NetworkDriver() {};
//	~FB2NetworkDriver() {};
//
//////	void Init();
////	void Init(const b2network::CredentialInfo& credential, const b2network::Endpoint& sessionServer);
//////	void Shutdown();
//////
//////    void CloseChannel(const std::string& reason);
//////
//////    bool Configure(const std::string& optionName, bool value);
//////
//////    bool SendB2Message(FB2MessageInfoPtr messageInfo);
//////
//////	void Tick(ELevelTick TickType, float DeltaSeconds);
//////
//////	void ChannelHeartbeat(const FString&);
//////
//////	// debug
//////    bool IsServiceAvailable() const;
//////	bool IsChannelDisconnected() const;
//////	void SetHeartbeatPeriod(int sessionPeriod, int channelPeriod);
//////
//////	int64 GetAccountID();
//////
//////    void Dump();
//////
//////private:
//////	//void LoadSessionServerFromConfig(b2network::Endpoint& serverPeer);
//////	FString GetUniqueDeviceId();	// NOTE temporal interface
//};
//////
//////using FB2NetworkDriverPtr = std::shared_ptr<FB2NetworkDriver>;
//////
//////void FB2NetworkDriver::Init()
//////{
//////	FB2AuthInfo& authInfo = FB2AuthInfo::GetInstance();
////////
////////	// credential
////////	//
////////	b2network::CredentialInfo credential;
////////	credential.m_platformType = authInfo.GetPlatformType();
////////	credential.m_osType = authInfo.GetOsType();
////////	credential.m_marketType = authInfo.GetMarketType();
////////	credential.m_countryCode = TCHAR_TO_UTF8(*authInfo.GetCountryCode());
////////	credential.m_langCode = TCHAR_TO_UTF8(*authInfo.GetLangCode());
////////	credential.m_pushToken = TCHAR_TO_UTF8(*authInfo.GetPushToken());
////////	credential.m_advertizeId = TCHAR_TO_UTF8(*authInfo.GetAdvertizeId());
////////	credential.m_accessToken = TCHAR_TO_UTF8(*authInfo.GetAccessToken());
////////
////////	if (credential.m_platformType == 0 && credential.m_platformUserId.empty())
////////	{	// NOTE temporal handling
////////		authInfo.SetPlatformUserId(GetUniqueDeviceId()); //이거 추가 해도 되나요??
////////		UE_LOG(LogBladeII, Display, TEXT("b2.auth PUID(%s) by deviceid"), *GetUniqueDeviceId());
////////	}
////////	credential.m_platformUserId = TCHAR_TO_UTF8(*authInfo.GetPlatformUserId());
////////
////////	// client cl
////////	FString ClientCLString = BladeIIGameImpl::GetClientDataStore().GetChangelistNum();
////////	if (ClientCLString.IsEmpty())
////////		ClientCLString = TEXT(PREPROCESSOR_TO_STRING(CL));
////////	credential.m_client_cl = TCHAR_TO_UTF8(*ClientCLString);
////////
////////	// session server
////////	//
////////	b2network::Endpoint sessionServer(TCHAR_TO_ANSI(*authInfo.GetSesssionAddr()), authInfo.GetSessionPort());
/////////*
////////#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1
////////	// 여기서 ip 주소를 읽어들이는 건 개발버전임. 최종 DLC 빌드에서는 로그인 이후 별도 루트를 통해 서버 정보를 받게 됨.
////////	if (!sessionServer.IsValid() && !IsFinalDLCCookRun())
////////	{
////////		LoadSessionServerFromConfig(sessionServer);
////////		UE_LOG(LogBladeII, Display, TEXT("b2.auth SessionServer(%s, %d) by config"), ANSI_TO_TCHAR(sessionServer.getAddress().c_str()), sessionServer.getPort());
////////	}
////////#endif // !#if !UE_BUILD_SHIPPING
////////*/
////////	// We always read server addresses from ini until Kakao gets us server addresses from their gateway.
////////#if B2_BUILD_LEVEL < B2_BUILD_LEVEL_QA // !B2_BUILD_LEVEL_LIVE !B2_BUILD_LEVEL_QA
////////	LoadSessionServerFromConfig(sessionServer);
////////#endif
////////	Init(credential, sessionServer);
//////}
//////
////////void FB2NetworkDriver::Init(const b2network::CredentialInfo& credential, const b2network::Endpoint& sessionServer)
////////{
////////    if (!sessionServer.IsValid())
////////    {
////////        if (sessionServer.getAddress().empty())
////////        {
////////            UE_LOG(LogBladeII, Error, TEXT("Empty session server address!"));
////////        }
////////        else
////////        {
////////            UE_LOG(LogBladeII, Error, TEXT("Invalid session server address!"));
////////        }
////////
////////        data_trader::ServerAddressErrorEventClass<>::GetInstance().Signal();
////////
////////        return;
////////    }
////////
////////	m_client = std::make_shared<b2network::Client>(credential, sessionServer);
////////	if (m_client)
////////	{
////////		// start network client module
////////		//
////////
////////        m_client->Start();
////////
////////        // notify listeners
////////        //
////////
////////        m_client->AttachNotifyListener("b2.protocol.session.SessionClosedNotify",
////////            "FB2MessageInfoSessionClosedNotify",
////////            [](FB2MessageInfoPtr messageInfo)
////////            {
////////                const auto b2MessageInfo = std::static_pointer_cast<b2network::FB2MessageInfoSessionClosedNotify>(messageInfo);
////////
////////                auto reason = b2MessageInfo->reason;
////////
////////                // TODO:
////////                // 서버가 클라이언트의 세션을 강제로 끊을 때 알려주는 Notify 이다.
////////                // reason의 값에 따라서 정해진 팝업과 게임 화면 전환을 진행한다.
////////                // 지금 구현된 reason의 값은 아래와 같다.
////////                // 2: 중복 접속 (같은 계정으로 이중으로 SignIn)
////////				data_trader::SessionClosedNotifyClass<int32>::GetInstance().Signal(reason);
////////            });
////////
////////        m_client->AttachNotifyListener("b2.protocol.session.ChatNotify",
////////            "FB2MessageInfoChatNotify",
////////            [](FB2MessageInfoPtr messageInfo)
////////            {
////////                const auto b2MessageInfo = std::static_pointer_cast<b2network::FB2MessageInfoChatNotify>(messageInfo);
////////
////////				data_trader::ChatMessageNotifyClass<b2network::FB2MessageInfoChatNotifyPtr>::GetInstance()
////////                    .Signal(b2MessageInfo);
////////            });
////////
////////		m_client->AttachNotifyListener("b2.protocol.session.GuildChatNotify",
////////			"FB2MessageInfoGuildChatNotify",
////////			[](FB2MessageInfoPtr messageInfo)
////////		{
////////			const auto b2MessageInfo = std::static_pointer_cast<b2network::FB2MessageInfoGuildChatNotify>(messageInfo);
////////
////////			data_trader::GuildChatMessageNotifyClass<b2network::FB2MessageInfoGuildChatNotifyPtr>::GetInstance()
////////				.Signal(b2MessageInfo);
////////		});
////////
////////        m_client->AttachNotifyListener("b2.protocol.session.InvitationNotify",
////////            "FB2MessageInfoInvitationNotify",
////////            [](FB2MessageInfoPtr messageInfo)
////////            {
////////                const auto b2MessageInfo = std::static_pointer_cast<b2network::FB2MessageInfoInvitationNotify>(messageInfo);
////////
////////                data_trader::InvitationNotifyClass<int32, const FString&, const FString&, b2network::B2RoomLocationPtr>::GetInstance()
////////                    .Signal(b2MessageInfo->invitation_type, b2MessageInfo->inviter, b2MessageInfo->invitation_ref, b2MessageInfo->room_location);
////////            });
////////
////////        m_client->AttachNotifyListener("b2.protocol.session.AcceptInvitationNotify",
////////            "FB2MessageInfoAcceptInvitationNotify",
////////            [](FB2MessageInfoPtr messageInfo)
////////            {
////////                const auto b2MessageInfo = std::static_pointer_cast<b2network::FB2MessageInfoAcceptInvitationNotify>(messageInfo);
////////				
////////				data_trader::AcceptInvitationNotifyClass<int32, const FString&, const FString&, b2network::B2RoomLocationPtr>::GetInstance()
////////					.Signal(b2MessageInfo->invitation_type, b2MessageInfo->invitee, b2MessageInfo->invitation_ref, b2MessageInfo->room_location);
////////            });
////////
////////        m_client->AttachNotifyListener("b2.protocol.session.DeclineInvitationNotify",
////////            "FB2MessageInfoDeclineInvitationNotify",
////////            [](FB2MessageInfoPtr messageInfo)
////////            {
////////                const auto b2MessageInfo = std::static_pointer_cast<b2network::FB2MessageInfoDeclineInvitationNotify>(messageInfo);
////////
////////                data_trader::DeclineInvitationNotifyClass<int32, const FString&, const FString&>::GetInstance()
////////                    .Signal(b2MessageInfo->invitation_type, b2MessageInfo->invitee, b2MessageInfo->invitation_ref);
////////            });
////////
////////        m_client->AttachNotifyListener("b2.protocol.channel.RoomParticipantJoinNotify", 
////////			"FB2MessageInfoRoomParticipantJoinNotify",
////////			[](FB2MessageInfoPtr messageInfo) 
////////			{
////////                const auto b2MessageInfo = std::static_pointer_cast<b2network::FB2MessageInfoRoomParticipantJoinNotify>(messageInfo);
////////
////////                data_trader::RoomParticipantJoinNotityClass<const FString&, b2network::B2RoomParticipantPtr>::GetInstance()
////////                    .Signal(b2MessageInfo->room_id, b2MessageInfo->joiner);
////////			});
////////
////////        m_client->AttachNotifyListener("b2.protocol.channel.RoomParticipantLeaveNotify",
////////			"FB2MessageInfoRoomParticipantLeaveNotify",
////////			[](FB2MessageInfoPtr messageInfo)
////////			{
////////                const auto b2MessageInfo = std::static_pointer_cast<b2network::FB2MessageInfoRoomParticipantLeaveNotify>(messageInfo);
////////
////////                data_trader::RoomParticipantLeaveNotityClass<const FString&, b2network::B2RoomParticipantPtr>::GetInstance()
////////                    .Signal(b2MessageInfo->room_id, b2MessageInfo->leaver);
////////			});
////////
////////        m_client->AttachNotifyListener("b2.protocol.channel.RoomMessageNotify", 
////////			"FB2MessageInfoRoomMessageNotify",
////////			[](FB2MessageInfoPtr messageInfo)
////////			{
////////                const auto b2MessageInfo = std::static_pointer_cast<b2network::FB2MessageInfoRoomMessageNotify>(messageInfo);
////////
////////                data_trader::RoomMessageNotifyClass<const FString&, b2network::B2RoomParticipantPtr, const FString&>::GetInstance()
////////                    .Signal(b2MessageInfo->room_id, b2MessageInfo->sender, b2MessageInfo->message);
////////			});
////////
////////        m_client->AttachNotifyListener("b2.protocol.channel.RoomHostChangedNotify",
////////            "FB2MessageInfoRoomHostChangedNotify",
////////            [](FB2MessageInfoPtr messageInfo)
////////            {
////////                const auto b2MessageInfo = std::static_pointer_cast<b2network::FB2MessageInfoRoomHostChangedNotify>(messageInfo);
////////
////////                data_trader::RoomHostChangedNotifyClass<const FString&, int32, int32>::GetInstance()
////////                    .Signal(b2MessageInfo->room_id, b2MessageInfo->room_type, b2MessageInfo->host_id);
////////            });
////////
////////        m_client->AttachNotifyListener("b2.protocol.channel.RoomPropertyUpdatedNotify",
////////            "FB2MessageInfoRoomPropertyUpdatedNotify",
////////            [](FB2MessageInfoPtr messageInfo)
////////            {
////////                const auto b2MessageInfo = std::static_pointer_cast<b2network::FB2MessageInfoRoomPropertyUpdatedNotify>(messageInfo);
////////
////////                data_trader::RoomPropertyUpdatedNotifyClass<const FString&, const TArray<b2network::B2RoomPropertyPtr>&>::GetInstance()
////////                    .Signal(b2MessageInfo->room_id, b2MessageInfo->properties);
////////            });
////////
////////		m_client->AttachNotifyListener("b2.protocol.session.QuestAlarmNotify",
////////			"FB2MessageInfoQuestAlarmNotify",
////////			[](FB2MessageInfoPtr messageInfo)
////////            {
////////                const auto b2MessageInfo = std::static_pointer_cast<b2network::FB2MessageInfoQuestAlarmNotify>(messageInfo);
////////
////////                data_trader::QuestAlarmNotifyClass<int32, b2network::B2QuestInfoPtr, const TArray<b2network::B2RewardPtr>>::GetInstance()
////////                    .Signal(b2MessageInfo->completed_slot, b2MessageInfo->next_quest_info, b2MessageInfo->received_rewards);
////////            }); 
////////
////////		m_client->AttachNotifyListener("b2.protocol.session.MissionAlarmNotify",
////////			"FB2MessageInfoMissionAlarmNotify",
////////			[](FB2MessageInfoPtr messageInfo)
////////            {
////////                const auto b2MessageInfo = std::static_pointer_cast<b2network::FB2MessageInfoMissionAlarmNotify>(messageInfo);
////////
////////                data_trader::MissionAlarmNotifyClass<int32, int32>::GetInstance()
////////                    .Signal(b2MessageInfo->completed_quest_id, b2MessageInfo->completed_quest_complete_step);
////////            });
////////
////////        m_client->AttachNotifyListener("b2.protocol.session.RaidInvitationNotify",
////////            "FB2MessageInfoRaidInvitationNotify",
////////            [](FB2MessageInfoPtr messageInfo)
////////            {
////////                const auto b2MessageInfo = std::static_pointer_cast<b2network::FB2MessageInfoRaidInvitationNotify>(messageInfo);
////////
////////                data_trader::RaidInvitationNotifyClass<const FString&, const FString&, b2network::B2RoomLocationPtr, int32, int32>::GetInstance()
////////                    .Signal(b2MessageInfo->inviter, b2MessageInfo->invitation_key, b2MessageInfo->room_location, b2MessageInfo->map_id, b2MessageInfo->difficulty);
////////            });
////////
////////        m_client->AttachNotifyListener("b2.protocol.channel.RoomHeartbeatNotify",
////////            "FB2MessageInfoRoomHeartbeatNotify",
////////            [](FB2MessageInfoPtr messageInfo)
////////            {
////////            });
////////
////////        m_client->AttachNotifyListener("b2.protocol.session.AdminMessageNotify",
////////            "FB2MessageInfoAdminMessageNotify",
////////            [](FB2MessageInfoPtr messageInfo)
////////            {
////////                const auto b2MessageInfo = std::static_pointer_cast<b2network::FB2MessageInfoAdminMessageNotify>(messageInfo);
////////
////////				for (const auto Each : b2MessageInfo->messages)
////////				{
////////					data_trader::AdminMessageNotifyClass<const FString&, int32>::GetInstance()
////////						.Signal(Each->message, Each->display_time);
////////				}
////////            });
////////
////////		m_client->AttachNotifyListener("b2.protocol.session.ContentsModeStateNotify",
////////			"FB2MessageInfoContentsModeStateNotify",
////////			[](FB2MessageInfoPtr messageInfo)
////////			{
////////				// B2ContentsMode, B2ContentsModeState, B2ContentsModeState
////////				const auto msg = std::static_pointer_cast<b2network::FB2MessageInfoContentsModeStateNotify>(messageInfo);
////////				data_trader::ContentsModeStateNotifyClass<int32, int32, int32>::GetInstance()
////////					.Signal(msg->mode, msg->prev_state, msg->current_state);
////////			});
////////
////////
////////        // event handlers
////////        //
////////
////////        m_client->AddEventHandler(b2network::Client::Event::ServiceConnecting,
////////            [](int errorCode)
////////            {
////////                data_trader::ServiceConnectingEventClass<>::GetInstance().Signal();
////////            });
////////
////////        m_client->AddEventHandler(b2network::Client::Event::ServiceConnected,
////////            [](int errorCode)
////////            {
////////                data_trader::ServiceConnectedEventClass<>::GetInstance().Signal();
////////            });
////////
////////        m_client->AddEventHandler(b2network::Client::Event::ServiceNotAvailable,
////////            [](int errorCode)
////////            {
////////                // NOTE 세션 서버와 접속이 끊김
////////                // 재연결 하려면 Init() 호출해야 한다.
////////                // 채널이 끊기지 않더라도 실시간 게임 모드는 중단 시켜야 하나?
////////#if FORCE_LOGGING_IN_ANDROID
////////                FPlatformMisc::LowLevelOutputDebugStringf(TEXT("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"));
////////				FPlatformMisc::LowLevelOutputDebugStringf(TEXT("@@@@@@@@@@@@@@@@ Session Disconnected! @@@@@@@@@@@@@@@@"));
////////				FPlatformMisc::LowLevelOutputDebugStringf(TEXT("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"));
////////#endif
////////
////////                // @ReplayMode FIX
////////                if (data_trader::ChannelManager::GetInstance().IsReplayMode())
////////                {
////////                    return;
////////                }
////////
////////                data_trader::ServiceNotAvailableEventClass<int32>::GetInstance().Signal(errorCode);
////////            });
////////
////////        m_client->AddEventHandler(b2network::Client::Event::ChannelDisconnected,
////////            [](int errorCode)
////////            {
////////                // NOTE 채널 서버와 연결 끊김
////////                // 일대일, 레이드, 점령전 같은 실시간 게임 모드 중단 시켜야 함
////////#if FORCE_LOGGING_IN_ANDROID
////////				FPlatformMisc::LowLevelOutputDebugStringf(TEXT("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"));
////////				FPlatformMisc::LowLevelOutputDebugStringf(TEXT("$$$$$$$$$$$$$$$$ Channel Disconnected! $$$$$$$$$$$$$$$$"));
////////				FPlatformMisc::LowLevelOutputDebugStringf(TEXT("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"));
////////#endif
////////                // @ReplayMode FIX
////////                if (data_trader::ChannelManager::GetInstance().IsReplayMode())
////////                {
////////                    return;
////////                }
////////
////////                data_trader::ChannelDisconnectedEventClass<int32>::GetInstance().Signal(errorCode);
////////            });
////////
////////        m_client->AddEventHandler(b2network::Client::Event::RequireGiveNickname,
////////            [](int errorCode)
////////            {
////////                data_trader::RequireNicknameEventClass<>::GetInstance().Signal();
////////            });
////////
////////        m_client->AddEventHandler(b2network::Client::Event::RequireUpdateProtocol,
////////            [](int errorCode)
////////            {
////////                // TODO: pop up the warning dialog saying 'Protocol mismatches, So you must update the client'
////////				data_trader::RequireUpdateProtocolEventClass<>::GetInstance().Signal();
////////            });
////////
////////        m_client->AddEventHandler(b2network::Client::Event::RequireUpdateVersion,
////////            [](int errorCode)
////////            {
////////                // TODO: pop up the warning dialog saying 'Protocol mismatches, So you must update the client'
////////                data_trader::RequireUpdateAppVersionEventClass<>::GetInstance().Signal();
////////            });
////////
////////		m_client->AddEventHandler(b2network::Client::Event::ServerTimeSync,
////////			[this](int errorCode)
////////			{
////////				const int64 serverTime = m_client->GetSessionServerTime();
////////				data_trader::ServerTimeSyncEventClass<int64>::GetInstance().Signal(serverTime);
////////			});
////////
////////		OnWorldTickStartDelegateHandle = FWorldDelegates::OnWorldTickStart.AddRaw(this, &FB2NetworkDriver::Tick);
////////		BII_CHECK(OnWorldTickStartDelegateHandle.IsValid());
////////    }
////////}
//////
//////void FB2NetworkDriver::Shutdown()
//////{
//////	//if (m_client)
//////	//{
//////	//	FWorldDelegates::OnWorldTickStart.Remove(OnWorldTickStartDelegateHandle);
//////
//////	//	m_client->Stop();
//////	//	m_client.reset();
//////	//}
////}
//
////void FB2NetworkDriver::CloseChannel(const std::string& reason)
////{
////    //if (m_client)
////    //{
////    //    m_client->CloseChannel(reason);
////    //}
////}
////
////bool FB2NetworkDriver::Configure(const std::string& optionName, bool value)
////{
////    if (m_client)
////    {
////        return m_client->Configure(optionName, value);
////    }
////
////    return false;
////}
////
////void FB2NetworkDriver::LoadSessionServerFromConfig(b2network::Endpoint& serverPeer)
////{
////	FString ServerType;
////	GConfig->GetString(TEXT("NetworkSettings"), TEXT("ServerType"), ServerType, GB2NetworkIni);
////
////	FString NetworkSettingCategory = TEXT("NetworkSetting_Public");
////
////	if (ServerType == TEXT("Alpha"))
////	{
////		NetworkSettingCategory = TEXT("NetworkSetting_Alpha");
////	}
////	else if (ServerType == TEXT("Beta"))
////	{
////		NetworkSettingCategory = TEXT("NetworkSetting_Beta");
////	}
////	else if (ServerType == TEXT("Local"))
////	{
////		NetworkSettingCategory = TEXT("NetworkSetting_Local");
////	}
////
////	FString SessionServerIp;
////	int32 SessionServerPort;
////	GConfig->GetString(*NetworkSettingCategory, TEXT("SESSION_IP"), SessionServerIp, GB2NetworkIni);
////	GConfig->GetInt(*NetworkSettingCategory, TEXT("SESSION_PORT"), SessionServerPort, GB2NetworkIni);
////
////	serverPeer = b2network::Endpoint(TCHAR_TO_ANSI(*SessionServerIp), SessionServerPort);
////}
////
////FString FB2NetworkDriver::GetUniqueDeviceId()
////{
////#if PLATFORM_IOS
////	FString uniqueDeviceId = FIOSPlatformMisc::GetDeviceId();
////#else
////	// TODO fix - use unique device id as a user uid temporary
////	FString uniqueDeviceId = FGenericPlatformMisc::GetDeviceId();
////
////#if !UE_BUILD_SHIPPING
////	FString CustomDeviceId;
////
////	if (NetFakeConfigure::GetInstance().IsCustomDeviceId())
////	{
////		uniqueDeviceId = NetFakeConfigure::GetInstance().GetDeviceId();
////	}
////#endif
////#endif
////
////	return uniqueDeviceId;
////}
////
////void FB2NetworkDriver::Tick(ELevelTick TickType, float DeltaSeconds)
////{
////#if WITH_EDITOR
////	if (GIsEditor && (!GIsPlayInEditorWorld && GPlayInEditorID == INDEX_NONE)) // 정식 fix 전까지 임시로.. 에디터의 Network 틱 감소.
////	{
////		return;
////	}
////#endif
////
////	if (m_client)
////	{
////        m_client->MainThreadUpdate();
////	}
////}
////
////void FB2NetworkDriver::ChannelHeartbeat(const FString& Id)
////{
////	if (m_client)
////	{
////		m_client->ChannelHeartbeat(TCHAR_TO_ANSI(*Id));
////	}
////}
////
////bool FB2NetworkDriver::IsServiceAvailable() const
////{
////    if (m_client)
////    {
////        return m_client->IsServiceAvailable();
////    }
////
////    return false;
////}
////
////bool FB2NetworkDriver::IsChannelDisconnected() const
////{
////	if (m_client)
////	{
////		return m_client->IsChannelDisconnected();
////	}
////	return true;
////}
////
////void FB2NetworkDriver::SetHeartbeatPeriod(int sessionPeriod, int channelPeriod)
////{
////	if (m_client)
////	{
////		m_client->SetHeartbeatPeriod(sessionPeriod, channelPeriod);
////	}
////}
////
////int64 FB2NetworkDriver::GetAccountID()
////{
////	if (m_client)
////		return m_client->GetAccountID();
////
////	return 0;
////}
////
////bool FB2NetworkDriver::SendB2Message(FB2MessageInfoPtr messageInfo)
////{
////	if (m_client)
////	{
////		return m_client->SendB2Message(messageInfo);
////	}
////	return false;
////}
////
////void FB2NetworkDriver::Dump()
////{
////    if (m_client)
////    {
////        m_client->Dump();
////    }
////}
//
////////////////////////////////////////////////////////////////////////////
//FB2NetworkBridgePtr FB2NetworkBridge::m_pInstance = nullptr;
//
//FB2NetworkBridge::FB2NetworkBridge()
//{
//
//}
//
//FB2NetworkBridge::~FB2NetworkBridge()
//{
//	Shutdown();
//}
//
//FB2NetworkBridgePtr FB2NetworkBridge::GetInstance()
//{
//	//if (!m_pInstance)
//	//{
//	//	m_pInstance = std::unique_ptr<FB2NetworkBridge>(new FB2NetworkBridge);
//	//}
//	//return m_pInstance;
//	return nullptr;
//}
//
//void FB2NetworkBridge::Init()
//{
//	//FB2NetworkDriverPtr B2NetworkDriver = std::unique_ptr<FB2NetworkDriver>(new FB2NetworkDriver);
//	//if (B2NetworkDriver)
//	//{
//	//	m_pNetworkDriver = B2NetworkDriver;
//	//	m_pNetworkDriver->Init();
//	//}
//}
//
//void FB2NetworkBridge::Shutdown()
//{
//	//FBladeIIBlockToSyncNetwork::GetInstance().SetIsNetworkInit(false);
//
//	//if (m_pNetworkDriver)
//	//{
//	//	m_pNetworkDriver->Shutdown();
//	//	m_pNetworkDriver.reset();
//	//}
//
//	//if (m_pInstance)
//	//{
//	//	m_pInstance.reset();
//	//}
//}
//
//void FB2NetworkBridge::CloseChannel(const std::string& reason)
//{
//  //  if (m_pNetworkDriver)
//  //  {
//		//m_pNetworkDriver->CloseChannel(reason);
//  //  }
//}
//
//bool FB2NetworkBridge::Configure(const std::string& optionName, bool value)
//{
//    //if (m_pInstance)
//    //{
//    //    if (m_pNetworkDriver)
//    //    {
//    //        return m_pNetworkDriver->Configure(optionName, value);
//    //    }
//    //}
//
//    return false;
//}
//
//void FB2NetworkBridge::ResetNetwork()
//{
//	Shutdown();
//	Init();
//}
//
//bool FB2NetworkBridge::SendB2Message(FB2MessageInfoPtr messageInfo)
//{
//	//if (m_pNetworkDriver)
//	//{
//	//	return m_pNetworkDriver->SendB2Message(messageInfo);
//	//}
//	return false;
//}
//
//void FB2NetworkBridge::ChannelHeartbeat(const FString& Id)
//{
//	//if (m_pNetworkDriver)
//	//{
//	//	m_pNetworkDriver->ChannelHeartbeat(Id);
//	//}
//}
//
//bool FB2NetworkBridge::IsServiceAvailable() const
//{
//    //if (m_pNetworkDriver)
//    //{
//    //    return m_pNetworkDriver->IsServiceAvailable();
//    //}
//
//    return false;
//}
//
//bool FB2NetworkBridge::IsChannelDisconnected() const
//{
////	if (m_pNetworkDriver)
////	{
////		return m_pNetworkDriver->IsChannelDisconnected();
////	}
//	return true;
//}
//
//void FB2NetworkBridge::SetHeartbeatPeriod(int sessionPeriod, int channelPeriod)
//{
//	//if (m_pNetworkDriver)
//	//{
//	//	m_pNetworkDriver->SetHeartbeatPeriod(sessionPeriod, channelPeriod);
//	//}
//}
//
//int64 FB2NetworkBridge::GetAccountID()
//{
////	if (m_pNetworkDriver)
////		return m_pNetworkDriver->GetAccountID();
//
//	return 0;
//}
//
//void FB2NetworkBridge::Dump()
//{
//    //if (m_pNetworkDriver)
//    //{
//    //    m_pNetworkDriver->Dump();
//    //}
//    //else
//    //{
//    //}
//}
//
//std::string hexify(const uint8* v, size_t len)
//{
//	std::string s;
//	//s.resize(len * 2 + 1);
//	//::sodium_bin2hex(&s[0], s.size(), v, len);
//	//s.resize(len * 2);
//	return s;
//}
//
//void FB2NetworkBridge::TestSodium()
//{
////	std::array<uint8, crypto_stream_chacha20_KEYBYTES>	serverPublicKey;
////	::randombytes_buf(&serverPublicKey[0], crypto_stream_chacha20_KEYBYTES);
////
////	b2network::Key	client_key, server_key;
////	auto client_public_key = client_key.GetPublicKey();
////	auto server_public_key = server_key.GetPublicKey();
////
////	auto public_client_key1 = b2network::hexify(&client_public_key[0], crypto_stream_chacha20_KEYBYTES);
////	auto public_server_key1 = b2network::hexify(&server_public_key[0], crypto_stream_chacha20_KEYBYTES);
////
////#if PLATFORM_IOS || PLATFORM_ANDROID
////#if FORCE_LOGGING_IN_ANDROID
////	FPlatformMisc::LowLevelOutputDebugStringf(TEXT("Client Generated Public Key : %s"), ANSI_TO_TCHAR(public_client_key1.c_str()));
////	FPlatformMisc::LowLevelOutputDebugStringf(TEXT("Client Generated Public Key : %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d"), 
////		client_public_key[0],  client_public_key[1],  client_public_key[2],  client_public_key[3],  client_public_key[4],  client_public_key[5],  client_public_key[6],  client_public_key[7], 
////		client_public_key[8],  client_public_key[9],  client_public_key[10], client_public_key[11], client_public_key[12], client_public_key[13], client_public_key[14], client_public_key[15], 
////		client_public_key[16], client_public_key[17], client_public_key[18], client_public_key[19], client_public_key[20], client_public_key[21], client_public_key[22], client_public_key[23], 
////		client_public_key[24], client_public_key[25], client_public_key[26], client_public_key[27], client_public_key[28], client_public_key[29], client_public_key[30], client_public_key[31]
////		);
////#endif
////#else
////	UE_LOG(LogBladeII, Log, TEXT("Client Generated Shared Key : %s"), ANSI_TO_TCHAR(public_client_key1.c_str()));
////#endif
////
////#if PLATFORM_IOS || PLATFORM_ANDROID
////#if FORCE_LOGGING_IN_ANDROID
////	FPlatformMisc::LowLevelOutputDebugStringf(TEXT("Server Generated Public Key : %s"), ANSI_TO_TCHAR(public_server_key1.c_str()));
////#endif
////#else
////	UE_LOG(LogBladeII, Log, TEXT("Client Generated Shared Key : %s"), ANSI_TO_TCHAR(public_server_key1.c_str()));
////#endif
////
////	client_key.MakeSharedKeyForClient(server_public_key);
////	server_key.MakeSharedKeyForServer(client_public_key);
////
////	auto shared_client_key = b2network::hexify(&client_key.GetRealKey()[0], crypto_stream_chacha20_KEYBYTES);
////	auto shared_server_key = b2network::hexify(&server_key.GetRealKey()[0], crypto_stream_chacha20_KEYBYTES);
////
////#if PLATFORM_IOS || PLATFORM_ANDROID
////#if FORCE_LOGGING_IN_ANDROID
////	FPlatformMisc::LowLevelOutputDebugStringf(TEXT("Client Generated Shared Key : %s"), ANSI_TO_TCHAR(shared_client_key.c_str()));
////#endif
////#else
////	UE_LOG(LogBladeII, Log, TEXT("Client Generated Shared Key : %s"), ANSI_TO_TCHAR(shared_client_key.c_str()));
////#endif
////
////#if PLATFORM_IOS || PLATFORM_ANDROID
////#if FORCE_LOGGING_IN_ANDROID
////	FPlatformMisc::LowLevelOutputDebugStringf(TEXT("Server Generated Shared Key : %s"), ANSI_TO_TCHAR(shared_server_key.c_str()));
////#endif
////#else
////	UE_LOG(LogBladeII, Log, TEXT("Server Generated Shared Key : %s"), ANSI_TO_TCHAR(shared_server_key.c_str()));
////#endif
////
////	b2network::ICrypto* crypto = new b2network::ChaCha20(client_key);
////
////	std::string message("This is original message.");
////	const auto size = message.length();
////	std::vector<uint8> body;
////	std::vector<uint8> encrypted_body;
////	std::vector<uint8> decrypted_body;
////
////	body.reserve(size+1);body[body.capacity()-1]='\0';
////	encrypted_body.reserve(size+1);encrypted_body[encrypted_body.capacity()-1]='\0';
////	decrypted_body.reserve(size+1);decrypted_body[decrypted_body.capacity()-1]='\0';
////	
////	std::copy(message.begin(), message.begin()+size, std::back_inserter(body));
////
////#if PLATFORM_IOS || PLATFORM_ANDROID
////	FPlatformMisc::LowLevelOutputDebugStringf(TEXT("Original Message : %s"), ANSI_TO_TCHAR(reinterpret_cast<char*>(body.data())));
////#else
////	UE_LOG(LogBladeII, Log, TEXT("Original Message : %s"), ANSI_TO_TCHAR(reinterpret_cast<char*>(body.data())));
////#endif
////
////	crypto->Encrypt(body.data(), body.size());
////
////#if PLATFORM_IOS || PLATFORM_ANDROID
////#if FORCE_LOGGING_IN_ANDROID
////	FPlatformMisc::LowLevelOutputDebugStringf(TEXT("Encrypted Message : %s"), ANSI_TO_TCHAR(reinterpret_cast<char*>(body.data())));
////#endif
////#else
////	UE_LOG(LogBladeII, Log, TEXT("Encrypted Message : %s"), ANSI_TO_TCHAR(reinterpret_cast<char*>(body.data())));
////#endif
////
////	crypto->Decrypt(body.data(), body.size());
////
////#if PLATFORM_IOS || PLATFORM_ANDROID
////#if FORCE_LOGGING_IN_ANDROID
////	FPlatformMisc::LowLevelOutputDebugStringf(TEXT("Decrypted Message : %s"), ANSI_TO_TCHAR(reinterpret_cast<char*>(body.data())));
////#endif
////#else
////	UE_LOG(LogBladeII, Log, TEXT("Decrypted Message : %s"), ANSI_TO_TCHAR(reinterpret_cast<char*>(body.data())));
////#endif
////
////	body.clear();
////
////	std::array<uint8, 160> random_bytes;
////	::randombytes_buf(&random_bytes[0], random_bytes.size());
////
////	std::copy(random_bytes.begin(), random_bytes.end(), std::back_inserter(body));
////
////	auto human_readable = b2network::hexify(body.data(), body.size());
////#if PLATFORM_IOS || PLATFORM_ANDROID
////#if FORCE_LOGGING_IN_ANDROID
////	FPlatformMisc::LowLevelOutputDebugStringf(TEXT("Original Message : %s"), ANSI_TO_TCHAR(human_readable.c_str()));
////#endif
////#else
////	UE_LOG(LogBladeII, Log, TEXT("Original Message : %s"), ANSI_TO_TCHAR(human_readable.c_str()));
////#endif
////
////	crypto->Encrypt(body.data(), body.size());
////
////	human_readable = b2network::hexify(body.data(), body.size());
////#if PLATFORM_IOS || PLATFORM_ANDROID
////#if FORCE_LOGGING_IN_ANDROID
////	FPlatformMisc::LowLevelOutputDebugStringf(TEXT("Encrypted Message : %s"), ANSI_TO_TCHAR(human_readable.c_str()));
////#endif
////#else
////	UE_LOG(LogBladeII, Log, TEXT("Encrypted Message : %s"), ANSI_TO_TCHAR(human_readable.c_str()));
////#endif
////
////	crypto->Decrypt(body.data(), body.size());
////
////	human_readable = b2network::hexify(body.data(), body.size());
////#if PLATFORM_IOS || PLATFORM_ANDROID
////#if FORCE_LOGGING_IN_ANDROID
////	FPlatformMisc::LowLevelOutputDebugStringf(TEXT("Decrypted Message : %s"), ANSI_TO_TCHAR(human_readable.c_str()));
////#endif
////#else
////	UE_LOG(LogBladeII, Log, TEXT("Decrypted Message : %s"), ANSI_TO_TCHAR(human_readable.c_str()));
////#endif
////
////	delete crypto;
//}
