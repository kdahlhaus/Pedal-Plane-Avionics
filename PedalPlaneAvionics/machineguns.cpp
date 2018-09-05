// Copyright 2018 by Kevin Dahlhausen
#include <ArduinoLog.h>

#include "machineguns.h"
#include "avionics_events.h"
#include "register_event_listener.h"

MachineGuns::MachineGuns()
{
    register_event_listener(MACHINEGUNS_START, makeFunctor((EventListener *)0, (*this), &MachineGuns::onEvent));
    register_event_listener(MACHINEGUNS_STOP, makeFunctor((EventListener *)0, (*this), &MachineGuns::onEvent));
}


void MachineGuns::start()
{
    Log.trace(F("MachineGuns::start\n"));
    // MACHINEGUN_PRIORITY
}

void MachineGuns::stop()
{
    Log.trace(F("MachineGuns::stop\n"));
}

void MachineGuns::onEvent(int event, void *param)
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
