// Copyright 2018 by Kevin Dahlhausen

#include "motor.h"

#include <ArduinoLog.h>
#include "avionics_events.h"
#include "sound_manager.h"
#include "sound_priorities.h"
#include "register_event_listener.h"

extern AudioPlaySdWav motorSdWav1;
extern AudioPlaySdWav motorSdWav2;
extern AudioEffectFade motorFade1;
extern AudioEffectFade motorFade2;

#define FADE_TIME_MS 400

Motor::Motor() :
    state(stopped), starterGain(1.7), startingGain(1.7), idleGain(1.0), runGain(1.0)
    channel1({.sdWav=motorSdWav1, .fader=motorFade1}), channel2{.sdWav=motorSdWav2, .fader=motorFade2}),
    startAndRun(false), shouldStop(false)
{
    currentChannel = &channel1;
    nextChannel = &channel2;

    register_event_listener(MOTOR_START, makeFunctor((EventListener *)0, (*this), &Motor::onEvent));
    register_event_listener(MOTOR_STOP, makeFunctor((EventListener *)0, (*this), &Motor::onEvent));
    register_event_listener(MOTOR_STARTER_START, makeFunctor((EventListener *)0, (*this), &Motor::onEvent));
    register_event_listener(MOTOR_STARTER_STOP, makeFunctor((EventListener *)0, (*this), &Motor::onEvent));
}


void Motor::start(bool viaStater)
{
    Log.trace(F("Motor::start\n"));

    //already running - should it restart? naw
    if (state == stopped || (state == fading && nextStateAfterFade == stopped)) {
        currentChannel->sdWav.play("starter.wav");
        currentChannel->timeStarted = millis();
        currentChannel->loop = viaStarter;
        state = starter;
        shouldStop = false;
    }
}

void Motor::stopStarter()
{
    if (state==starter) {
        changeFromStarterToStarting();
    } 
}

void Motor::stop()
{
    Log.trace(F("Motor::stop\n"));
    shouldStop = true;
}

void Motor::fadeTo( const char *fileName, float gain, bool loop,  State nextState)
{
    Log.trace(F("fadeTo(%s, .., %b, %d)\n"), fileName, loop, nextState);

    // not worrying if the current sound is less than FADE_TIME_MS to the end 
    // if it is, then it will stop a bit short while the new sound fades in
    state = fading;
    nextStateAfterFade = nextState;
    nextChannel->sdWav.play(fileName); 
    nextChannel->timeStarted = millis();
    nextChannel->fader.fadeIn(FADE_TIME_MS);
    currentChannel->fader.fadeOut(FADE_TIME_MS);
    currentChannel->timeStarted = millis();

    //swap pointers -> "current" channel is the new sound
    currentChannel = ( currentChannel == &channel1 ) ? &channel2 : &channel1;
    nextChannel = ( currentChannel == &channel1 ) ? &channel2 : &channel1;
}

void Motor:changeFromStarterToStarting()
{
    //fadeTo("starting.wav", startingGain, false, starting);
    currentChannel->sdWav.play("starting.wav");
    currentChannel->timeStarted = millis();
    currentChannel->loop = false;
    Log.trace("starter->starting\n");
}


void Motor::update()
{
    if ((state != fading) && shouldStop) {
        fadeTo("stop.wav", 1.0, false, stopped);
    }

    switch (state) {

        case fading:
            // fading starts fade out on old, in on new
            // once new begins playing, transitions to the next state
            // so could be in the new state while the volume is 
            // ramping up

            // stop original channel? (which is now on nextChannel)
            if ((millis() >= nextChannel->timeStarted + FADE_TIME_MS) ||
                !nextChannel->sdWav.isPlaying()) {
                nextChannel->sdWav.stop();
            }
            // time to transition to new state?
            if (soundStartDelayHasPassed() || currentChannelIsPlaying()) {
                state = nextStateAfterFade;
            }
            break;

        
        case stopped:
            break;
                                 

        case starter:
            if (soundStartDelayHasPassed() && !currentChannelIsPlaying()) { 
                // sound started to play and has stopped
                if (currentChannel->loop) {
                    currentChannel->sdWav.play("starter.wav");
                    currentChannel->timeStarted = millis();
                }
                else {
                    changeFromStarterToStarting();
                }
            }
            break;


        case starting:
            if (soundStartDelayHasPassed() && !currentChannelIsPlaying()) {
                fadeTo("idle.wav", idleGain, true, idle); 
            }
            break;
            

        case idle:
            if (soundStartDelayHasPassed() && !currentChannelIsPlaying()) {
                currentChannel->sdWav.play("idle.wav");
                currentChannel->timeStarted = millis();

                // TODO add code for transition to running - maybe time or movment?
            }
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

        case MOTOR_STARTER_START:
            start(true);
            break;

        case MOTOR_STARTER_STOP:
            stopStarter();
            break;
    }
} 

