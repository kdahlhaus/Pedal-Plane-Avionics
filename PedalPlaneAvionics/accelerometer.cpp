#include "accelerometer.h"
#include "avionics_events.h"
#include "send_event.h"
#include <ArduinoLog.h>

#define RAW_ACCEL_TO_TRIGGER_ZOOM 3000
#define MIN_TIME_BETWEEN_TRIGGERS 3000

#define LIS3DH_I2C_ADDRESS 0x18  // alt is 0x19

Accelerometer::Accelerometer() :
    lis3dh(), timeOfLastZoom(0)
{
    if (!lis3dh.begin(LIS3DH_I2C_ADDRESS)) {
        Log.error("Could not find LIS3DH at %h", LIS3DH_I2C_ADDRESS);
    }
    lis3dh.setRange(LIS3DH_RANGE_4_G);   // 2, 4, 8 or 16 G!
}

void Accelerometer::update()
{
    lis3dh.read();
    // using raw for now  
    // TODO: move to normalized #'s to see if it fixes the accel touchiness
    Log.trace("accel x:%d y:%d\n", lis3dh.x, lis3dh.y);
    if ((abs(lis3dh.y) >= RAW_ACCEL_TO_TRIGGER_ZOOM || abs(lis3dh.x) >= RAW_ACCEL_TO_TRIGGER_ZOOM) &&
        millis() > timeOfLastZoom + MIN_TIME_BETWEEN_TRIGGERS) {
        send_event(ZOOM2);
        timeOfLastZoom = millis();
        Log.trace("sending zoom due to accel\n");
    }
}