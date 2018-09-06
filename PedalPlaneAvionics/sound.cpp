#include "sound.h"

#include <ArduinoLog.h>
#include "register_event_listener.h"

Sound::Sound(const char *file_name, int priority, bool loop, int start_event, int stop_event)
{
    this->file_name = file_name;
    this->start_event = start_event;
    this->stop_event = stop_event;
    handle = 0;
    this->priority = priority;
    this->loop = false;
}

void Sound::register_el()
{
    register_event_listener(start_event, makeFunctor((EventListener *)0, (*this), &Sound::onEvent));
    if (stop_event > 0)
    {
        register_event_listener(stop_event, makeFunctor((EventListener *)0, (*this), &Sound::onEvent));
    }
}

void Sound::start()
{
    handle = theSoundManager->play(file_name, priority, loop);        
    Log.trace(F("Sound('%s').start: pri:%d, loop=%T handle:%d\n"), file_name, priority, loop, (int)handle);
}

void Sound::stop()
{
    Log.trace(F("Sound('%s').stop handle:%d\n"), file_name, (int)handle);
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
 
void Sound::onEvent(int event, void *param)
{
    Log.trace(F("Sound('%s') event: %d\n"), file_name, event);
    if (event == start_event) {
        start();
    }
    else if (event == stop_event) {
        stop();
    }
}
