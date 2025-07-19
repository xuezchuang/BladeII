#include "RequestManager.h"
#include "B2NetworkPlugin.h"

#include "LogManager.h"
#include "Exception.h"
#include "ProtobufFactory.h"
#include "Connection.h"

#include "MasterDataCacher.h"

using namespace b2network;

#if B2_BUILD_LEVEL < B2_BUILD_LEVEL_QA // !B2_BUILD_LEVEL_LIVE, !!B2_BUILD_LEVEL_QA
static auto logger = LogManager::GetLogger("RequestManager");
#endif

RequestManager::RequestManager(ProtobufFactory::ServerType serverType)
    :m_serverType(serverType),
     m_sendRequestPaused(true),
     m_responseOrderOption(true)
{
    switch (m_serverType)
    {
    case ProtobufFactory::Session:
        m_debug = "Session";
        break;

    case ProtobufFactory::Channel:
        m_debug = "Channel";
        break;

    default:
        m_debug = "?";
        break;
    }
}

void RequestManager::Initialize(NotifyCallback notifyCallback)
{
    m_notifyCallback = notifyCallback;

    m_sendRequestPaused = true;

    m_sendControlQueue.clear();
    m_recvControlQueue.clear();

    m_sendControlQueue.clear();
    m_recvControlQueue.clear();

    B2NETWORK_LOG_DEBUG(logger, "Initialized");
}

void RequestManager::Finalize()
{
    m_sendRequestPaused = true;

    m_notifyCallback = [this](ServerType serverType, MessagePtr notify)
    {
        // TODO
    };

    m_sendControlQueue.clear();
    m_recvControlQueue.clear();

    m_sendControlQueue.clear();
    m_recvControlQueue.clear();

    B2NETWORK_LOG_DEBUG(logger, "Finalized");
}

bool RequestManager::Configure(const std::string& optionName, bool value)
{
    if (optionName.compare("ResponseOrder") == 0)
    {
        m_responseOrderOption = value;

        B2NETWORK_LOG_INFO(logger, "ResponseOrder %s", m_responseOrderOption ? "on" : "off");

        return true;
    }

    return false;
}

bool RequestManager::IsEmpty() const
{
    return (m_sendControlQueue.empty() && m_recvControlQueue.empty()) &&
        (m_sendMessageQueue.empty() && m_recvMessageQueue.empty());
 }

void RequestManager::Reset()
{
    B2NETWORK_LOG_DEBUG(logger, "Reset...");

    ResetControls();
    ResetMessages();
}

void RequestManager::ResetControls()
{
    B2NETWORK_LOG_DEBUG(logger, "Reset controls...");

    m_sendControlQueue.clear();
    m_recvControlQueue.clear();
}

void RequestManager::ResetReponses()
{
    B2NETWORK_LOG_DEBUG(logger, "Reset reponses...");

    for (auto message : m_recvMessageQueue)
    {
        // TODO response error before clear
    }

    m_recvMessageQueue.clear();
}

void RequestManager::ResetMessages()
{
    B2NETWORK_LOG_DEBUG(logger, "Reset messages...");

    for (auto message : m_sendMessageQueue)
    {
        // TODO response error before clear
    }

    m_sendMessageQueue.clear();

    for (auto message : m_recvMessageQueue)
    {
        // TODO response error before clear
    }

    m_recvMessageQueue.clear();
}


bool RequestManager::PushControl(MessagePtr request, ResponseCallback callback)
{
    const auto now = std::chrono::steady_clock::now();

    m_sendControlQueue.emplace_back(request, callback, now);

    B2NETWORK_LOG_DEBUG(logger, "%s Control %s pushed %s", m_debug.c_str(), request->GetTypeName().c_str(), Dump().c_str());

    return true;
}

bool RequestManager::PushRequest(MessagePtr request, ResponseCallback callback)
{
    const auto now = std::chrono::steady_clock::now();

    // TODO fix
    // clear pending requests before push
    //m_sendMessageQueue.clear();

	const auto masterDataHash = ProtobufFactory::GetMasterDataHash();

	if (ProtobufFactory::IsTypeOf(request, b2::protocol::session::GetMasterDataRequest::default_instance()))
	{
		auto Result = MasterDataCacher::GetInstance().Query(CacheCategory::Key_MasterData);
		if ((!masterDataHash.empty() && !Result.Hash.empty()) && !( FString(Result.Hash.c_str()).Compare(masterDataHash.c_str(), ESearchCase::IgnoreCase /*stricmp*/) ))
		{
			auto getMasterDataResponse = std::make_shared<b2::protocol::session::GetMasterDataResponse>();
			getMasterDataResponse->ParseFromArray(Result.PBContent.GetData(), Result.PBContent.Num());

			callback(getMasterDataResponse);
			return true;
		}
	}

    SendMessageTask task{ProtobufFactory::Request, request, callback, now};
    m_sendMessageQueue.push_back(task);

    B2NETWORK_LOG_DEBUG(logger, "%s Request %s pushed %s", m_debug.c_str(), request->GetTypeName().c_str(), Dump().c_str());

    return true;
}

bool RequestManager::PushCommand(MessagePtr command)
{
    auto now = std::chrono::steady_clock::now();

    SendMessageTask task{ProtobufFactory::Command, command, [](MessagePtr) {}, now};
    m_sendMessageQueue.push_back(task);

    B2NETWORK_LOG_DEBUG(logger, "%s Command %s pushed %s", m_debug.c_str(), command->GetTypeName().c_str(), Dump().c_str());

    return true;
}

void RequestManager::PauseSendMessage()
{
    m_sendRequestPaused = true;

    B2NETWORK_LOG_DEBUG(logger, "%s Send message paused", m_debug.c_str());
}

void RequestManager::ResumeSendMessage()
{
    m_sendRequestPaused = false;

    B2NETWORK_LOG_DEBUG(logger, "%s Send message resumed, send %d message(s) recv %d messages(s)", m_debug.c_str(), m_sendMessageQueue.size(), m_recvMessageQueue.size());
}

void RequestManager::Update()
{
    const auto now = std::chrono::steady_clock::now();

    // receive message timeout processing

    for (auto it = m_recvControlQueue.begin(); it != m_recvControlQueue.end(); ++it)
    {
        auto task = *it;

        auto timeStamp = std::get<2>(task);

        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - timeStamp);
        if (elapsed.count() > CONTROL_TIMEOUT_MSEC)
        {
#if defined(B2NETWORK_ENVIRONMENT_GAME)
            B2NETWORK_LOG_ERROR(logger, "=== %s Drop control response in %d msec (timeout %d)...",
                m_debug.c_str(),
                elapsed,
                CONTROL_TIMEOUT_MSEC);

            it = m_recvControlQueue.erase(it);

            return;
#endif
        }
    }

    for (auto it = m_recvMessageQueue.begin(); it != m_recvMessageQueue.end(); ++it)
    {
        auto task = *it;

        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - task.sendMessageTask.timestamp);
        if (elapsed.count() > REQUEST_TIMEOUT_MSEC)
        {
#if defined(B2NETWORK_ENVIRONMENT_GAME)
            B2NETWORK_LOG_ERROR(logger, "=== %s Drop message response %s in %d msec (timeout %d)...",
                m_debug.c_str(),
                task.sendMessageTask.request->GetTypeName().c_str(),
                elapsed,
                REQUEST_TIMEOUT_MSEC);

            it = m_recvMessageQueue.erase(it);

            return;
#endif
        }
    }

    if (m_serverType == ProtobufFactory::Channel)
    {
        // channel server connection timeout
    }
}

void RequestManager::OnConnectedEvent()
{
    B2NETWORK_LOG_DEBUG(logger, "%s Received connected event", m_debug.c_str());
}

void RequestManager::OnConnectFailEvent()
{
    B2NETWORK_LOG_DEBUG(logger, "%s Received connect fail event", m_debug.c_str());
}

void RequestManager::OnDisconnectedEvent()
{
    B2NETWORK_LOG_DEBUG(logger, "%s Received disconnected event", m_debug.c_str());

    B2NETWORK_LOG_DEBUG(logger, "Clearing control send queue... %d", m_sendControlQueue.size());
    m_sendControlQueue.clear();

    B2NETWORK_LOG_DEBUG(logger, "Clearing control receive queue... %d", m_recvControlQueue.size());
    m_recvControlQueue.clear();

    for (auto message : m_sendMessageQueue)
    {
        // TODO response error before clear
    }

    m_sendMessageQueue.clear();

    for (auto message : m_recvMessageQueue)
    {
        // TODO response error before clear
    }

    m_recvMessageQueue.clear();

    // TODO fix this buggy stuff
    ResumeSendMessage();
}

void RequestManager::OnSendEvent(ConnectionPtr connection)
{
    if (!m_sendControlQueue.empty())
    {
        auto controlTask = m_sendControlQueue.front();

        auto request = std::get<0>(controlTask);
        auto callback = std::get<1>(controlTask);

        if (connection->Send(request))
        {
            m_sendControlQueue.pop_front();
            m_recvControlQueue.push_back(controlTask);
        }
        else
        {
            // TODO retry later
            B2NETWORK_LOG_DEBUG(logger, "Could not send control %s", request->GetTypeName().c_str());
        }

        return;
    }

    if (!m_recvControlQueue.empty())
    {
        // Wait for the response of the control request
        return;
    }

    if (m_sendRequestPaused)
    {
        // Paused
        return;
    }

    if (!m_recvMessageQueue.empty())
    {
        // NOTE Do not wait for the response of the request has been sent
        // due to PingRequest must be sent in time

        // TODO Implement not to send the same request type before receiving the response
    }

    if (!m_sendMessageQueue.empty())
    {
        int count = 0;

        while (!m_sendMessageQueue.empty())
        {
            auto task = m_sendMessageQueue.front();

            if (connection->Send(task.request))
            {
                m_sendMessageQueue.pop_front();

                count++;

                if (task.messageType == ProtobufFactory::Request)
                {
                    // NOTE update timestamp to prevent timeout
                    task.timestamp = std::chrono::steady_clock::now();

                    m_recvMessageQueue.push_back(RecvMessageTask{task, nullptr, false});
                    break;
                }
            }
            else
            {
                // TODO retry later
                B2NETWORK_LOG_DEBUG(logger, "Could not send request %s", task.request->GetTypeName().c_str());
                break;
            }
        }

        B2NETWORK_LOG_DEBUG(logger, "Sent %d request(s)", count, Dump().c_str());
    }
}

void RequestManager::OnRecvEvent(MessagePtr message)
{
    B2NETWORK_LOG_INFO(logger, "%s Received message %s...", m_debug.c_str(), message->GetTypeName().c_str());

    if (ProtobufFactory::IsResponse(message))
    {
        // Control Message

        while (!m_recvControlQueue.empty())
        {
            auto controlTask = m_recvControlQueue.front();

            auto request = std::get<0>(controlTask);
            auto callback = std::get<1>(controlTask);

            if (ProtobufFactory::IsResponseOf(request, message))
            {
                m_recvControlQueue.pop_front();

                B2NETWORK_LOG_DEBUG(logger, "%s Invoking control %s callback...",
                    m_debug.c_str(), message->GetTypeName().c_str());

                callback(message);

                return;
            }
            else
            {
                B2NETWORK_LOG_ERROR(logger, "%s Control Response does not match Request(%s) Response(%s)",
                    m_debug.c_str(), request->GetTypeName().c_str(), message->GetTypeName().c_str());

                // NOTE if not skip this, we can't reconnect to the server
                // TODO fix to make a safe way to ignore responses that are lost
                m_recvControlQueue.pop_front();

                continue;
            }
        }

        // Response

        if (!m_recvMessageQueue.empty())
        {
            for (auto it = m_recvMessageQueue.begin(); it != m_recvMessageQueue.end(); ++it)
            {
                auto& task = *it;

                if (!task.response)
                {
                    if (ProtobufFactory::IsResponseOf(task.sendMessageTask.request, message))
                    {
                        if (ProtobufFactory::IsTypeOf(message, b2::protocol::session::PingResponse::default_instance()) ||
                            ProtobufFactory::IsTypeOf(message, b2::protocol::channel::RoomHeartbeatResponse::default_instance()))
                        {
                            // set the flag for out of band
                            task.outOfBand = true;
                        }

                        // tie the response to the task to invoke its callback later
                        task.response = message;
                        break;
                    }
                }
            }

            bool deferred = true;

            for (auto it = m_recvMessageQueue.begin(); it != m_recvMessageQueue.end(); )
            {
                auto task = *it;

                auto now = std::chrono::steady_clock::now();
                auto rtt = std::chrono::duration_cast<std::chrono::milliseconds>(now - task.sendMessageTask.timestamp);

                if (!task.response)
                {
                    if (deferred)
                    {
                        B2NETWORK_LOG_ERROR(logger, "=== %s Waiting request %s with %d callback(s)...",
                            m_debug.c_str(),
                            task.sendMessageTask.request->GetTypeName().c_str(),
                            m_recvMessageQueue.size());
                    }

                    if (m_responseOrderOption)
                    {
                        if (task.outOfBand)
                        {
                            // NOTE do not defer if it's out of band!
                        }
                        else
                        {
                            // NOTE defer the execution to invoke response callback in order of request
                            break;
                        }
                    }

                    ++it;
                }
                else
                {
                    // NOTE Erase it before invoking due to re-enterancy
                    m_recvMessageQueue.erase(it);

                    B2NETWORK_LOG_INFO(logger, "<<< %s Invoking request %s callback (%d msec)...",
                        m_debug.c_str(), task.sendMessageTask.request->GetTypeName().c_str(), rtt.count());

                    task.sendMessageTask.callback(task.response);

                    deferred = false;

                    if (m_recvMessageQueue.empty())
                    {
                        break;
                    }
                    else
                    {
                        // NOTE Reset iterator due to re-enterancy
                        it = m_recvMessageQueue.begin();

                        // TODO invoke only one callback in the same tick?
                        continue;
                    }
                }
            }

            // Out of band

            for (auto it = m_recvMessageQueue.begin(); it != m_recvMessageQueue.end(); )
            {
                auto task = *it;

                auto now = std::chrono::steady_clock::now();
                auto rtt = std::chrono::duration_cast<std::chrono::milliseconds>(now - task.sendMessageTask.timestamp);

                if (task.response && task.outOfBand)
                {
                    // NOTE Erase it before invoking due to re-enterancy
                    m_recvMessageQueue.erase(it);

                    B2NETWORK_LOG_INFO(logger, "<<< %s Invoking OOB request %s callback (%d msec)...",
                        m_debug.c_str(), task.sendMessageTask.request->GetTypeName().c_str(), rtt.count());

                    task.sendMessageTask.callback(task.response);

                    if (m_recvMessageQueue.empty())
                    {
                        break;
                    }
                    else
                    {
                        // NOTE Reset iterator due to re-enterancy
                        it = m_recvMessageQueue.begin();

                        // TODO invoke only one callback in the same tick?
                        continue;
                    }
                }
                else
                {
                    ++it;
                }
            }
        }
        else
        {
            // TODO not implemented yet
            B2NETWORK_LOG_ERROR(logger, "%s Response queue is empty for %s", m_debug.c_str(), message->GetTypeName().c_str());
        }
    }
    else if (ProtobufFactory::IsNotify(message)) 
    {
        if (m_notifyCallback)
        {
            m_notifyCallback(m_serverType, message);
        }
    }
    else
    {
#if defined(B2NETWORK_PLATFORM_WINDOWS)
        throw Exception("Invalid Message Type");
#elif defined(B2NETWORK_PLATFORM_ANDROID)
        B2NETWORK_LOG_ERROR(logger, "Invalid Message Type");
#elif defined(B2NETWORK_PLATFORM_IOS)
        B2NETWORK_LOG_ERROR(logger, "Invalid Message Type");
#else
#error Platform not supported
#endif
    }
}

void RequestManager::OnErrorEvent()
{
    for (auto it = m_recvMessageQueue.begin(); it != m_recvMessageQueue.end(); ++it)
    {
        auto task = *it;

        if (ProtobufFactory::IsTypeOf(task.sendMessageTask.request, b2::protocol::session::MatchmakingRequest::default_instance()))
        {
            auto response = std::make_shared<b2::protocol::session::MatchmakingResponse>();
            response->mutable_result()->set_error_code(8406);

            task.sendMessageTask.callback(response);
        }
    }
}

std::string RequestManager::Dump()
{
    std::ostringstream oss;
    oss << "{ " << "type(" << m_debug.c_str() << ") "
        << "pause(" << (m_sendRequestPaused ? "T" : "F") << ") "
        << "ctl(snd=" << m_sendControlQueue.size()
        << ":rcv=" << m_recvControlQueue.size() << ") "
        << "msg(snd=" << m_sendMessageQueue.size()
        << ":rcv=" << m_recvMessageQueue.size() << ") "
        << "responseOrder(" << (m_responseOrderOption ? "on" : "off") << ") }";

    return oss.str();
}
