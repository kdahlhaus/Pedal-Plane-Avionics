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
        void machineGunGain(float gain) { _machineGunGain=gain; }

        float bombDropGain() { return _bombDropGain; }
        void bombDropGain(float gain) { _bombDropGain = gain; }

        float zoomGain() { return _zoomGain; }
        void zoomGain(float gain) { _zoomGain = gain; }

        float radioGain() { return _radioGain; }
        void radioGain(float gain) { _radioGain = gain; }

        float startupGain() { return _startupGain; }
        void startupGain(float gain) { _startupGain = gain; }
 

    protected:                                 

        float _motorGain;
        float _machineGunGain;
        float _bombDropGain;
        float _zoomGain;
        float _radioGain;
        float _startupGain;

};


// global Config instance
extern Config c;

#endif
