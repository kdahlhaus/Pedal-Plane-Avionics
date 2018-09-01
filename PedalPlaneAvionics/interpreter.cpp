#include "interpreter.h"

#include <ArduinoLog.h>
#
#include "avionics_events.h"
#include "send_event.h"

void unrecognized(const char *command) { Log.trace("unrecognized command: %s", command); }

void onboard_LED_on() { Log.trace(F("sending on\n")); send_event(ONBOARD_LED_ON); }
void onboard_LED_off() { Log.trace(F("sending off\n")); send_event(ONBOARD_LED_OFF); }

void motor_start() { send_event(MOTOR_START); }
void motor_stop() { send_event(MOTOR_STOP); }

void machine_guns_start() { send_event(MACHINEGUNS_START); }
void machine_guns_stop() { send_event(MACHINEGUNS_STOP); }

void bomb_drop() { send_event(DROP_BOMB); }



SerialInterpreter::SerialInterpreter()
{
    serial_command.setDefaultHandler(unrecognized);

    serial_command.addCommand("ol1", onboard_LED_on);
    serial_command.addCommand("ol0", onboard_LED_off);

    serial_command.addCommand("mo1", motor_start);
    serial_command.addCommand("mo0", motor_stop);

    serial_command.addCommand("mg1", machine_guns_start);
    serial_command.addCommand("mg0", machine_guns_stop);

    serial_command.addCommand("bd", bomb_drop);
    serial_command.addCommand("db", bomb_drop);
}
