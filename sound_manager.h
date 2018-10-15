// sound_manager.h 
// Copyright 2018 by Kevin Dahlhausen

#ifndef _sound_manager_h
#define _sound_manager_h

/*
    Manages playing of multiple sounds at once.
    Most often used by other objects and not directly.
*/

class SoundManager 
{
    public:
        SoundManager() { setup(); }

        // play the sound given the file name
        // return a HANDLE to the sound for future queries
        // or null if the sound can't be found or played
        // higher #s reflect higher priority and may 
        // replace lower priority sounds
        void *play(const char *filename, int priority, bool loop=false, float gain=1.0);

        void stop(void *handle);

        bool isPlaying(void *handle);

        // set overall gain
        void setOverallGain(float gain);

        void setGain(void *handle, float gain);

        void update();
        float readVolumePotentiometer();

    protected:
        float currentVolume;

    private:
        // called by ctor so no need to call manually
        void setup();

};

extern SoundManager *theSoundManager;

#endif
