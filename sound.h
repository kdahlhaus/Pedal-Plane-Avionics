// Copyright 2018 Kevin Dahlhausen

#ifndef _sound_h_
#define _sound_h_

#include "sound_manager.h"
#include <Functor.h>


/*
    A sound that is started/stopped by events or calls to start()/stop().
*/

class Sound
{
    public:
        /*
            file_name: full path/name to a 16bit 44.1 kHz PCM wav file
            priority: used to resolve contention for sound channels.  Sounds with 
                      higher numbered priorities will replace those with lower numbered
                      priorities if too many sounds are played at once.
            loop: loop the sound until stopped if true
            start_event:  id of event that starts sound when received
            gain: adjust volume of playback by this (%)
            gainFunction: (optional) a functor that takes no arguments and returns the float to use for gain.  This overrides 'gain' if it is set.
                e.g. makeFunctor((Functor0wRet<float> *)0, (c), &Config::motorGain)
            stop_event: id of event that stops sound when received

            e.g.:
                Sound *bomb_drop = new Sound("bombdrop.wav", BOMB_DROP_PRIORITY, false, DROP_BOMB, 0.20);
                
        */
        Sound(const char *file_name, int priority=1, bool loop=false, int start_event=0, float gain=1.0, Functor0wRet<float> gainFunction=0, int stop_event=0);

        void start();
        void stop();
        bool isPlaying();
        void setGain(float gain);
 
        void onEvent(int event, void *param);


    protected:
        const char *file_name;
        int start_event;
        int stop_event;
        void *handle;
        int priority;
        bool loop;
        float gain;
        Functor0wRet<float> gainFunction;
};

#endif
