#include "config.h"

#include <ArduinoLog.h>

#include "eeprom_stream_coliz.h"

/*

    // prob, don't necessarily know the file name
    sg rpm1.wav 0.75
    sg moss1.wav
 
    sc motorgain 0.75
    sc float motorgain 0.75

    sc autozoom 1
    sc bool autozoom true

    gc motorgain 
    wc // write config to EEPROM

    c.motorGain()
 
*/

#define CURRENT_VERSION 1
                                                                            
Config::Config() :                                                  
    _motorGain(1),                                                          
    _machineGunGain(1),
    _bombDropGain(1),
    _zoomGain(1),
    _radioGain(1),
    _startupGain(1)
{
}

void Config::load()
{
    Log.trace("about to load config\n");

    EepromStream in(0, 1024);

    int version;
    in.read(version);

    if (version < 32000) {

        char buffer[100];

        Log.trace("about to read motor gain\n");

        in.read(_motorGain);
        dtostrf(_motorGain, 4, 2, buffer);
        Log.trace("read motorgain  %s\n", buffer);

        in.read(_machineGunGain);
        dtostrf(_machineGunGain, 4, 2, buffer);
        Log.trace("read machinegungain %s\n", buffer);                                                     

        in.read(_bombDropGain);
        dtostrf(_bombDropGain, 4, 2, buffer);
        Log.trace("read bombdropgain  %s\n", buffer);
 
        in.read(_zoomGain);
        dtostrf(_zoomGain, 4, 2, buffer);
        Log.trace("read zoomgain  %s\n", buffer);
                                            
        in.read(_radioGain);
        dtostrf(_radioGain, 4, 2, buffer);
        Log.trace("read radiogain  %s\n", buffer);

        in.read(_startupGain);
        dtostrf(_startupGain, 4, 2, buffer);
        Log.trace("read startupgain  %s\n", buffer);
 
 
    } else {
        Log.trace("EEPROM unitialized, not changing config\n");
    }
                                                            
}

void Config::save()
{
    Log.trace("about to save config\n");

    EepromStream out(0, 1024);
    out.write((int)(CURRENT_VERSION));
    out.write(_motorGain);
    out.write(_machineGunGain);
    out.write(_bombDropGain);
    out.write(_zoomGain);
    out.write(_radioGain);
    out.write(_startupGain);

    Log.trace("saved config\n");
 }

Config c; // the global config
