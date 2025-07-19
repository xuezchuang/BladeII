#pragma once

#include "Config.h"
#include "Endpoint.h"
#include "AuthenticationFSM.h"
#include "CredentialInfo.h"

namespace b2network {

class Connection;
class MessageRouter;
class EventDispatcher;
class ConnectionManager;
class AuthenticationFSM;
class Key;

class AuthenticationManager
{
private:
    typedef google::protobuf::Message Message;
    typedef std::shared_ptr<Message> MessagePtr;
    typedef std::shared_ptr<Connection> ConnectionPtr;
	typedef std::shared_ptr<Key> KeyPtr;

    MessageRouter& m_messageRouter;
    EventDispatcher& m_eventDispatcher;
    AuthenticationFSM m_fsm;

	CredentialInfo m_credentialInfo;
    Endpoint m_sessionEndpoint;

	int64 m_accountId;
    std::string m_nickname;
    std::string m_sessionToken;

    static const int MAX_RECONNECTING_COUNT = 7;

    int m_reconnectingCount;
	KeyPtr	m_cipherKeyForSession;
	KeyPtr	m_cipherKeyForChannel;

    int m_sessionPingSendSeq;
    int m_sessionPingRecvSeq;
    std::chrono::time_point<std::chrono::steady_clock> m_sessionPingRecvTime;
	int64 m_sessionServerTimeMs;

    bool m_roomBackgroundHeartbeat;
    std::chrono::time_point<std::chrono::steady_clock> m_roomHeartbeatSendTime;
    int m_roomHeartbeatSendSeq;
    int m_roomHeartbeatRecvSeq;
    std::chrono::time_point<std::chrono::steady_clock> m_roomHeartbeatRecvTime;

    std::string m_roomJoined;

	struct Version
	{
		int32 Major;
		int32 Minor;
		int32 Revision;
		int32 Changelist;
		int32 BuildNum;
	};

	Version m_ClientVersion;

public:
    static const int DEFAULT_SESSION_HEARTBEAT_PERIOD = 7; // second(s)
    static const int DEFAULT_CHANNEL_HEARTBEAT_PERIOD = 2; // second(s)

    static const int LOCAL_PING_TIMEOUT = (DEFAULT_SESSION_HEARTBEAT_PERIOD * 2) + 3;

	int m_sessionHeartbeatPeriod;	// second(s)
	int m_channelHeartbeatPeriod;	// second(s)

public:
	AuthenticationManager(const CredentialInfo& credentialInfo, const Endpoint& sessionEndpoint, MessageRouter& messageRouter, EventDispatcher& eventDispatcher);

    void Start();
    void Stop();
    void Update();

	const CredentialInfo& GetCredentialInfo() const;
	const Endpoint& GetSessionEndPoint() const;

    bool IsActivated() const;

    // session server
    void ConnectToSession();
	void HelloSession();
    void SignInSession();
    bool Ping();
    void ResetSession();
	int64 GetSessionServerTime() const;
    
    // channel server
	void HelloChannel();
    void SignInChannel();
    void LogOutChannel();
    void RoomHeartbeatForeground(const std::string& roomId);
    bool RoomHeartbeatBackground();
    void ResetRoomHeartbeat();

    enum Event
    {
        // session
        ServiceConnecting,
        ServiceConnected,
        ServiceNotAvailable,

        // channel
        ChannelRoomHearbeatTimeout,
        ChannelDisconnected,
    };

    void Emit(const Event event, int error = 0);

    //
    void ResetReconnectingCount();
    bool CheckReconnectingCount(int& backoff);

	//
	void InjectCipherKeyIntoSessionConnection();
	void InjectCipherKeyIntoChannelConnection();

	//
	int GetSessionHeartbeatPeriod() const { return m_sessionHeartbeatPeriod; }
	void SetHeartbeatPeriod(int sessionHeartbeat, int channelHeartbeat);

	int64 GetAccountID() { return m_accountId;}

private:
    bool RoomHeartbeatImpl(bool bFore);

public:
    std::string Dump();
};

}
