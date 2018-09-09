// Copyright 2018 by Kevin Dahlhausen

#include <ArduinoLog.h>
#include <EventDispatcher.h>
#include <Tasker.h>

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
Sound zoom1("zoom1.wav", ZOOM_PRIORITY, false, ZOOM1);
Sound zoom2("zoom2.wav", ZOOM_PRIORITY, false, ZOOM2);
Sound zoom3("zoom3.wav", ZOOM_PRIORITY, false, ZOOM3);

// other
SerialInterpreter serialInterpreter;


void setup()
{
    Serial.begin(9600);
    while(!Serial && !Serial.available()){}

    Log.begin(LOG_LEVEL_VERBOSE, &Serial);

    // TODO: figure out why registrations don't work before setup. They should.
    bomb_drop.register_el();
    machineguns.register_el();
    motor.register_el();
    zoom1.register_el();
    zoom2.register_el();
    zoom3.register_el();

    //tasker.setInterval([&q](){q.enqueueEvent(MACHINEGUNS_START);}, 3500); // TESTING DELETE ME
    //tasker.setInterval([&q](){q.enqueueEvent(MACHINEGUNS_STOP);}, 3750); // TESTING DELETE ME

    theSoundManager->setup();

    Log.trace(F("setup complete\n"));
}


bool is_first_loop = true;


void loop()
{
    if (is_first_loop)
    {
        void *handle = theSoundManager->play("startup.wav", STARTUP_PRIORITY, false);
        Log.trace(F("startup handle = %d\n"), (int)handle);
        is_first_loop = false;
    }

    event_dispatcher.run();
    tasker.loop();

    // debounce and send events for switches
    motor_switch.update();
    machinegun_switch.update();

    theSoundManager->update();
    serialInterpreter.update();
    motor.update();
}
