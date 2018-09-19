#ifndef _accelerometer_h_
#define _accelerometer_h_

#include <Adafruit_LIS3DH.h>

class Accelerometer
{
    public:
        Accelerometer();
        void update();

    private:
        Adafruit_LIS3DH lis3dh;    
        uint32_t timeOfLastZoom;

};

#endif
