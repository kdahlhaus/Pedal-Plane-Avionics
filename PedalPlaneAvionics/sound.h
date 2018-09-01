// Copyright 2018 Kevin Dahlhausen

#ifndef _sound_h_
#define _sound_h_

#include "sound_manager.h"

class Sound
{
    public:
        Sound(const char *file_name, int priority, bool loop, int start_event, int stop_event = 0);
        void start();
        void stop();
        bool is_playing();
 
        void onEvent(int event, int param);

    protected:
        const char *file_name;
        int priority;
        bool loop;
        int start_event;
        int stop_event;
        void *handle;
};

#endif
