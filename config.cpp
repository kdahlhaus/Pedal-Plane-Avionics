#include "config.h"

/*
    sc motorgain value
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


ostream &operator<<( ostream &output, const Config &c ) { 

    output << (unsigned int)CURRENT_VERSION 
        << c._motorGain << c._machineGunGain;

    return output;            
}

istream &operator>>( istream  &input, Config &c ) { 
    unsigned int version;

    input >> version
        >> _motorGain >> _machineGunGain;

    return input;            
}
 
                               
void Config::load()
{
}

void Config::save()
{
}
