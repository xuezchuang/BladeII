#pragma once

#include "Config.h"

namespace b2network {

class StringHash
{
public:
    static uint32_t HashCode(const std::string& string)
    {
        uint32_t hash = 0;

        const int length = string.length();
        if (length > 0)
        {
            for (int i = 0; i < length; i++)
            {
                hash = 31 * hash + (uint32_t)(string.at(i));
            }
        }

        return hash;
    }
};

}
