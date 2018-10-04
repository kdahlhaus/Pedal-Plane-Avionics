// Copyright 2018 Kevin Dahlhausen

#include "navlights.h"
#include "avionics_events.h"
#include "register_event_listener.h"
#include <ArduinoLog.h>
#include <Curve.h>

#define FADE_TIME_MS 1160

Navlights::Navlights(int pin) :
    lights_on(false), fader(pin)
{
    off();
    fader.set_curve(Curve::exponential);
    register_event_listener(NAVLIGHTS_ON, makeFunctor((EventListener *)0, (*this), &Navlights::onEvent));
    register_event_listener(NAVLIGHTS_OFF, makeFunctor((EventListener *)0, (*this), &Navlights::onEvent));
}

void Navlights::on() { lights_on = true; fader.set_value(0); fader.fade(255, FADE_TIME_MS); Log.trace(F("navlights on")); } 
void Navlights::off() { lights_on = false; fader.set_value(0); Log.trace(F("navlights off")); } 



void Navlights::update()
{
    if (lights_on)
    {
        fader.update();
        if (!fader.is_fading())
        {
            // Fade from 255 - 0
            if (fader.get_value() == 255)
            {
              fader.fade(0, FADE_TIME_MS);
            }
            // Fade from 0 - 255
            else
            {
              fader.fade(255, FADE_TIME_MS);
            }
        }
    }
}


void Navlights::onEvent(int event, void *param)
{
    if (event == NAVLIGHTS_ON) {
        on();
    }
    else if (event == NAVLIGHTS_OFF) {
        off();
    }
}
