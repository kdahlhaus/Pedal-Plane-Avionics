// Copyright 2018 by Kevin Dahlhausen
#include <ArduinoLog.h>

#include "machineguns.h"
#include "avionics_events.h"
#include "register_event_listener.h"
#include "sound_manager.h"
#include "sound_priorities.h"

MachineGuns::MachineGuns()
{
    register_event_listener(MACHINEGUNS_START, makeFunctor((EventListener *)0, (*this), &MachineGuns::onEvent));
    register_event_listener(MACHINEGUNS_STOP, makeFunctor((EventListener *)0, (*this), &MachineGuns::onEvent));
}


void MachineGuns::start()
{
    handle = theSoundManager->play("machguns.wav", MACHINEGUN_PRIORITY, true);
    Log.trace(F("MachineGuns::start h=%d\n"), (int)handle);
    // MACHINEGUN_PRIORITY
}

void MachineGuns::stop()
{
    theSoundManager->stop(handle);
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
