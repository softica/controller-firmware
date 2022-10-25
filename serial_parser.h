#ifndef SERIAL_COMMANDS_H__
#define SERIAL_COMMANDS_H__

#include <Arduino.h>
#include <stdint.h>
#include <stdbool.h>
#include "debug.h"
#include "pinout.h"
#include "outputs.h"

//#define COMMAND_PARAMETERS_N MAX_OUTPUTS_N
#define COMMAND_PARAMETERS_N N_CHANNELS
#define DEBUG_PARSER false

class serial_parser_t
{
public:
    enum command_name_t
    {
        COMMAND_GET = 0x01,
        COMMAND_SET = 0x02,
        COMMAND_TESTMODE_START = 0xAA,
        COMMAND_INVALID = 0xFF
    };

    typedef struct
    {
        command_name_t command;
        int parameters[COMMAND_PARAMETERS_N];
    } command_t;

private:
    enum state_t
    {
        STATE_WAITING_FOR_COMMAND,
        STATE_RECEIVING,
        STATE_WAITING_FOR_ENDLINE
    };

    String state_to_str(state_t state)
    {
        switch (state)
        {
        case STATE_WAITING_FOR_COMMAND:
            return "cmd";
        case STATE_RECEIVING:
            return "rcv";
        case STATE_WAITING_FOR_ENDLINE:
            return "end";
        default:
            return "inv";
        }
    };

    command_name_t int_to_serial_command(int value)
    {
        switch (value)
        {
        case COMMAND_GET:
        case COMMAND_SET:
        case COMMAND_TESTMODE_START:
            return (command_name_t)value;
        default:
            return COMMAND_INVALID;
        }
    }

    state_t state = STATE_WAITING_FOR_COMMAND;
    command_t processed_command;
    command_t valid_command;
    bool have_valid_command = false;
    int parameter_processed = 0;

public:
    /** Class constructor
    */
    serial_parser_t()
    {
    }

    /**
      Initialize serial commands parser. Serial.begin() must be called before.
    */
    void init()
    {
    }

    /**
      Process serial communication. Call in loop() function
    */
    void process()
    {
        if (Serial.available())
        {
            int data = Serial.read();
            if (DEBUG_PARSER)
            {
                Serial.print("ser ");
                Serial.print(state_to_str(state));
                Serial.print(" ");
                Serial.println(data);
            }

            if (state == STATE_WAITING_FOR_COMMAND)
            {
                processed_command.command = int_to_serial_command(data);
                // Wait for first valid command character
                if (processed_command.command != COMMAND_INVALID)
                {
                    state = STATE_RECEIVING;
                    parameter_processed = 0;
                }
            }
            else if (state == STATE_RECEIVING)
            {
                processed_command.parameters[parameter_processed] = data;
                parameter_processed++;
                if (parameter_processed > COMMAND_PARAMETERS_N || processed_command.command == COMMAND_GET)
                {
                    state = STATE_WAITING_FOR_ENDLINE;
                }
            }
            if (state == STATE_WAITING_FOR_ENDLINE)
            {
                if (data == '\r' || data == '\n')
                {
                    memcpy(&valid_command, &processed_command, sizeof(command_t));
                    have_valid_command = true;
                    debug::println("serial command", valid_command.command);
                    debug::println("serial parameters", valid_command.parameters, COMMAND_PARAMETERS_N);
                }
                state = STATE_WAITING_FOR_COMMAND;
            }
            //else {
            //    state = STATE_WAITING_FOR_COMMAND;
            //}
        }
    }

    bool is_ready()
    {
        return have_valid_command;
    }

    command_t get_command()
    {
        
        if (have_valid_command)
        {
            have_valid_command = false;
            return valid_command;
        }
        else
        {
            return {COMMAND_INVALID, {0}};
        }
    }
};

#endif // SERIAL_COMMANDS_H__
