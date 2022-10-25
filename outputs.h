#ifndef OUTPUTS_H__
#define OUTPUTS_H__

#include <Arduino.h>
#include <stdint.h>
#include <stdbool.h>

// pin configuration is staticly pulled from pinout.h in outputs.cpp
// Number of supported channels - 2 or 4
#define N_CHANNELS 4

class outputs_t
{
public:
    /**
      Initialize outputs controller
      @param frequency PWM frequency
    */
    void init(int frequency);

    /**
      Process outputs controller communication. Call in loop() function
    */
    void process()
    {
        // Nothing to do here, kept for consistency
    }

    /**
      Set output power
      @param output Output number: 1-4
      @param power_percent Power level in percent from 0 to 100
    */
    void set_power(uint8_t output, uint8_t power_percent);
};

#endif // OUTPUTS_H__
