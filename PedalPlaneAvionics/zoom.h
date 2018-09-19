#ifndef _zoom_h_
#define _zoom_h_
#
#include "sound.h"

#include <Adafruit_LIS3DH.h>

class Zoom
{
    public:
        Zoom();
        void update();

    private:
        Adafruit_LIS3DH lis3dh;    
        uint32_t timeOfLastZoom;
        Sound *sounds[3];

};

#endif
