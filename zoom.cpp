#include "zoom.h"

#include "avionics_events.h"
#include "send_event.h"
#include "sound_priorities.h"
#include "config.h"
#include "register_event_listener.h"

#include <ArduinoLog.h>



#define RAW_ACCEL_TO_TRIGGER_ZOOM 4000
#define MIN_TIME_BETWEEN_TRIGGERS 5000

#define LIS3DH_I2C_ADDRESS 0x18  // alt is 0x19


Zoom::Zoom() :
    sounds("/zoom", ZOOM_PRIORITY, GAIN_FUNCTION(c, Config::zoomGain)),  lis3dh(), timeOfLastZoom(0)
{
    if (!lis3dh.begin(LIS3DH_I2C_ADDRESS)) {
        Log.error("Could not find LIS3DH at %h", LIS3DH_I2C_ADDRESS);
    }
    lis3dh.setRange(LIS3DH_RANGE_4_G);   // 2, 4, 8 or 16 G!
    register_event_listener(ZOOM, makeFunctor((EventListener *)0, (*this), &Zoom::onEvent)); 
}

void Zoom::onEvent(int event, void *param) 
{
    if (!sounds.isPlaying()) {
        sounds.start();
    }
}


void Zoom::update()
{
    lis3dh.read();
    // using raw for now  
    // TODO: move to normalized #'s to see if it fixes the accel touchiness
    if ((abs(lis3dh.y) >= RAW_ACCEL_TO_TRIGGER_ZOOM || abs(lis3dh.x) >= RAW_ACCEL_TO_TRIGGER_ZOOM) &&
        millis() > timeOfLastZoom + MIN_TIME_BETWEEN_TRIGGERS) {
        sounds.start();
        timeOfLastZoom = millis();
        Log.trace("Zoom! accel x:%d y:%d\n", lis3dh.x, lis3dh.y);
    }
}
