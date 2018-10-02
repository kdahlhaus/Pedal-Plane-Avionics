// Copyright 2018 by Kevin Dahlhausen

#ifndef tachometer_h
#define tachometer_h

#include "meanfilter.h"


/*
    IR sensor against the prop axle to determine if the plane is moving.
*/

class Tachometer
{
    public:

        Tachometer();

        bool isMoving() { return is_moving; }
        int RPM() { return rpm; }

        void update();
        // Update and debounce the tachometer and send events as appropriate.

    protected:
        int last_state;
        unsigned long time_of_last_state_change;
        unsigned long time_of_last_rpm_event;
        bool is_moving;
        int rpm;

        MeanFilter<int> rpm_filter;


};

#endif
