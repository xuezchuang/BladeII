#include "LeakyBucket.h"
#include "B2NetworkPlugin.h"

#include "Config.h"
#include "Exception.h"


using namespace b2network;

LeakyBucket::LeakyBucket()
    :m_enabled(false),
     m_capacity(1000),
     m_interval(0)
{
}

void LeakyBucket::Init(const int capacity, const int interval)
{
    if (capacity > 0)
    {
        m_capacity = capacity;
    }

    if (interval > 0)
    {
        m_interval = interval;
    }

    const auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();

    m_remained = m_capacity;
    m_nextUpdateTime = now + m_interval;

    m_enabled = true;
}

int LeakyBucket::Update()
{
    return Update(1);
}

int LeakyBucket::Update(const unsigned int count)
{
    if (!m_enabled)
    {
        return 0;
    }

    const int64 now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();

    if (now >= m_nextUpdateTime)
    {
        m_remained = m_capacity;
        m_nextUpdateTime = now + m_interval;
    }

    if (m_remained >= count)
    {
        m_remained -= count;

        return 0;
    }
    else
    {
        const auto duration = m_nextUpdateTime - now;

        if (duration < 0)
        {
            return 0;
        }
        else if (duration > 1000)
        {
            return 1000;
        }
        else
        {
            return duration;
        }
    }
}
