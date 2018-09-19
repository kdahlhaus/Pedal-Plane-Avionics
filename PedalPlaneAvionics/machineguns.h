// Copyright 2018 by Kevin Dahlhausen
#ifndef machineguns_h
#define machineguns_h

/*
    Plays the machine-gun sound on MACHINEGUNS_START event 
    or call to start().
*/

class MachineGuns
{
    public:

        MachineGuns();

        // start firing
        void start();

        // stop firing
        void stop();

        void onEvent(int event, void *param);

        void setGain(float gain);

    protected:
        void *handle; 
        float gain;
};

#endif
