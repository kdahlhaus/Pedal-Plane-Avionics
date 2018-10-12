// Copyright 2018 Kevin Dahlhausen

#ifndef _config_h_
#define _config_h_

/*
    A versioned system config.
*/

class Config
{
    public:

        Config();

        // load config from EEPROM
        void load(); 

        // save config to EEPROM
        void save();

        float motorGain() { return _motorGain; }
        void motorGain(float gain) { _motorGain = gain; }

        float machineGunGain() { return _machineGunGain; }
        void machineGunGain(float gain) { _motorGain=gain; }

    protected:                                 

        float _motorGain;
        float _machineGunGain;

};


// global Config instance
extern Config c;

#endif
