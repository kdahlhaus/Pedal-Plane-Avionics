// Copyright 2018 by Kevin Dahlhausen
#include <ArduinoLog.h>

#include "machineguns.h"
#include "config.h"
#include "avionics_events.h"
#include "register_event_listener.h"
#include "sound_manager.h"
#include "sound_priorities.h"

MachineGuns::MachineGuns() :
    handle(0)
{
    register_event_listener(MACHINEGUNS_START, makeFunctor((EventListener *)0, (*this), &MachineGuns::onEvent));
    register_event_listener(MACHINEGUNS_STOP, makeFunctor((EventListener *)0, (*this), &MachineGuns::onEvent));
}


void MachineGuns::start()
{
    handle = theSoundManager->play("machguns.wav", MACHINEGUN_PRIORITY, true, c.machineGunGain());
    Log.trace(F("MachineGuns::start h=%d\n"), (int)handle);
}

void MachineGuns::stop()
{
    if (handle) {
        theSoundManager->stop(handle);
        handle = 0;

        /*
         * tried this but not sure if it adds much fun
         * score a 'hit' - explosion then plane crash sound
         * TODO: set random seed in setup() 
         */
        if (random(100) <= c.machineGunHit()) { 
            theSoundManager->play("crash.wav", MACHINEGUN_PRIORITY, false, c.crashGain());
        }

        Log.trace(F("MachineGuns::stop\n"));
    }
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
