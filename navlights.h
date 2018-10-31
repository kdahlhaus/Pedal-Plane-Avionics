// Copyright 2018 Kevin Dahlhausen
#ifndef _navlights_h
#define _navlights_h

#include <LEDFader.h>
#include <SerialCommand.h>

/*

   Controls the navigation lights.
   They fade on and off.

*/

class Navlights
{
    public:
        Navlights(int out_pin=6);
        void on();
        void off();
        void onEvent(int event, void *param);
        void update();

        // used by interpreter to change the fade
        // curve via command.
        friend void navlights_curve(SerialCommand &);

    protected:
        bool lights_on;
        LEDFader fader;
};

#endif
