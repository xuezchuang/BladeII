#include "ProtobufCodec.h"
#include "B2NetworkPlugin.h"

#include "Config.h"
#include "LogManager.h"
#include "Exception.h"
#include "StringHash.h"
#include "ProtobufFactory.h"


using namespace b2network;

#if B2_BUILD_LEVEL < B2_BUILD_LEVEL_QA // !B2_BUILD_LEVEL_LIVE, !!B2_BUILD_LEVEL_QA
static auto logger = LogManager::GetLogger("ProtobufCodec");
#endif

ProtobufCodec::ProtobufCodec()
{
}

std::shared_ptr<ByteBuffer<>> ProtobufCodec::Encode(const MessagePtr message)
{
    auto buffer = std::make_shared<ByteBuffer>();

    const uint32_t payloadSize = message->ByteSize();
    const uint32_t payloadType = StringHash::HashCode(message->GetDescriptor()->full_name());

    buffer->Reserve(sizeof(payloadSize) + sizeof(payloadType) + payloadSize);

    const uint32_t size = htonl(payloadSize);
    buffer->Write((char*)(&size), sizeof(size));

    const uint32_t type = htonl(payloadType);
    buffer->Write((char*)(&type), sizeof(type));

    bool result = message->SerializeToArray(buffer->WritePtr(), payloadSize);
    if (!result)
    {
#if defined(B2NETWORK_PLATFORM_WINDOWS)
        throw ProtobufCodecException("Encode Error");
#elif defined(B2NETWORK_PLATFORM_ANDROID)
        B2NETWORK_LOG_ERROR(logger, "Encode Error");
        return nullptr;
#elif defined(B2NETWORK_PLATFORM_IOS)
        B2NETWORK_LOG_ERROR(logger, "Encode Error");
        return nullptr;
#else
#error Platform not supported
#endif
    }
    else
    {
        buffer->Seek(ByteBuffer::Wr, payloadSize);

        B2NETWORK_LOG_TRACE(logger, "Encoded %d byte(s)", buffer->Size());

        return buffer;
    }
}

ProtobufCodec::DecodeResult
ProtobufCodec::Decode(const char* data, uint32_t size, uint32_t& result, uint32_t& demand)
{
    uint32_t remain = size;

    uint32_t payloadSize = 0;
    uint32_t payloadType = 0;

    if (remain < sizeof(payloadSize) + sizeof(payloadType))
    {
        result = 0;
        demand = (sizeof(payloadType) + sizeof(payloadType)) - remain;
        return std::make_pair(NotEnoughHeader, nullptr);
    }

    char* ptr = const_cast<char*>(data);

    memcpy(&payloadSize, ptr, sizeof(payloadSize));
    payloadSize = ntohl(payloadSize);
    ptr += sizeof(payloadSize);
    remain -= sizeof(payloadSize);

    memcpy(&payloadType, ptr, sizeof(payloadType));
    payloadType = ntohl(payloadType);
    ptr += sizeof(payloadType);
    remain -= sizeof(payloadType);

    if (remain < payloadSize)
    {
        result = 0;
        demand = payloadSize - remain;
        return std::make_pair(NotEnoughBody, nullptr);
    }

    auto message = ProtobufFactory::CreateMessage(payloadType);

    bool br = message->ParseFromArray(ptr, payloadSize);
    if (!br)
    {
        B2NETWORK_LOG_ERROR(logger, "Could not parse message %s, %d",
            message->GetDescriptor()->name().c_str(), payloadSize);

#if defined(B2NETWORK_PLATFORM_WINDOWS)
        throw ProtobufCodecException("Decode Error");
#elif defined(B2NETWORK_PLATFORM_ANDROID)
        B2NETWORK_LOG_ERROR(logger, "Decode Error");
        return std::make_pair(Error, nullptr);
#elif defined(B2NETWORK_PLATFORM_IOS)
        B2NETWORK_LOG_ERROR(logger, "Decode Error");
        return std::make_pair(Error, nullptr);
#else
#error Platform not supported
#endif
    }
    else
    {
        result = sizeof(payloadSize) + sizeof(payloadType) + payloadSize;
        B2NETWORK_LOG_TRACE(logger, "Decoded %d byte(s)", result);

        return std::make_pair(Ok, message);
    }
}
