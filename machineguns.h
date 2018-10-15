// Copyright 2018 by Kevin Dahlhausen
#ifndef machineguns_h
#define machineguns_h

/*
    Plays the machine-gun sound on MACHINEGUNS_START event 
    or call to start().
    Stops on MACHINEGUNS_STOP event or call to stop().

    (This functionality is not just a 'Sound' instance because
     I expect to have it also control barrel lights in the 
     future.)
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

    protected:
        void *handle; 
};

#endif
