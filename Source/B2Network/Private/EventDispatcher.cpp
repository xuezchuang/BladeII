#include "EventDispatcher.h"
#include "B2NetworkPlugin.h"

#include "ErrorCode.h"
#include "LogManager.h"
#include "Connection.h"


using namespace b2network;

#if B2_BUILD_LEVEL < B2_BUILD_LEVEL_QA // !B2_BUILD_LEVEL_LIVE, !!B2_BUILD_LEVEL_QA
static auto logger = LogManager::GetLogger("EventDispatcher");
#endif

EventDispatcher::EventDispatcher()
    :m_enabled(false)
{
}

void EventDispatcher::Start()
{
    B2NETWORK_LOG_DEBUG(logger, "Starting...");

    m_enabled = true;
}

void EventDispatcher::Stop()
{
    B2NETWORK_LOG_DEBUG(logger, "Stopping...");

    m_enabled = false;

    m_connectedCallbacks.clear();
    m_disconnectedCallbacks.clear();

    m_requestFilters.clear();
    m_responseFilters.clear();
    m_notifyFilters.clear();
}

void EventDispatcher::AddConnectedCallback(ConnectionCallback callback)
{
    m_connectedCallbacks.emplace_back(callback);
}

void EventDispatcher::AddConnectFailCallback(ConnectionCallback callback)
{
    m_connectFailCallbacks.emplace_back(callback);
}

void EventDispatcher::AddDisconnectedCallback(ConnectionCallback callback)
{
    m_disconnectedCallbacks.emplace_back(callback);
}

void EventDispatcher::OnConnectedEvent(Connection::ServerType serverType)
{
    if (m_enabled)
    {
        std::for_each(m_connectedCallbacks.begin(), m_connectedCallbacks.end(),
            [serverType](ConnectionCallback callback)
            {
                callback(serverType, (int) ErrorCode::Success);
            });
    }
}

void EventDispatcher::OnConnectFailEvent(Connection::ServerType serverType, int errorCode)
{
    if (m_enabled)
    {
        std::for_each(m_connectFailCallbacks.begin(), m_connectFailCallbacks.end(),
            [serverType, errorCode](ConnectionCallback callback)
            {
                callback(serverType, errorCode);
            });
    }
}

void EventDispatcher::OnDisconnectedEvent(Connection::ServerType serverType, int errorCode)
{
    if (m_enabled)
    {
        std::for_each(m_disconnectedCallbacks.begin(), m_disconnectedCallbacks.end(),
            [serverType, errorCode](ConnectionCallback callback)
            {
                callback(serverType, errorCode);
            });
    }
}

void EventDispatcher::AddRequestFilter(MessageFilter filter)
{
    m_requestFilters.emplace_back(filter);
}

void EventDispatcher::AddResponseFilter(MessageFilter filter)
{
    m_responseFilters.emplace_back(filter);
}

void EventDispatcher::AddNotifyFilter(MessageFilter filter)
{
    m_notifyFilters.emplace_back(filter);
}

void EventDispatcher::OnRequestEvent(MessagePtr request)
{
    if (m_enabled)
    {
        std::for_each(m_requestFilters.begin(),
            m_requestFilters.end(),
            [request](MessageFilter filter)
            {
                B2NETWORK_LOG_DEBUG(logger, "Invoking request(%s) filter...", request->GetTypeName().c_str());
                filter(request);
            });
    }
}

void EventDispatcher::OnResponseEvent(MessagePtr response)
{
    if (m_enabled)
    {
        std::for_each(m_responseFilters.begin(),
            m_responseFilters.end(),
            [response](MessageFilter filter)
            {
                B2NETWORK_LOG_DEBUG(logger, "Invoking response(%s) filter...", response->GetTypeName().c_str());
                filter(response);
            });
    }
}

void EventDispatcher::OnNotifyEvent(MessagePtr notify)
{
    if (m_enabled)
    {
        std::for_each(m_notifyFilters.begin(),
            m_notifyFilters.end(),
            [notify](MessageFilter filter)
            {
                B2NETWORK_LOG_DEBUG(logger, "Invoking notify(%s) filter...", notify->GetTypeName().c_str());
                filter(notify);
            });
    }
}

void EventDispatcher::AddEventCallback(const Event event, EventCallback callback)
{
    m_eventCallbacks[event] = callback;
}

void EventDispatcher::Emit(const Event event, int errorCode /*= 0*/)
{
    auto it = m_eventCallbacks.find(event);
    if (it != m_eventCallbacks.end())
    {
        it->second(event, errorCode);
    }
    else
    {
        B2NETWORK_LOG_ERROR(logger, "Event(%d) handler not found (%d)!", event, errorCode);
    }
}
