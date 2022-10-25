# W&W Astro dew heater controller firmware.

## Compilation and flashing

Firware can be compiled using standard Arduino IDE.
Flashing can be done using USB cable connected from host computer to
controller. In case controlled doesn't respond on serial, the only
way to recover is to use Atmel ICE to flash with JTAG.

## Serial protocol

General format for protocol is:
<command - 1 byte>, [<output level - 1 byte per channnel>,...], 0x10

GET (0x01) - returns the state of the outputs

SET (0x02) - sets the output power for ports
Values set for the output level are 0-4 (0x00 - 0x04).
Value 0 represents output disabled. Values 1-4 represent PWM output as follows:
 * 1: 25% of power output
 * 2: 50% of power output
 * 3: 75% of power output
 * 4: 100% of power output

Response from both commands is identical and depends on the type of the controller.
2 channel controller will reply with 3 bytes, 4 channel with 5 bytes.
1st byte repeats the commands received by the controller.
Remaining bytes represent the levels of power outputed for each channel.
Byte 2 represents channel 1, byte 3 - channel 2 and so on. Output levels
are decoded as in SET command.
