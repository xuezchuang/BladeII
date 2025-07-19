#pragma once

#include "Config.h"
#include "RequestManager.h"

namespace b2network {

class Endpoint;
class Connection;

class MessageRouter
{
private:
    typedef google::protobuf::Message Message;
    typedef std::shared_ptr<Message> MessagePtr;
    typedef std::shared_ptr<Connection> ConnectionPtr;
    typedef std::function<void(MessagePtr)> ResponseCallback;
    typedef ProtobufFactory::ServerType ServerType;

    typedef std::function<void(MessagePtr, const Endpoint&, ResponseCallback)> ControlRouter;
    typedef std::function<void(MessagePtr, const Endpoint&, ResponseCallback)> RequestRouter;
    typedef std::function<void(MessagePtr, const Endpoint&)> CommandRouter;

    typedef std::unordered_map<ServerType, Endpoint> EndpointTable;

    typedef std::unordered_map<ServerType, ControlRouter> ControlRouteTable;
    typedef std::unordered_map<ServerType, RequestRouter> RequestRouteTable;
    typedef std::unordered_map<ServerType, CommandRouter> CommandRouteTable;

    typedef std::function<void(MessagePtr)> NotifyHandler;
    typedef std::map<uint32, NotifyHandler> NotifyHandlerTable;

    ConnectionManager& m_connectionManager;

    RequestManager m_sessionReqManager;
    RequestManager m_channelReqManager;
    EndpointTable m_endpoints;

    ControlRouteTable m_controlRouteTable;
    RequestRouteTable m_requestRouteTable;
    CommandRouteTable m_commandRouteTable;

    NotifyHandlerTable m_notifyHandlerTable;

public:
    MessageRouter(ConnectionManager& connectionManager);

    void Initialize(const Endpoint& sessionEndpoint);
    void Finalize();

    void Update();

    //

    bool IsWaiting();

    ServerType GetServerType(MessagePtr message);

    // configure

    bool Configure(const std::string& optionName, bool value);

    // send

    bool SendControl(MessagePtr request, const Endpoint& endpoint, ResponseCallback callback);
    bool SendRequest(MessagePtr request, ResponseCallback callback);
    int SendCommand(MessagePtr command);

    void Reset();

    // notification

    void AttachNotifyHandler(std::string typeName, NotifyHandler handler);
    void DetachNotifyHandler(std::string typeName);

    void RouteNotify(ServerType serverType, MessagePtr notify);

    // message control

    void PauseSendMessage();
    void ResumeSendMessage();

    // session routing

    void SetSessionRoute(const Endpoint& endpoint);
    void ClearSessionControls();
    void ClearSessionResponses();

    // channel routing

    Endpoint GetChannelRoute() const;
    void SetChannelRoute(const Endpoint& endpoint);
    void ClearChannelRoute();
    void ClearChannelRequests();
    void CloseChannel();

    // event handler

    void OnConnectedEvent(Connection::ServerType serverType);
    void OnConnectFailEvent(ConnectionPtr connection);
    void OnDisconnectedEvent(ConnectionPtr connection);
    void OnSendMessageEvent(ConnectionPtr connection);
    void OnRecvMessageEvent(MessagePtr message);
    void OnErrorEvent();

	// inject cipher_key into connection

	void InjectCipherKeyIntoConnection(Connection::ServerType, const Key&);

    // dump

    std::string Dump();
};

}
