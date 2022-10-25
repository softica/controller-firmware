/*
 * LED controller
 */

#ifndef TLC59025_H__
#define TLC59025_H__

#include <Arduino.h>
#include <stdint.h>
#include <stdbool.h>
#include "pinout.h"
#include "TLC591x.h"
#include "TLC59025.h"
#include "debug.h"
#include "outputs.h"

// TODO: TLC59025
// TODO: brightness control (AnalogWrite on OE pin should be sufficient)

class led_t
{
public:
    /** Class constructor
     * @param controller_type Type of controller mounted on board
     */
    led_t(const led_controller_type_t controller_type, int pin_CLK, int pin_SDI, int pin_LE, int pin_OE)
        : driver_TLC5916{1, pin_SDI, pin_CLK, pin_LE, pin_OE}, driver_TLC59025{pin_CLK, pin_SDI, pin_LE, pin_OE}
    {
        type = controller_type;

        // Set mask for blinking LEDs - set bits will not blink
        for (int i = 0; i < POWER_LEVEL_N; i++)
        {
            bitSet(blink_mask, power_led_numbers[i]);
        }
    }

    /**
     Initialize LED controller
     */
    void init(int _blink_period_ms, int _blink_duty_cycle_ms)
    {
        blink_period_ms = _blink_period_ms;
        blink_duty_cycle_ms = _blink_duty_cycle_ms;
        blink_threshold_ms = blink_period_ms;
        if (type == LED_CONTROLLER_TLC5916)
        {
            uint8_t n[1];
            driver_TLC5916.specialMode();
            n[0] = 0xff;
            driver_TLC5916.printDirect(n);
            driver_TLC5916.normalMode();
            driver_TLC5916.displayEnable();
            driver_TLC5916.print(int(0));
        }
        else // LED_CONTROLLER_TLC59025
        {
            driver_TLC59025.init();
        }
        int dbg[] = {blink_period_ms, blink_duty_cycle_ms, blink_mask};
        debug::println("led: init", dbg, 3);
    }

    /**
     Process LED controller communication. Call in loop() function
     */
    void process()
    {
        unsigned long elapsed_ms = millis() - last_set_time_ms;
        if (elapsed_ms >= blink_period_ms)
        {
            blink_state = true;
            update = true;
            last_set_time_ms = millis();
        }
        else if (elapsed_ms >= blink_threshold_ms && blink_state == true)
        {
            blink_state = false;
            update = true;
        }

        if (update == true)
        {
            if (type == LED_CONTROLLER_TLC5916)
            {
                if (blink_state == true)
                {
                    driver_TLC5916.print(led_register);
                }
                else
                {
                    driver_TLC5916.print(led_register & blink_mask);
                }
            }
            else // LED_CONTROLLER_TLC59025
            {
                if (blink_state == true)
                {
                    driver_TLC59025.write(led_register);
                }
                else
                {
                    driver_TLC59025.write(led_register & blink_mask);
                }
            }
            update = false;
        }
    }

    /**
     Turn LED for selected output ON, other OFF
     @param output_number: 1-4, if 0 or else - turn all output LEDs OFF
     @param blink: is output LED blinking
    */
    void set_output_leds(uint8_t output_number, bool blink)
    {
        for (int i = 0; i < MAX_OUTPUTS_N; i++)
        {
            if (output_number == (i + 1))
            {
                bitSet(led_register, output_led_numbers[i]);
            }
            else
            {
                bitClear(led_register, output_led_numbers[i]);
            }
        }
        if (blink == true)
        {
            blink_threshold_ms = blink_duty_cycle_ms;
            last_set_time_ms = millis() - blink_period_ms; // force LEDs on
        }
        else
        {
            blink_threshold_ms = blink_period_ms;
        }
        update = true;
        debug::println("led: led", led_register);
        debug::println("led: blink_thr", blink_threshold_ms);
    }

    /**
     Turn ON power level LEDs according to selected power level. level: for 0 - all OFF, to 4 - all ON
     */
    void set_level_leds(uint8_t level)
    {
        for (int i = 0; i < POWER_LEVEL_N; i++)
        {
            if (i < level)
            {
                bitSet(led_register, power_led_numbers[i]);
            }
            else
            {
                bitClear(led_register, power_led_numbers[i]);
            }
        }
        update = true;
        debug::println("led: led", led_register);
    }

private:
    led_controller_type_t type;
    bool update = false;
    unsigned int led_register = 0;
    TLC591x driver_TLC5916;
    TLC59025_t driver_TLC59025;
    uint8_t output_led_numbers[N_CHANNELS] = LED_OUTPUT_LIST;
    uint8_t power_led_numbers[MAX_OUTPUTS_N] = LED_POWER_LIST;
    int blink_threshold_ms;
    int blink_period_ms;
    int blink_duty_cycle_ms;
    bool blink_state = true;
    int blink_mask = 0;
    unsigned long last_set_time_ms;
};

#endif // TLC59025_H__
