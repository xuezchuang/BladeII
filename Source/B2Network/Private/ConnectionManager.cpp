#include "ConnectionManager.h"
#include "B2NetworkPlugin.h"

#include "Config.h"
#include "LogManager.h"
#include "Exception.h"
#include "MessageRouter.h"
#include "EventDispatcher.h"
#include "ConnectionFSM.h"


using namespace b2network;

#if B2_BUILD_LEVEL < B2_BUILD_LEVEL_QA // !B2_BUILD_LEVEL_LIVE, !!B2_BUILD_LEVEL_QA
static auto logger = LogManager::GetLogger("ConnectionManager");
#endif

ConnectionManager::ConnectionManager(MessageRouter& messageRouter,
                                     EventDispatcher& eventDispatcher)
    :m_messageRouter(messageRouter),
     m_eventDispatcher(eventDispatcher)
{
}

void ConnectionManager::Start(const Endpoint& sessionEndpoint)
{
    B2NETWORK_LOG_DEBUG(logger, "Starting... Session Server Endpoint(%s:%d)", sessionEndpoint.getAddress().c_str(), sessionEndpoint.getPort());

    ActivateSessionConnection(sessionEndpoint);
}

void ConnectionManager::Stop()
{
    B2NETWORK_LOG_DEBUG(logger, "Stop...");

    std::vector<ConnectionFSMPtr> stateMachines{m_sessionFSM, m_channelFSM};
    for (auto fsm : stateMachines)
    {
        if (fsm)
        {
            fsm->Stop();
        }
    }

    if (m_channelFSM)
    {
        m_channelFSM.reset();
    }

    if (m_sessionFSM)
    {
        m_sessionFSM.reset();
    }
}

void ConnectionManager::Update()
{
    std::vector<ConnectionFSMPtr> stateMachines{m_sessionFSM, m_channelFSM};

    fd_set readfds, writefds, exceptfds;

    FD_ZERO(&readfds);
    FD_ZERO(&writefds);
    FD_ZERO(&exceptfds);

    int nfds = 0;

    for (auto fsm : stateMachines)
    {
        if (!fsm)
        {
            continue;
        }
        else
        {
            auto connection = fsm->GetConnection();

#if defined(B2NETWORK_PLATFORM_WINDOWS)
            auto socket = connection->GetSocket();
            if (socket != INVALID_SOCKET)
            {
                FD_SET(socket, &readfds);
                FD_SET(socket, &writefds);
                FD_SET(socket, &exceptfds);

                nfds++;
            }
#elif defined(B2NETWORK_PLATFORM_ANDROID)
            auto fd = connection->GetSocket();
            if (fd > 0)
            {
                if (fd >= FD_SETSIZE)
                {
                    B2NETWORK_LOG_ERROR(logger, "Socket Exceeds Limit (%d)", FD_SETSIZE);
                    return;
                }

                FD_SET(fd, &readfds);
                FD_SET(fd, &writefds);
                FD_SET(fd, &exceptfds);

                if (fd >= nfds)
                {
                    nfds = fd + 1;
                }
            }
#elif defined(B2NETWORK_PLATFORM_IOS)
            auto fd = connection->GetSocket();
            if (fd > 0)
            {
                if (fd >= FD_SETSIZE)
                {
                    B2NETWORK_LOG_ERROR(logger, "Socket Exceeds Limit (%d)", FD_SETSIZE);
                    return;
                }

                FD_SET(fd, &readfds);
                FD_SET(fd, &writefds);
                FD_SET(fd, &exceptfds);

                if (fd >= nfds)
                {
                    nfds = fd + 1;
                }
            }
#else
#error Platform not supported
#endif
        }
    }

    if (nfds > 0)
    {
        struct timeval timeout = { 0, 1 };
        int result = select(nfds, &readfds, &writefds, &exceptfds, &timeout);
        if (result > 0)
        {
            // ok
        }
        else if (result == 0)
        {
            // timeout
        }
        else
        {
            // error
#if defined(B2NETWORK_PLATFORM_WINDOWS)
            int ec = WSAGetLastError();
#elif defined(B2NETWORK_PLATFORM_ANDROID)
            int ec = errno;
#elif defined(B2NETWORK_PLATFORM_IOS)
            int ec = errno;
#else
#error Platform not supported
#endif

            B2NETWORK_LOG_ERROR(logger, "Can't select, nfds=%d, result=%d, error=%d", nfds, result, ec);
        }
    }
    else
    {
        //B2NETWORK_LOG_INFO(logger, "No socket to select");
    }

    for (auto fsm : stateMachines)
    {
        if (!fsm)
        {
            continue;
        }
        else
        {
            auto connection = fsm->GetConnection();

#if defined(B2NETWORK_PLATFORM_WINDOWS)
            SOCKET socket = connection->GetSocket();
#elif defined(B2NETWORK_PLATFORM_ANDROID)
            int socket = connection->GetSocket();
#elif defined(B2NETWORK_PLATFORM_IOS)
            int socket = connection->GetSocket();
#else
#error Platform not supported
#endif

            Connection::UpdateEvent events = Connection::NO_EVENT;

#if defined(B2NETWORK_PLATFORM_WINDOWS)
            if (socket != INVALID_SOCKET)
            {
#elif defined(B2NETWORK_PLATFORM_ANDROID)
            if (socket > 0)
            {
#elif defined(B2NETWORK_PLATFORM_IOS)
            if (socket > 0)
            {
#else
#error Platform not supported
#endif
                if (FD_ISSET(socket, &readfds) != 0)
                {
                    events = (Connection::UpdateEvent)(events | Connection::RD_EVENT);
                }

                if (FD_ISSET(socket, &writefds) != 0)
                {
                    events = (Connection::UpdateEvent)(events | Connection::WR_EVENT);
                }

                if (FD_ISSET(socket, &exceptfds) != 0)
                {
                    events = (Connection::UpdateEvent)(events | Connection::EX_EVENT);
                }
            }

            if (fsm->IsStopped())
            {
                // ignore if stopped
            }
            else
            {
                // update
                fsm->Update(events);
            }
        }
    }
}

void ConnectionManager::ActivateSessionConnection(const Endpoint& sessionEndpoint)
{
    if (!sessionEndpoint.IsValid())
    {
        B2NETWORK_LOG_DEBUG(logger, "Could not Activate session due to invalid endpoint(%s:%d)!",
            sessionEndpoint.getAddress().c_str(), sessionEndpoint.getPort());

        // TODO fix

        return;
    }

    if (m_sessionFSM)
    {
        if (m_sessionFSM->GetEndpoint() != sessionEndpoint)
        {
            B2NETWORK_LOG_DEBUG(logger, "Session endpoint has changed! (%s)->(%s)",
                m_channelFSM->GetEndpoint().ToString().c_str(), sessionEndpoint.ToString().c_str());

            m_sessionFSM->Stop();
            m_sessionFSM.reset();
        }
    }
    
    if (!m_sessionFSM)
    {
        m_sessionFSM = std::make_shared<ConnectionFSM>(*this, Connection::SESSION_SERVER, sessionEndpoint);

        B2NETWORK_LOG_DEBUG(logger, "Starting session connection... <%s>(%s)",
            m_sessionFSM->GetState().c_str(), sessionEndpoint.ToString().c_str());

        m_sessionFSM->Start();
    }
    else if (m_sessionFSM->IsDisconnected())
    {
        B2NETWORK_LOG_DEBUG(logger, "Reconnecting session connection... <%s>(%s)",
            m_sessionFSM->GetState().c_str(), sessionEndpoint.ToString().c_str());

        // reconnect
        m_sessionFSM->Start();
    }
    else if (m_sessionFSM->IsConnectFail())
    {
        B2NETWORK_LOG_DEBUG(logger, "Reconnecting session connection... <%s>(%s)",
            m_sessionFSM->GetState().c_str(), sessionEndpoint.ToString().c_str());

        // reconnect
        m_sessionFSM->Start();
    }
    else
    {
//      B2NETWORK_LOG_DEBUG(logger, "Updating session connection... <%s>(%s)",
//          m_sessionFSM->GetState().c_str(), sessionEndpoint.ToString().c_str());

        m_sessionFSM->Update(Connection::NO_EVENT);
    }
}

void ConnectionManager::ActivateChannelConnection(const Endpoint& channelEndpoint)
{
    if (!channelEndpoint.IsValid())
    {
        // TODO fix
        B2NETWORK_LOG_DEBUG(logger, "Could not activate channel due to invalid endpoint(%s)!", channelEndpoint.ToString().c_str());

        return;
    }

    if (m_channelFSM)
    {
        if (m_channelFSM->GetEndpoint() != channelEndpoint)
        {
            B2NETWORK_LOG_DEBUG(logger, "Channel endpoint has changed! (%s)->(%s)",
                m_channelFSM->GetEndpoint().ToString().c_str(), channelEndpoint.ToString().c_str());

            m_channelFSM->Stop();
            m_channelFSM.reset();
        }
    }

    if (!m_channelFSM)
    {
        m_channelFSM = std::make_shared<ConnectionFSM>(*this, Connection::CHANNEL_SERVER, channelEndpoint);

        m_channelFSM->Start();
    }
    else if (m_channelFSM->IsDisconnected())
    {
        B2NETWORK_LOG_DEBUG(logger, "Reconnecting channel connection... (%s) (%s)",
            m_channelFSM->GetState().c_str(), channelEndpoint.ToString().c_str());

        // reconnect
        m_channelFSM->Start();
    }
    else if (m_channelFSM->IsConnectFail())
    {
        B2NETWORK_LOG_DEBUG(logger, "Reconnecting channel connection... (%s) (%s)",
            m_channelFSM->GetState().c_str(), channelEndpoint.ToString().c_str());

        // reconnect
        m_channelFSM->Start();
    }
    else
    {
//      B2NETWORK_LOG_DEBUG(logger, "Updating channel connection... (%s) (%s)",
//          m_channelFSM->GetState().c_str(), m_channelEndpoint.ToString().c_str());

        m_channelFSM->Update(Connection::NO_EVENT);
    }
}

void ConnectionManager::DeactivateChannelConnection(const Endpoint& channelEndpoint)
{
    if (m_channelFSM)
    {
        if (m_channelFSM->GetEndpoint() != channelEndpoint)
        {
            return;
        }

        B2NETWORK_LOG_INFO(logger, "Deactivating channel connection... (%s) (%s)",
            m_channelFSM->GetState().c_str(), channelEndpoint.ToString().c_str());

        m_channelFSM->Stop();
        m_channelFSM.reset();
    }
}

void ConnectionManager::CloseChannelConnection(const std::string& reason)
{
    if (m_channelFSM)
    {
        B2NETWORK_LOG_INFO(logger, "Closing channel connection... (%s) (%s)", m_channelFSM->GetState().c_str(), reason.c_str());

        m_channelFSM->Stop();
        m_channelFSM.reset();
    }
}

bool ConnectionManager::IsChannelDisconnected() const
{
	if (m_channelFSM)
	{
		return m_channelFSM->IsDisconnected();
	}

	return true;
}

void ConnectionManager::OnConnectedEvent(Connection::ServerType serverType)
{
    m_eventDispatcher.OnConnectedEvent(serverType);
    m_messageRouter.OnConnectedEvent(serverType);
}

void ConnectionManager::OnConnectFailEvent(Connection::ServerType serverType, int errorCode /*= 0*/)
{
    m_eventDispatcher.OnConnectFailEvent(serverType, errorCode);

    // TODO fix
    //m_messageRouter.OnConnectFailEvent(serverType);
}

void ConnectionManager::OnErrorEvent(Connection::ServerType serverType, int errorCode /*= 0*/)
{
    if (serverType == Connection::SESSION_SERVER)
    {
        m_messageRouter.OnErrorEvent();
    }
}

void ConnectionManager::OnDisconnectedEvent(Connection::ServerType serverType, int errorCode /*= 0*/)
{
    m_eventDispatcher.OnDisconnectedEvent(serverType, errorCode);

    // TODO fix
    //m_messageRouter.OnDisconnectedEvent(serverType);
}

void ConnectionManager::OnSendMessageEvent(ConnectionPtr connection)
{
    m_messageRouter.OnSendMessageEvent(connection);
}

void ConnectionManager::OnRecvMessageEvent(MessagePtr message)
{
    // NOTE keep this sequence

    m_messageRouter.OnRecvMessageEvent(message);

    m_eventDispatcher.OnResponseEvent(message);

    m_eventDispatcher.OnNotifyEvent(message);
}

void ConnectionManager::InjectCipherKeyIntoConnection(Connection::ServerType serverType, const Key& key)
{
	switch (serverType)
	{
	case Connection::SESSION_SERVER:
        if (m_sessionFSM)
        {
            m_sessionFSM->InjectCipherKeyIntoConnection(key);
        }
		break;

	case Connection::CHANNEL_SERVER:
        if (m_channelFSM)
        {
            m_channelFSM->InjectCipherKeyIntoConnection(key);
        }
		break;

    default:
        break;
	}
}

std::string ConnectionManager::Dump()
{
    std::ostringstream oss;
    oss << "Session(" << (m_sessionFSM ? m_sessionFSM->ToString() : "") << ")"
        << "\t\tChannel(" << (m_channelFSM ? m_channelFSM->ToString() : "") << ")";

    return oss.str();
}
