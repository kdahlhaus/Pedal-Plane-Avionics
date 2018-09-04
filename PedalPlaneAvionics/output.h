// Copyright 2018 Kevin Dahlhausen

#ifndef _output_h
#define _output_h

#include <ArduinoLog.h>


class Output
{
    public:
        Output(int pin, int high_event, int low_event)
        {
            Log.trace("Output(pin=%d...\n", pin);
            pinMode(pin, OUTPUT);
            this->pin = pin;
            this->high_event = high_event;
            this->low_event = low_event;
        }

        void setHigh();
        void setLow();
        void onEvent(int event, int param);

    protected:
        int pin;
        int high_event;
        int low_event;

};

#endif
