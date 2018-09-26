// Copyright 2018 by Kevin Dahlhausen

#ifndef _avionics_events_h
#define _avionics_events_h

/*
    These are the events sent w/send_event and 
    received by items calling register_event_handler.
*/

enum AvionicsEvents
{
    // NO_EVENT is '0'

    MOTOR_START = 1,
    MOTOR_STOP,
    MOTOR_SET_SPEED, // param: speed 0-100
    MOTOR_STARTER_START,
    MOTOR_STARTER_STOP,

    MACHINEGUNS_START,
    MACHINEGUNS_STOP,

    DROP_BOMB,

    ZOOM1, ZOOM2, ZOOM3,

    NAVLIGHTS_ON,
    NAVLIGHTS_OFF,

    ONBOARD_LED_ON,
    ONBOARD_LED_OFF,

    TACH_STARTED_MOVING,
    TACH_STOPPED_MOVING,
    TACH_RPM,
};

#endif
