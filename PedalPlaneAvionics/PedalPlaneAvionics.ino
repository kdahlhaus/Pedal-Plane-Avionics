// Copyright 2018 by Kevin Dahlhausen

// libraries
#include <ArduinoLog.h>
#include <EventQueue.h>
#include <EventDispatcher.h>
#include <Tasker.h>

#include "avionics_events.h"
#include "interpreter.h"
#include "machineguns.h"
#include "motor.h"
#include "sound.h"
#include "switch.h"

EventQueue q;
EventDispatcher event_dispatcher(&q);

Tasker tasker;

// input objects
Switch motor_switch(0, 0, MOTOR_START, -1);
Switch machinegun_switch(0, 0, MACHINEGUNS_START, MACHINEGUNS_STOP);

// domain objects
MachineGuns machineguns;
Motor motor;


void setup()
{
    Serial.begin(9600);
    while(!Serial && !Serial.available()){}


    Log.begin(LOG_LEVEL_VERBOSE, &Serial);

    event_dispatcher.addEventListener(MACHINEGUNS_START, MEMBER_CALLBACK(machineguns, onEvent));
    event_dispatcher.addEventListener(MACHINEGUNS_STOP, MEMBER_CALLBACK(machineguns, onEvent));

    tasker.setInterval([&q](){q.enqueueEvent(MACHINEGUNS_START);}, 1500); // TESTING DELETE ME
    tasker.setInterval([&q](){q.enqueueEvent(MACHINEGUNS_STOP);}, 1750); // TESTING DELETE ME

    Log.trace(F("setup complete"));
}

void loop()
{
  event_dispatcher.run();
  tasker.loop();

  motor_switch.update();
  machinegun_switch.update();
}
