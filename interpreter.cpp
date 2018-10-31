#include "interpreter.h"

#include <ArduinoLog.h>

#include "avionics_events.h"
#include "config.h"
#include "send_event.h"
#include "motor.h"
#include "machineguns.h"
#include "sound.h"
#include "sound_manager.h"

//for changing fader alg
#include "navlights.h"
#include <Curve.h>

extern Navlights  *navlights;
extern SerialInterpreter *serialInterpreter;                           
extern Motor *motor;
extern MachineGuns *machineguns;
extern Sound *bomb_drop;


void unrecognized(const char *command) { Log.trace(F("unrecognized command: %s\n"), command); }


// These functions implement the interpreter commands.  
void onboard_LED_on(SerialCommand &theSerial) { Log.trace(F("sending on\n")); send_event(ONBOARD_LED_ON); }
void onboard_LED_off(SerialCommand &theSerial) { Log.trace(F("sending off\n")); send_event(ONBOARD_LED_OFF); }
void motor_start(SerialCommand &theSerial) { send_event(MOTOR_START); }
void motor_stop(SerialCommand &theSerial) { send_event(MOTOR_STOP); }
void machine_guns_start(SerialCommand &theSerial) { send_event(MACHINEGUNS_START); }
void machine_guns_stop(SerialCommand &theSerial) { send_event(MACHINEGUNS_STOP); }
void do_bomb_drop(SerialCommand &theSerial) { send_event(DROP_BOMB); }
void navlights_on(SerialCommand &theSerial) { send_event(NAVLIGHTS_ON); }
void navlights_off(SerialCommand &theSerial) { send_event(NAVLIGHTS_OFF); }
void navlights_curve(SerialCommand &serialCommand) {
    char *curve = serialCommand.next();
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

void motor_set_speed(SerialCommand &serialCommand)
{
    char *speed_str = serialCommand.next();
    int speed = atoi(speed_str);
    send_event(MOTOR_SET_SPEED, (void *)speed);
}

void zoom(SerialCommand &theSerial) { send_event(ZOOM); }

void motor_starter_start(SerialCommand &theSerial) { send_event(MOTOR_STARTER_START); }
void motor_starter_stop(SerialCommand &theSerial) { send_event(MOTOR_STARTER_STOP); }

void radio_start_chatter(SerialCommand &theSerial) { send_event(RADIO_CHATTER_ON); }
void radio_stop_chatter(SerialCommand &theSerial) { send_event(RADIO_CHATTER_OFF); }

void set_config(SerialCommand &serialCommand) {
    char *field = serialCommand.next();  
    if (field) {
        char *gain_str = serialCommand.next();
        if (gain_str) {                                                                 
            float gain = atof(gain_str);
            if (!strcmp(field, "gain")) { c.gain(gain); }
            if (!strcmp(field, "motorgain")) { c.motorGain(gain); }
            if (!strcmp(field, "machinegungain")) { c.machineGunGain(gain); }
            if (!strcmp(field, "crashgain")) { c.crashGain(gain); }
            if (!strcmp(field, "bombdropgain")) { c.bombDropGain(gain); }
            if (!strcmp(field, "zoomgain")) { c.zoomGain(gain); }
            if (!strcmp(field, "radiogain")) { c.radioGain(gain); }
            if (!strcmp(field, "startupgain")) { c.startupGain(gain); }
            if (!strcmp(field, "machinegunhit")) { c.machineGunHit(gain); }

        }
    }
}                                   

void get_config(SerialCommand &serialCommand) {
    char *field = serialCommand.next();  
    if (field) {
        if (!strcmp(field, "gain")) { serialCommand.stream().println(c.gain()); }
        if (!strcmp(field, "motorgain")) { serialCommand.stream().println(c.motorGain()); }
        if (!strcmp(field, "machinegungain")) { serialCommand.stream().println(c.machineGunGain()); }
        if (!strcmp(field, "crashgain")) { serialCommand.stream().println(c.crashGain()); }
        if (!strcmp(field, "bombdropgain")) { serialCommand.stream().println(c.bombDropGain()); }
        if (!strcmp(field, "zoomgain")) { serialCommand.stream().println(c.zoomGain()); }
        if (!strcmp(field, "radiogain")) { serialCommand.stream().println(c.radioGain()); }
        if (!strcmp(field, "startupgain")) { serialCommand.stream().println(c.startupGain()); }
        if (!strcmp(field, "machinegunhit")) { serialCommand.stream().println(c.machineGunHit()); }

    }
}

void write_config(SerialCommand &) { c.save(); } 
void load_config(SerialCommand &) { c.load(); } 

void addInterpreterCommands(SerialCommand &serial_command) 
{
    serial_command.setDefaultHandler(unrecognized);

    serial_command.addCommand("ol1", onboard_LED_on);
    serial_command.addCommand("ol0", onboard_LED_off);

    serial_command.addCommand("mo1", motor_start);
    serial_command.addCommand("mo0", motor_stop);
                                                                                        
    serial_command.addCommand("moss", motor_set_speed);

    serial_command.addCommand("mos1", motor_starter_start);
    serial_command.addCommand("mos0", motor_starter_stop);

    serial_command.addCommand("mg1", machine_guns_start);
    serial_command.addCommand("mg0", machine_guns_stop);

    serial_command.addCommand("bd", do_bomb_drop);
    serial_command.addCommand("db", do_bomb_drop);

    serial_command.addCommand("nl0", navlights_off);
    serial_command.addCommand("nl1", navlights_on);
    serial_command.addCommand("nlc", navlights_curve);

    serial_command.addCommand("z", zoom);

    serial_command.addCommand("rc1", radio_start_chatter);
    serial_command.addCommand("rc0", radio_stop_chatter);

    // config stuff
    serial_command.addCommand("sc", set_config);
    serial_command.addCommand("gc", get_config);
    serial_command.addCommand("wc", write_config);
    serial_command.addCommand("lc", load_config); // TODO: can remove this after testing

} 


SerialInterpreter::SerialInterpreter(Stream &serialToUse) :
    serial_command(serialToUse)
{
    addInterpreterCommands(serial_command);
}
