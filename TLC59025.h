/*
 * TLC59025 LED driver
 */

#ifndef LED_H__
#define LED_H__

#include <Arduino.h>
#include <stdint.h>
#include <stdbool.h>
#include "pinout.h"
#include "TLC591x.h"
#include "debug.h"

class TLC59025_t
{
public:
    /** Class constructor
     * @param controller_type Type of controller mounted on board
     */
    TLC59025_t(int _pin_CLK, int _pin_SDI, int _pin_LE, int _pin_OE)
    {
        pin_CLK = _pin_CLK;
        pin_SDI = _pin_SDI;
        pin_LE = _pin_LE;
        pin_OE = _pin_OE;
    }

    /**
     Initialize driver
     */
    void init()
    {
        pinMode(pin_CLK, OUTPUT);
        pinMode(pin_SDI, OUTPUT);
        pinMode(pin_LE, OUTPUT);
        pinMode(pin_OE, OUTPUT);
        digitalWrite(pin_CLK, 0);
        digitalWrite(pin_SDI, 0);
        digitalWrite(pin_LE, 0);
        digitalWrite(pin_OE, 1); // Disable
        analogWrite(pin_OE, 252);
    }

    /**
     Write 16 bits of data
     */
    void write(unsigned int leds)
    {
        digitalWrite(pin_OE, 1); // Disable

        shiftOut(pin_SDI, pin_CLK, MSBFIRST, leds >> 8);   // Send upper byte
        shiftOut(pin_SDI, pin_CLK, MSBFIRST, leds & 0xFF); // Send lower byte

        digitalWrite(pin_LE, 1); // Latch pulse
        delayMicroseconds(10);
        digitalWrite(pin_LE, 0);

        digitalWrite(pin_OE, 0); // Enable
        analogWrite(pin_OE, 252);
    }

private:
    int pin_CLK;
    int pin_SDI;
    int pin_LE;
    int pin_OE;
};

#endif // LED_H__
