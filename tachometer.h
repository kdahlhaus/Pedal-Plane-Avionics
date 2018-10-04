// Copyright 2018 by Kevin Dahlhausen

#ifndef tachometer_h
#define tachometer_h

#include "meanfilter.h"


/*
    A sensor against the prop axle to determine if the plane is moving.
*/

class Tachometer
{
    public:

        /*
            pin = pin number, pinMode will be set in the ctor
            changes_per_rotation = total number of low->high and high->low
              transitions per rotation
        */
        Tachometer(int pin=5, int changes_per_rotation=4);

        bool isMoving() { return is_moving; }
        int RPM() { return rpm; }

        // Update,  debounce the tachometer and send events as appropriate.
        void update();

    protected:
        int pin;
        int changes_per_rotation;
        int last_state;
        unsigned long time_of_last_state_change;
        unsigned long time_of_last_rpm_event;
        bool is_moving;
        int rpm;

        MeanFilter<int> rpm_filter;


};

#endif
