// Copyright 2018 Kevin Dahlhausen

#ifndef random_sound_h_
#define random_sound_h_

#include "sound_manager.h"

/*
    Play a psueudo-random sound from a directory of sound clips.
    Randomly picks a place in the list of sounds to start,
    then plays the sounds in sequence after that, looping
    back to the first in the dir after the last is played.
*/


// '/12345678/12345678/12345678/12345678.123\0 arbritrary limit 
// to the depth of directories in our file structure
#define MAX_SOUND_FILENAME_LENGTH 45


class RandomSound
{
    public:
        /*
            directory: directory containing one or more sound files of this type
            priority: used to resolve contention for sound channels.  Sounds with 
                      higher numbered priorities will replace those with lower numbered
                      priorities if too many sounds are played at once.
            loop: loop the sound until stopped if true
            start_event:  id of event that starts sound when received, 0 to ignore events
            stop_event: id of event that stops sound when received, 0 to ignore events

            e.g.:
                RandomSound *radio = new Sound("/radio", RADIO_PRIORITY, false, RADIO_CLIP_PLAY, 0.20);
                
        */
        RandomSound(const char *directory, int priority = 1, bool loop = false, int start_event = 0, int stop_event = 0);

        void start();
        void stop();
        bool isPlaying();
        void onEvent(int event, void *param);

    protected:
        const char *directory;
        int start_event;
        int stop_event;
        void *handle;
        int priority;
        bool loop;


        char absoluteSoundFileName[MAX_SOUND_FILENAME_LENGTH];
        const char *fileNameOfNextSoundToPlay;
        int numberOfSounds;
        int indexOfNextSound;

        // determine the next sound to play 
        // and set fileNameOfNextSoundToPlay appropriately
        void setNextSoundToPlay();

        int getNumberOfSoundsInDirectory(const char *directory);

};

#endif
