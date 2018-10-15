#include "radio.h"

#include <ArduinoLog.h>

#include "avionics_events.h"
#include "register_event_listener.h"
#include "sound_priorities.h"
#include "config.h"

#define MIN_MS_BETWEEN_SOUNDS 30000
#define MS_RANGE_ADDED_TO_MIN 60000

#define MIN_MS_BEFORE_INITIAL_SOUND 8000
#define MS_RANGE_FOR_INITIAL_SOUND 10000

Radio::Radio() :
    onSound("radioon.wav", RADIO_CHATTER_PRIORITY),
    offSound("radiooff.wav", RADIO_CHATTER_PRIORITY),
    chatterSounds("/radio", RADIO_CHATTER_PRIORITY, GAIN_FUNCTION(c, Config::radioGain)), chatterIsEnabled(false)
{
    register_event_listener(RADIO_CHATTER_ON, makeFunctor((EventListener *)0, (*this), &Radio::onEvent));
    register_event_listener(RADIO_CHATTER_OFF, makeFunctor((EventListener *)0, (*this), &Radio::onEvent));
}

void Radio::startChatter()
{
    if (!chatterIsEnabled) {
        Log.trace(F("Radio startChatter\n"));
        onSound.start();
        timeOfNextSound = millis() +  MIN_MS_BEFORE_INITIAL_SOUND + random(0, MS_RANGE_FOR_INITIAL_SOUND);
        chatterIsEnabled = true;
    }
}

void Radio::stopChatter()
{
    Log.trace(F("Radio stopChatter\n"));
    chatterIsEnabled = false;
    if (onSound.isPlaying()) { onSound.stop();}
    if (chatterSounds.isPlaying()) {
        chatterSounds.stop();
    }
    if (!offSound.isPlaying()) { offSound.start(); }
}

void Radio::startNextSound() {
    /* start playing the next sound and determine time of next */
    Log.trace(F("Radio starting next sound\n"));
    if (!chatterSounds.isPlaying()) {
        chatterSounds.start();
    }
    timeOfNextSound = millis() + MIN_MS_BETWEEN_SOUNDS + random(0, MS_RANGE_ADDED_TO_MIN);
}

void Radio::update() 
{
    if (chatterIsEnabled && millis() >= timeOfNextSound) {
        startNextSound();
    }
}

void Radio::onEvent(int event, void *param)
{
    Log.trace(F("Radio.onEvent(%d)\n"), event);
    switch (event) 
    {
        case RADIO_CHATTER_ON:
            startChatter();
            break;

        case RADIO_CHATTER_OFF:
            stopChatter();
            break;
    }
}
