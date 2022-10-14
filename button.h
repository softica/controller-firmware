#ifndef BUTTONS_H__
#define BUTTONS_H__

#include <Arduino.h>
#include <stdint.h>
#include <stdbool.h>
#include "Bounce2.h"
#include "debug.h"

class button_t
{
public:
    enum event_t
    {
        BUTTON_NO_EVENT,
        BUTTON_SHORT_PRESSED,
        BUTTON_LONG_PRESSED
    };
    /** Class constructor
       @param controller_type Type of controller mounted on board
    */
    button_t(int _pin, uint32_t _long_press_time_ms)
    {
        pin = _pin;
        long_press_time_ms = _long_press_time_ms;
        bounce = Bounce();
    }

    /**
      Initialize buttons controller
    */
    void init()
    {
        bounce.attach(pin, INPUT_PULLUP);
        bounce.interval(5);
        bounce.update();
        if (bounce.read() == LOW)
        {
            startup_ignore = true;
        }
        debug::println("button: init pin", pin);
    }

    /**
      Process buttons controller communication. Call in loop() function
    */
    void process()
    {
        bounce.update();
        if (bounce.read() == LOW && startup_ignore == true)
        {
            // Ignore low level at startup
        }
        else if (bounce.read() == HIGH && startup_ignore == true)
        {
            // Stop ignoring
            startup_ignore = false;
        }
        else if (bounce.read() == LOW && bounce.currentDuration() >= long_press_time_ms)
        {
            // Generate long press event when button is pressed for specified time. It don't have to be released
            if (long_press_detected == false)
            {
                debug::println("button: long press");
                long_press_detected = true;
                long_press_event = true;
            }
        }
        else if (bounce.rose())
        {
            // Generate short press event when button is pressed shorter than threshold time and released
            if (long_press_detected == false)
            {
                debug::println("button: short press", bounce.previousDuration());
                short_press_event = true;
            }
            else
            {
                long_press_detected = false;
            }
        }
    }

    /**
      Check if button event happened
    */
    bool has_event()
    {
        return short_press_event || long_press_event;
    }

    /**
      Get button event
    */
    event_t get_event()
    {
        if (short_press_event)
        {
            short_press_event = false;
            return BUTTON_SHORT_PRESSED;
        }
        else if (long_press_event)
        {
            long_press_event = false;
            return BUTTON_LONG_PRESSED;
        }
        else
        {
            return BUTTON_NO_EVENT;
        }
    }

private:
    int pin;
    uint32_t long_press_time_ms;
    bool long_press_detected = false;
    bool long_press_event = false;
    bool short_press_event = false;
    bool startup_ignore = false;
    Bounce bounce;
};

#endif // BUTTONS_H__
