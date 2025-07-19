#pragma once

namespace b2network {

class LeakyBucket
{
private:
    bool m_enabled;
    unsigned int m_capacity; // count
    int m_interval; // msec

    unsigned int m_remained;
    int64 m_nextUpdateTime;

public:
    LeakyBucket();

    void Init(const int capacity, const int interval);

    int Update();
    int Update(const unsigned int count);
};

}
