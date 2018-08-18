// Copyright 2018 by Kevin Dahlhausen

#include <ArduinoLog.h>
#
#include "machineguns.h"
#include "avionics_events.h"


void MachineGuns::start()
{
    Log.trace("MachineGuns::start\n");
}

void MachineGuns::stop()
{
    Log.trace("MachineGuns::stop\n");
}

void MachineGuns::onEvent(int event, int param)
{
    switch (event)
    {
        case MACHINEGUNS_START: 
            this->start();
            break;

        case MACHINEGUNS_STOP:
            this->stop();
            break;
    }
}
