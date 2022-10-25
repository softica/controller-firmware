/*
 * Pinout file
 */

#ifndef PINOUT_H__
#define PINOUT_H__
#include "outputs.h"

// Hardware versions configuration
#define HW_VERSION 200 // HW version 1.00
// #define HW_VERSION 200 // HW version 2.00

#define MAX_OUTPUTS_N 4
#define POWER_LEVEL_N 4

enum led_controller_type_t
{
    LED_CONTROLLER_TLC5916,
    LED_CONTROLLER_TLC59025
};

// clang-format off
#if (HW_VERSION == 100)
    #define button_pin 13

    #define LED_CONTROLLER_TYPE LED_CONTROLLER_TLC5916
    #define LED_OE_PIN          3
    #define LED_LE_PIN          SS
    #define LED_POWER_LIST      {0,1,2,3}
#if (N_CHANNELS == 4)
    #define LED_OUTPUT_LIST     {7,6,5,4}
#endif
#if (N_CHANNELS == 2)
    #define LED_OUTPUT_LIST     {7,4}
#endif

    #define OUT_PIN1            10
    #define OUT_DDR1            DDRB
    #define OUT_PORT1           PORTB
    #define OUT_PORTPIN1        PINB6

    #define OUT_PIN2            9
    #define OUT_DDR2            DDRB
    #define OUT_PORT2           PORTB
    #define OUT_PORTPIN2        PINB5

    #define OUT_PIN3            5
    #define OUT_DDR3            DDRC
    #define OUT_PORT3           PORTC
    #define OUT_PORTPIN3        PINC6

    #define OUT_PIN4            11
    #define OUT_DDR4            DDRB
    #define OUT_PORT4           PORTB
    #define OUT_PORTPIN4        PINB7

#else
    #define button_pin 7

    #define LED_CONTROLLER_TYPE LED_CONTROLLER_TLC59025
    #define LED_OE_PIN          3
    #define LED_LE_PIN          SS
    #define LED_POWER_LIST      {0,1,2,3}
#if (N_CHANNELS == 4)
    #define LED_OUTPUT_LIST     {12,13,14,15}
#endif
#if (N_CHANNELS == 2)
    #define LED_OUTPUT_LIST     {12,15}
#endif

    #define OUT_PIN1            10
    #define OUT_DDR1            DDRB
    #define OUT_PORT1           PORTB
    #define OUT_PORTPIN1        PINB6

    #define OUT_PIN2            9
    #define OUT_DDR2            DDRB
    #define OUT_PORT2           PORTB
    #define OUT_PORTPIN2        PINB5

    #define OUT_PIN3            19
    #define OUT_DDR3            DDRD
    #define OUT_PORT3           PORTD
    #define OUT_PORTPIN3        PIND1

    #define OUT_PIN4            11
    #define OUT_DDR4            DDRB
    #define OUT_PORT4           PORTB
    #define OUT_PORTPIN4        PINB7
#endif
// clang-format on

#endif // PINOUT_H__
