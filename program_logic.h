#ifndef PROGRAM_LOGIC_H__
#define PROGRAM_LOGIC_H__

#include <Arduino.h>
#include <stdint.h>
#include <stdbool.h>
#include "led.h"
#include "outputs.h"
#include "button.h"
#include "serial_parser.h"

// TODO: production test mode
// TODO: start - led blink, ignore button change for ~2s

class program_logic_t
{
public:
    /** Class constructor
     * @param _n_channels Number of channels supported
     */
    program_logic_t(int _n_channels)
    {
        if (_n_channels > 3)
        {
            n_channels = 4;
        }
        else
        {
            n_channels = 2;
        }
    }

    /**
      Initialize program logic
    */
    void init(const uint8_t *_power_level_steps)
    {
        power_level_steps = _power_level_steps;
    }

    /**
      Process program logic. Call in loop() function
    */
    void process(button_t &button, serial_parser_t &serial_parser, led_t &led, outputs_t &outputs)
    {
        static int level = 0;
        static int out = 0;
        static bool blink = false;

        // UI state machine
        if (state == STATE_READ)
        {
            auto event = button.get_event();
            if (event == button_t::event_t::BUTTON_SHORT_PRESSED)
            {
                current_output++;
                if (current_output > MAX_OUTPUTS_N)
                {
                    current_output = 1;
                }
                if (n_channels == 2 && current_output > 1)
                {
                    current_output = MAX_OUTPUTS_N;
                }
                debug::println("logic: show out", current_output);
                update_leds(led);
            }
            else if (event == button_t::event_t::BUTTON_LONG_PRESSED)
            {
                state = STATE_WRITE;
                debug::println("logic: go to write");
                update_leds(led);
            }
        }
        else if (state == STATE_WRITE)
        {
            auto event = button.get_event();
            if (event == button_t::event_t::BUTTON_SHORT_PRESSED)
            {
                power_levels[current_output - 1]++;
                if (power_levels[current_output - 1] > POWER_LEVEL_N)
                {
                    power_levels[current_output - 1] = 0;
                }
                outputs.set_power(current_output, power_level_steps[power_levels[current_output - 1]]);
                debug::println("logic: set power", power_levels, MAX_OUTPUTS_N);
                update_leds(led);
            }
            else if (event == button_t::event_t::BUTTON_LONG_PRESSED)
            {
                state = STATE_READ;
                debug::println("logic: go to read");
                update_leds(led);
            }
        }
        else // error
        {
            state = STATE_READ;
            update_leds(led);
        }

        auto command = serial_parser.get_command();
        if (command.command == serial_parser_t::command_name_t::COMMAND_SET)
        {
            for (int i = 0; i < MAX_OUTPUTS_N; i++)
            {
                power_levels[i] = command.parameters[i];
                outputs.set_power(i + 1, power_level_steps[command.parameters[i]]);
            }
            update_leds(led);
        }
        if (command.command == serial_parser_t::command_name_t::COMMAND_SET ||
            command.command == serial_parser_t::command_name_t::COMMAND_GET)
        {
            char write_arr[] = {command.command,
                                power_levels[0], power_levels[1], power_levels[2], power_levels[3]};
            Serial.write(write_arr, MAX_OUTPUTS_N + 1);
        }
    }

private:
    enum state_t
    {
        STATE_READ,
        STATE_WRITE,
        STATE_INVALID = 0xFF
    };
    state_t state = STATE_INVALID;
    int n_channels;
    int power_levels[MAX_OUTPUTS_N] = {0};
    uint8_t current_output = 1;
    uint8_t *power_level_steps;
    void update_leds(led_t &led)
    {
        bool blink;
        if (state == STATE_WRITE)
        {
            blink = true;
        }
        else
        {
            blink = false;
        }
        led.set_output_leds(current_output, blink);
        led.set_level_leds(power_levels[current_output - 1]);
    }
};

#endif // PROGRAM_LOGIC_H__
