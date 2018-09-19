// Copyright 2018 by Kevin Dahlhausen

#define SERIALCOMMAND_DEBUG 1

#include <ArduinoLog.h>
#include <EventDispatcher.h>
//#include <Tasker.h>

#include "accelerometer.h"
#include "avionics_events.h"
#include "interpreter.h"
#include "machineguns.h"
#include "motor.h"
#include "navlights.h"
#include "output.h"
#include "sound.h"
#include "sound_manager.h"
#include "sound_priorities.h"
#include "switch.h"

//Tasker tasker;
extern EventDispatcher event_dispatcher;

// input objects
Switch *motor_switch;
Switch *machinegun_switch;
Switch *bombdrop_switch;
Accelerometer *accelerometer;

// domain objects
// allocated dynamically from setup() so they
// can initialize pins and things as 
// needed. 
MachineGuns *machineguns;
Motor *motor;
Sound *bomb_drop;
Sound *zoom1;
Sound *zoom2;
Sound *zoom3;

// output objects
Output *onboard_LED;
Navlights *navlights; 

// other
SerialInterpreter *serialInterpreter;
SerialInterpreter *bluetoothInterpreter;


void setup()
{
    Serial.begin(9600);
    while(!Serial && !Serial.available()){}

    Serial1.begin(9600);
    while(!Serial1 && !Serial1.available()){}
    //delay(10) // alternative if it hangs

    Log.begin(LOG_LEVEL_VERBOSE, &Serial);

    // Inputs
    motor_switch = new Switch(2, INPUT_PULLUP,  MOTOR_STARTER_START, MOTOR_STARTER_STOP);
    machinegun_switch = new Switch(3, INPUT_PULLUP,  MACHINEGUNS_START, MACHINEGUNS_STOP);
    bombdrop_switch = new Switch(4, INPUT_PULLUP, DROP_BOMB); 
    accelerometer = new Accelerometer();

    // Domain Objects0
    machineguns = new MachineGuns();
    motor = new Motor();
    bomb_drop = new Sound("bombdrop.wav", BOMB_DROP_PRIORITY, false, DROP_BOMB, 0.20);
    zoom1 = new Sound("zoom1.wav", ZOOM_PRIORITY, false, ZOOM1);
    zoom2 = new Sound("zoom2.wav", ZOOM_PRIORITY, false, ZOOM2, 0.5);
    zoom3 = new Sound("zoom3.wav", ZOOM_PRIORITY, false, ZOOM3, 0.5);
 

    //outputs
    onboard_LED = new Output(LED_BUILTIN, ONBOARD_LED_ON, ONBOARD_LED_OFF);
    navlights = new Navlights();

    theSoundManager->setup();
    serialInterpreter = new SerialInterpreter();
    bluetoothInterpreter = new SerialInterpreter(Serial1);

    //tasker.setInterval([&q](){q.enqueueEvent(MACHINEGUNS_START);}, 3500); // TESTING DELETE ME
    //tasker.setInterval([&q](){q.enqueueEvent(MACHINEGUNS_STOP);}, 3750); // TESTING DELETE ME
    Log.trace(F("setup complete\n"));
}

uint32_t FreeMem(){ // for Teensy 3.0
    uint32_t stackTop;
    uint32_t heapTop;

    // current position of the stack.
    stackTop = (uint32_t) &stackTop;

    // current position of heap.
    void* hTop = malloc(1);
    heapTop = (uint32_t) hTop;
    free(hTop);

    // The difference is (approximately) the free, available ram.
    return stackTop - heapTop;
}


void loop()
{
    static bool is_first_loop = true;
    if (is_first_loop)
    {
        void *handle = theSoundManager->play("startup.wav", STARTUP_PRIORITY, false);
        Log.trace(F("startup handle = %d\n"), (int)handle);
        is_first_loop = false;
        Log.trace(F("Free mem: %d\n"), FreeMem());
    }

    event_dispatcher.run();
    //tasker.loop();

    // debounce and send events for switches
    motor_switch->update();
    machinegun_switch->update();
    bombdrop_switch->update();

    theSoundManager->update();
    serialInterpreter->update();
    bluetoothInterpreter->update();
    motor->update();
    navlights->update();
    accelerometer->update();
}
