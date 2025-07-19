#include "ConnectionFSM.h"
#include "B2NetworkPlugin.h"

#include "LogManager.h"
#include "Connection.h"
#include "Endpoint.h"
#include "ConnectionManager.h"
#include "ErrorCode.h"


using namespace b2network;

#if B2_BUILD_LEVEL < B2_BUILD_LEVEL_QA // !B2_BUILD_LEVEL_LIVE, !!B2_BUILD_LEVEL_QA
static auto logger = LogManager::GetLogger("ConnectionFSM");
#endif

ConnectionFSM::InitialState ConnectionFSM::s_initial;
ConnectionFSM::ConnectState ConnectionFSM::s_connect;
ConnectionFSM::ConnectingState ConnectionFSM::s_connecting;
ConnectionFSM::ConnectFailState ConnectionFSM::s_connectFail;
ConnectionFSM::ConnectedState ConnectionFSM::s_connected;
ConnectionFSM::DisconnectedState ConnectionFSM::s_disconnected;
ConnectionFSM::FinalState ConnectionFSM::s_final;

ConnectionFSM::ConnectionFSM(ConnectionManager& connectionManager, const ServerType serverType, const Endpoint& endpoint)
    :m_connectionManager(connectionManager),
     m_endpoint(endpoint),
     m_stopped(false)
{
    m_current = &s_initial;

    m_connection = std::make_shared<Connection>(serverType);
}

ConnectionFSM::~ConnectionFSM()
{
}

void ConnectionFSM::Start()
{
    B2NETWORK_LOG_DEBUG(logger, "Starting...");

    m_stopped = false;

    m_current = &s_initial;
    m_current->Enter(this, m_connection, m_endpoint);
}

void ConnectionFSM::Stop()
{
    B2NETWORK_LOG_DEBUG(logger, "Stopping...");

    if (m_current != &s_final)
    {
        m_current->Leave(this, m_connection);

        m_current = &s_final;
        m_current->Enter(this, m_connection, m_endpoint);
    }

    m_stopped = true;
}

void ConnectionFSM::Update(const Connection::UpdateEvent events)
{
    if (m_stopped)
    {
        B2NETWORK_LOG_ERROR(logger, "Could not update stopped fsm!");
        return;
    }

    auto state = m_current->Update(this, m_connection, events);
    if (state != nullptr)
    {
        m_current->Leave(this, m_connection);

        B2NETWORK_LOG_DEBUG(logger, "%s <%s>:(%d) -> <%s>...", m_connection->ToString().c_str(),
            m_current->GetState().c_str(), events, state->GetState().c_str());

        m_current = state;
        m_current->Enter(this, m_connection, m_endpoint);
    }
}

std::string ConnectionFSM::GetState()
{
    return m_current->GetState();
}

void ConnectionFSM::OnConnectedEvent()
{
    B2NETWORK_LOG_DEBUG(logger, "%s Connected Event...", ToString().c_str());

    m_connectionManager.OnConnectedEvent(m_connection->GetServerType());
}

void ConnectionFSM::OnConnectFailEvent(int ErrorCode)
{
    B2NETWORK_LOG_DEBUG(logger, "%s Connect Fail Event...", ToString().c_str());

    m_connectionManager.OnConnectFailEvent(m_connection->GetServerType(), ErrorCode);
}

void ConnectionFSM::OnDisconnectedEvent(int ErrorCode)
{
    B2NETWORK_LOG_DEBUG(logger, "%s Disconnected Event...", ToString().c_str());

    m_connectionManager.OnDisconnectedEvent(m_connection->GetServerType(), ErrorCode);
}

void ConnectionFSM::OnRecvEvent()
{
    B2NETWORK_LOG_DEBUG(logger, "%s Recv Event...", ToString().c_str());

    while (true)
    {
        auto message = m_connection->Recv();
        if (message)
        {
            m_connectionManager.OnRecvMessageEvent(message);
        }
        else
        {
            break;
        }
    };
}

void ConnectionFSM::OnSendEvent()
{
    //B2NETWORK_LOG_DEBUG(logger, "%s Send Event...", ToString().c_str());

    m_connectionManager.OnSendMessageEvent(m_connection);
}

void ConnectionFSM::OnErrorEvent(int ErrorCode)
{
    m_connectionManager.OnErrorEvent(m_connection->GetServerType(), ErrorCode);
}

void ConnectionFSM::InjectCipherKeyIntoConnection(const Key& key)
{
	m_connection->InjectCipherKeyIntoConnection(key);
}

std::string ConnectionFSM::ToString()
{
    std::ostringstream oss;
    oss << m_connection->GetServerTypeString()
        << "(" << m_endpoint.ToString() << ")"
        << "(" << m_current->GetState().c_str() << ")"
        << (m_stopped ? "stopped" : "");
    return oss.str();
}

// InitialState

void ConnectionFSM::InitialState::Enter(ConnectionFSM* fsm, ConnectionPtr connection, const Endpoint& endpoint)
{
}

void ConnectionFSM::InitialState::Leave(ConnectionFSM* fsm, ConnectionPtr connection)
{
}

ConnectionFSM::State*
ConnectionFSM::InitialState::Update(ConnectionFSM* fsm, ConnectionPtr connection, Connection::UpdateEvent events)
{
    return &s_connect;
}

// ConnectState

void ConnectionFSM::ConnectState::Enter(ConnectionFSM* fsm, ConnectionPtr connection, const Endpoint& endpoint)
{
    m_endpoint = endpoint;
}

void ConnectionFSM::ConnectState::Leave(ConnectionFSM* fsm, ConnectionPtr connection)
{
}

ConnectionFSM::State*
ConnectionFSM::ConnectState::Update(ConnectionFSM* fsm, ConnectionPtr connection, Connection::UpdateEvent events)
{
    auto result = connection->Connect(m_endpoint.getAddress(), m_endpoint.getPort());
    if (result == Connection::CONNECT_SUCCESS)
    {
        return &s_connected;
    }
    else if (result == Connection::CONNECT_ERROR)
    {
        return &s_connectFail;
    }
    else if (result == Connection::CONNECT_ERROR_DNS_FAILURE)
    {
        return &s_final;
    }
    else if (result == Connection::CONNECT_WAITING)
    {
        return &s_connecting;
    }
    else
    {
        B2NETWORK_LOG_ERROR(logger, "Unknown Result %d!", result);
        return &s_connectFail;
    }
}

// ConnectingState

void ConnectionFSM::ConnectingState::Enter(ConnectionFSM* fsm, ConnectionPtr connection, const Endpoint& endpoint)
{
    m_updated = std::chrono::steady_clock::now();
}

void ConnectionFSM::ConnectingState::Leave(ConnectionFSM* fsm, ConnectionPtr connection)
{
}

ConnectionFSM::State*
ConnectionFSM::ConnectingState::Update(ConnectionFSM* fsm, ConnectionPtr connection, Connection::UpdateEvent events)
{
    auto result = connection->WaitForConnect(events);
    if (result == Connection::CONNECT_SUCCESS)
    {
        return &s_connected;
    }
    else if (result == Connection::CONNECT_ERROR)
    {
        return &s_connectFail;
    }
    else if (result == Connection::CONNECT_ERROR_DNS_FAILURE)
    {
        return &s_final;
    }
    else if (result == Connection::CONNECT_WAITING)
    {
        const auto now = std::chrono::steady_clock::now();

        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - m_updated).count();
        if (elapsed > CONNECT_TIMEOUT_INIT)
        {
            return &s_connectFail;
        }

        return nullptr;
    }
    else 
    {
        B2NETWORK_LOG_ERROR(logger, "Unknown Result %d", result);
        return &s_connectFail;
    }
}

// ConnectFailState

void ConnectionFSM::ConnectFailState::Enter(ConnectionFSM* fsm, ConnectionPtr connection, const Endpoint& endpoint)
{
    m_once = false;
    m_updated = std::chrono::steady_clock::now();
}

void ConnectionFSM::ConnectFailState::Leave(ConnectionFSM* fsm, ConnectionPtr connection)
{
}

ConnectionFSM::State*
ConnectionFSM::ConnectFailState::Update(ConnectionFSM* fsm, ConnectionPtr connection, Connection::UpdateEvent events)
{
    // NOTE delayed event
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(m_updated - std::chrono::steady_clock::now());
    if (elapsed.count() > 2)
    {
        if (!m_once)
        {
            m_once = true;
            fsm->OnConnectFailEvent();
        }
    }

    return nullptr;
}

// ConnectedState

void ConnectionFSM::ConnectedState::Enter(ConnectionFSM* fsm, ConnectionPtr connection, const Endpoint& endpoint)
{
    fsm->OnConnectedEvent();
}

void ConnectionFSM::ConnectedState::Leave(ConnectionFSM* fsm, ConnectionPtr connection)
{
}

ConnectionFSM::State*
ConnectionFSM::ConnectedState::Update(ConnectionFSM* fsm, ConnectionPtr connection, Connection::UpdateEvent events)
{
    const auto result = connection->ProcessEvent(events);

    if (result == Connection::EVENT_NONE)
    {
        return nullptr;
    }
    else if (result == Connection::EVENT_RECV_SUCCESS)
    {
        fsm->OnRecvEvent();
        return nullptr;
    }
    else if (result == Connection::EVENT_RECV_NONE)
    {
        return nullptr;
    }
    else if (result == Connection::EVENT_RECV_CLOSED)
    {
        B2NETWORK_LOG_DEBUG(logger, "Closed Event");
        return &s_disconnected;
    }
    else if (result == Connection::EVENT_RECV_DISCONNECTED)
    {
        B2NETWORK_LOG_DEBUG(logger, "Disconnected Event");
		const auto serverType = connection->GetServerType();
		if (serverType == ServerType::SESSION_SERVER)
		{
			fsm->OnErrorEvent((int)ErrorCode::SessionRecv_Disconnected);
		}
		else if (serverType == ServerType::CHANNEL_SERVER)
		{
			fsm->OnDisconnectedEvent((int)ErrorCode::ChannelRecv_Disconnected);
		}
        return &s_disconnected;
    }
    else if (result == Connection::EVENT_RECV_ABORTED)
    {
        const auto serverType = connection->GetServerType();
        if (serverType == ServerType::SESSION_SERVER)
        {
            fsm->OnErrorEvent((int)ErrorCode::SessionRecv_Aborted);
        }
        else if (serverType == ServerType::CHANNEL_SERVER)
        {
            // NOTE Work around the bug that can't receive an channel disconnected event!
            fsm->OnDisconnectedEvent((int)ErrorCode::ChannelRecv_Aborted);
        }

        return &s_disconnected;
    }
    else if (result == Connection::EVENT_RECV_ERROR)
    {
        B2NETWORK_LOG_DEBUG(logger, "Error Event %d!", result);
		const auto serverType = connection->GetServerType();
		if (serverType == ServerType::SESSION_SERVER)
		{
			fsm->OnErrorEvent((int)ErrorCode::SessionRecv_Error);
		}
		else if (serverType == ServerType::CHANNEL_SERVER)
		{
			// NOTE Work around the bug that can't receive an channel disconnected event!
			fsm->OnDisconnectedEvent((int)ErrorCode::ChannelRecv_Error);
		}
        return &s_final;
    }
    else if (result == Connection::EVENT_SEND_READY)
    {
        fsm->OnSendEvent();
        return nullptr;
    }
    else
    {
        B2NETWORK_LOG_ERROR(logger, "Unknown Event %d!", result);
        return &s_final;
    }
}

// DisconnectedState

void ConnectionFSM::DisconnectedState::Enter(ConnectionFSM* fsm, ConnectionPtr connection, const Endpoint& endpoint)
{
    m_once = false;
    m_updated = std::chrono::steady_clock::now();

    // reset encryption keys
    connection->ExtractCipherKeyOutofConnection();
}

void ConnectionFSM::DisconnectedState::Leave(ConnectionFSM* fsm, ConnectionPtr connection)
{
}

ConnectionFSM::State*
ConnectionFSM::DisconnectedState::Update(ConnectionFSM* fsm, ConnectionPtr connection, Connection::UpdateEvent events)
{
    // NOTE delayed event
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(m_updated - std::chrono::steady_clock::now());
    if (elapsed.count() > 3)
    {
        if (!m_once)
        {
            m_once = true;
            fsm->OnDisconnectedEvent();
        }
    }

    return nullptr;
}

// FinalState

void ConnectionFSM::FinalState::Enter(ConnectionFSM* fsm, ConnectionPtr connection, const Endpoint& endpoint)
{
    connection->Disconnect("");
}

void ConnectionFSM::FinalState::Leave(ConnectionFSM* fsm, ConnectionPtr connection)
{
}

ConnectionFSM::State*
ConnectionFSM::FinalState::Update(ConnectionFSM* fsm, ConnectionPtr connection, Connection::UpdateEvent events)
{
    return nullptr;
}
