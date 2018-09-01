// Copyright 2018 by Kevin Dahlhausen

#include "motor.h"

#include <ArduinoLog.h>
#include "avionics_events.h"
#include "sound_manager.h"
#include "sound_priorities.h"


Motor::Motor()
{
    state = stopped;
}


void Motor::start()
{
    Log.trace(F("Motor::start\n"));

    //already running - should it restart? naw
    if (state != stopped) { return; }
    sound_handle = theSoundManager->play("mostart.wav", MOTOR_SOUND_PRIORITY, false);
}

void Motor::stop()
{
    Log.trace(F("Motor::stop\n"));
    if (state == stopped || state == stopping) { return; }
    state = stopping;
    sound_handle = theSoundManager->play("mostop.wav", MOTOR_SOUND_PRIORITY, false);

}

void Motor::update()
{
    switch (state)
    {
        case stopped:
            break;

        case starting:
            if (theSoundManager->is_playing(sound_handle))
            {
                break;
            }
            // starting sound ended
            state = running;
            sound_handle = theSoundManager->play("morun.wav", MOTOR_SOUND_PRIORITY, true);
            break;

        case running:
            break;

        case stopping:
            if (theSoundManager->is_playing(sound_handle))
            {
                break;
            }
            state = stopped;
            break;
    }
}

 


void Motor::onEvent(int event, int param)
{
    switch (event)
    {
        case MOTOR_START: 
            this->start();
            break;

        case MOTOR_STOP:
            this->stop();
            break;
    }
} 
