// Copyright 2018 by Kevin Dahlhausen

#include <ArduinoLog.h>
#include <EventDispatcher.h>

#include "avionics_events.h"
#include "free_mem.h"
#include "interpreter.h"
#include "machineguns.h"
#include "motor.h"
#include "navlights.h"
#include "output.h"
#include "sound.h"
#include "sound_manager.h"
#include "sound_priorities.h"
#include "switch.h"
#include "tachometer.h"
#include "zoom.h"

// Architecture:  See ../README.md

extern EventDispatcher event_dispatcher;


/*
    Major objects are allocated dynamically from setup()
    so they can control their own initialization, but this
    initialization must often take place within the call to
    setup.  While dynamically allocated, objects are generally
    not deleted so memory fragmentation is not an issue here.
*/


// input objects
Switch *motor_switch;
Switch *machinegun_switch;
Switch *bombdrop_switch;
Tachometer *tachometer;

// domain objects
MachineGuns *machineguns;
Motor *motor;
Sound *bomb_drop;
Zoom *zoom;

// output objects
Output *onboard_LED;
Navlights *navlights; 

// other
SerialInterpreter *serialInterpreter;
SerialInterpreter *bluetoothInterpreter;
SoundManager *theSoundManager;

void setup()
{
    // USB terminal
    Serial.begin(9600);
    while(!Serial && !Serial.available()){}

    // bluetooth
    Serial1.begin(9600);
    while(!Serial1 && !Serial1.available()){}

    Log.begin(LOG_LEVEL_VERBOSE, &Serial);

    // Inputs
    motor_switch = new Switch(2, INPUT_PULLUP,  MOTOR_STARTER_START, MOTOR_STARTER_STOP);
    machinegun_switch = new Switch(3, INPUT_PULLUP,  MACHINEGUNS_START, MACHINEGUNS_STOP);
    bombdrop_switch = new Switch(4, INPUT_PULLUP, DROP_BOMB); 
    tachometer = new Tachometer();

    // Domain Objects
    machineguns = new MachineGuns();
    motor = new Motor();
    bomb_drop = new Sound("bombdrop.wav", BOMB_DROP_PRIORITY, false, DROP_BOMB, 0.20);
    zoom = new Zoom();
 
    //outputs
    onboard_LED = new Output(LED_BUILTIN, ONBOARD_LED_ON, ONBOARD_LED_OFF);
    navlights = new Navlights();

    theSoundManager = new SoundManager();
    serialInterpreter = new SerialInterpreter();
    bluetoothInterpreter = new SerialInterpreter(Serial1);

    Log.trace(F("setup complete\n"));
}


void loop()
{
    static bool is_first_loop = true;

    // play 'startup_wav' once at power up
    if (is_first_loop)
    {
        theSoundManager->play("startup.wav", STARTUP_PRIORITY, false);
        is_first_loop = false;
        Log.trace(F("Free mem: %d\n"), FreeMem());
    }

    // debounce and send events for switches
    motor_switch->update();
    machinegun_switch->update();
    bombdrop_switch->update();
    tachometer->update();

    // update system objects
    theSoundManager->update();
    serialInterpreter->update();
    bluetoothInterpreter->update();
    motor->update();
    navlights->update();
    zoom->update();

    event_dispatcher.run();
}
