// Copyright 2018 by Kevin Dahlhausen

#include "motor.h"

#include <ArduinoLog.h>
#include "avionics_events.h"
#include "config.h"
#include "sound_manager.h"
#include "sound_priorities.h"
#include "register_event_listener.h"

extern AudioPlaySdWav motorSdWav1;
extern AudioPlaySdWav motorSdWav2;
extern AudioEffectFade motorFade1;
extern AudioEffectFade motorFade2;

extern AudioMixer4 finalMixer; // for setting gain

// returns num elements in array x
#define NUM_ELEMENTS(x)  (sizeof(x) / sizeof((x)[0]))  



Motor::Motor() :
    state(stopped), 
    channel1({.sdWav=motorSdWav1, .fader=motorFade1, .finalMixerChannel=2}),
    channel2({.sdWav=motorSdWav2, .fader=motorFade2, .finalMixerChannel=3}),
    speed(0), shouldStop(false)
{
    currentChannel = &channel1;
    nextChannel = &channel2;

    register_event_listener(MOTOR_START, makeFunctor((EventListener *)0, (*this), &Motor::onEvent));
    register_event_listener(MOTOR_STOP, makeFunctor((EventListener *)0, (*this), &Motor::onEvent));
    register_event_listener(MOTOR_STARTER_START, makeFunctor((EventListener *)0, (*this), &Motor::onEvent));
    register_event_listener(MOTOR_STARTER_STOP, makeFunctor((EventListener *)0, (*this), &Motor::onEvent));
    register_event_listener(MOTOR_SET_SPEED, makeFunctor((EventListener *)0, (*this), &Motor::onEvent));
}


void Motor::start(bool viaStarter)
{
    Log.trace(F("Motor::start(%b)\n"), viaStarter);

    //already running - should it restart? naw
    if (state == stopped || (state == fading && nextStateAfterFade == stopped)) {
        Log.trace(F("about to play staters.wav\n"));
        currentChannel->sdWav.play("starters.wav");
        currentChannel->absolutePathToSound = "starters.wav";
        currentChannel->fader.fadeIn(1);
        currentChannel->timeStarted = millis();
        currentChannel->loop = viaStarter;
        finalMixer.gain(currentChannel->finalMixerChannel, c.motorGain());
        state = starter_starting;
        shouldStop = false;
    }
    shouldLoopStarter = viaStarter;
}

void Motor::stopStarter()
{
    if (state==starter_looping) {
        changeFromStarterToStarting();
    } 
    else if (state == starter_starting)
    {
        // at a minmum we want to play the entire starters.wav
        shouldLoopStarter = false;
        currentChannel->loop = false;
    }
}

void Motor::stop()
{
    Log.trace(F("Motor::stop\n"));
    shouldStop = true;
}

void Motor::setSpeed(int speed)
{
    //TODO: refine seed ranges
    Log.trace(F("setSpeed(%d)\n"), speed);

    struct motor_speed_item {
        const char *filename;
        int upper_speed; // this or below to the next lower will play
    };

    static struct motor_speed_item speed_map[] = { 
        {"idle.wav", 10},
        {"rpm1.wav", 40},
        {"rpm2.wav", 70},
        {"rpm3.wav", 100}
    };
    static int num_entries = (int)NUM_ELEMENTS(speed_map);

    for (int i=0; i<num_entries; i++) {
        if (speed <= speed_map[i].upper_speed) {
            // TODO: eliminate idle state and make it rpm0
            State nextState = (i == 0) ? idle : running;
            int fadeTimeMs = (i == 0) ? SHORT_FADE_TIME_MS : LONG_FADE_TIME_MS;
            fadeTo(speed_map[i].filename, c.motorGain(), true, nextState, fadeTimeMs);
            return;
        }
    }
}

void Motor::fadeTo( const char *fileName, float gain, bool loop,  State nextState, int fadeTimeMs)
{
    Log.trace(F("fadeTo(%s, .., %b, %d)\n"), fileName, loop, nextState);

    // not worrying if the current sound is less than fadeTimeMs to the end 
    // if it is, then it will stop a bit short while the new sound fades in
    state = fading;
    currentFadeTimeMs = fadeTimeMs;
    nextStateAfterFade = nextState;
    nextChannel->sdWav.play(fileName); 
    nextChannel->absolutePathToSound = fileName;
    nextChannel->timeStarted = millis();
    nextChannel->fader.fadeIn(fadeTimeMs);
    finalMixer.gain(nextChannel->finalMixerChannel, gain); 
    currentChannel->fader.fadeOut(fadeTimeMs);
    currentChannel->timeStarted = millis();

    //swap pointers -> "current" channel is the new sound
    currentChannel = ( currentChannel == &channel1 ) ? &channel2 : &channel1;
    nextChannel = ( currentChannel == &channel1 ) ? &channel2 : &channel1;
}

void Motor::changeFromStarterToStarting()
{
    Log.trace("starter->starting\n");
    //fadeTo("starting.wav", startingGain, false, starting, SHORT_FADE_TIME_MS);
    currentChannel->sdWav.play("starting.wav");
    currentChannel->absolutePathToSound = "starting.wav";
    currentChannel->timeStarted = millis();
    currentChannel->loop = false;
    finalMixer.gain(currentChannel->finalMixerChannel, c.motorGain()); 
    state = starting;
}


void Motor::update()
{
    if ((state != fading) && shouldStop) {
        shouldStop = false;
        fadeTo("stop.wav", c.motorGain(), false, stopped, LONG_FADE_TIME_MS);
    }

    switch (state) {

        case fading:
            // fading starts fade out on old, in on new
            // once new begins playing, transitions to the next state
            // so could be in the new state while the volume is 
            // ramping up

            // stop original channel? (which is now on nextChannel)
            if ((millis() >= nextChannel->timeStarted + currentFadeTimeMs) ||
                !nextChannel->sdWav.isPlaying()) {
                Log.trace("fading - original channel faded, stopping it\n");
                nextChannel->sdWav.stop();
            }
            // time to transition to new state?
            if (soundStartDelayHasPassed() || currentChannelIsPlaying()) {
                state = nextStateAfterFade;
                Log.trace("fading - set new state to %d\n", state);
            }
            else
            {
                Log.trace("fading - w4 new sound to start then stop: %b %b\n", soundStartDelayHasPassed(), currentChannelIsPlaying());
            }
            break;

        
        case stopped:
            break;
                                 
        case starter_starting:
            if (soundStartDelayHasPassed() && !currentChannelIsPlaying()) {
                if (!shouldLoopStarter) { 
                    changeFromStarterToStarting();
                }
                else {
                // change to starterl
                    currentChannel->sdWav.play("staterl.wav"); // TODO: fix typo in sound
                    currentChannel->absolutePathToSound = "staterl.wav";
                    currentChannel->timeStarted = millis();
                    finalMixer.gain(currentChannel->finalMixerChannel, c.motorGain()); 
                    state = starter_looping;
                    Log.trace(F("change to starter looping\n"));
                }
                
            }
            break;

        case starter_looping:
            // TODO: shorten starter loop sound
            if (soundStartDelayHasPassed()) {
                if (currentChannel->loop) {
                    if (!currentChannelIsPlaying()) {
                        currentChannel->sdWav.play("staterl.wav"); // TODO fix typo in sound
                        currentChannel->absolutePathToSound = "staterl.wav"; // TODO fix typo in sound
                        currentChannel->timeStarted = millis();
                        finalMixer.gain(currentChannel->finalMixerChannel, c.motorGain()); 
                        Log.trace(F("starter loop restart\n"));
                    }
                }
                else {
                    if ((currentChannel->sdWav.positionMillis() >= (currentChannel->sdWav.lengthMillis()-SHORT_FADE_TIME_MS)) ||
                        !currentChannelIsPlaying()) {
                            changeFromStarterToStarting();
                        }
                }
            }
            break;


        case starting:
            if (soundStartDelayHasPassed() && (
                (currentChannel->sdWav.positionMillis() >= (currentChannel->sdWav.lengthMillis()-SHORT_FADE_TIME_MS)) ||
                !currentChannelIsPlaying())) {  
                    fadeTo("idle.wav", c.motorGain(), true, idle); 
             }
            break;
            

        case idle:
            if (soundStartDelayHasPassed() && !currentChannelIsPlaying()) {
                currentChannel->sdWav.play("idle.wav");
                currentChannel->absolutePathToSound = "idle.wav";
                currentChannel->timeStarted = millis();
                finalMixer.gain(currentChannel->finalMixerChannel, c.motorGain()); 

                // TODO add code for transition to running - maybe time or movment?
            }
            break;

        case running:
            //Log.trace(F("warning -> handle running state\n"));
            if (soundStartDelayHasPassed() && !currentChannelIsPlaying()) {
                currentChannel->sdWav.play(currentChannel->absolutePathToSound);
                currentChannel->timeStarted = millis();
                finalMixer.gain(currentChannel->finalMixerChannel, c.motorGain()); 

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
            if (state == stopped) {
                start(true);
            }
            else {
                // TODO: double function - stop motor, maybe rename the message sent by switch?
                stop();
            }
            
            break;

        case MOTOR_STARTER_STOP:
            stopStarter();
            break;

        case MOTOR_SET_SPEED:
            int new_speed = (int)(param);
            setSpeed(new_speed);
            break;
    }
} 
