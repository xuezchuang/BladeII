#include "AuthenticationFSM.h"
#include "B2NetworkPlugin.h"

#include "ErrorCode.h"
#include "LogManager.h"
#include "AuthenticationManager.h"

#include "NetConfigure.h"

using namespace b2network;

#if B2_BUILD_LEVEL < B2_BUILD_LEVEL_QA // !B2_BUILD_LEVEL_LIVE, !!B2_BUILD_LEVEL_QA
static auto logger = LogManager::GetLogger("AuthenticationFSM");
#endif

AuthenticationFSM::InitialState AuthenticationFSM::s_initial;
AuthenticationFSM::HelloSessionState AuthenticationFSM::s_helloSession;
AuthenticationFSM::SignInSessionState AuthenticationFSM::s_signInSession;
AuthenticationFSM::ReconnectingState AuthenticationFSM::s_reconnecting;
AuthenticationFSM::AuthenticatedState AuthenticationFSM::s_authenticated;
AuthenticationFSM::NoServiceState AuthenticationFSM::s_noService;

AuthenticationFSM::AuthenticationFSM()
    :m_manager(nullptr)
{
    m_current = &s_initial;
}

AuthenticationFSM::~AuthenticationFSM()
{
}

void AuthenticationFSM::Start(AuthenticationManager* manager)
{
    B2NETWORK_LOG_INFO(logger, "Starting...");

    m_manager = manager;

    m_current = &s_initial;
    m_current->Enter(m_manager);
}

void AuthenticationFSM::Stop()
{
    // TODO not implemented yet
    B2NETWORK_LOG_INFO(logger, "Stopped");
}

void AuthenticationFSM::Update(Event event, int errorCode /*= 0*/)
{
    //B2NETWORK_LOG_DEBUG(logger, "Updating <%s> with event(%d)...", m_current->GetStateName().c_str(), event);

    auto state = m_current->Update(event, errorCode, m_manager);
    if (state != nullptr)
    {
        m_current->Leave(m_manager);

        B2NETWORK_LOG_INFO(logger, "<%s>:(%d) -> <%s>", m_current->GetStateName().c_str(), event, state->GetStateName().c_str());

        m_current = state;
        m_current->Enter(m_manager);
    }
}

bool AuthenticationFSM::IsActivated() const
{
    return m_current != &s_noService;
}

std::string AuthenticationFSM::GetState()
{
    return m_current->GetStateName();
}

std::string AuthenticationFSM::ToString()
{
    std::ostringstream oss;
    oss << m_current->GetStateName();
    return oss.str();
}

// InitialState

void AuthenticationFSM::InitialState::Enter(AuthenticationManager* manager)
{
    manager->ResetReconnectingCount();
}

void AuthenticationFSM::InitialState::Leave(AuthenticationManager* manager)
{
}

AuthenticationFSM::State*
AuthenticationFSM::InitialState::Update(Event event, int errorCode, AuthenticationManager* manager)
{
    if (event == AuthenticationFSM::EVENT_TICK_UPDATE)
    {
        return nullptr;
    }
    else
    {
        return &s_helloSession;
    }
}

// HelloSessionState

void AuthenticationFSM::HelloSessionState::Enter(AuthenticationManager* manager)
{
	manager->Emit(AuthenticationManager::ServiceConnecting);

    const auto now = std::chrono::steady_clock::now();
    m_lastHelloSession = now;
    m_maxHelloSessionTries = 0;

    manager->HelloSession();
}

void AuthenticationFSM::HelloSessionState::Leave(AuthenticationManager* manager)
{
	manager->ResetReconnectingCount();
}

AuthenticationFSM::State*
AuthenticationFSM::HelloSessionState::Update(Event event, int errorCode, AuthenticationManager* manager)
{
	if (event == AuthenticationFSM::EVENT_CONNECTED_TO_SESSION)
	{
        // ignore
	}
	else if (event == AuthenticationFSM::EVENT_CONNECT_FAIL_TO_SESSION)
	{
		manager->ResetReconnectingCount();
		return &s_reconnecting;
	}
	else if (event == AuthenticationFSM::EVENT_DISCONNECTED_FROM_SESSION)
	{
		return &s_initial;
	}
	else if (event == AuthenticationFSM::EVENT_HELLO_SESSION_OK)
	{
		manager->InjectCipherKeyIntoSessionConnection();
		return &s_signInSession;
	}
    else if (event == AuthenticationFSM::EVENT_HELLO_SESSION_ERROR)
    {
        return &s_initial;
    }

    const auto now = std::chrono::steady_clock::now();

    const auto helloSessionElapsed = std::chrono::duration_cast<std::chrono::seconds>(now - m_lastHelloSession);
    if (helloSessionElapsed.count() > 3)
    {
        if (m_maxHelloSessionTries++ < 5)
        {
            // hello session again
            manager->HelloSession();

            m_lastHelloSession = now;
        }
        else
        {
			manager->Emit(AuthenticationManager::ServiceNotAvailable, (int)ErrorCode::HelloSessionFailed);
            return &s_noService;
        }
    }

	return nullptr;
}

// SignInSessionState

void AuthenticationFSM::SignInSessionState::Enter(AuthenticationManager* manager)
{
    manager->Emit(AuthenticationManager::ServiceConnecting);
    manager->SignInSession();
}

void AuthenticationFSM::SignInSessionState::Leave(AuthenticationManager* manager)
{
    manager->ResetReconnectingCount();
}

AuthenticationFSM::State*
AuthenticationFSM::SignInSessionState::Update(Event event, int errorCode, AuthenticationManager* manager)
{
    if (event == AuthenticationFSM::EVENT_CONNECTED_TO_SESSION)
    {
        // FIX seems to sign in again
        // TODO resolve duplication in the request manager
        return nullptr;
    }
    else if (event == AuthenticationFSM::EVENT_CONNECT_FAIL_TO_SESSION)
    {
        manager->ResetReconnectingCount();

        return &s_reconnecting;
    }
    else if (event == AuthenticationFSM::EVENT_DISCONNECTED_FROM_SESSION)
    {
        return &s_initial;
    }
    else if (event == AuthenticationFSM::EVENT_SIGNIN_SESSION_OK)
    {
        return &s_authenticated;
    }
    else if (event == AuthenticationFSM::EVENT_SIGNIN_SESSION_FAILED)
    {
        return nullptr;
    }
    else if (event == AuthenticationFSM::EVENT_SIGNIN_SESSION_INVALID_TOKEN)
    {
		manager->Emit(AuthenticationManager::ServiceNotAvailable, (int)ErrorCode::SigningSessionInvalidToken);
        return &s_noService;
    }
    else if (event == AuthenticationFSM::EVENT_PROTOCOL_MISMATCH)
    {
		manager->Emit(AuthenticationManager::ServiceNotAvailable, (int)ErrorCode::SessionProtocolMissMatch);
        return &s_noService;
    }
    else if (event == AuthenticationFSM::EVENT_SESSION_BANNED)
    {
		manager->Emit(AuthenticationManager::ServiceNotAvailable, (int)ErrorCode::SessionBan);
        return &s_noService;
    }
    else
    {
        return nullptr;
    }
}

// ReconnectingState

void AuthenticationFSM::ReconnectingState::Enter(AuthenticationManager* manager)
{
    B2NETWORK_LOG_DEBUG(logger, "Reconnecting to session server...");

    // init backoff
    m_waiting = false;
    m_backoff = 0;
    m_updated = std::chrono::steady_clock::now();

    manager->Emit(AuthenticationManager::ServiceConnecting);
    manager->ConnectToSession();
}

void AuthenticationFSM::ReconnectingState::Leave(AuthenticationManager* manager)
{
}

AuthenticationFSM::State*
AuthenticationFSM::ReconnectingState::Update(Event event, int errorCode, AuthenticationManager* manager)
{
    if (event == AuthenticationFSM::EVENT_CONNECTED_TO_SESSION)
    {
        return &s_helloSession;
    }
    else if (event == AuthenticationFSM::EVENT_CONNECT_FAIL_TO_SESSION)
    {
        int backoff = 0;
        if (manager->CheckReconnectingCount(backoff))
        {
            // set backoff
            m_waiting = true;
            m_backoff = backoff;
            m_updated = std::chrono::steady_clock::now();

            return nullptr;
        }
        else
        {
			manager->Emit(AuthenticationManager::ServiceNotAvailable, (int)ErrorCode::SessionReconnectedFail);
            return &s_noService;
        }
    }
    else if (event == AuthenticationFSM::EVENT_DISCONNECTED_FROM_SESSION)
    {
        return &s_reconnecting;
    }
    else if (event == AuthenticationFSM::EVENT_SIGNIN_SESSION_OK)
    {
        return nullptr;
    }
    else if (event == AuthenticationFSM::EVENT_SIGNIN_SESSION_FAILED)
    {
        return nullptr;
    }
    else if (event == AuthenticationFSM::EVENT_SIGNIN_SESSION_INVALID_TOKEN)
    {
        return nullptr;
    }
    else if (event == AuthenticationFSM::EVENT_SESSION_BANNED)
    {
		manager->Emit(AuthenticationManager::ServiceNotAvailable, (int)ErrorCode::SessionBan);
        return &s_noService;
    }
    else
    {
        if (m_waiting)
        {
            auto elapsed = std::chrono::duration_cast<
                std::chrono::seconds>(std::chrono::steady_clock::now() - m_updated).count();
            if (elapsed > m_backoff)
            {
                manager->ConnectToSession();

                m_waiting = false;
            }
        }

        return nullptr;
    }
}

// AuthenticatedState

void AuthenticationFSM::AuthenticatedState::Enter(AuthenticationManager* manager)
{
    const auto now = std::chrono::steady_clock::now();

    m_lastSessionPing = now - std::chrono::seconds(manager->GetSessionHeartbeatPeriod());

    // NOTE need a description
    manager->ResetSession();

    manager->Emit(AuthenticationManager::ServiceConnected);
}

void AuthenticationFSM::AuthenticatedState::Leave(AuthenticationManager* manager)
{
}

AuthenticationFSM::State*
AuthenticationFSM::AuthenticatedState::Update(Event event, int errorCode, AuthenticationManager* manager)
{
    // session events
    if (event == AuthenticationFSM::EVENT_CONNECTED_TO_SESSION)
    {
        return &s_helloSession;
    }
    else if (event == AuthenticationFSM::EVENT_DISCONNECTED_FROM_SESSION)
    {
        manager->ResetReconnectingCount();

        return &s_reconnecting;
    }
    // channel events
    else if (event == AuthenticationFSM::EVENT_CONNECTED_TO_CHANNEL)
    {
        m_signedInChannel = false;

        manager->HelloChannel();

        return nullptr;
    }
	else if (event == AuthenticationFSM::EVENT_HELLO_CHANNEL_OK)
	{
		manager->InjectCipherKeyIntoChannelConnection();
		manager->SignInChannel();

        return nullptr;
	}
    else if (event == AuthenticationFSM::EVENT_CONNECT_FAIL_TO_CHANNEL)
    {
        // TODO it works?
        manager->Emit(AuthenticationManager::ChannelDisconnected, errorCode);

        return nullptr;
    }
    else if (event == AuthenticationFSM::EVENT_DISCONNECTED_FROM_CHANNEL)
    {
        m_signedInChannel = false;

        manager->LogOutChannel();

        return nullptr;
    }
    else if (event == AuthenticationFSM::EVENT_SIGNIN_CHANNEL_OK)
    {
        m_signedInChannel = true;

        return nullptr;
    }
    else if (event == AuthenticationFSM::EVENT_SIGNIN_CHANNEL_FAILED)
    {
        manager->LogOutChannel();

        return nullptr;
    }
    else if (event == AuthenticationFSM::EVENT_SIGNIN_CHANNEL_INVALID_TOKEN)
    {
        return &s_helloSession;
    }
    else if (event == AuthenticationFSM::EVENT_SESSION_BANNED)
    {
		manager->Emit(AuthenticationManager::ServiceNotAvailable, (int)ErrorCode::SessionBan);
        return &s_noService;
    }
    else if (event == AuthenticationFSM::EVENT_TICK_UPDATE)
    {
        // NOTE send a heartbeat to the channel that are connected
        if (m_signedInChannel)
        {
            manager->RoomHeartbeatBackground();
        }

        const auto now = std::chrono::steady_clock::now();
        const auto sessionPingElapsed = std::chrono::duration_cast<std::chrono::seconds>(now - m_lastSessionPing);

        // NOTE send a session ping
        if (sessionPingElapsed.count() >= manager->GetSessionHeartbeatPeriod())
        {
            auto result = manager->Ping();
            if (result)
            {
                m_lastSessionPing = now;
            }
            else
            {
				manager->Emit(AuthenticationManager::ServiceNotAvailable, (int)ErrorCode::SessionPingFailed2);
                return &s_noService;
            }
        }

        return nullptr;
    }
    else
    {
        return nullptr;
    }
}

// NoServiceState

void AuthenticationFSM::NoServiceState::Enter(AuthenticationManager* manager)
{
  //manager->Emit(AuthenticationManager::ServiceNotAvailable, (int) ErrorCode::NoServiceStateInAuthFSM);
}

void AuthenticationFSM::NoServiceState::Leave(AuthenticationManager* manager)
{
}

AuthenticationFSM::State*
AuthenticationFSM::NoServiceState::Update(Event event, int errorCode, AuthenticationManager* manager)
{
    return nullptr;
}
