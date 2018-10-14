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
void onboard_LED_on() { Log.trace(F("sending on\n")); send_event(ONBOARD_LED_ON); }
void onboard_LED_off() { Log.trace(F("sending off\n")); send_event(ONBOARD_LED_OFF); }
void motor_start() { send_event(MOTOR_START); }
void motor_stop() { send_event(MOTOR_STOP); }
void machine_guns_start() { send_event(MACHINEGUNS_START); }
void machine_guns_stop() { send_event(MACHINEGUNS_STOP); }
void do_bomb_drop() { send_event(DROP_BOMB); }
void navlights_on() { send_event(NAVLIGHTS_ON); }
void navlights_off() { send_event(NAVLIGHTS_OFF); }
void navlights_curve() {
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

void motor_set_speed()
{
    char *speed_str = serialInterpreter->serial_command.next();
    int speed = atoi(speed_str);
    send_event(MOTOR_SET_SPEED, (void *)speed);
}

void set_gain()
{
    //char *obj = serialInterpreter->serial_command.next();
    //if (obj)
    //{
        char *gain_str = serialInterpreter->serial_command.next();
        if (gain_str)
        {
            float gain = atof(gain_str);
            theSoundManager->setGain(gain);
            ////if (strcmp(obj, "mos")==0) { motor->setStartGain(gain); };
            //Log.trace(F("set_gain: id:%s gain: %s %d\n"), obj, gain_str, (int)gain*100);
            
        }
    //}
}

void zoom1() { send_event(ZOOM1); }
void zoom2() { send_event(ZOOM2); }
void zoom3() { send_event(ZOOM3); }

void motor_starter_start() { send_event(MOTOR_STARTER_START); }
void motor_starter_stop() { send_event(MOTOR_STARTER_STOP); }

void radio_start_chatter() { send_event(RADIO_CHATTER_ON); }
void radio_stop_chatter() { send_event(RADIO_CHATTER_OFF); }



void set_config() {
    char *field = serialInterpreter->serial_command.next();  
    if (field) {
        char *gain_str = serialInterpreter->serial_command.next();
        if (gain_str) {                                                                 
            float gain = atof(gain_str);
            if (!strcmp(field, "motorgain")) { c.motorGain(gain); }
            if (!strcmp(field, "machinegungain")) { c.machineGunGain(gain); }
            if (!strcmp(field, "bompdropgain")) { c.bombDropGain(gain); }
            if (!strcmp(field, "zoomgain")) { c.zoomGain(gain); }
            if (!strcmp(field, "radiogain")) { c.radioGain(gain); }
            if (!strcmp(field, "startupgain")) { c.startupGain(gain); }

        }
    }
}                                   

void get_config() {
    char *field = serialInterpreter->serial_command.next();  
    if (field) {
        if (!strcmp(field, "motorgain")) { serialInterpreter->serial_command.stream().println(c.motorGain()); }
        if (!strcmp(field, "machinegungain")) { serialInterpreter->serial_command.stream().println(c.machineGunGain()); }
        if (!strcmp(field, "bombdropgain")) { serialInterpreter->serial_command.stream().println(c.bombDropGain()); }
        if (!strcmp(field, "zoomgain")) { serialInterpreter->serial_command.stream().println(c.zoomGain()); }
        if (!strcmp(field, "radiogain")) { serialInterpreter->serial_command.stream().println(c.radioGain()); }
        if (!strcmp(field, "startupgain")) { serialInterpreter->serial_command.stream().println(c.startupGain()); }

    }
}

void write_config() { c.save(); } 
void load_config() { c.load(); } 

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

    serial_command.addCommand("z1", zoom1);
    serial_command.addCommand("z2", zoom2);
    serial_command.addCommand("z3", zoom3);

    serial_command.addCommand("rc1", radio_start_chatter);
    serial_command.addCommand("rc0", radio_stop_chatter);

    serial_command.addCommand("sg", set_gain);

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
