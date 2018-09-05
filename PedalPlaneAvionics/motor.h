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

        void update();

    protected:

        enum State {
            stopped,
            starting,
            running,
            stopping
        };
        State state;

        void *sound_handle;
};
