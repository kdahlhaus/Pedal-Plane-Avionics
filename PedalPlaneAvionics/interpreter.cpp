#include "interpreter.h"

#include "avionics_events.h"
#include "send_event.h"

void onboard_LED_on() { send_event(ONBOARD_LED_ON); }
void onboard_LED_off() { send_event(ONBOARD_LED_OFF); }

SerialInterpreter::SerialInterpreter()
{
    serial_command.addCommand("ol1", onboard_LED_on);
    serial_command.addCommand("ol0", onboard_LED_off);
}
