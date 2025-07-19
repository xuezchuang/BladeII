#pragma once

#include "Config.h"
#include "ProtobufFactory.h"
#include "ConnectionManager.h"

namespace b2network {

class Connection;

class RequestManager
{
private:
    static const int CONTROL_TIMEOUT_MSEC = 13 * 1000;
    static const int REQUEST_TIMEOUT_MSEC = 31 * 1000; // NOTE must be longer than matchmaking timeout

    typedef google::protobuf::Message Message;
    typedef std::shared_ptr<Message> MessagePtr;
    typedef std::shared_ptr<Connection> ConnectionPtr;
    typedef ProtobufFactory::ServerType ServerType;
    typedef std::function<void(MessagePtr)> ResponseCallback;
    typedef std::chrono::time_point<std::chrono::steady_clock> TimeStamp;

    typedef std::tuple<MessagePtr, ResponseCallback, TimeStamp> ControlTask;
    typedef std::deque<ControlTask> ControlQueue;

    struct SendMessageTask
    {
        ProtobufFactory::MessageType messageType;
        MessagePtr request;
        ResponseCallback callback;
        TimeStamp timestamp;
    };

    struct RecvMessageTask
    {
        SendMessageTask sendMessageTask;

        MessagePtr response;
        bool outOfBand;
    };

    typedef std::deque<SendMessageTask> SendMessageQueue;
    typedef std::deque<RecvMessageTask> RecvMessageQueue;

    typedef std::function<void(ServerType, MessagePtr)> NotifyCallback;

    const ServerType m_serverType;

    ControlQueue m_sendControlQueue;
    ControlQueue m_recvControlQueue;

    SendMessageQueue m_sendMessageQueue;
    RecvMessageQueue m_recvMessageQueue;

    bool m_sendRequestPaused;

    NotifyCallback m_notifyCallback;

    // options
    bool m_responseOrderOption;

    std::string m_debug;

public:
    RequestManager(ProtobufFactory::ServerType serverType);

    void Initialize(NotifyCallback notifyCallback);
    void Finalize();

    bool Configure(const std::string& optionName, bool value);

    bool IsEmpty() const;

    void Reset();
    void ResetControls();
    void ResetReponses();
    void ResetMessages();

    bool PushControl(MessagePtr request, ResponseCallback responseCallback);
    bool PushRequest(MessagePtr request, ResponseCallback responseCallback);
    bool PushCommand(MessagePtr command);

    void PauseSendMessage();
    void ResumeSendMessage();

    void Update();

    void OnConnectedEvent();
    void OnConnectFailEvent();
    void OnDisconnectedEvent();
    void OnSendEvent(ConnectionPtr connection);
    void OnRecvEvent(MessagePtr message);
    void OnErrorEvent();

    std::string Dump();
};

}
