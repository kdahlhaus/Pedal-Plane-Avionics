#include "config.h"

#include <ArduinoLog.h>

//#include "EEPROMSTREAM_COLIZ.h"
#include <EepromStream.h>

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
    _machineGunGain(1)
{
}

void Config::load()
{
    Log.trace("about to load config\n");

    EepromStream in(0, 1024);
    int version;
    version = in.parseInt(',');
    if (version < 32000) {

        _motorGain = in.parseFloat(',');
        _machineGunGain = in.parseFloat(',');

        Log.trace("loaded config\n");
    } else {
        Log.trace("EEPROM unitialized, not changing config\n");
    }

}

void Config::save()
{
    Log.trace("about to save config\n");

    EepromStream out(0, 1024);
    out.print((int)(CURRENT_VERSION)); out.print(','); 
    out.print(_motorGain); out.print(',');
    out.print(_machineGunGain); out.print(',');

    Log.trace("saved config\n");
 }

Config c; // the global config
