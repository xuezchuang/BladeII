#pragma once

#include "Config.h"
#include "MessageRouter.h"
#include "EventDispatcher.h"
#include "ConnectionManager.h"
#include "AuthenticationManager.h"
#include "ClientUpdater.h"

#if defined(B2NETWORK_ENVIRONMENT_TOOL)
#elif defined(B2NETWORK_ENVIRONMENT_GAME)
#include "B2MessageInfo.h"
#else
#error Environment not specified
#endif

namespace b2network {

class B2NETWORK_API Client : public std::enable_shared_from_this<Client>
{
public:
    typedef google::protobuf::Message Message;
    typedef std::shared_ptr<Message> MessagePtr;
    typedef std::function<void(int)> EventHandler;
    typedef std::function<void(MessagePtr)> ResponseCallback;
    typedef std::function<void(FB2MessageInfoPtr)> NotifyCallback;
    typedef std::function<void(int)> ResultCallback;

	enum Event
	{
		// service (session server)
		ServiceConnecting = 0,
		ServiceConnected = 1,
		ServiceNotAvailable = 2,
		ServerTimeSync = 3,

		// channel
		ChannelDisconnected = 5,

		// require
		RequireGiveNickname = 10,
		RequireUpdateProtocol = 11,
        RequireUpdateVersion = 12,

		// etc
		RequestTimeout = 20,
		RequestTryAgain = 21,
	};
	    
    static const char* GetEventString(const Event e);

private:
    typedef std::unordered_map<int, EventHandler> EventHandlerMap;

    bool m_enabled;

    MessageRouter m_messageRouter;
    EventDispatcher m_eventDispatcher;
    ConnectionManager m_connectionManager;

    AuthenticationManager m_authManager;

    EventHandlerMap m_eventHandlers;

    std::chrono::time_point<std::chrono::steady_clock> m_lastAuthUpdate;
    std::chrono::time_point<std::chrono::steady_clock> m_lastConnUpdate;

    using ResponseQueue = std::deque<std::pair<ResponseCallback, MessagePtr>>;
    ResponseQueue m_responseQueue;

    using NotifyQueue = std::deque<std::tuple<NotifyCallback, std::string, std::string, MessagePtr>>;
    NotifyQueue m_notifyQueue;

    using EventQueue = std::deque<std::tuple<Event, int, EventHandler>>;
    EventQueue m_eventQueue;

    bool m_exit;

    ClientUpdater m_updater;

    mutable FCriticalSection m_lock;

public:
    Client(const CredentialInfo& credentialInfo, const Endpoint& sessionServer);
    virtual ~Client();

    // credential info
    //

    const CredentialInfo& GetCredentialInfo() const;

    // start and stop
    //

	void Start();
    void Stop();

    // channel
    //

    void ChannelHeartbeat(const std::string& id);
    void CloseChannel(const std::string& reason);

    // status
    //

    bool IsServiceAvailable() const;

    // configuration
    //

    bool Configure(const std::string& optionName, bool value);

    // event handling
    //

    void AddEventHandler(const Event event, EventHandler handler);
    void RemoveEventHandler(const Event event);

    // message sending
    //

#if defined(B2NETWORK_ENVIRONMENT_TOOL)
#elif defined(B2NETWORK_ENVIRONMENT_GAME)
    bool SendB2Message(FB2MessageInfoPtr messageInfo);
#else
#error Environment not specified
#endif

    void SendRequest(const MessagePtr message, ResponseCallback callback);
    void SendCommand(const MessagePtr command);

    // notification handling
    //

	void AttachNotifyListener(std::string typeName, std::string messageType, NotifyCallback callback);
    void DetachNotifyListener(std::string typeName);

    // debugging support
    //

    void Dump();
	void ForceDisconnect();
	bool IsChannelDisconnected() const;
	void SetHeartbeatPeriod(int sessionHeartbeat, int channelHeartbeat);

    // update (tick)
    //

    void MainThreadUpdate();
    void WorkThreadUpdate();

	int64 GetAccountID() { return m_authManager.GetAccountID(); }

	// etc
	int64 GetSessionServerTime() const;

private:
    void InvokeEventHandler(const Event event, int errorCode = 0);
};

}
