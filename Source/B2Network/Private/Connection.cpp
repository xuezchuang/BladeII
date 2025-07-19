#include "Connection.h"
#include "B2NetworkPlugin.h"

#include "Config.h"
#include "Exception.h"
#include "LogManager.h"


using namespace b2network;

#if B2_BUILD_LEVEL < B2_BUILD_LEVEL_QA // !B2_BUILD_LEVEL_LIVE, !!B2_BUILD_LEVEL_QA
static auto logger = LogManager::GetLogger("ConnectionSVR");
#endif

Connection::Connection(ServerType type)
    :m_type(type),
#if defined(B2NETWORK_PLATFORM_WINDOWS)
     m_socket(INVALID_SOCKET)
#elif defined(B2NETWORK_PLATFORM_ANDROID)
     m_socket(-1)
#elif defined(B2NETWORK_PLATFORM_IOS)
     m_socket(-1)
#else
#error Platform not supported
#endif
{
    m_host = "";
    m_port = 0;

    std::ostringstream oss;
    switch (m_type)
    {
    case SESSION_SERVER:        oss << "Session";        break;
    case CHANNEL_SERVER:        oss << "Channel";        break;
    default:                    oss << "Unknown";        break;
    }

    m_info = oss.str();

	m_crypto = std::make_shared<Dummy>();

    if (m_type == SESSION_SERVER)
    {
        // NOTE apply traffic control to session connection only!
        // limit 100 message per one second
        m_leakyBucket.Init(100, 1000);
    }
}

Connection::~Connection()
{
    CloseSocket();
}

Connection::ServerType Connection::GetServerType() const
{
    return m_type;
}

std::string Connection::GetServerTypeString() const
{
    switch (m_type)
    {
    case SESSION_SERVER:
        return "Session";
        break;

    case CHANNEL_SERVER:
        return "Channel";
        break;

    default:
        return "Unknown";
        break;
    }
}

#if defined(B2NETWORK_PLATFORM_WINDOWS)
SOCKET Connection::GetSocket()
#elif defined(B2NETWORK_PLATFORM_ANDROID)
int Connection::GetSocket()
#elif defined(B2NETWORK_PLATFORM_IOS)
int Connection::GetSocket()
#else
#error Platform not supported
#endif
{
    return m_socket;
}

Connection::ConnectResult Connection::Connect(const std::string& host, int port)
{
    m_host = host;
    m_port = port;

    const std::string portString = TCHAR_TO_ANSI(*FString::FromInt(m_port));

    B2NETWORK_LOG_INFO(logger, "%s Resolving %s:%d...", m_info.c_str(), m_host.c_str(), m_port);

    addrinfo hint;
    FMemory::Memzero(&hint, sizeof(hint));
    hint.ai_family = AF_UNSPEC;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_protocol = IPPROTO_TCP;

    addrinfo* addrList = NULL;
    struct addrinfo* resolved = NULL;

    int errorCode = getaddrinfo(host.c_str(), portString.c_str(), &hint, &addrList);
    if (errorCode == 0)
    {
        for (struct addrinfo* addrInfo = addrList; addrInfo != NULL; addrInfo = addrInfo->ai_next)
        {
            void* address = NULL;

            switch (addrInfo->ai_family)
            {
            case AF_INET:
                resolved = addrInfo;
                address = &((struct sockaddr_in*) addrInfo->ai_addr)->sin_addr;
                break;

            case AF_INET6:
                resolved = addrInfo;
                address = &((struct sockaddr_in6*) addrInfo->ai_addr)->sin6_addr;
                break;

            default:
                B2NETWORK_LOG_ERROR(logger, "Unknown type address %s:%d (%d)", m_host.c_str(), m_port, addrInfo->ai_family);
                continue;
            }

            if (address == NULL)
            {
                B2NETWORK_LOG_ERROR(logger, "Can't resolve %s:%d", m_host.c_str(), m_port);
            }
            else
            {
                char buffer[INET6_ADDRSTRLEN];

                if (inet_ntop(addrInfo->ai_family, address, buffer, sizeof(buffer)) != NULL)
                {
                    m_resolvedHost = std::string(buffer);

                    B2NETWORK_LOG_INFO(logger, "Resolved: %d %s", addrInfo->ai_family, m_resolvedHost.c_str());
                    break;
                }
                else
                {
                    B2NETWORK_LOG_ERROR(logger, "Can't print resolved address (%s:%d)!", m_host.c_str(), m_port);
                }
            }
        }
    }
    else
    {
        B2NETWORK_LOG_ERROR(logger, "Error on resolving address (%d)(%s)", errorCode, gai_strerror(errorCode));
    }

    B2NETWORK_LOG_INFO(logger, "%s Resolved %s as %s", m_info.c_str(), m_host.c_str(), m_resolvedHost.c_str());

    if (resolved == NULL)
    {
        // TODO: try it with ip v4
        return CONNECT_ERROR_DNS_FAILURE;
    }

    B2NETWORK_LOG_INFO(logger, "%s Connecting to %s(%s):%d...", m_info.c_str(), m_host.c_str(), m_resolvedHost.c_str(), m_port);

    OpenSocket(resolved->ai_family, resolved->ai_socktype, resolved->ai_protocol);

    int result = connect(m_socket, resolved->ai_addr, resolved->ai_addrlen);

    freeaddrinfo(addrList);

#if defined(B2NETWORK_PLATFORM_WINDOWS)
    const bool error = (result == SOCKET_ERROR);
    const int ec = WSAGetLastError();
    const bool block = (ec == WSAEWOULDBLOCK);
#elif defined(B2NETWORK_PLATFORM_ANDROID)
    const bool error = (result != 0);
    const int ec = errno;
    const bool block = (ec == EINPROGRESS);
#elif defined(B2NETWORK_PLATFORM_IOS)
    const bool error = (result != 0);
    const int ec = errno;
    const bool block = (ec == EINPROGRESS);
#else
#error Platform not supported
#endif    

    if (result == 0)
    {
        // connection has succeeded immediately

        B2NETWORK_LOG_INFO(logger, "%s connected to %s:%d", m_info.c_str(), m_host.c_str(), m_port);

        return CONNECT_SUCCESS;
    }
    else if (block)
    {
        // connection attempt is in progress

        B2NETWORK_LOG_INFO(logger, "%s waiting for connection to %s:%d, result=%d", m_info.c_str(), m_host.c_str(), m_port, result);

        return CONNECT_WAITING;
    }
    else
    {
        // connection error

        B2NETWORK_LOG_ERROR(logger, "%s could not connect to %s:%d, result=%d, error=%d", m_info.c_str(), m_host.c_str(), m_port, result, ec);

#if defined(B2NETWORK_PLATFORM_WINDOWS)
        throw ConnectionException("Connect Error");
#elif defined(B2NETWORK_PLATFORM_ANDROID)
        B2NETWORK_LOG_ERROR(logger, "Connect Error(%d)", ec);
#elif defined(B2NETWORK_PLATFORM_IOS)
        B2NETWORK_LOG_ERROR(logger, "Connect Error(%d)", ec);
#else
#error Platform not supported
#endif

        return CONNECT_ERROR;
    }
}

Connection::ConnectResult Connection::WaitForConnect(const UpdateEvent events)
{
    //B2NETWORK_LOG_INFO(logger, "%s Waiting for connection... events(%d)", m_info.c_str(), events);

    if (events & WR_EVENT)
    {
#if defined(B2NETWORK_PLATFORM_WINDOWS)
        int optval;
        int optlen = sizeof(optval);

        int result = getsockopt(m_socket,
            SOL_SOCKET, SO_ERROR, reinterpret_cast<char*>(&optval), &optlen);

        bool error = (result == SOCKET_ERROR);
#elif defined(B2NETWORK_PLATFORM_ANDROID)
        int optval;
		socklen_t optlen = sizeof(optval);

        int result = getsockopt(m_socket,
            SOL_SOCKET, SO_ERROR, reinterpret_cast<char*>(&optval), &optlen);

        bool error = (result < 0);
#elif defined(B2NETWORK_PLATFORM_IOS)
        int optval;
        socklen_t optlen = sizeof(optval);

        int result = getsockopt(m_socket,
            SOL_SOCKET, SO_ERROR, reinterpret_cast<char*>(&optval), &optlen);

        bool error = (result < 0);
#else
#error Platform not supported
#endif

        if (error)
        {
#if defined(B2NETWORK_PLATFORM_WINDOWS)
            throw ConnectionException("Unknown Connect Error");
#elif defined(B2NETWORK_PLATFORM_ANDROID)
            B2NETWORK_LOG_ERROR(logger, "Unknown Connect Error %d", result);
#elif defined(B2NETWORK_PLATFORM_IOS)
            B2NETWORK_LOG_ERROR(logger, "Unknown Connect Error %d", result);
#else
#error Platform not supported
#endif

            return CONNECT_ERROR;
        }

        if (optval == 0)
        {
            // connection established

            return CONNECT_SUCCESS;
        }
        else
        {
            // connection error

#if defined(B2NETWORK_PLATFORM_WINDOWS)
            throw ConnectionException("Unknown Connect Error");
#elif defined(B2NETWORK_PLATFORM_ANDROID)
            B2NETWORK_LOG_ERROR(logger, "Unknown Socket Error %d", optval);
#elif defined(B2NETWORK_PLATFORM_IOS)
            B2NETWORK_LOG_ERROR(logger, "Unknown Socket Error %d", optval);
#else
#error Platform not supported
#endif

            return CONNECT_ERROR;
        }
    }
    else if (events & EX_EVENT)
    {
        // connection attempt failed

        B2NETWORK_LOG_INFO(logger, "%s Can't connect to %s:%d", m_info.c_str(), m_host.c_str(), m_port);

        return CONNECT_ERROR;
    }
    else
    {
        // TODO implement connection timeout

        return CONNECT_WAITING;
    }
}

Connection::ProcessResult Connection::ProcessEvent(const UpdateEvent events)
{
    auto result = EVENT_NONE;

    if (events & RD_EVENT)
    {
        result = OnRecvEvent();
    }

    if (result == EVENT_NONE || result == EVENT_RECV_NONE)
    {
        if (events & WR_EVENT)
        {
            OnSendEvent();

            result = EVENT_SEND_READY;
        }
    }

    if (events & EX_EVENT)
    {
        B2NETWORK_LOG_ERROR(logger, "%s Process exception event...", m_info.c_str());

        OnExceptionEvent();
    }

    return result;
}

Connection::MessagePtr Connection::Recv()
{
    if (m_recvMessages.empty())
    {
        return nullptr;
    }
    else
    {
        auto message = m_recvMessages.front();
        m_recvMessages.pop_front();
        return message;
    }
}

bool Connection::Send(MessagePtr message)
{
    B2NETWORK_LOG_DEBUG(logger, "%s Sending... %s", m_info.c_str(), message->ShortDebugString().c_str());

    int delay = m_leakyBucket.Update();
    if (delay > 0)
    {
        B2NETWORK_LOG_ERROR(logger, "%s Delaying %s in %d msec...",  m_info.c_str(), message->ShortDebugString().c_str(), delay);

        FPlatformProcess::Sleep(delay);
    }

    auto buffer = m_codec.Encode(message);

	std::vector<uint8>	cipher_buffer(buffer->Size());
	::memcpy(cipher_buffer.data(), buffer->ReadPtr(), buffer->Size());
	m_crypto->Encrypt(cipher_buffer.data(), cipher_buffer.size());

	const auto sent = send(m_socket, reinterpret_cast<char*>(cipher_buffer.data()), cipher_buffer.size(), 0);
    if (sent == buffer->Size())
    {
        B2NETWORK_LOG_INFO(logger, "%s ===>>> Sent %s %d bytes(s)", ToString().c_str(), message->GetDescriptor()->name().c_str(), sent);

#if B2_BUILD_LEVEL < B2_BUILD_LEVEL_QA  // !B2_BUILD_LEVEL_LIVE, !!B2_BUILD_LEVEL_QA
        if (logger.IsDebugEnabled())
        {
            // packet dump

            std::istringstream iss(message->DebugString().c_str());

            int count = 1;
            for (std::string line; std::getline(iss, line, '\n'); )
            {
                B2NETWORK_LOG_DEBUG(logger, "%-30s (%.8d)>>> %s",
                    message->GetDescriptor()->name().c_str(),
                    count++,
                    line.c_str());
            }
		}
#endif

        return true;
    }
    else if (sent > 0)
    {
        // TODO not yet implemented
#if defined(B2NETWORK_PLATFORM_WINDOWS)
        throw ConnectionException("Partial Send");
#elif defined(B2NETWORK_PLATFORM_ANDROID)
        B2NETWORK_LOG_ERROR(logger, "Partial Send");
#elif defined(B2NETWORK_PLATFORM_IOS)
        B2NETWORK_LOG_ERROR(logger, "Partial Send");
#else
#error Platform not supported
#endif

        B2NETWORK_LOG_ERROR(logger, "%s Send incompleted, %d/%d", m_info.c_str(), sent, buffer->Size());

        return false;
    }
    else
    {
#if defined(B2NETWORK_PLATFORM_WINDOWS)
        int ec = WSAGetLastError();
#elif defined(B2NETWORK_PLATFORM_ANDROID)
        int ec = errno;
#elif defined(B2NETWORK_PLATFORM_IOS)
        int ec = errno;
#else
#error Platform not supported
#endif

        B2NETWORK_LOG_ERROR(logger, "%s Send failed, error=%d", m_info.c_str(), ec);

        return false;
    }
}

void Connection::Disconnect(std::string reason)
{
    B2NETWORK_LOG_DEBUG(logger, "%s Disconnecting... %s:%d", m_info.c_str(), m_host.c_str(), m_port);

    CloseConnection(reason);
}

void Connection::Close()
{
    B2NETWORK_LOG_DEBUG(logger, "%s Closing... %s:%d", m_info.c_str(), m_host.c_str(), m_port);

    CloseSocket();
}

void Connection::InjectCipherKeyIntoConnection(const Key& key)
{
	B2NETWORK_LOG_INFO(logger, "Shared Secret Key : %s", hexify(&key.GetRealKey()[0], crypto_stream_chacha20_KEYBYTES).c_str());
	B2NETWORK_LOG_INFO(logger, "Shared Nonce Key : %s", hexify(&key.GetNonceKey()[0], crypto_stream_chacha20_NONCEBYTES).c_str());

	auto temp = std::static_pointer_cast<ICrypto>(std::make_shared<ChaCha20>(key));
	m_crypto.swap(temp);
}

void Connection::ExtractCipherKeyOutofConnection()
{
	auto dummy = std::static_pointer_cast<ICrypto>(std::make_shared<Dummy>());
	m_crypto.swap(dummy);
}

std::string Connection::ToString()
{
    std::ostringstream oss;
    oss << m_info << "(" << m_host << ":" << m_port << ")";

    return oss.str();
}

Connection::ProcessResult Connection::OnRecvEvent()
{
    B2NETWORK_LOG_DEBUG(logger, "%s OnRecvEvent...", ToString().c_str());

    m_recvBuffer.Reserve(RecvBuffer::MinSize);

    int received = recv(m_socket, m_recvBuffer.WritePtr(), m_recvBuffer.Capacity(), 0);
    if (received > 0)
    {
        B2NETWORK_LOG_TRACE(logger, "%s Bytes received %d", ToString().c_str(), received);

        m_recvBuffer.Seek(RecvBuffer::Wr, received);

        while (true)
        {
            uint32_t decoded = 0;
            uint32_t demanded = 0;

			std::vector<uint8>	cipher_buffer(m_recvBuffer.Size());
			::memcpy(cipher_buffer.data(), m_recvBuffer.ReadPtr(), m_recvBuffer.Size());
			m_crypto->Decrypt(cipher_buffer.data(), cipher_buffer.size());

//            auto result = m_codec.Decode(m_recvBuffer.ReadPtr(), m_recvBuffer.Size(), decoded, demanded);
			auto result = m_codec.Decode(reinterpret_cast<char*>(cipher_buffer.data()), cipher_buffer.size(), decoded, demanded);
            if (result.first ==  ProtobufCodec::Ok)
            {
                auto message = result.second;

                B2NETWORK_LOG_INFO(logger, "%s <<<=== Recv %s %d byte(s)",
                        ToString().c_str(),
                        message->GetDescriptor()->name().c_str(),
                        decoded);

#if B2_BUILD_LEVEL < B2_BUILD_LEVEL_QA  // !B2_BUILD_LEVEL_LIVE, !!B2_BUILD_LEVEL_QA
                if (logger.IsDebugEnabled())
                {
                    // packet dump

                    std::istringstream iss(message->DebugString().c_str());

                    int count = 1;
                    for (std::string line; std::getline(iss, line, '\n'); )
                    {
                        B2NETWORK_LOG_DEBUG(logger, "%-30s (%.8d)<<< %s",
                            message->GetDescriptor()->name().c_str(),
                            count++,
                            line.c_str());
                    }
                }
#endif

                m_recvBuffer.Seek(RecvBuffer::Rd, decoded);

                m_recvMessages.push_back(message);
            }
            else if (result.first == ProtobufCodec::NotEnoughHeader)
            {
                break;
            }
            else if (result.first == ProtobufCodec::NotEnoughBody)
            {
                if (m_recvBuffer.Reserve(demanded))
                {
                    B2NETWORK_LOG_DEBUG(logger, "%s Recv buffer capacity enlarged. result=%d, demand=%d, %s",
                        ToString().c_str(),
                        result.first,
                        demanded,
                        m_recvBuffer.ToString().c_str());
                }

                break;
            }
            else
            {
                B2NETWORK_LOG_ERROR(logger, "protobuf decode error!");

                // TODO fix
                return EVENT_RECV_ERROR;
            }
        }

        // TODO need to find a proper threshold
        if (m_recvBuffer.Shrink(RecvBuffer::MaxSize * 4))
        {
            B2NETWORK_LOG_DEBUG(logger, "%s Recv buffer shrank. %s", ToString().c_str(), m_recvBuffer.ToString().c_str());
        }

        return (m_recvMessages.empty() ? EVENT_RECV_NONE : EVENT_RECV_SUCCESS);
    }
    else if (received == 0)
    {
        B2NETWORK_LOG_INFO(logger, "%s Closed (Recv 0 Bytes)", ToString().c_str());

        CloseSocket();

        return EVENT_RECV_CLOSED;
    }
    else
    {
#if defined(B2NETWORK_PLATFORM_WINDOWS)
        int ec = WSAGetLastError();
#elif defined(B2NETWORK_PLATFORM_ANDROID)
        int ec = errno;
#elif defined(B2NETWORK_PLATFORM_IOS)
        int ec = errno;
#else
#error Platform not supported
#endif

        B2NETWORK_LOG_ERROR(logger, "%s Error (error=%d)", ToString().c_str(), ec);

#if defined(B2NETWORK_PLATFORM_WINDOWS)
#elif defined(B2NETWORK_PLATFORM_ANDROID)
        if (errno == 103)
        {
            // NOTE 103 error code seems to occur when mobile phones are taking over network devices
            return EVENT_RECV_ABORTED;
        }
#elif defined(B2NETWORK_PLATFORM_IOS)
        if (errno == 103)
        {
        }
#else
#error Platform not supported
#endif

        return EVENT_RECV_DISCONNECTED;
    }
}

void Connection::OnSendEvent()
{
    //B2NETWORK_LOG_DEBUG(logger, "%s (%s:%d) OnSendEvent...", ToString().c_str(), m_host.c_str(), m_port);
}

void Connection::OnExceptionEvent()
{
    B2NETWORK_LOG_DEBUG(logger, "%s (%s:%d) OnExceptionEvent...", ToString().c_str(), m_host.c_str(), m_port);

#if defined(B2NETWORK_PLATFORM_WINDOWS)
    // TODO not yet implemented
    throw ConnectionException("Caught Exception Event");
#elif defined(B2NETWORK_PLATFORM_ANDROID)
    B2NETWORK_LOG_ERROR(logger, "Caught Exception Event");
#elif defined(B2NETWORK_PLATFORM_IOS)
    B2NETWORK_LOG_ERROR(logger, "Caught Exception Event");
#else
#error Platform not supported
#endif
}

void Connection::CloseConnection(const std::string reason)
{
    B2NETWORK_LOG_INFO(logger, "%s Closing connection...(%s)", m_info.c_str(), reason.c_str());

    bool error = false;
    int ec = 0;

#if defined(B2NETWORK_PLATFORM_WINDOWS)
    if (m_socket != INVALID_SOCKET)
    {
        int result = shutdown(m_socket, SD_SEND);
        error = (result == SOCKET_ERROR);
        ec = WSAGetLastError();
    }
#elif defined(B2NETWORK_PLATFORM_ANDROID)
    if (m_socket != -1)
    {
        int result = shutdown(m_socket, SHUT_WR);
        error = (result == -1);
        ec = errno;
    }
#elif defined(B2NETWORK_PLATFORM_IOS)
    if (m_socket != -1)
    {
        int result = shutdown(m_socket, SHUT_WR);
        error = (result == -1);
        ec = errno;
    }
#else
#error Platform not supported
#endif

    if (error)
    {
        B2NETWORK_LOG_INFO(logger, "%s shutdown error=%d", m_info.c_str(), ec);
    }

	ExtractCipherKeyOutofConnection();
    CloseSocket();
}

void Connection::OpenSocket(int family, int socktype, int protocol)
{
#if defined(B2NETWORK_PLATFORM_WINDOWS)
    if (m_socket != INVALID_SOCKET)
    {
        CloseSocket();
    }
#elif defined(B2NETWORK_PLATFORM_ANDROID)
    if (m_socket != -1)
    {
        CloseSocket();
    }
#elif defined(B2NETWORK_PLATFORM_IOS)
    if (m_socket != -1)
    {
        CloseSocket();
    }
#else
#error Platform not supported
#endif

    //m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    m_socket = socket(family, socktype, protocol);

#if defined(B2NETWORK_PLATFORM_WINDOWS)
    if (m_socket == INVALID_SOCKET)
    {
        throw ConnectionException("Socket Open Error");
    }
#elif defined(B2NETWORK_PLATFORM_ANDROID)
    if (m_socket == -1)
    {
        // TODO fix
        B2NETWORK_LOG_ERROR(logger, "Socket Open Error");
    }
#elif defined(B2NETWORK_PLATFORM_IOS)
    if (m_socket == -1)
    {
        // TODO fix
        B2NETWORK_LOG_ERROR(logger, "Socket Open Error");
    }
#else
#error Platform not supported
#endif

    int result;

#if defined(B2NETWORK_PLATFORM_WINDOWS)
    u_long arg = 1;
    result = ioctlsocket(m_socket, FIONBIO, &arg);
    if (result != NO_ERROR)
    {
        throw ConnectionException("Socket Option Error");
    }
#elif defined(B2NETWORK_PLATFORM_ANDROID)
    int flag = fcntl(m_socket, F_GETFL);
    assert(flag >= 0);
    result = fcntl(m_socket, F_SETFL, O_NONBLOCK | flag);
    if (result < 0)
    {
        // TODO fix
        B2NETWORK_LOG_ERROR(logger, "Socket Open Error %d", result);
    }
#elif defined(B2NETWORK_PLATFORM_IOS)
    int flag = fcntl(m_socket, F_GETFL);
    assert(flag >= 0);
    result = fcntl(m_socket, F_SETFL, O_NONBLOCK | flag);
    if (result < 0)
    {
        // TODO fix
        B2NETWORK_LOG_ERROR(logger, "Socket Open Error %d", result);
    }
#else
#error Platform not supported
#endif

    int optval = 1;
    const int optlen = sizeof(optval);

    result = setsockopt(m_socket, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<char*>(&optval), optlen);

    int ec;
#if defined(B2NETWORK_PLATFORM_WINDOWS)
    bool error = (result == SOCKET_ERROR);
    ec = WSAGetLastError();
#elif defined(B2NETWORK_PLATFORM_ANDROID)
    bool error = (result == -1);
    ec = errno;
#elif defined(B2NETWORK_PLATFORM_IOS)
    bool error = (result == -1);
    ec = errno;
#else
#error Platform not supported
#endif

    if (error)
    {
        B2NETWORK_LOG_DEBUG(logger, "Can't disable neagle's algorithm (%d)", ec);
    }
}

void Connection::CloseSocket()
{
#if defined(B2NETWORK_PLATFORM_WINDOWS)
    if (m_socket == INVALID_SOCKET)
    {
        return;
    }

    int result = closesocket(m_socket);
    if (result == SOCKET_ERROR)
    {
        B2NETWORK_LOG_INFO(logger, "%s Can't close socket result=%d, error=%d", m_info.c_str(), result, WSAGetLastError());
    }

    m_socket = INVALID_SOCKET;
#elif defined(B2NETWORK_PLATFORM_ANDROID)
    if (m_socket == -1)
    {
        return;
    }

    close(m_socket);

    m_socket = -1;
#elif defined(B2NETWORK_PLATFORM_IOS)
    if (m_socket == -1)
    {
        return;
    }

    close(m_socket);

    m_socket = -1;
#else
#error Platform not supported
#endif

    // reset socket receive buffer
    m_recvBuffer.Clear();
}
