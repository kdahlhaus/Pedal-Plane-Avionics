  // Copyright 2018 by Kevin Dahlhausen

// libraries
#include <ArduinoLog.h>
#include <EventQueue.h>
#include <EventDispatcher.h>
#include <Tasker.h>

// pedal plane avionics headers
#include "avionics_events.h"
#include "interpreter.h"
#include "machineguns.h"
#include "motor.h"
#include "output.h"
#include "sound_manager.h"
#include "switch.h"

EventQueue q;
EventDispatcher event_dispatcher(&q);

Tasker tasker;

// input objects
Switch motor_switch(0, 0, MOTOR_START);
Switch machinegun_switch(0, 0, MACHINEGUNS_START, MACHINEGUNS_STOP);

// output objects
Output onboard_LED(LED_BUILTIN, ONBOARD_LED_ON, ONBOARD_LED_OFF);

// domain objects
MachineGuns machineguns;
Motor motor;

// other
SerialInterpreter serialInterpreter;


void setup()
{
    Serial.begin(9600);
    while(!Serial && !Serial.available()){}

    Log.begin(LOG_LEVEL_VERBOSE, &Serial);

    event_dispatcher.addEventListener(MACHINEGUNS_START, MEMBER_CALLBACK(machineguns, onEvent));
    event_dispatcher.addEventListener(MACHINEGUNS_STOP, MEMBER_CALLBACK(machineguns, onEvent));

    //tasker.setInterval([&q](){q.enqueueEvent(MACHINEGUNS_START);}, 3500); // TESTING DELETE ME
    //tasker.setInterval([&q](){q.enqueueEvent(MACHINEGUNS_STOP);}, 3750); // TESTING DELETE ME

    theSoundManager->setup();

    Log.trace(F("setup complete\n"));
}

bool first_loop = true;

void loop()
{
    if (first_loop)
    {
        void *handle = theSoundManager->play("startup.wav", 1, false);
        Log.trace(F("startup handle = %d\n"), (int)handle);
        first_loop = false;
    }

    event_dispatcher.run();
    tasker.loop();

    // debounce and send events for all switches
    motor_switch.update();
    machinegun_switch.update();

    serialInterpreter.update();
}
