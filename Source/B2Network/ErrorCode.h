#pragma once

namespace b2network {

enum class ErrorCode : int
{
    // Success
    Success = 0,

    // Error

    ServerEndpointNotFound = 2000,
    CommandRouteNotFound = 2001,
    NoServiceStateInAuthFSM = 2002,

    HelloSessionFailed = 3000,
    SessionPingFailed = 3001,
    SessionConnectError = 3002,
	SessionPingFailed2 = 3003,
	SigningSessionInvalidToken = 3004,
	SessionProtocolMissMatch = 3005,
	SessionBan = 3006,
	SessionReconnectedFail = 3007,
	SessionRecv_Aborted = 3008,
	SessionRecv_Disconnected = 3009,
	SessionRecv_Error = 3010,

    ChannelConnectError = 4000,
	ChannelForePingFailed = 4001,
	ChannelBackPingFailed = 4002,
	ChannelRecv_Aborted = 4003,
	ChannelRecv_Disconnected = 4004,
	ChannelRecv_Error = 4005,

    // RT server error codes

    ErrorTryAgainLater = 8410,
};

enum class DisconnectedErrorCode : int 
{
	SocketDisconnected = 100,
	BackgroundPingTimeOut = 201,
	HeartBeatResponseError = 202,
	CreateProtoBuffer = 300,
	SendPacketFail = 400,
	ReceivePacketFail = 500,
};
}
