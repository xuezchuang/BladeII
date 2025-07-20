#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <assert.h>

#include <string>
#include <sstream>
#include <memory>
#include <functional>
#include <vector>
#include <deque>
#include <list>
#include <unordered_map>
#include <iterator>
#include <algorithm>
#include <chrono>
#include <ctime>

#include "CoreMinimal.h"

#if defined(B2NETWORK_PLATFORM_WINDOWS)
#include "Windows/AllowWindowsPlatformTypes.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "Windows/HideWindowsPlatformTypes.h"
#elif defined(B2NETWORK_PLATFORM_ANDROID)
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <fcntl.h>
#include <assert.h>
#elif defined(B2NETWORK_PLATFORM_IOS)
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <fcntl.h>
#include <assert.h>
#else
#error Platform not specified
#endif

#ifndef B2NETWORK_PLATFORM_IOS
// TODO fix
#pragma warning(disable:4125)
#endif

#include <google/protobuf/message.h>

//#define B2NETWORK_ENVIRONMENT_TOOL
#define B2NETWORK_ENVIRONMENT_GAME

#if defined(B2NETWORK_ENVIRONMENT_TOOL)
#define B2NETWORK_API 
#include "protobuf/CommonData.pb.h"
#include "protobuf/SessionProtocol.pb.h"
#include "protobuf/ChannelProtocol.pb.h"
#include "protobuf/SessionConsole.pb.h"
#elif defined(B2NETWORK_ENVIRONMENT_GAME)
#include "protobuf/CompiledProtoFiles/CommonData.pb.h"
#include "protobuf/CompiledProtoFiles/SessionProtocol.pb.h"
#include "protobuf/CompiledProtoFiles/ChannelProtocol.pb.h"
#include "protobuf/CompiledProtoFiles/SessionConsole.pb.h"
#else
#error Environment not specified
#endif

DECLARE_LOG_CATEGORY_EXTERN(LogB2Network, Log, All);
