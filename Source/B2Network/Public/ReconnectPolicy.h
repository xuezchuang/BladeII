#pragma once

#include "Config.h"

namespace b2network {

class ReconnectPolicy
{
private:
    bool m_enabled;
    float m_delayMillsec;

public:
    ReconnectPolicy(const bool enabled, const float delayMillisec);

    bool IsEnabled() const;
    float GetDelayMillisec();
};

}