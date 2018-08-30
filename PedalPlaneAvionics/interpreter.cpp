#include "interpreter.h"

#include <ArduinoLog.h>
#
#include "avionics_events.h"
#include "send_event.h"

void onboard_LED_on() { Log.trace(F("sending on\n")); send_event(ONBOARD_LED_ON); }
void onboard_LED_off() { Log.trace(F("sending off\n")); send_event(ONBOARD_LED_OFF); }
void unrecognized(const char *command) { Log.trace("unrecognized command: %s", command); }

SerialInterpreter::SerialInterpreter()
{
    serial_command.addCommand("ol1", onboard_LED_on);
    serial_command.addCommand("ol0", onboard_LED_off);
    serial_command.setDefaultHandler(unrecognized);
}
