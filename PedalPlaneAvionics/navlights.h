// Copyright 2018 Kevin Dahlhausen

#ifndef _navlights_h
#define _navlights_h

#include <LEDFader.h>

class Navlights
{
    public:
        Navlights();
        void on();
        void off();
        void onEvent(int event, void *param);
        void update();

        friend void navlights_curve();

    protected:
        bool lights_on;
        LEDFader fader;
};

#endif
