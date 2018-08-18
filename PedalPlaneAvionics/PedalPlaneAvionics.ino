// Copyright 2018 by Kevin Dahlhausen

#include <EventQueue.h>
#include <EventDispatcher.h>

#include "avionics_events.h"
#include "interpreter.h"
#include "machineguns.h"
#include "motor.h"
#include "sound.h"
#include "switch.h"

EventQueue q;
EventDispatcher event_dispatcher(&q);

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
