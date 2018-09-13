// Copyright 2018 by Kevin Dahlhausen

/*
 * starter.wav
 * starting.wav
 * idle.wav
 * run.wav
 * stop.wav
 */

#include <Audio.h>

class Motor
{
    public:

        Motor();

        void start(bool viaStarter=false);
        // viaStarter = if true, will run the 'starting' sound
        // until MOTOR_STARTER_STOP event received
        // otherwise will run the start sound once and transition
        // to starting sound
        
        void stop();

        void onEvent(int event, void *param);
        void setStartGain(float gain);
        void setIdleGain(float gain);
        void setRunGain(float gain);

        void update();

    protected:

        enum State {
            stopped,
            starter,
            starting,
            idle,
            transitioning_to_run,
            running,
            transitioning_to_stopped,
            fading

        };
        State state;

        float starterGain;
        float startingGain;
        float idleGain;
        float runGain;

        typedef struct {
            AudioPlaySdWav sdWav;
            AudioEffectFase fader;
            bool loop;
            uint32_t timeStarted;
        } SoundChannel;

        SoundChannel channel1;
        SoundChannel channel2;

        SoundChannel *currentChannel;
        SoundChannel *nextChannel;

        // set true is we receive 'stop'
        // (will complete any transitions and then transition to stopped)
        bool shouldStop;

        // state to change to after current fade
        State nextStateAfterFade;

        void fadeTo( const char *fileName, float gain, bool loop,  State nextState);
        // fade into the sound at fileName and transition to 'nextState'

        inline bool soundStartDelayHasPassed()
        // has enough time passed since the sound started that isPlaying is valid?
        {
            return millis() >= currentChannel->timeStarted + 4; // PJRC max 3 ms to start sound
        }

        inline bool currentChannelIsPlaying()
        {
            currentChannel->sdWav.isPlaying();
        }

        void changeFromStarterToStarting();
        // this is done in multiple places so refctored to a func.


};
