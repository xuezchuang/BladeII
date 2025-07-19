#pragma once

#include "Config.h"

#include "ReconnectPolicy.h"
#include "ByteBuffer.h"
#include "ProtobufCodec.h"
#include "Cryption.h"
#include "LeakyBucket.h"

namespace b2network {

class Connection : public std::enable_shared_from_this<Connection>
{
public:
    enum ServerType
    {
        SESSION_SERVER,
        CHANNEL_SERVER
    };

    enum UpdateEvent
    {
        NO_EVENT = 0,
        RD_EVENT = 1,
        WR_EVENT = 2,
        EX_EVENT = 4
    };

    enum ConnectResult
    {
        CONNECT_SUCCESS,
        CONNECT_ERROR,
        CONNECT_ERROR_DNS_FAILURE,
        CONNECT_WAITING,
    };

    enum ProcessResult
    {
        EVENT_NONE,

        // read
        EVENT_RECV_SUCCESS,
        EVENT_RECV_NONE,
        EVENT_RECV_CLOSED,
        EVENT_RECV_DISCONNECTED,
        EVENT_RECV_ABORTED,
        EVENT_RECV_ERROR,

        // send
        EVENT_SEND_READY,
    };

public:
    typedef ByteBuffer<1024, 4, 8 * 1024> RecvBuffer;

    typedef google::protobuf::Message Message;
    typedef std::shared_ptr<Message> MessagePtr;

    typedef std::shared_ptr<Connection> ConnectionPtr;

	typedef std::shared_ptr<ICrypto> CryptoPtr;

private:
    const ServerType m_type;

#if defined(B2NETWORK_PLATFORM_WINDOWS)
    SOCKET m_socket;
#elif defined(B2NETWORK_PLATFORM_ANDROID)
    int m_socket;
#elif defined(B2NETWORK_PLATFORM_IOS)
    int m_socket;
#else
#error Platform not supported
#endif

    std::string m_host;
    int m_port;

    std::string m_resolvedHost;

    ProtobufCodec m_codec;
    RecvBuffer m_recvBuffer;

    std::deque<MessagePtr> m_recvMessages;

    std::string m_info;

	CryptoPtr	m_crypto;

    LeakyBucket m_leakyBucket;

public:
    Connection(const ServerType type);
    virtual ~Connection();

    ServerType GetServerType() const;
    std::string GetServerTypeString() const;

#if defined(B2NETWORK_PLATFORM_WINDOWS)
    SOCKET GetSocket();
#elif defined(B2NETWORK_PLATFORM_ANDROID)
    int GetSocket();
#elif defined(B2NETWORK_PLATFORM_IOS)
    int GetSocket();
#else
#error Platform not supported
#endif

    ConnectResult Connect(const std::string& host, int port);
    ConnectResult WaitForConnect(const UpdateEvent events);

    ProcessResult ProcessEvent(const UpdateEvent events);

    MessagePtr Recv();

    bool Send(MessagePtr message);
    void Disconnect(const std::string reason);
    void Close();

	void InjectCipherKeyIntoConnection(const Key& key);
	void ExtractCipherKeyOutofConnection();

    std::string ToString();

protected:
    ProcessResult OnRecvEvent();
    void OnSendEvent();
    void OnExceptionEvent();

    void CloseConnection(const std::string reason);

    void OpenSocket(int family, int socktype, int protocol);
    void CloseSocket();
};

}
