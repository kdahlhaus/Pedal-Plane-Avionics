// Copyright 2018 by Kevin Dahlhausen

/*
 * mostart.wav
 * morun.wav
 * mostop.wav
 *
 */
class Motor
{
    public:

        Motor();

        void start();
        void stop();

        void onEvent(int event, void *param);
        void setStartGain(float gain);
        void setRunGain(float gain);

        void update();

    protected:

        enum State {
            stopped,
            waiting_for_starting,
            starting,
            running,
            stopping
        };
        State state;

        float startGain;
        float runGain;

        void *sound_handle;
};
