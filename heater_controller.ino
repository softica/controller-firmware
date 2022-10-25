#include <Arduino.h>
#include <stdint.h>
#include <stdbool.h>
#include "led.h"
#include "outputs.h"
#include "button.h"
#include "serial_parser.h"
#include "program_logic.h"
#include "debug.h"
#include "pinout.h"

// AVR fuses: Extended:F9, High:98, Low:FF

// ================== Configuration of device ==================
// For different hardware version (PCB), change HW_VERSION in pinout.h



// Minimum long press time for button
#define LONG_PRESS_TIME_MS 600
// PWM frequency - higher means more heating of a driver
#define PWM_FREQUENCY 50
// LED blink period in ms
#define LED_BLINK_PERIOD_MS 1000
// LED blink duty cycle in ms
#define LED_BLINK_DUTY_CYCLE_MS (LED_BLINK_PERIOD_MS / 3)
// Serial port baudrate
#define BAUDRATE 9600
// Enable debug prints
#define DEBUG_ENABLED false

// power level steps
const uint8_t power_level_steps_percent[POWER_LEVEL_N + 1] = {0, 25, 50, 75, 100};

// ================== End of configuration ==================

// Program variables
button_t button(button_pin, LONG_PRESS_TIME_MS);
serial_parser_t serial_parser;
program_logic_t program_logic(N_CHANNELS);
led_t leds(LED_CONTROLLER_TYPE, SCK, MOSI, LED_LE_PIN, LED_OE_PIN);
outputs_t outputs; // pin configuration is staticly pulled from pinout.h in outputs.cpp

void setup()
{
    Serial.begin(BAUDRATE);
    unsigned long timeout_end = millis() + 2000;
    if (DEBUG_ENABLED)
    {
        while (!Serial && millis() < timeout_end)
        { // Wait for Serial to initialize
        }
    }
    debug::init(DEBUG_ENABLED);
    button.init();
    serial_parser.init();
    program_logic.init(power_level_steps_percent);
    outputs.init(PWM_FREQUENCY);
    leds.init(LED_BLINK_PERIOD_MS, LED_BLINK_DUTY_CYCLE_MS);
    debug::println("Init complete");
}

void loop()
{
    button.process();
    serial_parser.process();
    program_logic.process(button, serial_parser, leds, outputs);
    leds.process();
    outputs.process();
}
