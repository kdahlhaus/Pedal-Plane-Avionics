// Copyright 2018 by Kevin Dahlhausen

#include <Events.h>
#include <EventQueue.h>
#include <EventDispatcher.h>

#include "switch.h"
#include "interpreter.h"
#include "sound.h"
#include "machineguns.h"
#include "motor.h"

EventQueue q;
EventDispatcher event_dispatcher(&q);


// events
enum AvionicsEvents {

    MOTOR_START,
    MOTOR_STOP,
    MACHINEGUNS_START,
    MACHINEGUNS_STOP,
};

// input objects
Switch motor_switch(0, 0, MOTOR_START, -1);
Switch machinegun_switch(0, 0, MACHINEGUNS_START, MACHINEGUNS_STOP);

// domain objects
MachineGuns machineguns;
Motor motor;

void setup() 
{
    event_dispatcher.addEventListener(MACHINEGUNS_START, [&machineguns](int event, int param){ machineguns.onEvent(event, param);});
}

void loop() 
{
  event_dispatcher.run();
}
