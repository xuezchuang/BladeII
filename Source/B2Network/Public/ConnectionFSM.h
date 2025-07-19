#pragma once

#include "Config.h"
#include "Connection.h"
#include "Endpoint.h"

#define DECLARE_CONNECTION_STATE(className) \
    class className : public State \
    { \
        virtual void Enter(ConnectionFSM* fsm, ConnectionPtr connection, const Endpoint& endpoint) final; \
        virtual void Leave(ConnectionFSM* fsm, ConnectionPtr connection) final; \
        virtual State* Update(ConnectionFSM* fsm, ConnectionPtr connection, Connection::UpdateEvent events) final; \
        virtual std::string GetState() final { return #className; } \
    }

namespace b2network {

class ConnectionManager;
class Endpoint;

class ConnectionFSM
{
    static const int CONNECT_TIMEOUT_INIT = 3; // secons

    typedef google::protobuf::Message Message;
    typedef std::shared_ptr<Message> MessagePtr;
    typedef std::shared_ptr<Connection> ConnectionPtr;
    typedef Connection::ServerType ServerType;

    class State 
    {
    protected:
        // ConnectState
        Endpoint m_endpoint;

        // ConnectingState
        //bool m_connected;

        // ConnectFailState, DisconnectedState
        bool m_once;

        std::chrono::time_point<std::chrono::steady_clock> m_updated;

    public:
		virtual ~State() {}
        virtual void Enter(ConnectionFSM* fsm, ConnectionPtr connection, const Endpoint& endpoint) = 0;
        virtual void Leave(ConnectionFSM* fsm, ConnectionPtr connection) = 0;
        virtual State* Update(ConnectionFSM* fsm, ConnectionPtr connection, Connection::UpdateEvent events) = 0;
        virtual std::string GetState() = 0;
    };

    DECLARE_CONNECTION_STATE(InitialState);
    DECLARE_CONNECTION_STATE(ConnectState);
    DECLARE_CONNECTION_STATE(ConnectingState);
    DECLARE_CONNECTION_STATE(ConnectFailState);
    DECLARE_CONNECTION_STATE(ConnectedState);
    DECLARE_CONNECTION_STATE(DisconnectedState);
    DECLARE_CONNECTION_STATE(FinalState);

    static InitialState s_initial;
    static ConnectState s_connect;
    static ConnectingState s_connecting;
    static ConnectFailState s_connectFail;
    static ConnectedState s_connected;
    static DisconnectedState s_disconnected;
    static FinalState s_final;

    ConnectionManager& m_connectionManager;
    const Endpoint m_endpoint;

    State* m_current;
    ConnectionPtr m_connection;

    bool m_stopped;

public:
    ConnectionFSM(ConnectionManager& connectionManager, const ServerType serverType, const Endpoint& endpoint);
    ~ConnectionFSM();

    void Start();
    void Stop();
    void Update(const Connection::UpdateEvent events);

    const Endpoint& GetEndpoint()
    {
        return m_endpoint;  
    }

    ConnectionPtr GetConnection()
    { 
        return m_connection; 
    }

    std::string GetState();

    bool IsDisconnected() const
    {
        return (m_current == &s_disconnected);
    }

    bool IsConnectFail() const
    {
        return (m_current == &s_connectFail);
    }

    bool IsStopped() const
    {
        return (m_current == &s_final);
    }

	void InjectCipherKeyIntoConnection(const Key& key);

    std::string ToString();

protected:
    void OnConnectedEvent();
    void OnConnectFailEvent(int ErrorCode = 0);
    void OnDisconnectedEvent(int ErrorCode = 0);
    void OnRecvEvent();
    void OnSendEvent();
    void OnErrorEvent(int ErrorCode = 0);
};

}
