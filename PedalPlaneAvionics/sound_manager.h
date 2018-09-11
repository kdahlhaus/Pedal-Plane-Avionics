// sound_manager.h 
// Copyright 2018 by Kevin Dahlhausen

#ifndef _sound_manager_h
#define _sound_manager_h

class SoundManager 
{
    public:
        // play the sound given the file name
        // return a HANDLE to the sound for future queries
        // or null if the sound can't be found or played
        // higher #s reflect higher priority and may 
        // replace lower priority sounds
        void *play(const char *filename, int priority, bool loop=false);

        void stop(void *handle);

        bool is_playing(void *handle);

        void setup();

        void update();

};

extern SoundManager *theSoundManager;

#endif
