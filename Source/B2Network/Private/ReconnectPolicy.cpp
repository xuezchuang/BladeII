#include "ReconnectPolicy.h"
#include "B2NetworkPlugin.h"



using namespace b2network;

ReconnectPolicy::ReconnectPolicy(const bool enabled, const float delayMillisec)
    :m_enabled(enabled),
     m_delayMillsec(delayMillisec)
{
}

bool ReconnectPolicy::IsEnabled() const
{
    return m_enabled;
}

float ReconnectPolicy::GetDelayMillisec()
{
    return m_delayMillsec;
}