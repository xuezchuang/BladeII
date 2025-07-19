#include "MessageRouter.h"
#include "B2NetworkPlugin.h"

#include "ErrorCode.h"
#include "LogManager.h"
#include "Exception.h"
#include "StringHash.h"
#include "ProtobufFactory.h"
#include "Connection.h"


using namespace b2network;

#if B2_BUILD_LEVEL < B2_BUILD_LEVEL_QA // !B2_BUILD_LEVEL_LIVE, !!B2_BUILD_LEVEL_QA
static auto logger = LogManager::GetLogger("MessageRouter");
#endif

MessageRouter::MessageRouter(ConnectionManager& connectionManager)
    :m_connectionManager(connectionManager),
     m_sessionReqManager(ProtobufFactory::Session),
     m_channelReqManager(ProtobufFactory::Channel)
{
}

void MessageRouter::Initialize(const Endpoint& sessionEndpoint)
{
    B2NETWORK_LOG_INFO(logger, "Initializing with Sesion Server (%s)...", sessionEndpoint.ToString().c_str());

    auto notifyCallback = [this](ServerType serverType, MessagePtr notify)
    {
        RouteNotify(serverType, notify);
    };

    // initialize request managers

    m_sessionReqManager.Initialize(notifyCallback);
    m_channelReqManager.Initialize(notifyCallback);

    // initialize endpoint table

    m_endpoints.clear();
    m_endpoints[ProtobufFactory::Session] = sessionEndpoint;

    // control routing rules

    m_controlRouteTable.clear();

    m_controlRouteTable.emplace(ProtobufFactory::Session,
        [this](MessagePtr request, const Endpoint& endpoint, ResponseCallback responseCallback)
        {
            m_sessionReqManager.PushControl(request, responseCallback);
            m_connectionManager.ActivateSessionConnection(endpoint);
        });

    m_controlRouteTable.emplace(ProtobufFactory::Channel,
        [this](MessagePtr request, const Endpoint& endpoint, ResponseCallback responseCallback)
        {
            m_channelReqManager.PushControl(request, responseCallback);
            m_connectionManager.ActivateChannelConnection(endpoint);
        });

    // request routing rules

    m_requestRouteTable.clear();

    m_requestRouteTable.emplace(ProtobufFactory::Session,
        [this](MessagePtr request, const Endpoint& endpoint, ResponseCallback responseCallback)
        {
            m_sessionReqManager.PushRequest(request, responseCallback);
            m_connectionManager.ActivateSessionConnection(endpoint);
        });

    m_requestRouteTable.emplace(ProtobufFactory::Channel,
        [this](MessagePtr request, const Endpoint& endpoint, ResponseCallback responseCallback)
        {
            m_channelReqManager.PushRequest(request, responseCallback);
            m_connectionManager.ActivateChannelConnection(endpoint);
        });

    // command routing rules

    m_commandRouteTable.clear();

    m_commandRouteTable.emplace(ProtobufFactory::Session,
        [this](MessagePtr command, const Endpoint& endpoint)
        {
            m_sessionReqManager.PushCommand(command);
            m_connectionManager.ActivateSessionConnection(endpoint);
        });

    m_commandRouteTable.emplace(ProtobufFactory::Channel,
        [this](MessagePtr command, const Endpoint& endpoint)
        {
            m_channelReqManager.PushCommand(command);
            m_connectionManager.ActivateChannelConnection(endpoint);
        });

    // initialize notify handler table

    m_notifyHandlerTable.clear();
}

void MessageRouter::Finalize()
{
    B2NETWORK_LOG_INFO(logger, "Finalizing...");

    m_notifyHandlerTable.clear();

    m_commandRouteTable.clear();
    m_requestRouteTable.clear();
    m_controlRouteTable.clear();

    m_endpoints.clear();

    m_channelReqManager.Finalize();
    m_sessionReqManager.Finalize();
}

void MessageRouter::Update()
{
    if (!m_sessionReqManager.IsEmpty())
    {
        auto& sessionEndpoint = m_endpoints[ProtobufFactory::Session];
        m_connectionManager.ActivateSessionConnection(sessionEndpoint);
    }

    m_sessionReqManager.Update();

    if (!m_channelReqManager.IsEmpty())
    {
        // TODO timeout
    }

    m_channelReqManager.Update();
}

bool MessageRouter::IsWaiting()
{
    if (m_sessionReqManager.IsEmpty() && m_channelReqManager.IsEmpty())
    {
        return false;
    }

    return true;
}

ProtobufFactory::ServerType MessageRouter::GetServerType(MessagePtr message)
{
    const auto metadata = ProtobufFactory::GetMetadata(message);
    return metadata.serverType;
}

bool MessageRouter::Configure(const std::string& optionName, bool value)
{
    bool result1 = m_sessionReqManager.Configure(optionName, value);
    bool result2 = m_channelReqManager.Configure(optionName, value);

    return (result1 && result2);
}

bool MessageRouter::SendControl(MessagePtr request, const Endpoint& endpoint, ResponseCallback callback)
{
    const auto metadata = ProtobufFactory::GetMetadata(request);

    if (endpoint.IsValid())
    {
        const auto it = m_controlRouteTable.find(metadata.serverType);
        if (it != m_controlRouteTable.end())
        {
            // update server endpoint
            m_endpoints[metadata.serverType] = endpoint;

            // route control message
            it->second(request, endpoint, callback);

            return true;
        }
    }

    B2NETWORK_LOG_ERROR(logger, "Could not send control(%s), type=%d, endpoint=%s",
        request->GetDescriptor()->full_name().c_str(), metadata.serverType, endpoint.ToString().c_str());

    return false;
}

bool MessageRouter::SendRequest(MessagePtr request, ResponseCallback callback)
{
    const auto metadata = ProtobufFactory::GetMetadata(request);

    Endpoint endpoint;

    const auto itEndpoint = m_endpoints.find(metadata.serverType);
    if (itEndpoint != m_endpoints.end())
    {
        endpoint = itEndpoint->second;
    }

    if (endpoint.IsValid())
    {
        const auto itRoute = m_requestRouteTable.find(metadata.serverType);
        if (itRoute != m_requestRouteTable.end())
        {
            itRoute->second(request, endpoint, callback);

            B2NETWORK_LOG_INFO(logger, "Routing... %s", request->GetDescriptor()->full_name().c_str());

            return true;
        }
    }

    B2NETWORK_LOG_ERROR(logger, "Could not send request(%s), type=%d, endpoint=%s",
        request->GetDescriptor()->full_name().c_str(), metadata.serverType, endpoint.ToString().c_str());

    return false;
}

int MessageRouter::SendCommand(MessagePtr command)
{
    const auto metadata = ProtobufFactory::GetMetadata(command);

    const auto itEndpoint = m_endpoints.find(metadata.serverType);
    if (itEndpoint != m_endpoints.end())
    {
        const auto endpoint = itEndpoint->second;

        const auto itRoute = m_commandRouteTable.find(metadata.serverType);
        if (itRoute != m_commandRouteTable.end())
        {
            itRoute->second(command, endpoint);

            return (int) ErrorCode::Success;
        }
        else
        {
            B2NETWORK_LOG_ERROR(logger, "Could not send command(%s), type=%d, endpoint=%s",
                command->GetDescriptor()->full_name().c_str(), metadata.serverType, endpoint.ToString().c_str());

            return (int) ErrorCode::CommandRouteNotFound;
        }
    }

    B2NETWORK_LOG_ERROR(logger, "Could not send command(%s), type=%d, no endpoint found!",
        command->GetDescriptor()->full_name().c_str(), metadata.serverType);

    return (int) ErrorCode::ServerEndpointNotFound;
}

void MessageRouter::Reset()
{
    B2NETWORK_LOG_INFO(logger, "Reset...");

    m_endpoints.clear();

    m_sessionReqManager.Reset();
    m_channelReqManager.Reset();
}

void MessageRouter::AttachNotifyHandler(std::string typeName, NotifyHandler handler)
{
    auto hash = StringHash::HashCode(typeName);
    if (m_notifyHandlerTable.find(hash) == m_notifyHandlerTable.end())
    {
        m_notifyHandlerTable[hash] = handler;
    }
    else
    {
        B2NETWORK_LOG_ERROR(logger, "Can't attach notify handler for %s, hash(%u) duplicated!", typeName.c_str(), hash);
    }
}

void MessageRouter::DetachNotifyHandler(std::string typeName)
{
    auto hash = StringHash::HashCode(typeName);
    m_notifyHandlerTable.erase(hash);
}

void MessageRouter::RouteNotify(ServerType serverType, MessagePtr notify)
{
    B2NETWORK_LOG_DEBUG(logger, "Routing notify message... %d %s", serverType, notify->GetTypeName().c_str());

    auto typeName = notify->GetTypeName();
    auto hash = StringHash::HashCode(typeName);

    auto it = m_notifyHandlerTable.find(hash);
    if (it != m_notifyHandlerTable.end())
    {
        it->second(notify);
    }
    else
    {
        B2NETWORK_LOG_DEBUG(logger, "Notify handler not found for %s(%d)", typeName.c_str(), hash);
    }
}

void MessageRouter::PauseSendMessage()
{
    B2NETWORK_LOG_DEBUG(logger, "Pausing request managers to send...");

    m_sessionReqManager.PauseSendMessage();
    m_channelReqManager.PauseSendMessage();
}

void MessageRouter::ResumeSendMessage()
{
    B2NETWORK_LOG_DEBUG(logger, "Resuming request managers to send...");

    m_sessionReqManager.ResumeSendMessage();
    m_channelReqManager.ResumeSendMessage();
}

Endpoint MessageRouter::GetChannelRoute() const
{
    auto it = m_endpoints.find(ProtobufFactory::Channel);
    if (it != m_endpoints.end())
    {
        return it->second;
    }
    else
    {
        // NOTE return an invalid endpoint
        return Endpoint();
    }
}

void MessageRouter::SetSessionRoute(const Endpoint& endpoint)
{
    B2NETWORK_LOG_DEBUG(logger, "Set session route %s...", endpoint.ToString().c_str());

    m_connectionManager.ActivateSessionConnection(endpoint);
}

void MessageRouter::ClearSessionControls()
{
    B2NETWORK_LOG_DEBUG(logger, "Clear session control queue...");

    m_sessionReqManager.ResetControls();
}

void MessageRouter::ClearSessionResponses()
{
    B2NETWORK_LOG_DEBUG(logger, "Clear session control queue...");

    m_sessionReqManager.ResetMessages();
}

void MessageRouter::SetChannelRoute(const Endpoint& endpoint)
{
    B2NETWORK_LOG_DEBUG(logger, "Set channel route %s...", endpoint.ToString().c_str());

    m_endpoints[ProtobufFactory::Channel] = endpoint;

    m_connectionManager.ActivateChannelConnection(endpoint);
}

void MessageRouter::ClearChannelRoute()
{
    B2NETWORK_LOG_DEBUG(logger, "Clear channel route..");

    // NOTE do not reset the channel request manager here

    auto endpoint = m_endpoints[ProtobufFactory::Channel];

    m_endpoints.erase(ProtobufFactory::Channel);

    if (endpoint.IsValid())
    {
        m_connectionManager.DeactivateChannelConnection(endpoint);
    }
}

void MessageRouter::ClearChannelRequests()
{
    B2NETWORK_LOG_DEBUG(logger, "Clear channel request queue...");

    m_channelReqManager.Reset();
}

void MessageRouter::CloseChannel()
{
    B2NETWORK_LOG_INFO(logger, "Closing channel...");

    m_endpoints.erase(ProtobufFactory::Channel);

    m_channelReqManager.Reset();

    m_connectionManager.CloseChannelConnection("");
}

void MessageRouter::OnConnectedEvent(Connection::ServerType serverType)
{
    B2NETWORK_LOG_DEBUG(logger, "ConnectedEvent from %d...", serverType);

    if (serverType == Connection::SESSION_SERVER)
    {
        m_sessionReqManager.OnConnectedEvent();
    }
    else if (serverType == Connection::CHANNEL_SERVER)
    {
        m_channelReqManager.OnConnectedEvent();
    }
    else
    {
#if defined(B2NETWORK_PLATFORM_WINDOWS)
        throw Exception("Invalid Server Type");
#elif defined(B2NETWORK_PLATFORM_ANDROID)
        B2NETWORK_LOG_ERROR(logger, "Invalid Server Type");
#elif defined(B2NETWORK_PLATFORM_IOS)
        B2NETWORK_LOG_ERROR(logger, "Invalid Server Type");
#else
#error Platformn not supported
#endif
    }
}

void MessageRouter::OnConnectFailEvent(ConnectionPtr connection)
{
    B2NETWORK_LOG_DEBUG(logger, "ConnectFailEvent from %d...", connection->GetServerType());

    auto serverType = connection->GetServerType();

    if (serverType == Connection::SESSION_SERVER)
    {
        m_sessionReqManager.OnConnectFailEvent();
    }
    else if (serverType == Connection::CHANNEL_SERVER)
    {
        m_channelReqManager.OnConnectFailEvent();
    }
    else
    {
#if defined(B2NETWORK_PLATFORM_WINDOWS)
        throw Exception("Invalid Server Type");
#elif defined(B2NETWORK_PLATFORM_ANDROID)
        B2NETWORK_LOG_ERROR(logger, "Invalid Server Type");
#elif defined(B2NETWORK_PLATFORM_IOS)
        B2NETWORK_LOG_ERROR(logger, "Invalid Server Type");
#else
#error Platformn not supported
#endif
    }
}

void MessageRouter::OnDisconnectedEvent(ConnectionPtr connection)
{
    B2NETWORK_LOG_DEBUG(logger, "DisconnectedEvent from %d...", connection->GetServerType());

    auto serverType = connection->GetServerType();

    if(serverType == Connection::SESSION_SERVER)
    {
        m_sessionReqManager.OnDisconnectedEvent();
    }
    else if (serverType == Connection::CHANNEL_SERVER)
    {
        m_channelReqManager.OnDisconnectedEvent();
    }
    else
    {
#if defined(B2NETWORK_PLATFORM_WINDOWS)
        throw Exception("Invalid Server Type");
#elif defined(B2NETWORK_PLATFORM_ANDROID)
        B2NETWORK_LOG_ERROR(logger, "Invalid Server Type");
#elif defined(B2NETWORK_PLATFORM_IOS)
        B2NETWORK_LOG_ERROR(logger, "Invalid Server Type");
#else
#error Platform not supported
#endif
    }
}

void MessageRouter::OnSendMessageEvent(ConnectionPtr connection)
{
    //B2NETWORK_LOG_DEBUG(logger, "SendMessageEvent from %d...", connection->GetServerType());

    auto serverType = connection->GetServerType();

    if (serverType == Connection::SESSION_SERVER)
    {
        m_sessionReqManager.OnSendEvent(connection);
    }
    else if (serverType == Connection::CHANNEL_SERVER)
    {
        m_channelReqManager.OnSendEvent(connection);
    }
    else
    {
#if defined(B2NETWORK_PLATFORM_WINDOWS)
        throw Exception("Invalid Server Type");
#elif defined(B2NETWORK_PLATFORM_ANDROID)
        B2NETWORK_LOG_ERROR(logger, "Invalid Server Type");
#elif defined(B2NETWORK_PLATFORM_IOS)
        B2NETWORK_LOG_ERROR(logger, "Invalid Server Type");
#else
#error Platform not supported
#endif
    }
}

void MessageRouter::OnRecvMessageEvent(MessagePtr message)
{
    B2NETWORK_LOG_DEBUG(logger, "RecvMessageEvent... %s", message->GetTypeName().c_str());

    auto metadata = ProtobufFactory::GetMetadata(message);

    if (metadata.serverType == ProtobufFactory::Session)
    {
        m_sessionReqManager.OnRecvEvent(message);
    }
    else if (metadata.serverType == ProtobufFactory::Channel)
    {
        m_channelReqManager.OnRecvEvent(message);
    }
    else
    {
#if defined(B2NETWORK_PLATFORM_WINDOWS)
        throw Exception("Invalid Server Type");
#elif defined(B2NETWORK_PLATFORM_ANDROID)
        B2NETWORK_LOG_ERROR(logger, "Invalid Server Type");
#elif defined(B2NETWORK_PLATFORM_IOS)
        B2NETWORK_LOG_ERROR(logger, "Invalid Server Type");
#else
#error Platform not supported
#endif
    }
}

void MessageRouter::OnErrorEvent()
{
    m_sessionReqManager.OnErrorEvent();
}

void MessageRouter::InjectCipherKeyIntoConnection(Connection::ServerType servertype, const Key& key)
{
	m_connectionManager.InjectCipherKeyIntoConnection(servertype, key);
}

std::string MessageRouter::Dump()
{
    std::ostringstream oss;
    oss << "SessionReqManager " << m_sessionReqManager.Dump() << "," << std::endl
        << "\t\tChannelReqManager " << m_channelReqManager.Dump();

    return oss.str();
}
