// Copyright 2018 by Kevin Dahlhausen
#include <ArduinoLog.h>

#include <EventDispatcher.h>
#include <EventQueue.h>

#include "machineguns.h"
#include "avionics_events.h"

extern EventDispatcher event_dispatcher;

MachineGuns::MachineGuns()
{
}


void MachineGuns::start()
{
    Log.trace(F("MachineGuns::start\\n"));
    // MACHINEGUN_PRIORITY
}

void MachineGuns::stop()
{
    Log.trace(F("MachineGuns::stop\\n"));
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
