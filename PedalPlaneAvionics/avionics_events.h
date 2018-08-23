// Copyright 2018 by Kevin Dahlhausen

#ifndef _avionics_events_h
#define _avionics_events_h

enum AvionicsEvents
{
    // NO_EVENT is '0'

    MOTOR_START = 1,
    MOTOR_STOP,

    MACHINEGUNS_START,
    MACHINEGUNS_STOP,

    DROP_BOMB,

    ONBOARD_LED_ON,
    ONBOARD_LED_OFF,
};

#endif
