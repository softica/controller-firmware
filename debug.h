/*
    Debug print class
*/

#ifndef DEBUG_H__
#define DEBUG_H__

#include <Arduino.h>
#include <stdint.h>
#include <stdbool.h>

class debug
{
private:
    inline static bool enabled;

public:
    static void init(bool _enabled)
    {
        enabled = _enabled;
    }

    static inline void println(String str)
    {
        if (enabled)
        {
            Serial.println(str);
        }
    }

    static inline void println(String str, int val)
    {
        if (enabled)
        {
            Serial.print(str);
            Serial.print(" = ");
            Serial.println(val);
        }
    }

    static inline void println(String str, int *arr, int len)
    {
        if (enabled)
        {
            Serial.print(str);
            Serial.print(" = [");
            for (int i = 0; i < len; i++)
            {
                Serial.print(" 0x");
                Serial.print(arr[i], HEX);
            }
            Serial.println("]");
        }
    }
};

#endif // DEBUG_H__
