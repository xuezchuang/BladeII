#pragma once

#include "Config.h"
#include "Endpoint.h"
#include "Connection.h"

namespace b2network {

class MessageRouter;
class EventDispatcher;
class Connection;
class ConnectionFSM;

class ConnectionManager
{
    typedef google::protobuf::Message Message;
    typedef std::shared_ptr<Message> MessagePtr;
    typedef std::shared_ptr<ConnectionFSM> ConnectionFSMPtr;
    typedef std::function<void(MessagePtr)> ResponseCallback;
    typedef std::shared_ptr<Connection> ConnectionPtr;

    MessageRouter& m_messageRouter;
    EventDispatcher& m_eventDispatcher;

    ConnectionFSMPtr m_sessionFSM;
    ConnectionFSMPtr m_channelFSM;

public:
    ConnectionManager(MessageRouter& messageRouter, EventDispatcher& eventDispatcher);

    void Start(const Endpoint& sessionEndpoint);
    void Stop();

    void Update();

    // session
    void ActivateSessionConnection(const Endpoint& sessionEndpoint);

    // channel
    void ActivateChannelConnection(const Endpoint& channelEndpoint);
    void DeactivateChannelConnection(const Endpoint& channelEndpoint);
    void CloseChannelConnection(const std::string& reason);
	bool IsChannelDisconnected() const;

    // connection events
    void OnConnectedEvent(Connection::ServerType serverType);
    void OnConnectFailEvent(Connection::ServerType serverType, int errorCode = 0);
    void OnErrorEvent(Connection::ServerType serverType, int errorCode = 0);
    void OnDisconnectedEvent(Connection::ServerType serverType, int errorCode = 0);

    // send events
    void OnSendMessageEvent(ConnectionPtr connection);

    // recv events
    void OnRecvMessageEvent(MessagePtr message);

	// 
	void InjectCipherKeyIntoConnection(Connection::ServerType, const Key&);

    std::string Dump();
};

}
