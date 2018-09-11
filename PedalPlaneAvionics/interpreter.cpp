#include "interpreter.h"

#include <ArduinoLog.h>
#
#include "avionics_events.h"
#include "send_event.h"

//for changing fader alg
#include "Navlights.h"
#include <Curve.h>

void unrecognized(const char *command) { Log.trace(F("unrecognized command: %s\n"), command); }

void onboard_LED_on() { Log.trace(F("sending on\n")); send_event(ONBOARD_LED_ON); }
void onboard_LED_off() { Log.trace(F("sending off\n")); send_event(ONBOARD_LED_OFF); }

void motor_start() { send_event(MOTOR_START); }
void motor_stop() { send_event(MOTOR_STOP); }

void machine_guns_start() { send_event(MACHINEGUNS_START); }
void machine_guns_stop() { send_event(MACHINEGUNS_STOP); }

void bomb_drop() { send_event(DROP_BOMB); }

void navlights_on() { send_event(NAVLIGHTS_ON); }
void navlights_off() { send_event(NAVLIGHTS_OFF); }
void navlights_curve() {
    extern Navlights  *navlights;
    extern SerialInterpreter *serialInterpreter;
    char *curve = serialInterpreter->serial_command.next();
    if (curve != NULL)
    {
        if (*curve == 'e') { navlights->fader.set_curve(Curve::exponential); Log.trace(F("expontential fader set\n")); }
        if (*curve == 'l') { navlights->fader.set_curve(Curve::linear); Log.trace(F("linear fader set\n")); }
        if (*curve == 'r') { navlights->fader.set_curve(Curve::reverse); Log.trace(F("reverse fader set\n")); }
        if (*curve == '0') { navlights->fader.set_curve(0); Log.trace(F("no curve fader set\n")); }
    }
    else
    {
        Log.trace(F("Err: nlc requires one of elr0\n"));
    }
        
}

void zoom1() { send_event(ZOOM1); }
void zoom2() { send_event(ZOOM2); }
void zoom3() { send_event(ZOOM3); }


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

    serial_command.addCommand("nl0", navlights_off);
    serial_command.addCommand("nl1", navlights_on);
    serial_command.addCommand("nlc", navlights_curve);

    serial_command.addCommand("z1", zoom1);
    serial_command.addCommand("z2", zoom2);
    serial_command.addCommand("z3", zoom3);
}
