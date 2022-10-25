#include "outputs.h"
#include "pinout.h"
#include "SoftPWM.h"
#include "debug.h"

// Half of the outputs are inverted. That way, it will reduce peak supply current when power is set to 50% or lower
SOFTPWM_DEFINE_CHANNEL(0, OUT_DDR1, OUT_PORT1, OUT_PORTPIN1);
#if N_CHANNELS == 4
SOFTPWM_DEFINE_CHANNEL_INVERT(1, OUT_DDR2, OUT_PORT2, OUT_PORTPIN2);
SOFTPWM_DEFINE_CHANNEL(2, OUT_DDR3, OUT_PORT3, OUT_PORTPIN3);
SOFTPWM_DEFINE_CHANNEL_INVERT(3, OUT_DDR4, OUT_PORT4, OUT_PORTPIN4);
#endif
#if N_CHANNELS == 2
SOFTPWM_DEFINE_CHANNEL_INVERT(1, OUT_DDR4, OUT_PORT4, OUT_PORTPIN4);
#endif

SOFTPWM_DEFINE_OBJECT_WITH_PWM_LEVELS(4, 100);

void outputs_t::init(int frequency)
{
    Palatis::SoftPWM.begin(frequency);
    for (int i = 1; i <= N_CHANNELS; i++)
    {
        set_power(i, 100);
    }
}

void outputs_t::set_power(uint8_t output, uint8_t power_percent)
{
    debug::println("outputs: set_power output:", output);
    debug::println("outputs: set_power level:", power_percent);
    if (power_percent > 100)
    {
        power_percent = 100;
    }
    if (output % 2 == 1) // Outputs 1, 3 are not inverted
    {
        Palatis::SoftPWM.set(output - 1, power_percent);
    }
    else // Outputs 2,4 are inverted
    {
        Palatis::SoftPWM.set(output - 1, 100 - power_percent);
    }
}

// Palatis::SoftPWM.set(i, 10);
