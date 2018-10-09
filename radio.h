// Copyright 2018 Kevin Dahlhausen

#ifndef _radio_h_
#define _radio_h_

/*
    Plays radio "chatter" every so-often.
*/

#include "random_sound.h"

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

        RandomSound sounds;
        bool chatterIsEnabled;
        long timeOfNextSound;
};

#endif
