#include "Client.h"
#include "B2NetworkPlugin.h"

#include "Config.h"
#include "LogManager.h"
#include "Exception.h"
#include "ProtobufFactory.h"
#include "ProtobufCodec.h"
#include "Connection.h"
#include "MessageRouter.h"
#include "EventDispatcher.h"


using namespace b2network;

#if B2_BUILD_LEVEL < B2_BUILD_LEVEL_QA // !B2_BUILD_LEVEL_LIVE, !!B2_BUILD_LEVEL_QA
static auto logger = LogManager::GetLogger("Client");
#endif

const char* Client::GetEventString(const Event e)
{
    switch (e)
    {
    case ServiceConnecting:
        return "ServiceConnecting";

    case ServiceConnected:
        return "ServiceConnected";

    case ServiceNotAvailable:
        return "ServiceNotAvailable";

	case ServerTimeSync:
		return "ServerTimeSync";

    case ChannelDisconnected:
        return "ChannelDisconnected";

    case RequireGiveNickname:
        return "RequireGiveNickname";

    case RequireUpdateProtocol:
        return "RequireUpdateProtocol";

    case RequireUpdateVersion:
        return "RequireUpdateVersion";

    default:
        return "Unknown";
    }
}

Client::Client(const CredentialInfo& credentialInfo, const Endpoint& sessionServer)
	:m_enabled(false),
     m_messageRouter(m_connectionManager),
     m_connectionManager(m_messageRouter, m_eventDispatcher),
     m_authManager(credentialInfo, sessionServer, m_messageRouter, m_eventDispatcher),
     m_exit(false),
     m_updater(this)
{
	B2NETWORK_LOG_INFO(logger, "B2Nework Client Started ========================================================");

    if (!sessionServer.IsValid())
    {
        B2NETWORK_LOG_ERROR(logger, "Invalid session server address(%s)!", sessionServer.ToString().c_str());

        throw Exception("InvalidSessionServer");
    }

	ProtobufFactory::Initialize();
}

Client::~Client()
{
    ProtobufFactory::Finalize();

    B2NETWORK_LOG_INFO(logger, "B2Nework Client Stopped ========================================================");
}

void Client::Start()
{
    FScopeLock lock(&m_lock);

	const auto sessionEndpoint = m_authManager.GetSessionEndPoint();

    B2NETWORK_LOG_INFO(logger, "Starting with SessionServer(%s)...", sessionEndpoint.ToString().c_str());

    if (m_enabled)
    {
        B2NETWORK_LOG_DEBUG(logger, "Already started");
        return;
    }

    if (!FPlatformProcess::SupportsMultithreading())
    {
        B2NETWORK_LOG_ERROR(logger, "Platform does not support multithreading!")
    }

    auto result = m_updater.Start();
    if (result)
    {
        B2NETWORK_LOG_INFO(logger, "Updater thread id is [%.8d]", m_updater.GetThreadId());
    }
    else
    {
        B2NETWORK_LOG_ERROR(logger, "Could not run updater thread!");

        throw Exception("NoUpdateThread");
    }

    if (!sessionEndpoint.IsValid())
    {
        B2NETWORK_LOG_ERROR(logger, "Invalid session server endpoint(%s)!", sessionEndpoint.ToString().c_str())
    }

    m_messageRouter.Initialize(sessionEndpoint);

    m_eventDispatcher.AddResponseFilter([this](std::shared_ptr<Message> response)
        {
            if (ProtobufFactory::IsTypeOf(response, b2::protocol::session::SignInSessionResponse::default_instance()))
            {
                auto signInSessionResponse = std::static_pointer_cast<b2::protocol::session::SignInSessionResponse>(response);

                auto ec = signInSessionResponse->result().error_code();
                if (ec == 0)
                {
                    // TODO
                }
            }
        });

    m_eventDispatcher.AddEventCallback(EventDispatcher::ServiceConnecting,
        [this](const EventDispatcher::Event event, int error)
        {
            const auto e = ServiceConnecting;

            B2NETWORK_LOG_INFO(logger, ">>>>>>>> Event %s...", GetEventString(e));
            InvokeEventHandler(e);
        });

    m_eventDispatcher.AddEventCallback(EventDispatcher::ServiceConnected,
        [this](const EventDispatcher::Event event, int error)
        {
            const auto e = ServiceConnected;

            B2NETWORK_LOG_INFO(logger, ">>>>>>>> Event %s...", GetEventString(e));
            InvokeEventHandler(e);
        });

    m_eventDispatcher.AddEventCallback(EventDispatcher::ServiceNotAvailable,
        [this](const EventDispatcher::Event event, int errorCode)
        {
            const auto e = ServiceNotAvailable;

            B2NETWORK_LOG_ERROR(logger, ">>>>>>>> Event %s error(%d)...", GetEventString(e), errorCode);

            InvokeEventHandler(e, errorCode);

            // TODO: Make it stop!!!

            m_connectionManager.Stop();
            //m_eventDispatcher.Stop(); not reenterable
            m_authManager.Stop();

            //m_enabled = false;
        });

    m_eventDispatcher.AddEventCallback(EventDispatcher::ChannelRoomHeartbeatTimeout,
        [this](const EventDispatcher::Event event, int errorCode)
        {
            B2NETWORK_LOG_ERROR(logger, ">>>>>>>> Event ChannelRoomHeartbeatTimeout...");

            // TODO need refactoring
            // it causes that client can't not send messages to the channel server
            // due to non-empty response queue for channel
            m_messageRouter.ClearChannelRoute();
            m_messageRouter.ClearChannelRequests();

            // NOTE invoke event handler with ChannelDisconnected event
            InvokeEventHandler(ChannelDisconnected, errorCode);
        });

    m_eventDispatcher.AddEventCallback(EventDispatcher::ChannelDisconnected,
        [this](const EventDispatcher::Event event, int errorCode)
        {
            const auto e = ChannelDisconnected;

            B2NETWORK_LOG_ERROR(logger, ">>>>>>>> Event %s error(%d)...", GetEventString(e), errorCode);

            InvokeEventHandler(e, errorCode);
        });

    m_eventDispatcher.AddEventCallback(EventDispatcher::RequireGiveNickname,
        [this](const EventDispatcher::Event event, int errorCode)
        {
            const auto e = RequireGiveNickname;

            B2NETWORK_LOG_INFO(logger, ">>>>>>>> Event %s...", GetEventString(e));
            InvokeEventHandler(e);
        });

    m_eventDispatcher.AddEventCallback(EventDispatcher::RequireUpdateProtocol,
        [this](const EventDispatcher::Event event, int errorCode)
        {
            const auto e = RequireUpdateProtocol;

            B2NETWORK_LOG_INFO(logger, ">>>>>>>> Event %s...", GetEventString(e));
            InvokeEventHandler(e);
        });

    m_eventDispatcher.AddEventCallback(EventDispatcher::RequireUpdateVersion,
        [this](const EventDispatcher::Event event, int errorCode)
        {
            const auto e = RequireUpdateVersion;

            B2NETWORK_LOG_INFO(logger, ">>>>>>>> Event %s...", GetEventString(e));
            InvokeEventHandler(e);
        });

	m_eventDispatcher.AddEventCallback(EventDispatcher::ServerTimeSync,
		[this](const EventDispatcher::Event event, int errorCode)
		{
			const auto e = ServerTimeSync;

			B2NETWORK_LOG_INFO(logger, ">>>>>>>> Event %s...", GetEventString(e));
			InvokeEventHandler(e);
		});

    m_connectionManager.Start(sessionEndpoint);

    m_eventDispatcher.Start();

	m_authManager.Start();

    m_enabled = true;

    B2NETWORK_LOG_INFO(logger, "Started");

    m_connectionManager.ActivateSessionConnection(sessionEndpoint);

    m_lastAuthUpdate = std::chrono::steady_clock::now();
    m_lastConnUpdate = std::chrono::steady_clock::now();
}

void Client::Stop()
{
    {
        FScopeLock lock(&m_lock);

        B2NETWORK_LOG_INFO(logger, "Stopping...");

        m_exit = true;

        if (!m_enabled)
        {
            B2NETWORK_LOG_DEBUG(logger, "Already stopped");
            return;
        }

        B2NETWORK_LOG_INFO(logger, "Waiting for updated thread to exit...");
    }

    m_updater.Wait();

    // clear tasks in queues
    m_eventQueue.clear();
    m_notifyQueue.clear();
    m_responseQueue.clear();

    m_eventHandlers.clear();

    m_authManager.Stop();

    m_eventDispatcher.Stop();

    m_connectionManager.Stop();

    m_messageRouter.Finalize();

    m_enabled = false;

    B2NETWORK_LOG_INFO(logger, "Stopped");
}

void Client::ChannelHeartbeat(const std::string& channel)
{
    FScopeLock lock(&m_lock);

    // NOTE the 'channel' argument is the room id from channel server

    m_authManager.RoomHeartbeatForeground(channel);
}

void Client::CloseChannel(const std::string& reason)
{
    FScopeLock lock(&m_lock);

    m_connectionManager.CloseChannelConnection(reason);
}

bool Client::Configure(const std::string& optionName, bool value)
{
    FScopeLock lock(&m_lock);

    B2NETWORK_LOG_INFO(logger, "Applying Option... %s %s", optionName.c_str(), value ? "on" : "off");

    if (optionName.compare("ResponseOrder") == 0)
    {
        return m_messageRouter.Configure(optionName, value);
    }

    return false;
}

const CredentialInfo& Client::GetCredentialInfo() const
{
    FScopeLock lock(&m_lock);

    return m_authManager.GetCredentialInfo();
}

void Client::AddEventHandler(const Event event, EventHandler handler)
{
    FScopeLock lock(&m_lock);

    const auto it = m_eventHandlers.find(event);
    if (it == m_eventHandlers.end())
    {
        m_eventHandlers.insert({event, handler});

        B2NETWORK_LOG_INFO(logger, "Event %d handler added", event);
    }
    else
    {
        m_eventHandlers[event] = handler;

        B2NETWORK_LOG_INFO(logger, "Event %d handler changed", event);
    }
}

void Client::RemoveEventHandler(const Event event)
{
    FScopeLock lock(&m_lock);

    auto count = m_eventHandlers.erase(event);

    B2NETWORK_LOG_INFO(logger, "Event %d handler %d removed", event, count);
}

#if defined(B2NETWORK_ENVIRONMENT_TOOL)
#error Environment not supported
#elif defined(B2NETWORK_ENVIRONMENT_GAME)
bool Client::SendB2Message(FB2MessageInfoPtr messageInfo)
{
    FScopeLock lock(&m_lock);

    auto fn = FB2MessageHandlerManager::GetInstance()->GetHandler(messageInfo->GetType());
    if (fn)
    {
        fn(shared_from_this(), messageInfo);
        return true;
    }
    else
    {
        return false;
    }
}
#else
#error Environment not specified
#endif

void Client::SendRequest(const MessagePtr message, ResponseCallback callback)
{
    bool result = false;

    {
        FScopeLock lock(&m_lock);

        result = m_messageRouter.SendRequest(message,
            [this, callback](MessagePtr response)
            {
                // NOTE seems no need to lock
                m_responseQueue.emplace_back(callback, response);
            });
    }

    if (!result)
    {
        const auto serverType = m_messageRouter.GetServerType(message);
        if (serverType == ProtobufFactory::Session)
        {
            InvokeEventHandler(Client::ServiceNotAvailable);
        }
        else if (serverType == ProtobufFactory::Channel)
        {
            InvokeEventHandler(Client::ChannelDisconnected);
        }
        else
        {
            InvokeEventHandler(Client::ServiceNotAvailable);
        }
    }
}

void Client::SendCommand(const MessagePtr message)
{
    int error = 0;

    {
        FScopeLock lock(&m_lock);

        error = m_messageRouter.SendCommand(message);
    }

    if (error != 0)
    {
        const auto serverType = m_messageRouter.GetServerType(message);
        if (serverType == ProtobufFactory::Session)
        {
            InvokeEventHandler(Client::ServiceNotAvailable, error);
        }
        else if (serverType == ProtobufFactory::Channel)
        {
            InvokeEventHandler(Client::ChannelDisconnected, error);
        }
        else
        {
            InvokeEventHandler(Client::ServiceNotAvailable, error);
        }
    }
}

#if defined(B2NETWORK_ENVIRONMENT_TOOL)
#error Environment not supported
#elif defined(B2NETWORK_ENVIRONMENT_GAME)
void Client::AttachNotifyListener(std::string typeName, std::string messageType, NotifyCallback callback)
{
    FScopeLock lock(&m_lock);

    m_messageRouter.AttachNotifyHandler(typeName,
       [this, typeName, messageType, callback](MessagePtr message)
       {
           // NOTE seems no need to lock
           m_notifyQueue.emplace_back(callback, typeName, messageType, message);
       });
}
#else
#error Environment not specified
#endif

void Client::DetachNotifyListener(std::string typeName)
{
    FScopeLock lock(&m_lock);

    m_messageRouter.DetachNotifyHandler(typeName);
}

void Client::InvokeEventHandler(const Event event, int errorCode /*= 0*/)
{
    FScopeLock lock(&m_lock);

    auto it = m_eventHandlers.find(event);
    if (it != m_eventHandlers.end())
    {
        m_eventQueue.emplace_back(event, errorCode, it->second);
    }
}

void Client::Dump()
{
    FScopeLock lock(&m_lock);

    // Dump object state on the console

    auto now = std::chrono::steady_clock::now();

    const std::chrono::duration<double> authElapsed = now - m_lastAuthUpdate;
    const std::chrono::duration<double> connElapsed = now - m_lastConnUpdate;

    std::ostringstream oss;
    oss << ">>>>>>>>>> B2Network Dump begins... " << std::endl
        << "\tServiceStatus: " << (IsServiceAvailable() ? "OK" : "NA") << std::endl
        << "\tEnabled: " << (m_enabled ? "True" : "False") << std::endl
        << "\tAuthManager: { " << m_authManager.Dump() << "} Elapsed: " << authElapsed.count() << " second(s)" << std::endl
        << "\tConnectionManager: { " << m_connectionManager.Dump() << " } Elapsed: " << connElapsed.count() << " second(s)" << std::endl
        << "\tMessageRouter: { " << m_messageRouter.Dump() << " }" << std::endl
        << ">>>>>>>>>> B2Network Dump ended. " << std::endl;

    std::istringstream iss(oss.str());

    for (std::string line; std::getline(iss, line, '\n'); )
    {
        //UE_LOG(LogB2Network, Display, TEXT("b2.net %s"), *FString(line.c_str()));
        B2NETWORK_LOG_INFO(logger, "%s", line.c_str());
    }
}

void Client::ForceDisconnect()
{
    FScopeLock lock(&m_lock);

    // TODO not implemented yet?
}

bool b2network::Client::IsServiceAvailable() const
{
    return m_authManager.IsActivated();
}

bool b2network::Client::IsChannelDisconnected() const
{
	return m_connectionManager.IsChannelDisconnected();
}

void b2network::Client::SetHeartbeatPeriod(int sessionHeartbeat, int channelHeartbeat)
{
	m_authManager.SetHeartbeatPeriod(sessionHeartbeat, channelHeartbeat);
}

void Client::MainThreadUpdate()
{
    // TODO fix to use the scoped guard for the lock
    m_lock.Lock();

#if B2_BUILD_LEVEL < B2_BUILD_LEVEL_QA  // !B2_BUILD_LEVEL_LIVE, !!B2_BUILD_LEVEL_QA
	logger.Tick();
#endif

    if (!m_responseQueue.empty())
    {
        ResponseQueue responses;
        std::swap(m_responseQueue, responses);

        m_lock.Unlock();

        std::for_each(responses.begin(), responses.end(),
            [](ResponseQueue::value_type& task)
            {
                task.first(task.second);
            });

        m_lock.Lock();
    }

    if (!m_notifyQueue.empty())
    {
        NotifyQueue notifies;
        std::swap(m_notifyQueue, notifies);

        m_lock.Unlock();

        std::for_each(notifies.begin(),
            notifies.end(),
            [this](NotifyQueue::value_type& task)
            {
                auto callback = std::get<0>(task);
                auto typeName = std::get<1>(task);
                auto messageType = std::get<2>(task);
                auto message = std::get<3>(task);

                auto listener = FB2MessageHandlerManager::GetInstance()->GetListener(messageType);
                if (listener)
                {
                    B2NETWORK_LOG_DEBUG(logger, "Invoking notify listener for %s, %s...",
                        typeName.c_str(), messageType.c_str());

                    auto messageInfo = listener(shared_from_this(), message);
                    callback(messageInfo);
                }
                else
                {
                    B2NETWORK_LOG_ERROR(logger, "Fail to invoke notify listener for %s, %s...",
                        typeName.c_str(), messageType.c_str());
                }
            });

        m_lock.Lock();
    }

    if (!m_eventQueue.empty())
    {
        EventQueue events;
        std::swap(m_eventQueue, events);

        m_lock.Unlock();

        std::for_each(events.begin(), events.end(),
            [](EventQueue::value_type& task)
            {
                const auto event = std::get<0>(task);
                const auto errorCode = std::get<1>(task);
                const auto callback = std::get<2>(task);

                B2NETWORK_LOG_DEBUG(logger, "Invoking event(%d)(%s) error(%d) handler...", event, GetEventString(event), errorCode);

                callback(errorCode);
            });

        m_lock.Lock();
    }

    m_lock.Unlock();
}

void Client::WorkThreadUpdate()
{
    while (true)
    {
        auto start = std::chrono::steady_clock::now();

        {
            FScopeLock lock(&m_lock);

            //B2NETWORK_LOG_INFO(logger, "Updater tick....");

            if (m_exit)
            {
                B2NETWORK_LOG_INFO(logger, "Exit update loop for worker thread");

                return;
            }

            //B2NETWORK_LOG_DEBUG(logger, "Updating...");

            if (m_enabled)
            {
#if defined(B2NETWORK_PLATFORM_WINDOWS)
                try
                {
#endif
                    {
                        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(start - m_lastAuthUpdate);
                        if (elapsed.count() > 1000)
                        {
                            m_authManager.Update();

                            m_messageRouter.Update();

                            m_lastAuthUpdate = start;
                        }
                    }

                    {
                        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(start - m_lastConnUpdate);
                        if (elapsed.count() > 1)
                        {
                            m_connectionManager.Update();

                            m_lastConnUpdate = start;
                        }
                    }

#if defined(B2NETWORK_PLATFORM_WINDOWS)
                }
                catch (Exception& e)
                {
                    B2NETWORK_LOG_ERROR(logger, "Caught exception (%s)", e.GetMessage().c_str());
					std::ignore = e;
                }
#endif
            }
        }

        auto finish = std::chrono::steady_clock::now();

        if (m_messageRouter.IsWaiting())
        {
            // don't sleep
        }
        else
        {
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count();

            auto duration = 10 - elapsed;
            if (duration > 10)
            {
                duration = 10;
            }

            if (duration > 0)
            {
                // NOTE max 100 updates per second
                FPlatformProcess::Sleep(duration / 1000.0f);
            }
        }
    };
}


int64 b2network::Client::GetSessionServerTime() const
{
	return m_authManager.GetSessionServerTime();
}
