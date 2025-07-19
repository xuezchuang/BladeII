#pragma once

#include "Config.h"
#include "ByteBuffer.h"

namespace b2network {

class ProtobufCodec
{
private:
    typedef b2network::ByteBuffer<> ByteBuffer;
    typedef std::shared_ptr<ByteBuffer> ByteBufferPtr;
    typedef google::protobuf::Message Message;
    typedef std::shared_ptr<Message> MessagePtr;

public:
    enum DecodeStatus
    {
        Ok,
        Error,
        NotEnoughHeader,
        NotEnoughBody
    };

    typedef std::pair<DecodeStatus, MessagePtr> DecodeResult;

public:
    ProtobufCodec();

    ByteBufferPtr Encode(const MessagePtr message);
    DecodeResult Decode(const char* data, uint32_t size, uint32_t& result, uint32_t& demand);
};

}
