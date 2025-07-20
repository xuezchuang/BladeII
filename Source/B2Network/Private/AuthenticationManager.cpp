#include "AuthenticationManager.h"
#include "B2NetworkPlugin.h"

#include "ErrorCode.h"
#include "LogManager.h"
#include "Connection.h"
#include "MessageRouter.h"
#include "EventDispatcher.h"
#include "ConnectionManager.h"
#include "AuthenticationFSM.h"

#include "ProtobufFactory.h"
#include "MasterDataCacher.h"
#include "B2MessageInfoCommonStruct.h"
#include "Cryption.h"
#include "B2Version.h"

using namespace b2network;

#if B2_BUILD_LEVEL < B2_BUILD_LEVEL_QA // !B2_BUILD_LEVEL_LIVE, !!B2_BUILD_LEVEL_QA
static auto logger = LogManager::GetLogger("AuthenticationManager");
#endif

AuthenticationManager::AuthenticationManager(const CredentialInfo& credentialInfo,
	const Endpoint& sessionEndpoint,
	MessageRouter& messageRouter,
	EventDispatcher& eventDispatcher)
	:m_messageRouter(messageRouter),
	m_eventDispatcher(eventDispatcher),
	m_credentialInfo(credentialInfo),
	m_sessionEndpoint(sessionEndpoint),
	m_reconnectingCount(0),
	m_sessionServerTimeMs(0)
{
	m_sessionHeartbeatPeriod = DEFAULT_SESSION_HEARTBEAT_PERIOD;
	m_channelHeartbeatPeriod = DEFAULT_CHANNEL_HEARTBEAT_PERIOD;

    // channel heartbeat initialization
    m_roomBackgroundHeartbeat = true;

	TArray<FString> version_tokens;
	FString version_str(TEXT(PREPROCESSOR_TO_STRING(VERSION)));
	version_str.ParseIntoArray(version_tokens, TEXT("."), 1);

	m_ClientVersion.Major		= FCString::Atoi(*version_tokens[0]);
	m_ClientVersion.Minor		= FCString::Atoi(*version_tokens[1]);
	m_ClientVersion.Revision	= FCString::Atoi(*version_tokens[2]);

	FString changelist_str(TEXT(PREPROCESSOR_TO_STRING(CL)));
	m_ClientVersion.Changelist = FCString::Atoi(*changelist_str);

	FString buildNumber_str(TEXT(PREPROCESSOR_TO_STRING(BUILDNUMBER)));
	m_ClientVersion.BuildNum = FCString::Atoi(*buildNumber_str);
}

void AuthenticationManager::Start()
{
    B2NETWORK_LOG_DEBUG(logger, "Starting... Session Server(%s)", m_sessionEndpoint.ToString().c_str());

    m_eventDispatcher.AddConnectedCallback([this](Connection::ServerType serverType, int errorCode)
        {
            if (serverType == Connection::SESSION_SERVER)
            {
                m_fsm.Update(AuthenticationFSM::EVENT_CONNECTED_TO_SESSION);
            }
            else if (serverType == Connection::CHANNEL_SERVER)
            {
                m_fsm.Update(AuthenticationFSM::EVENT_CONNECTED_TO_CHANNEL);
            }
            else
            {
                B2NETWORK_LOG_ERROR(logger, "Unknwon ServerType %d", serverType);
            }
        });

    m_eventDispatcher.AddConnectFailCallback([this](Connection::ServerType serverType, int erroCode)
        {
            if (serverType == Connection::SESSION_SERVER)
            {
                m_fsm.Update(AuthenticationFSM::EVENT_CONNECT_FAIL_TO_SESSION, (int) ErrorCode::SessionConnectError);
            }
            else if (serverType == Connection::CHANNEL_SERVER)
            {
                m_fsm.Update(AuthenticationFSM::EVENT_CONNECT_FAIL_TO_CHANNEL, (int) ErrorCode::ChannelConnectError);
            }
            else
            {
                B2NETWORK_LOG_ERROR(logger, "Unknwon ServerType %d", serverType);
            }
        });

    m_eventDispatcher.AddDisconnectedCallback([this](Connection::ServerType serverType, int errorCode)
        {
            if (serverType == Connection::SESSION_SERVER)
            {
                m_fsm.Update(AuthenticationFSM::EVENT_DISCONNECTED_FROM_SESSION);
            }
            else if (serverType == Connection::CHANNEL_SERVER)
            {
                m_fsm.Update(AuthenticationFSM::EVENT_DISCONNECTED_FROM_CHANNEL);

                m_eventDispatcher.Emit(EventDispatcher::ChannelDisconnected);
            }
            else
            {
                B2NETWORK_LOG_ERROR(logger, "Unknwon ServerType %d", serverType);
            }
        });

    m_eventDispatcher.AddRequestFilter([this](MessagePtr request)
        {
            auto metadata = ProtobufFactory::GetMetadata(request);
            if (metadata.serverType == ProtobufFactory::Session)
            {
                if (!ProtobufFactory::IsTypeOf(request, b2::protocol::session::SignInSessionRequest::default_instance()))
                {
                    m_fsm.Update(AuthenticationFSM::EVENT_SESSION_REQUEST);
                }
            }
        });

    m_eventDispatcher.AddRequestFilter([this](MessagePtr request)
        {
            auto metadata = ProtobufFactory::GetMetadata(request);
            if (metadata.serverType == ProtobufFactory::Channel)
            {
                if (!ProtobufFactory::IsTypeOf(request, b2::protocol::channel::SignInChannelRequest::default_instance()))
                {
                    m_fsm.Update(AuthenticationFSM::EVENT_CHANNEL_REQUEST);
                }
            }
        });

    m_eventDispatcher.AddResponseFilter([this](MessagePtr response)
        {
            if (ProtobufFactory::IsTypeOf(response, b2::protocol::session::MatchmakingResponse::default_instance()))
            {
                auto matchmakingResponse = std::static_pointer_cast<b2::protocol::session::MatchmakingResponse>(response);

                auto ec = matchmakingResponse->result().error_code();
                if (ec == 0)
                {
                    if (matchmakingResponse->has_room_location())
                    {
                        const auto roomLocation = matchmakingResponse->room_location();
                        const auto roomId = roomLocation.room_id();
                        const auto channelEndpoint = roomLocation.channel_endpoint();

                        m_messageRouter.SetChannelRoute(Endpoint{channelEndpoint.host(), channelEndpoint.port()});
                    }
                }
            }
            else if (ProtobufFactory::IsTypeOf(response, b2::protocol::session::RaidMatchmakingResponse::default_instance()))
            {
                auto matchmakingResponse = std::static_pointer_cast<b2::protocol::session::RaidMatchmakingResponse>(response);

                auto ec = matchmakingResponse->result().error_code();
                if (ec == 0)
                {
                    if (matchmakingResponse->has_channel_endpoint())
                    {
                        const auto& channelEndpoint = matchmakingResponse->channel_endpoint();

                        m_messageRouter.SetChannelRoute(Endpoint{ channelEndpoint.host(), channelEndpoint.port()});
                    }
                    else if (matchmakingResponse->has_room_location())
                    {
                        const auto roomLocation = matchmakingResponse->room_location();
                        const auto roomId = roomLocation.room_id();
                        const auto channelEndpoint = roomLocation.channel_endpoint();

                        m_messageRouter.SetChannelRoute(Endpoint{channelEndpoint.host(), channelEndpoint.port()});
                    }
                }
            }
            else if (ProtobufFactory::IsTypeOf(response, b2::protocol::session::AssaultMatchmakingResponse::default_instance()))
            {
                auto matchmakingResponse = std::static_pointer_cast<b2::protocol::session::AssaultMatchmakingResponse>(response);

                auto ec = matchmakingResponse->result().error_code();
                if (ec == 0)
                {
                    if (matchmakingResponse->has_channel_endpoint())
                    {
                        const auto& channelEndpoint = matchmakingResponse->channel_endpoint();

                        m_messageRouter.SetChannelRoute(Endpoint{channelEndpoint.host(), channelEndpoint.port()});
                    }
                }
            }
            else if (ProtobufFactory::IsTypeOf(response, b2::protocol::session::AcceptInvitationNotify::default_instance()))
            {
                auto acceptInvitationNotify = std::static_pointer_cast<b2::protocol::session::AcceptInvitationNotify>(response);

				if (acceptInvitationNotify->has_room_location())
				{
					const auto& roomLocation = acceptInvitationNotify->room_location();
					if (roomLocation.has_channel_endpoint())
					{
                        const auto& channelEndpoint = roomLocation.channel_endpoint();

						m_messageRouter.SetChannelRoute(Endpoint{channelEndpoint.host(), channelEndpoint.port()});
					}
				}
            }
            else if (ProtobufFactory::IsTypeOf(response, b2::protocol::channel::RoomJoinResponse::default_instance()))
            {
                auto roomJoinResponse = std::static_pointer_cast<b2::protocol::channel::RoomJoinResponse>(response);

                auto ec = roomJoinResponse->result().error_code();
                if (ec == 0)
                {
                    const std::string& roomId = roomJoinResponse->room_id();

                    B2NETWORK_LOG_INFO(logger, "Response Filter: RoomJoinResponse id(%s) type(%d)",
                        roomJoinResponse->room_id().c_str(), roomJoinResponse->room_type());

                    m_roomJoined = roomId;

                    ResetRoomHeartbeat();
                }
            }
            else if (ProtobufFactory::IsTypeOf(response, b2::protocol::channel::RoomLeaveResponse::default_instance()))
            {
                auto roomLeaveResponse = std::static_pointer_cast<b2::protocol::channel::RoomLeaveResponse>(response);

                bool closeChannel = true;

                auto ec = roomLeaveResponse->result().error_code();
                if (ec == 0)
                {
                    for (auto roomId : roomLeaveResponse->room_id_list())
                    {
                        if (roomId == m_roomJoined)
                        {
                            m_roomJoined.clear();
                            break;
                        }
                    }
                }
                else if (ec == static_cast<int>(ErrorCode::ErrorTryAgainLater))
                {
                    closeChannel = false;
                }

                // NOTE Room Leave 이후에 채널 세션을 유지하면 채널에 캐싱한 계정 데이터가 갱신되지 않아
                // 관련된 버그가 발생하여 RoomLeaveResponse에서 무조건 채널 연결을 끊도록 처리한다.
                if (closeChannel)
                {
                    m_messageRouter.CloseChannel();
                }
            }
            else if (ProtobufFactory::IsTypeOf(response, b2::protocol::channel::RoomHeartbeatResponse::default_instance()))
            {
                
            }
			else if (ProtobufFactory::IsTypeOf(response, b2::protocol::session::GetMasterDataResponse::default_instance()))
			{
				auto getMasterDataResponse	= std::static_pointer_cast<b2::protocol::session::GetMasterDataResponse>(response);
				auto byte_length			= getMasterDataResponse->ByteSize();
				auto* contents_array		= new uint8[byte_length];

				getMasterDataResponse->SerializeToArray(contents_array, byte_length);
				MasterDataCacher::GetInstance().Update(CacheCategory::Key_MasterData, contents_array, byte_length);

				delete [] contents_array;
			}
        });

    m_eventDispatcher.AddNotifyFilter([this](MessagePtr notify)
        {
            if (ProtobufFactory::IsTypeOf(notify, b2::protocol::session::SessionClosedNotify::default_instance()))
            {
                auto sessionClosedNotify = std::static_pointer_cast<b2::protocol::session::SessionClosedNotify>(notify);

                auto reason = sessionClosedNotify->reason();

                m_fsm.Update(AuthenticationFSM::EVENT_SESSION_BANNED);
            }
            else if (ProtobufFactory::IsTypeOf(notify, b2::protocol::session::RaidInvitationNotify::default_instance()))
            {
                auto raidInvitationNotify = std::static_pointer_cast<b2::protocol::session::RaidInvitationNotify>(notify);

                if (raidInvitationNotify->has_room_location())
                {
                    const auto roomLocation = raidInvitationNotify->room_location();
                    const auto roomId = roomLocation.room_id();
                    const auto channelEndpoint = roomLocation.channel_endpoint();

                    m_messageRouter.SetChannelRoute(Endpoint{channelEndpoint.host(), channelEndpoint.port()});
                }
            }
        });

    m_fsm.Start(this);
}

void AuthenticationManager::Stop()
{
    B2NETWORK_LOG_DEBUG(logger, "Stopping...");

    m_fsm.Stop();

    m_sessionToken.clear();

    // TODO fix
    m_messageRouter.PauseSendMessage();
    m_messageRouter.Reset();
}

void AuthenticationManager::Update()
{
    m_fsm.Update(AuthenticationFSM::EVENT_TICK_UPDATE);
}

const CredentialInfo& AuthenticationManager::GetCredentialInfo() const
{
	return m_credentialInfo;
}

const Endpoint& AuthenticationManager::GetSessionEndPoint() const
{
	return m_sessionEndpoint;
}

bool AuthenticationManager::IsActivated() const
{
    return m_fsm.IsActivated();
}

void AuthenticationManager::ConnectToSession()
{
    B2NETWORK_LOG_DEBUG(logger, "Connecting to session %s...", m_sessionEndpoint.ToString().c_str());

    m_messageRouter.SetSessionRoute(m_sessionEndpoint);
}

void AuthenticationManager::HelloSession()
{
	B2NETWORK_LOG_DEBUG(logger, "HelloSession (%s) on the SessionServer(%s) With Token(%s)...",
		m_credentialInfo.ToStringPUID().c_str(), m_sessionEndpoint.ToString().c_str(), m_sessionToken.c_str());

	auto newKey = std::make_shared<Key>();
	m_cipherKeyForSession.swap(newKey);

	FString B2NetworkIni;
	FConfigCacheIni::LoadGlobalIniFile(B2NetworkIni, TEXT("B2Network"));

	FString MasterData, CommonData, SessionProtocol, ChannelProtocol;

	GConfig->GetString(TEXT("ProtocolHash"), TEXT("MasterDataHash"), MasterData, B2NetworkIni);
	GConfig->GetString(TEXT("ProtocolHash"), TEXT("CommonDataHash"), CommonData, B2NetworkIni);
	GConfig->GetString(TEXT("ProtocolHash"), TEXT("SessionProtocolHash"), SessionProtocol, B2NetworkIni);
	GConfig->GetString(TEXT("ProtocolHash"), TEXT("ChannelProtocolHash"), ChannelProtocol, B2NetworkIni);

	auto request = std::make_shared<b2::protocol::session::HelloSessionRequest>();
	//// version
	//auto clientVersion = request->mutable_client_version();
	//clientVersion->set_major(0);
	//clientVersion->set_minor(20);
	//clientVersion->set_revision(0);
	//clientVersion->set_changelist(0);
	//clientVersion->set_build_num(0);
	// protocol
	auto clientProtocol = request->mutable_client_protocol();

#if PLATFORM_IOS
	//iOS의 경우 런타임에 std::string을 임시변수로 생성하면 알 수 없는 크래시가 발생되어 분기 처리함.
	//클래스의 멤버변수일경우 문제 없음..
	clientProtocol->set_master_data_hash(TCHAR_TO_ANSI(*MasterData));
	clientProtocol->set_common_data_hash(TCHAR_TO_ANSI(*CommonData));
	clientProtocol->set_session_message_hash(TCHAR_TO_ANSI(*SessionProtocol));
	clientProtocol->set_channel_message_hash(TCHAR_TO_ANSI(*ChannelProtocol));
#else
	std::string MasterDataHash = TCHAR_TO_ANSI(*MasterData);
	std::string CommonDataHash = TCHAR_TO_ANSI(*CommonData);
	std::string SessionProtocolHash = TCHAR_TO_ANSI(*SessionProtocol);
	std::string ChannelProtocolHash = TCHAR_TO_ANSI(*ChannelProtocol);

	clientProtocol->set_master_data_hash(MasterDataHash.c_str());
	clientProtocol->set_common_data_hash(CommonDataHash.c_str());
	clientProtocol->set_session_message_hash(SessionProtocolHash.c_str());
	clientProtocol->set_channel_message_hash(ChannelProtocolHash.c_str());
#endif
	// public key
	auto hexifiedKeyToVerify = m_cipherKeyForSession->GetPublicKeyToString();
	request->set_client_public_key(hexifiedKeyToVerify);

    const auto result = m_messageRouter.SendControl(request, m_sessionEndpoint,
        [this](MessagePtr response)
        {
            auto helloSessionResponse = std::static_pointer_cast<b2::protocol::session::HelloSessionResponse>(response);

            auto ec = helloSessionResponse->result().error_code();
            if (ec == 0)
            {
                auto publicKey = helloSessionResponse->server_public_key();
                B2NETWORK_LOG_INFO(logger, "Session Server Public Key : %s", publicKey.c_str());

                KeyType serverPublicKey;
                //unhexify(&serverPublicKey[0], crypto_stream_chacha20_KEYBYTES, publicKey);
                m_cipherKeyForSession->MakeSharedKeyForClient(serverPublicKey);

                //B2NETWORK_LOG_INFO(logger, "Session Shared Secret Key : %s", hexify(&m_cipherKeyForSession->GetRealKey()[0], crypto_stream_chacha20_KEYBYTES).c_str());
                //B2NETWORK_LOG_INFO(logger, "Session Shared Nonce Key : %s", hexify(&m_cipherKeyForSession->GetNonceKey()[0], crypto_stream_chacha20_NONCEBYTES).c_str());

                m_fsm.Update(AuthenticationFSM::EVENT_HELLO_SESSION_OK);
            }
            else
            {
                B2NETWORK_LOG_DEBUG(logger, "Failed to exchange a key from session server, error=%d", ec);

                m_sessionToken.clear();

                m_fsm.Update(AuthenticationFSM::EVENT_HELLO_SESSION_ERROR);
            }
        });

    if (!result)
    {
        m_eventDispatcher.Emit(EventDispatcher::ServiceNotAvailable, (int) ErrorCode::HelloSessionFailed);
    }
}

void AuthenticationManager::SignInSession()
{
    B2NETWORK_LOG_DEBUG(logger, "SignIn UDID(%s) on the SessionServer(%s) With Token(%s)...",
        m_credentialInfo.ToStringPUID().c_str(), m_sessionEndpoint.ToString().c_str(),  m_sessionToken.c_str());

	FString B2NetworkIni;
	FConfigCacheIni::LoadGlobalIniFile(B2NetworkIni, TEXT("B2ProtoHash"));

	FString MasterData, CommonData, SessionProtocol, ChannelProtocol;

	GConfig->GetString(TEXT("ProtocolHash"), TEXT("MasterDataHash"), MasterData, B2NetworkIni);
	GConfig->GetString(TEXT("ProtocolHash"), TEXT("CommonDataHash"), CommonData, B2NetworkIni);
	GConfig->GetString(TEXT("ProtocolHash"), TEXT("SessionProtocolHash"), SessionProtocol, B2NetworkIni);
	GConfig->GetString(TEXT("ProtocolHash"), TEXT("ChannelProtocolHash"), ChannelProtocol, B2NetworkIni);

    auto request = std::make_shared<b2::protocol::session::SignInSessionRequest>();
    
	// version
    auto clientVersion = request->mutable_client_version();
    clientVersion->set_major(m_ClientVersion.Major);
    clientVersion->set_minor(m_ClientVersion.Minor);
    clientVersion->set_revision(m_ClientVersion.Revision);
	clientVersion->set_changelist(m_ClientVersion.Changelist);
	clientVersion->set_build_num(m_ClientVersion.BuildNum);

    // protocol
	auto clientProtocol = request->mutable_client_protocol();
#if PLATFORM_IOS
	//iOS의 경우 런타임에 std::string을 임시변수로 생성하면 알 수 없는 크래시가 발생되어 분기 처리함.
	//클래스의 멤버변수일경우 문제 없음.. -shinyoungbae
	clientProtocol->set_master_data_hash(TCHAR_TO_ANSI(*MasterData));
	clientProtocol->set_common_data_hash(TCHAR_TO_ANSI(*CommonData));
	clientProtocol->set_session_message_hash(TCHAR_TO_ANSI(*SessionProtocol));
	clientProtocol->set_channel_message_hash(TCHAR_TO_ANSI(*ChannelProtocol));
#else
	std::string MasterDataHash = TCHAR_TO_ANSI(*MasterData);
	std::string CommonDataHash = TCHAR_TO_ANSI(*CommonData);
	std::string SessionProtocolHash = TCHAR_TO_ANSI(*SessionProtocol);
	std::string ChannelProtocolHash = TCHAR_TO_ANSI(*ChannelProtocol);

    clientProtocol->set_master_data_hash(MasterDataHash.c_str());
    clientProtocol->set_common_data_hash(CommonDataHash.c_str());
    clientProtocol->set_session_message_hash(SessionProtocolHash.c_str());
    clientProtocol->set_channel_message_hash(ChannelProtocolHash.c_str());
#endif
    // credential
    auto credential = request->mutable_credential();
    credential->set_platform_uid(m_credentialInfo.m_platformUserId);
    credential->set_platform_type(m_credentialInfo.m_platformType);
	credential->set_os_type(m_credentialInfo.m_osType);
	credential->set_market_type(m_credentialInfo.m_marketType);
	credential->set_country_code(m_credentialInfo.m_countryCode);
	credential->set_lang_code(m_credentialInfo.m_langCode);
	credential->set_push_token(m_credentialInfo.m_pushToken);
	credential->set_ad_id(m_credentialInfo.m_advertizeId);
	credential->set_access_token(m_credentialInfo.m_accessToken);

    if (!m_sessionToken.empty())
    {
        request->set_session_token(m_sessionToken);
    }

	if (!m_credentialInfo.m_client_cl.empty())
	{
		request->set_client_cl(m_credentialInfo.m_client_cl);
	}

    const auto result = m_messageRouter.SendControl(request, m_sessionEndpoint,
        [this](MessagePtr response)
        {
            auto signInSessionResponse = std::static_pointer_cast<b2::protocol::session::SignInSessionResponse>(response);

            auto ec = signInSessionResponse->result().error_code();
            if (ec == 0)
            {
                m_sessionToken = signInSessionResponse->session_token();
                m_nickname = signInSessionResponse->global_account().nickname();
                m_accountId = signInSessionResponse->global_account().account_id();

                // @TODO set more global account info, option etc

                ProtobufFactory::SetMasterDataHash(signInSessionResponse->master_data_hash());

                B2NETWORK_LOG_DEBUG(logger, "Succeeded to sign in on the session server (%d)(%s)(%s)(%s)",
                    m_accountId, m_nickname.c_str(), m_sessionToken.c_str(), ProtobufFactory::GetMasterDataHash().c_str());

                m_messageRouter.ResumeSendMessage();

                m_fsm.Update(AuthenticationFSM::EVENT_SIGNIN_SESSION_OK);
            }
            else if (ec == 103)
            {
                B2NETWORK_LOG_DEBUG(logger, "Succeeded to sign in on the session server (with no nickname)");

                m_messageRouter.ResumeSendMessage();

                m_fsm.Update(AuthenticationFSM::EVENT_SIGNIN_SESSION_OK);

                // TODO: fix
                m_eventDispatcher.Emit(EventDispatcher::RequireGiveNickname);
            }
            else if (ec == 110)
            {
                B2NETWORK_LOG_DEBUG(logger, "Invalid session token %s", m_sessionToken.c_str());

                m_sessionToken.clear();

                m_fsm.Update(AuthenticationFSM::EVENT_SIGNIN_SESSION_INVALID_TOKEN);
            }
            else if (ec == 8010)
            {
                B2NETWORK_LOG_ERROR(logger, "Version mismatch!");

                m_eventDispatcher.Emit(EventDispatcher::RequireUpdateVersion);

                m_fsm.Update(AuthenticationFSM::EVENT_VERSION_MISMATCH);
            }
            else if (ec == 8011)
            {
                B2NETWORK_LOG_ERROR(logger, "Protocol mismatch!");

                m_eventDispatcher.Emit(EventDispatcher::RequireUpdateProtocol);

                // TODO: fix
                bool acceptOldVersion = true;
                if (acceptOldVersion)
                {
                    m_sessionToken = signInSessionResponse->session_token();
                    m_nickname = signInSessionResponse->global_account().nickname();
                    m_accountId = signInSessionResponse->global_account().account_id();

                    // @TODO set more global account info, option etc

                    ProtobufFactory::SetMasterDataHash(signInSessionResponse->master_data_hash());

                    B2NETWORK_LOG_DEBUG(logger, "Succeeded to sign in on the session server (%d)(%s)(%s)(%s)",
                        m_accountId, m_nickname.c_str(), m_sessionToken.c_str(), ProtobufFactory::GetMasterDataHash().c_str());

                    m_messageRouter.ResumeSendMessage();

                    m_fsm.Update(AuthenticationFSM::EVENT_SIGNIN_SESSION_OK);
                }
                else
                {
                    m_fsm.Update(AuthenticationFSM::EVENT_PROTOCOL_MISMATCH);
                }
            }
            else
            {
                B2NETWORK_LOG_DEBUG(logger, "Failed to sign in on the session server, error=%d", ec);

                m_sessionToken.clear();

                m_fsm.Update(AuthenticationFSM::EVENT_SIGNIN_SESSION_FAILED);
            }
        });

    if (!result)
    {
        //
    }
}

bool AuthenticationManager::Ping()
{
    if (m_sessionPingSendSeq > m_sessionPingRecvSeq)
    {
        if (m_sessionPingRecvSeq > 0)
        {
            auto now = std::chrono::steady_clock::now();

            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - m_sessionPingRecvTime).count();
            if (elapsed > LOCAL_PING_TIMEOUT)
            {
                // local timeout
                return false;
            }
        }

        // skip
        return true;
    }

    m_sessionPingSendSeq++;

    auto request = std::make_shared<b2::protocol::session::PingRequest>();
    request->set_seq(m_sessionPingSendSeq);

	auto sendTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    auto result = m_messageRouter.SendRequest(request, [this, sendTimeMs](MessagePtr response)
    {
        auto pingResponse = std::static_pointer_cast<b2::protocol::session::PingResponse>(response);

        auto ec = pingResponse->result().error_code();
        if (ec == 0)
        {
            m_sessionPingRecvSeq = pingResponse->seq();
            m_sessionPingRecvTime = std::chrono::steady_clock::now();

			if (pingResponse->has_current_server_time())
			{
				auto recvTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
				auto halfLatency = (recvTimeMs - sendTimeMs) / 2;
				m_sessionServerTimeMs = pingResponse->current_server_time() + halfLatency;

				m_eventDispatcher.Emit(EventDispatcher::ServerTimeSync);
			}
        }
        else
        {
            Emit(AuthenticationManager::ServiceNotAvailable, (int) ErrorCode::SessionPingFailed);
        }
    });

    return result;
}

void AuthenticationManager::ResetSession()
{
    // reset ping sequence
    m_sessionPingSendSeq = 0;
    m_sessionPingRecvSeq = 0;
}

int64 b2network::AuthenticationManager::GetSessionServerTime() const
{
	return m_sessionServerTimeMs;
}

void AuthenticationManager::HelloChannel()
{
	auto channelEndpoint = m_messageRouter.GetChannelRoute();

	B2NETWORK_LOG_DEBUG(logger, "SignIn on the ChannelServer(%s) with Token(%s)...", channelEndpoint.ToString().c_str(), m_sessionToken.c_str());

	if (!channelEndpoint.IsValid())
	{
		// TODO fix

		B2NETWORK_LOG_ERROR(logger, "Could not sign in channel - endpoint is invalid!");
		return;
	}

	if (m_sessionToken.empty())
	{
		// TODO fix

		B2NETWORK_LOG_ERROR(logger, "Could not sign in channel - No session token");
		return;
	}

	auto newKey = std::make_shared<Key>();
	m_cipherKeyForChannel.swap(newKey);

	FString B2NetworkIni;
	FConfigCacheIni::LoadGlobalIniFile(B2NetworkIni, TEXT("B2Network"));

	FString MasterData, CommonData, SessionProtocol, ChannelProtocol;

	GConfig->GetString(TEXT("ProtocolHash"), TEXT("MasterDataHash"), MasterData, B2NetworkIni);
	GConfig->GetString(TEXT("ProtocolHash"), TEXT("CommonDataHash"), CommonData, B2NetworkIni);
	GConfig->GetString(TEXT("ProtocolHash"), TEXT("SessionProtocolHash"), SessionProtocol, B2NetworkIni);
	GConfig->GetString(TEXT("ProtocolHash"), TEXT("ChannelProtocolHash"), ChannelProtocol, B2NetworkIni);

	auto request = std::make_shared<b2::protocol::channel::HelloChannelRequest>();
	//// version
	//auto clientVersion = request->mutable_client_version();
	//clientVersion->set_major(0);
	//clientVersion->set_minor(20);
	//clientVersion->set_revision(0);
	//clientVersion->set_changelist(0);
	//clientVersion->set_build_num(0);

	// protocol
	auto clientProtocol = request->mutable_client_protocol();
#if PLATFORM_IOS
	//iOS의 경우 런타임에 std::string을 임시변수로 생성하면 알 수 없는 크래시가 발생되어 분기 처리함.
	//클래스의 멤버변수일경우 문제 없음.. -shinyoungbae
	clientProtocol->set_master_data_hash(TCHAR_TO_ANSI(*MasterData));
	clientProtocol->set_common_data_hash(TCHAR_TO_ANSI(*CommonData));
	clientProtocol->set_session_message_hash(TCHAR_TO_ANSI(*SessionProtocol));
	clientProtocol->set_channel_message_hash(TCHAR_TO_ANSI(*ChannelProtocol));
#else
	std::string MasterDataHash = TCHAR_TO_ANSI(*MasterData);
	std::string CommonDataHash = TCHAR_TO_ANSI(*CommonData);
	std::string SessionProtocolHash = TCHAR_TO_ANSI(*SessionProtocol);
	std::string ChannelProtocolHash = TCHAR_TO_ANSI(*ChannelProtocol);

	clientProtocol->set_master_data_hash(MasterDataHash.c_str());
	clientProtocol->set_common_data_hash(CommonDataHash.c_str());
	clientProtocol->set_session_message_hash(SessionProtocolHash.c_str());
	clientProtocol->set_channel_message_hash(ChannelProtocolHash.c_str());
#endif

	// public key
	//auto hexifiedKey = hexify(&m_cipherKeyForChannel->GetPublicKey()[0], crypto_stream_chacha20_KEYBYTES);
	//B2NETWORK_LOG_INFO(logger, "Channel Client Public Key : %s", hexifiedKey.c_str());
	//request->set_client_public_key(hexifiedKey);

    const auto result = m_messageRouter.SendControl(request, channelEndpoint,
        [this](MessagePtr response)
        {
		    auto helloChannelResponse = std::static_pointer_cast<b2::protocol::channel::HelloChannelResponse>(response);

            auto ec = helloChannelResponse->result().error_code();
            if (ec == 0)
            {
                auto publicKey = helloChannelResponse->server_public_key();
                B2NETWORK_LOG_INFO(logger, "Channel Server Public Key : %s", publicKey.c_str());

                KeyType serverPublicKey;
                //unhexify(&serverPublicKey[0], crypto_stream_chacha20_KEYBYTES, publicKey);
                m_cipherKeyForChannel->MakeSharedKeyForClient(serverPublicKey);

                B2NETWORK_LOG_INFO(logger, "Channel Shared Secret Key : %s", hexify(&m_cipherKeyForChannel->GetRealKey()[0], crypto_stream_chacha20_KEYBYTES).c_str());
                B2NETWORK_LOG_INFO(logger, "Channel Shared Nonce Key : %s", hexify(&m_cipherKeyForChannel->GetNonceKey()[0], crypto_stream_chacha20_NONCEBYTES).c_str());

                m_fsm.Update(AuthenticationFSM::EVENT_HELLO_CHANNEL_OK);
            }
            else
            {
                B2NETWORK_LOG_DEBUG(logger, "Failed to exchange a key from channel server, error=%d", ec);
            }
        });

    if (!result)
    {
        // TODO
    }
}

void AuthenticationManager::SignInChannel()
{
    auto channelEndpoint = m_messageRouter.GetChannelRoute();

    B2NETWORK_LOG_DEBUG(logger, "SignIn on the ChannelServer(%s) with Token(%s)...", channelEndpoint.ToString().c_str(), m_sessionToken.c_str());

    if (!channelEndpoint.IsValid())
    {
        // TODO fix

        B2NETWORK_LOG_ERROR(logger, "Could not sign in channel - endpoint is invalid!");
        return;
    }

    if (m_sessionToken.empty())
    {
        // TODO fix

        B2NETWORK_LOG_ERROR(logger, "Could not sign in channel - No session token");
        return;
    }

    auto request = std::make_shared <b2::protocol::channel::SignInChannelRequest>();
    request->set_session_token(m_sessionToken);
	request->set_account_id(m_accountId);

	if (!m_credentialInfo.m_client_cl.empty())
	{
		request->set_client_cl(m_credentialInfo.m_client_cl);
	}

    const auto result = m_messageRouter.SendControl(request, channelEndpoint,
        [this](MessagePtr response)
        {
            auto signInChannelResponse = std::static_pointer_cast<b2::protocol::channel::SignInChannelResponse>(response);

            auto ec = signInChannelResponse->result().error_code();
            if (ec == 0)
            {
                B2NETWORK_LOG_DEBUG(logger, "Succeeded to sign in on the channel server");

                // TODO refactoring
                m_roomJoined.clear();

                // TODO fix this buggy stuff
                m_messageRouter.ResumeSendMessage();

                m_fsm.Update(AuthenticationFSM::EVENT_SIGNIN_CHANNEL_OK);
            }
            else if (ec == 110)
            {
                B2NETWORK_LOG_DEBUG(logger, "Invalid session token %s", m_sessionToken.c_str());

                m_fsm.Update(AuthenticationFSM::EVENT_SIGNIN_CHANNEL_INVALID_TOKEN);
            }
            else if (ec == 8406)
            {
                // TODO timeout
                m_fsm.Update(AuthenticationFSM::EVENT_SIGNIN_CHANNEL_FAILED);
            }
            else
            {
                B2NETWORK_LOG_DEBUG(logger, "Failed to sign in on the channel server, error=%d", ec);

                m_fsm.Update(AuthenticationFSM::EVENT_SIGNIN_CHANNEL_FAILED);
            }
        });

    if (!result)
    {
        // TODO
    }
}

void AuthenticationManager::LogOutChannel()
{
    B2NETWORK_LOG_DEBUG(logger, "LogOut from the ChannelServer...");

    m_roomJoined.clear();

    m_messageRouter.ClearChannelRoute();
}

void AuthenticationManager::RoomHeartbeatForeground(const std::string& roomId)
{
    if (m_roomJoined.empty())
    {
        // NOTE turn on the background heartbeat
        m_roomBackgroundHeartbeat = true;

        return;
    }

    if (m_roomJoined != roomId)
    {
        B2NETWORK_LOG_ERROR(logger, "Heartbeat Room id mismatch Joined(%s) Channel(%s)!",
            m_roomJoined.c_str(), roomId.c_str());

        // NOTE turn on the background heartbeat
        m_roomBackgroundHeartbeat = true;

        return;
    }

    auto now = std::chrono::steady_clock::now();

    if (m_roomBackgroundHeartbeat)
    {
        // the first heartbeat from the main thread
        RoomHeartbeatImpl(true);
        m_roomHeartbeatSendTime = now;

        // NOTE turn off the background heartbeat
        m_roomBackgroundHeartbeat = false;
    }
    else
    {
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_roomHeartbeatSendTime).count();

        // NOTE local timeout 8 seconds
        // it must be longer than 1vs1, raid, assault timeout in channel server

        if (elapsed > m_channelHeartbeatPeriod * 1000 * 4)
        {
            B2NETWORK_LOG_ERROR(logger, "Foreground hearbeat local timeout! (%s)(%d)", roomId.c_str(), elapsed);

            Emit(AuthenticationManager::ChannelRoomHearbeatTimeout, (int)ErrorCode::ChannelForePingFailed);

            return;
        }

        auto period = (m_channelHeartbeatPeriod * 1000) / 3;
        period = std::max(period, 667);
        if (elapsed > period)
        {
            RoomHeartbeatImpl(true);
            m_roomHeartbeatSendTime = now;
        }
    }
}

bool AuthenticationManager::RoomHeartbeatBackground()
{
    if (m_roomBackgroundHeartbeat)
    {
        return RoomHeartbeatImpl(false);
    }
    else
    {
        return true;
    }
}

bool AuthenticationManager::RoomHeartbeatImpl(bool bFore)
{
    if (m_roomJoined.empty())
    {
        return true;
    }

    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - m_roomHeartbeatRecvTime).count();

    if (m_roomHeartbeatSendSeq > m_roomHeartbeatRecvSeq)
    {
        if (m_roomHeartbeatRecvSeq > 0)
        {
            if (elapsed > m_channelHeartbeatPeriod * 3)
            {
                // local timeout
                B2NETWORK_LOG_INFO(logger, "Room Heartbeat (%d) seconds Timeout!", elapsed);

                m_roomJoined.clear();

                Emit(AuthenticationManager::ChannelRoomHearbeatTimeout, bFore ? (int)ErrorCode::ChannelForePingFailed : (int)ErrorCode::ChannelBackPingFailed);

                return false;
            }
        }

        // skip
        return true;
    }

    if (elapsed < m_channelHeartbeatPeriod)
    {
        // skip
        return true;
    }

    m_roomHeartbeatSendSeq++;

    auto request = std::make_shared<b2::protocol::channel::RoomHeartbeatRequest>();
    request->set_room_id(m_roomJoined);
    request->set_seq(m_roomHeartbeatSendSeq);

    auto result = m_messageRouter.SendRequest(request, [this, bFore](MessagePtr response)
    {
        auto roomHeartbeatResponse = std::static_pointer_cast<b2::protocol::channel::RoomHeartbeatResponse>(response);

        auto ec = roomHeartbeatResponse->result().error_code();
        if (ec == 0)
        {
            if (m_roomJoined == roomHeartbeatResponse->room_id())
            {
                m_roomHeartbeatRecvSeq = roomHeartbeatResponse->seq();
                m_roomHeartbeatRecvTime = std::chrono::steady_clock::now();
            }
            else
            {
                B2NETWORK_LOG_ERROR(logger, "Room Heartbeat Error (no room joined)");
                // TODO
            }
        }
        else
        {
            B2NETWORK_LOG_ERROR(logger, "Room Heartbeat Error(%d)", ec);

            // TODO this causes a reentrancy problem in the request manager
            // it seems to modify the queue of the request manager while iterating
            Emit(AuthenticationManager::ChannelRoomHearbeatTimeout, bFore ? (int)ErrorCode::ChannelForePingFailed : (int)ErrorCode::ChannelBackPingFailed);
        }
    });

    return result;
}

void AuthenticationManager::ResetRoomHeartbeat()
{
    m_roomBackgroundHeartbeat = true;
    m_roomHeartbeatSendSeq = 0;
    m_roomHeartbeatRecvSeq = 0;
}

void AuthenticationManager::Emit(const Event event, int errorCode /*= 0*/)
{
    switch (event)
    {
    case ServiceConnecting:
        m_eventDispatcher.Emit(EventDispatcher::Event::ServiceConnecting);
        break;

    case ServiceConnected:
        m_eventDispatcher.Emit(EventDispatcher::Event::ServiceConnected);
        break;

    case ServiceNotAvailable:
        m_eventDispatcher.Emit(EventDispatcher::Event::ServiceNotAvailable, errorCode);
        break;

    case ChannelRoomHearbeatTimeout:
        m_eventDispatcher.Emit(EventDispatcher::Event::ChannelRoomHeartbeatTimeout, errorCode);
        break;

    case ChannelDisconnected:
        m_eventDispatcher.Emit(EventDispatcher::Event::ChannelDisconnected, errorCode);
        break;

    default:
        B2NETWORK_LOG_ERROR(logger, "Unhandled event(%d)!", event);
        break;
    }
}

void AuthenticationManager::ResetReconnectingCount()
{
    m_reconnectingCount = 0;
}

bool AuthenticationManager::CheckReconnectingCount(int& backoff)
{
    bool result = m_reconnectingCount < MAX_RECONNECTING_COUNT;
    if (result)
    {
        // slots (0 ~ 6) * 100ms
        //        0,     1,     2,     3,     4,     5,      6
        //        0,   100,   400,   900,  1600,  2500,   4900,
        //        0,   100,   500,  1400,  3000,  5500,  10400,  

        backoff = (m_reconnectingCount * m_reconnectingCount) * 100;

    }

    m_reconnectingCount++;

    return result;
}

void AuthenticationManager::InjectCipherKeyIntoSessionConnection()
{
	m_messageRouter.InjectCipherKeyIntoConnection(Connection::SESSION_SERVER, *(m_cipherKeyForSession.get()));
}

void AuthenticationManager::InjectCipherKeyIntoChannelConnection()
{
	m_messageRouter.InjectCipherKeyIntoConnection(Connection::CHANNEL_SERVER, *(m_cipherKeyForChannel.get()));
}

std::string AuthenticationManager::Dump()
{
    std::ostringstream oss;
	oss << m_credentialInfo.ToStringPUID() << ", ";
    oss << "accountId=" << m_accountId << ", "
        << "nickname=" << m_nickname << ", "
        << "token=" << m_sessionToken << ", "
        << "state=" << m_fsm.GetState() << ", "
        << "reconnecting=" << m_reconnectingCount << ", "
        << "room=" << m_roomJoined;

    return oss.str();
}

void b2network::AuthenticationManager::SetHeartbeatPeriod(int sessionHeartbeat, int channelHeartbeat)
{
	m_sessionHeartbeatPeriod = sessionHeartbeat;
	m_channelHeartbeatPeriod = channelHeartbeat;
}

