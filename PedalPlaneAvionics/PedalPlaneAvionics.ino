// Copyright 2018 by Kevin Dahlhausen

// libraries
#include <ArduinoLog.h>
#include <EventDispatcher.h>
#include <Tasker.h>

// pedal plane avionics headers
#include "avionics_events.h"
#include "interpreter.h"
#include "machineguns.h"
#include "motor.h"
#include "output.h"
#include "sound.h"
#include "sound_manager.h"
#include "sound_priorities.h"
#include "switch.h"


Tasker tasker;
extern EventDispatcher event_dispatcher;

// input objects
Switch motor_switch(0, 0, MOTOR_START);
Switch machinegun_switch(0, 0, MACHINEGUNS_START, MACHINEGUNS_STOP);

// output objects
Output onboard_LED(LED_BUILTIN, ONBOARD_LED_ON, ONBOARD_LED_OFF);

// domain objects
MachineGuns machineguns;
Motor motor;
Sound bomb_drop("bombdrop.wav", BOMB_DROP_PRIORITY, false, DROP_BOMB);

// other
SerialInterpreter serialInterpreter;


void setup()
{
    Serial.begin(9600);
    while(!Serial && !Serial.available()){}

    Log.begin(LOG_LEVEL_VERBOSE, &Serial);

    bomb_drop.register_el();
    machineguns.register_el();

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
        void *handle = theSoundManager->play("startup.wav", STARTUP_PRIORITY, false);
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
