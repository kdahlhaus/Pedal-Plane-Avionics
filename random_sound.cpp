
#include "sound.h"

#include <ArduinoLog.h>
#include "register_event_listener.h"

RandomSound::RandomSound(const char *directory, int priority, bool loop, int start_event, float gain, int stop_event):
    directory(directory), start_event(start_event), stop_event(stop_event),
    handle(0), priority(priority), loop(false), gain(gain)
{
    if (start_event) {
        register_event_listener(start_event, makeFunctor((EventListener *)0, (*this), &RandomSound::onEvent));
    }
    if (stop_event) {
        register_event_listener(stop_event, makeFunctor((EventListener *)0, (*this), &RandomSound::onEvent));
    } 

    // start randomly somewhere in the list of sounds
    numberOfSounds = getNumberOfSoundsInDirectory(directory);
    indexOfNextSound = random(numberOfSounds);
    setNextSoundToPlay();

}


void RandomSound::start()
{
    handle = theRandomSoundManager->play(directory, priority, loop, gain);        
    Log.trace(F("RandomSound('%s').start: pri:%d, loop=%T handle:%d gain:%F\n"), directory, priority, loop, (int)handle, gain);
}

void RandomSound::stop()
{
    Log.trace(F("RandomSound('%s').stop handle:%d\n"), directory, (int)handle);
    if (theRandomSoundManager->is_playing(handle)) {
        theRandomSoundManager->stop(handle);
        handle = 0;
    }
}

bool RandomSound::is_playing()
{
    return handle && theRandomSoundManager->is_playing(handle);
}

void RandomSound::setGain(float gain)
{
    Log.trace(F("RandomSound setGain=%F\n"), gain);
    this->gain=gain;
    if (is_playing()) {
        theRandomSoundManager->setGain(handle, gain);
    }
}
 
void RandomSound::onEvent(int event, void *param)
{
    Log.trace(F("RandomSound('%s') event: %d\n"), directory, event);
    if (event == start_event) {
        start();
    }
    else if (event == stop_event) {
        stop();
    }
}




void RandomSound::setNextSoundToPlay()
{
    indexOfNextSound = ++indexOfNextSound % numberOfSounds;
    int num = 0;
    File dir = SD.open(directory);
    while (true) {
        File entry = dir.openNextFile();              
        if (!entry) {
            Log.error(F("Error find the %d sound in %s\n"), indexOfNextSound, directory);
            return;
        }
        if (num==indexOfNextSound) {
            //entry.close();
            strncpy(absoluteSoundFileName, directory, MAX_SOUND_FILENAME_LENGTH);
            stncat(absoluteSoundFileName, entry.name(), MAX_SOUND_FILENAME_LENGTH);
            entry.close(); 
            return;
        } else {
            ++num;
            entry.close(); 
        }
    }
}

int RandomSound::getNumberOfSoundsInDirectory(const char *aDirectory)
{
    int num=0;
    File dir = SD.open(aDirectory);
    while (true)
    {
        File entry = dir.openNextFile();
        if (!entry)
        {
            return num;
        }
        ++num;
        entry.close();
    }
    // return is a few lines above
}
