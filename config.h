// Copyright 2018 Kevin Dahlhausen

#ifndef _config_h_
#define _config_h_

#include <iostream>


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
        void machineGunGain(float gain) { return _motorGain; }

        friend ostream &operator<<( ostream &output, const Const &c );
        friend istream &operator>>( istream  &input, Const &c )

    protected:                                 

        float _motorGain;
        float _machineGunGain;

};

#endif
