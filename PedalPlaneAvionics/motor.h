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
        void register_el();

    protected:

        enum State {
            stopped,
            waiting_for_starting,
            starting,
            running,
            stopping
        };
        State state;

        void *sound_handle;
};
