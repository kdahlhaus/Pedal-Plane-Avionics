// Copyright 2018 Kevin Dahlhausen

#ifndef _radio_h_
#define _radio_h_

/*
    Plays "radioon.wav" on startChatter, then a random sound
    from "/radio" every so often.  Plays "radiooff.wav" on
    stopChatter.
*/

#include "random_sound.h"
#include "sound.h"

class Radio
{
    public:
        Radio();
        void startChatter();
        void stopChatter();
        void onEvent(int event, void *param);
        void update();

    protected:

        void startNextSound();

        Sound onSound;
        Sound offSound;
        RandomSound chatterSounds;
        bool chatterIsEnabled;
        long timeOfNextSound;
};

#endif
