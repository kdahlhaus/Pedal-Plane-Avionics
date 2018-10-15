// Copyright 2018 Kevin Dahlhausen

#ifndef _config_h_
#define _config_h_

/*
    A versioned system config.

    Steps to add a new config item:
        1. add new variable _<config item>
        2. initialize it in the constructor
        3. add set/get functions named after the new variable
        4. increase "CURRENT_VERSION" by 1
        5. add new clause to Config::load after the last existing variable load statements.
            if (version >= <new value of CURRENT_VERSION)
        6. add in.read(<new variable) and statements to write it to Log.trace(...)
        7. add new line to set_config in interpreter.cpp
        8. add new line to get_config in interpreter.cpp
        9. Update the Android app config screen.

    The CURRENT_VERSION check allows existing confiurations to be loaded even though they
    do not contain the new variable.

*/

class Config
{
    public:

        Config();

        // load config from EEPROM
        void load(); 

        // save config to EEPROM
        void save();

        // overall gain 
        float gain() { return _gain; }
        void gain(float gain) { _gain = gain; }

        float motorGain() { return _motorGain; }
        void motorGain(float gain) { _motorGain = gain; }

        float machineGunGain() { return _machineGunGain; }
        void machineGunGain(float gain) { _machineGunGain=gain; }

        // % chance of "hitting" when firing the machine guns
        float machineGunHit() { return _machineGunHit; }
        void machineGunHit(float hit) { _machineGunHit=hit; }


        float crashGain() { return _crashGain; }
        void crashGain(float gain) { _crashGain=gain; }

        float bombDropGain() { return _bombDropGain; }
        void bombDropGain(float gain) { _bombDropGain = gain; }

        float zoomGain() { return _zoomGain; }
        void zoomGain(float gain) { _zoomGain = gain; }

        float radioGain() { return _radioGain; }
        void radioGain(float gain) { _radioGain = gain; }

        float startupGain() { return _startupGain; }
        void startupGain(float gain) { _startupGain = gain; }
 

    protected:                                 

        float _gain;
        float _motorGain;
        float _machineGunGain;
        float _crashGain;
        float _bombDropGain;
        float _zoomGain;
        float _radioGain;
        float _startupGain;
        float _machineGunHit;

};


// global Config instance
extern Config c;

#endif
