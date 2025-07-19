#pragma once

#include "Config.h"

namespace b2network {

class Exception
{
protected:
    std::string m_message;

public:
    Exception()
    {
    }

    Exception(const std::string& message)
        :m_message(message)
    {
    }

	virtual ~Exception() {}

    virtual const std::string& GetMessage() const
    {
        return m_message;
    }
};

class ByteBufferException : public Exception
{
public:
    ByteBufferException(const std::string& message)
    {
        m_message = std::string("ByteBuffer ") + message;
    }
};

class ProtobufCodecException : public Exception
{
public:
    ProtobufCodecException(const std::string& message)
    {
        m_message = std::string("ProtobufCodec ") + message;
    }
};

class ConnectionException : public Exception
{
public:
    ConnectionException(const std::string& message)
    {
        m_message = std::string("Connection ") + message;
    }
};

}
