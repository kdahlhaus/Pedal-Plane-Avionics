// Copyright 2018 by Kevin Dahlhausen
#ifndef machineguns_h
#define machineguns_h

class MachineGuns
{
    public:
        // start firing
        void start();

        // stop firing
        void stop();

        void onEvent(int event, int param);
};

#endif
