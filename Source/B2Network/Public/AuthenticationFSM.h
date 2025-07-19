#pragma once

#include "Config.h"

#define DECLARE_AUTHENTICATION_STATE(className) \
    class className : public State \
    { \
        virtual void Enter(AuthenticationManager* manager) override; \
        virtual void Leave(AuthenticationManager* manager) override; \
        virtual State* Update(Event event, int errorCode, AuthenticationManager* manager) override; \
        virtual std::string GetStateName() override { return #className; } \
    }

namespace b2network {

class AuthenticationManager;

class AuthenticationFSM
{
public:
    enum Event
    {
        // tick
        EVENT_TICK_UPDATE,

        // session server
        EVENT_CONNECTED_TO_SESSION,
        EVENT_CONNECT_FAIL_TO_SESSION,
        EVENT_DISCONNECTED_FROM_SESSION,
        EVENT_SESSION_REQUEST,
        EVENT_SIGNIN_SESSION_OK,
        EVENT_SIGNIN_SESSION_FAILED,
        EVENT_SIGNIN_SESSION_INVALID_TOKEN,
        EVENT_VERSION_MISMATCH,
        EVENT_PROTOCOL_MISMATCH,
        EVENT_SESSION_BANNED,
        EVENT_HELLO_SESSION_OK,
        EVENT_HELLO_SESSION_ERROR,

        // channel server
        EVENT_CONNECTED_TO_CHANNEL,
        EVENT_CONNECT_FAIL_TO_CHANNEL,
        EVENT_DISCONNECTED_FROM_CHANNEL,
        EVENT_CHANNEL_REQUEST,
        EVENT_SIGNIN_CHANNEL_OK,
        EVENT_SIGNIN_CHANNEL_FAILED,
        EVENT_SIGNIN_CHANNEL_INVALID_TOKEN,
		EVENT_HELLO_CHANNEL_OK,
    };

private:
    class State
    {
    protected:
        // HelloSessionState
        std::chrono::time_point<std::chrono::steady_clock> m_lastHelloSession;
        int m_maxHelloSessionTries = 0;

        // AuthenticatedState
        bool m_signedInChannel;
        std::chrono::time_point<std::chrono::steady_clock> m_lastSessionPing;

        // ReconnectingState
        bool m_waiting;
        int m_backoff;
        std::chrono::time_point<std::chrono::steady_clock> m_updated;

    public:
        virtual void Enter(AuthenticationManager* manager) = 0;
        virtual void Leave(AuthenticationManager* manager) = 0;
        virtual State* Update(Event event, int errorCode, AuthenticationManager* manager) = 0;
        virtual std::string GetStateName() = 0;
    };

    DECLARE_AUTHENTICATION_STATE(InitialState);
	DECLARE_AUTHENTICATION_STATE(HelloSessionState);
    DECLARE_AUTHENTICATION_STATE(SignInSessionState);
    DECLARE_AUTHENTICATION_STATE(ReconnectingState);
    DECLARE_AUTHENTICATION_STATE(AuthenticatedState);
    DECLARE_AUTHENTICATION_STATE(NoServiceState);

    static InitialState s_initial;
	static HelloSessionState s_helloSession;
    static SignInSessionState s_signInSession;
    static ReconnectingState s_reconnecting;
    static AuthenticatedState s_authenticated;
    static NoServiceState s_noService;

    State* m_current;
    AuthenticationManager* m_manager;

public:
    AuthenticationFSM();
    ~AuthenticationFSM();

    void Start(AuthenticationManager* manager);
    void Stop();
    void Update(Event event, int error = 0);

    bool IsActivated() const;

    std::string GetState();

    std::string ToString();
};

}
