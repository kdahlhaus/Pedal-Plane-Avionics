#include "sound.h"

#include <ArduinoLog.h>


Sound::Sound(const char *file_name, int priority, bool loop, int start_event, int stop_event)
{
    file_name = file_name;
    start_event = start_event;
    stop_event = stop_event;
    handle = 0;
    priority = priority;
    loop = false;
}

void Sound::start()
{
    handle = theSoundManager->play(file_name, priority, loop);        
}

void Sound::stop()
{
    if (theSoundManager->is_playing(handle))
    {
        theSoundManager->stop(handle);
        handle = 0;
    }
}

bool Sound::is_playing()
{
    return handle && theSoundManager->is_playing(handle);
}
 


void Sound::onEvent(int event, int param)
{
    Log.trace(F("Sound('%s')  event: %d"), file_name, event);
    if (event == start_event) {
        start();
    }
    else if (event == stop_event) {
        stop();
    }
}
