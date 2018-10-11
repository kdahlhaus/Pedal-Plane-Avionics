#include "sound.h"

#include <ArduinoLog.h>
#include "register_event_listener.h"

Sound::Sound(const char *file_name, int priority, bool loop, int start_event, float gain, int stop_event):
    file_name(file_name), start_event(start_event), stop_event(stop_event),
    handle(0), priority(priority), loop(false), gain(gain)
{
    if (start_event) {
        register_event_listener(start_event, makeFunctor((EventListener *)0, (*this), &Sound::onEvent));
    }
    if (stop_event > 0) {
        register_event_listener(stop_event, makeFunctor((EventListener *)0, (*this), &Sound::onEvent));
    } 
}


void Sound::start()
{
    handle = theSoundManager->play(file_name, priority, loop, gain);        
    Log.trace(F("Sound('%s').start: pri:%d, loop=%T handle:%d gain:%F\n"), file_name, priority, loop, (int)handle, gain);
}

void Sound::stop()
{
    Log.trace(F("Sound('%s').stop handle:%d\n"), file_name, (int)handle);
    if (theSoundManager->isPlaying(handle)) {
        theSoundManager->stop(handle);
        handle = 0;
    }
}

bool Sound::isPlaying()
{
    return handle && theSoundManager->isPlaying(handle);
}

void Sound::setGain(float gain)
{
    Log.trace(F("Sound setGain=%F\n"), gain);
    this->gain=gain;
    if (isPlaying()) {
        theSoundManager->setGain(handle, gain);
    }
}
 
void Sound::onEvent(int event, void *param)
{
    Log.trace(F("Sound('%s') event: %d\n"), file_name, event);
    if (event == start_event) { start();
    }
    else if (event == stop_event) {
        stop();
    }
}
