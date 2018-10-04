// Copyright 2018 Kevin Dahlhausen
#ifndef _navlights_h
#define _navlights_h

#include <LEDFader.h>

/*

   Controls the navigation lights.
   They fade on and off.

*/

class Navlights
{
    public:
        Navlights(int pin=6);
        void on();
        void off();
        void onEvent(int event, void *param);
        void update();

        // used by interpreter to change the fade
        // curve via command.
        friend void navlights_curve();

    protected:
        bool lights_on;
        LEDFader fader;
};

#endif
