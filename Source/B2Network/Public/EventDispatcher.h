#pragma once

#include "Config.h"
#include "Connection.h"

namespace b2network {

class EventDispatcher
{
public:
    enum Event
    {
        // service
        ServiceConnecting,
        ServiceConnected,
        ServiceNotAvailable,
		ServerTimeSync,

        // require
        RequireUpdateVersion,
        RequireUpdateProtocol,
        RequireGiveNickname,

        // channel
        ChannelRoomHeartbeatTimeout,
        ChannelDisconnected,
    };

    typedef std::function<void(Event, int)> EventCallback;

private:
    typedef std::function<void(Connection::ServerType, int errorCode)> ConnectionCallback;
    typedef std::list<ConnectionCallback> ConnectionCallbackList;

    typedef google::protobuf::Message Message;
    typedef std::shared_ptr<Message> MessagePtr;
    typedef std::function<void(MessagePtr)> MessageFilter;
    typedef std::list<MessageFilter> MessageFilterList;

    bool m_enabled;

    ConnectionCallbackList m_connectedCallbacks;
    ConnectionCallbackList m_connectFailCallbacks;
    ConnectionCallbackList m_disconnectedCallbacks;

    // TODO
    MessageFilterList m_requestFilters;
    MessageFilterList m_responseFilters;
    MessageFilterList m_notifyFilters;

    typedef std::unordered_map<int32, EventCallback> EventCallbackMap;
    EventCallbackMap m_eventCallbacks;

public:
    EventDispatcher();

    void Start();
    void Stop();

    void AddConnectedCallback(ConnectionCallback callback);
    void AddConnectFailCallback(ConnectionCallback callback);
    void AddDisconnectedCallback(ConnectionCallback callback);

    void OnConnectedEvent(Connection::ServerType serverType);
    void OnConnectFailEvent(Connection::ServerType serverType, int errorCode);
    void OnDisconnectedEvent(Connection::ServerType serverType, int errorCode);

    void AddRequestFilter(MessageFilter filter);
    void AddResponseFilter(MessageFilter filter);
    void AddNotifyFilter(MessageFilter filter);

    void OnRequestEvent(MessagePtr message);
    void OnResponseEvent(MessagePtr message);
    void OnNotifyEvent(MessagePtr message);

public:
    void AddEventCallback(const Event event, EventCallback callback);
    void Emit(const Event event, int errorCode = 0);
};

}
