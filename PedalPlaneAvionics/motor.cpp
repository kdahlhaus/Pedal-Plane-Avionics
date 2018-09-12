// Copyright 2018 by Kevin Dahlhausen

#include "motor.h"

#include <ArduinoLog.h>
#include "avionics_events.h"
#include "sound_manager.h"
#include "sound_priorities.h"
#include "register_event_listener.h"


Motor::Motor() :
    state(stopped), startGain(1.7), runGain(1.0)
{
    state = stopped;
    register_event_listener(MOTOR_START, makeFunctor((EventListener *)0, (*this), &Motor::onEvent));
    register_event_listener(MOTOR_STOP, makeFunctor((EventListener *)0, (*this), &Motor::onEvent));
}


void Motor::start()
{
    Log.trace(F("Motor::start\n"));

    //already running - should it restart? naw
    if (state != stopped) { return; }
    state = waiting_for_starting;
    sound_handle = theSoundManager->play("mostart.wav", MOTOR_SOUND_PRIORITY, false, startGain);
}

void Motor::stop()
{
    Log.trace(F("Motor::stop\n"));
    if (state == stopped || state == stopping) { return; }
    state = stopping;
    theSoundManager->stop(sound_handle);
    sound_handle = theSoundManager->play("mostop.wav", MOTOR_SOUND_PRIORITY, false);

}

void Motor::update()
{
    switch (state)
    {
        case stopped:
            break;

        case waiting_for_starting:
            if (theSoundManager->is_playing(sound_handle))
            {
                state = starting;
                Log.trace("starting\n");
            }
            break;

        case starting:
            if (theSoundManager->is_playing(sound_handle))
            {
                break;
            }
            // starting sound ended
            Log.trace("start sound ended\n");
            state = running;
            sound_handle = theSoundManager->play("morun.wav", MOTOR_SOUND_PRIORITY, true, runGain);
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

void Motor::onEvent(int event, void *param)
{
    switch (event)
    {
        case MOTOR_START: 
            start();
            break;

        case MOTOR_STOP:
            stop();
            break;
    }
} 

void Motor::setStartGain(float gain)
{
    this->startGain=gain;
    if (state==starting)
    {
        theSoundManager->setGain(sound_handle, gain);
    }
}

void Motor::setRunGain(float gain)
{
    this->runGain=gain;
    if (state==running)
    {
        theSoundManager->setGain(sound_handle, gain);
    }
}
