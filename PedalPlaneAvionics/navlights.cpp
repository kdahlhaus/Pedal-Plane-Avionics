// Copyright 2018 Kevin Dahlhausen

#include "navlights.h"
#include "avionics_events.h"
#include "register_event_listener.h"
#include <ArduinoLog.h>
#include <Curve.h>

#define FADE_TIME_MS 1000

Navlights::Navlights() :
    lights_on(false), fader(6)
{
    off();
    fader.set_curve(Curve::exponential);
    register_event_listener(NAVLIGHTS_ON, makeFunctor((EventListener *)0, (*this), &Navlights::onEvent));
    register_event_listener(NAVLIGHTS_OFF, makeFunctor((EventListener *)0, (*this), &Navlights::onEvent));
}

void Navlights::on() { lights_on = true; fader.set_value(0); fader.fade(255, 3000); Log.trace(F("navlights on")); } 
void Navlights::off() { lights_on = false; fader.set_value(0); Log.trace(F("navlights off")); } 


float rate = 1100;

void Navlights::update()
{
    if (lights_on)
    {
        // temp to adjust rate remove when determined
        float newrate = analogRead(15);
        newrate = newrate / 1024;
        newrate = newrate * 4000;
        if (abs(newrate - rate) > 5)
        {
            Log.trace(F("nl fader old rate:%d, new:%d\n"), (int)(rate), (int)(newrate));
            rate = newrate;
        }

        
        fader.update();
        if (!fader.is_fading())
        {
            // Fade from 255 - 0
            if (fader.get_value() == 255)
            {
              fader.fade(0, (int)(rate));
            }
            // Fade from 0 - 255
            else
            {
              fader.fade(255, (int)(rate));
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
