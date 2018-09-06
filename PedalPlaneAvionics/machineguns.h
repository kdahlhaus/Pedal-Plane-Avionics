// Copyright 2018 by Kevin Dahlhausen
#ifndef machineguns_h
#define machineguns_h

class MachineGuns
{
    public:

        void register_el();

        // start firing
        void start();

        // stop firing
        void stop();

        void onEvent(int event, void *param);

    protected:
        void *handle; 
};

#endif
