
#include "random_sound.h"

#include <ArduinoLog.h>
#include <SD.h>

#include "register_event_listener.h"

RandomSound::RandomSound(const char *directory, int priority, bool loop, int start_event, int stop_event):
    directory(directory), start_event(start_event), stop_event(stop_event),
    handle(0), priority(priority), loop(false)
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
    if (numberOfSounds) {
    float gain = 1.0; // TODO handle gain in RandomSound
    handle = theSoundManager->play(absoluteSoundFileName, priority, loop, gain);        
    Log.trace(F("RandomSound('%s').started: pri:%d, loop=%T handle:%d gain:%F\n"), absoluteSoundFileName, priority, loop, (int)handle, gain);
    }
    else {
        Log.error(F("found no sounds in '%s' to play\n"), directory);
    }
}

void RandomSound::stop()
{
    Log.trace(F("RandomSound('%s').stop handle:%d\n"), directory, (int)handle);
    if (theSoundManager->is_playing(handle)) {
        theSoundManager->stop(handle);
        handle = 0;
    }
}

bool RandomSound::isPlaying()
{
    return handle && theSoundManager->is_playing(handle);
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
            Log.error(F("Error finding the %d sound in %s\n"), indexOfNextSound, directory);
            return;
        }
        if (num==indexOfNextSound) {
            //entry.close();
            strncpy(absoluteSoundFileName, directory, MAX_SOUND_FILENAME_LENGTH);
            Log.trace(F("entry.name=%s\n"), entry.name());
            strncat(absoluteSoundFileName, "/", MAX_SOUND_FILENAME_LENGTH);
            strncat(absoluteSoundFileName, entry.name(), MAX_SOUND_FILENAME_LENGTH);
            Log.trace(F("absoluteSoundFileName: '%s'\n"), absoluteSoundFileName);
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
