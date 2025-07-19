#pragma once

#include "Config.h"

namespace b2network {

class Endpoint
{
    std::string m_address;
    int m_port;

public:
    Endpoint()
        :m_address(""),
         m_port(0)
    {
    }

    Endpoint(const std::string& address, const int port)
        :m_address(address),
         m_port(port)
    {
    }

    Endpoint(const Endpoint& other)
        :m_address(other.m_address),
         m_port(other.m_port)
    {
    }

    Endpoint& operator=(const Endpoint& other)
    {
        m_address = other.m_address;
        m_port = other.m_port;
        return *this;
    }

    bool operator!=(const Endpoint& other) const
    {
        return (m_address != other.getAddress() || m_port != other.getPort());
    }

    const std::string& getAddress() const
    {
        return m_address;
    }

    int getPort() const
    {
        return m_port;
    }

    bool IsValid() const
    {
        return (!m_address.empty() && m_port > 0);
    }

    std::string ToString() const
    {
        std::ostringstream oss;
        oss << m_address << ":" << m_port;
        return oss.str();
    }
};

}
