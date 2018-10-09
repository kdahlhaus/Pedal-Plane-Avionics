#include "radio.h"

#include <ArduinoLog.h>

#include "avionics_events.h"
#include "register_event_listener.h"
#include "sound_priorities.h"

#define MIN_MS_BETWEEN_SOUNDS 30000
#define MS_RANGE_ADDED_TO_MIN 60000

Radio::Radio() :
    sounds("/radio", RADIO_CHATTER_PRIORITY), chatterIsEnabled(false)
{
    register_event_listener(RADIO_CHATTER_ON, makeFunctor((EventListener *)0, (*this), &Radio::onEvent));
    register_event_listener(RADIO_CHATTER_OFF, makeFunctor((EventListener *)0, (*this), &Radio::onEvent));
}

void Radio::startChatter()
{
    if (!chatterIsEnabled) {
        Log.trace(F("Radio startChatter\n"));
        startNextSound();
        chatterIsEnabled = true;
    }
}

void Radio::stopChatter()
{
    Log.trace(F("Radio stopChatter\n"));
    if (sounds.isPlaying()) {
        sounds.stop();
    }
    chatterIsEnabled = false;
}

void Radio::startNextSound() {
    /* start playing the next sound and determine time of next */
    Log.trace(F("Radio starting next sound\n"));
    if (!sounds.isPlaying()) {
        sounds.start();
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
