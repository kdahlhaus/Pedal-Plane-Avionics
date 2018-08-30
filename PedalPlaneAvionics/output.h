// Copyright 2018 Kevin Dahlhausen

#ifndef _output_h
#define _output_h

#include <ArduinoLog.h>


class Output
{
    public:
        Output(int pin, int high_event, int low_event, int initial_state=LOW)
        {
            this->pin = pin;
            this->high_event = high_event;
            this->low_event = low_event;
            if (initial_state == LOW) { setLow(); } else { setHigh(); }
        }

        void setHigh() { Log.trace(F("setHigh")); digitalWrite(pin, HIGH); } 
        void setLow() { Log.trace(F("setLow")); digitalWrite(pin, LOW); }

        void onEvent(int event, int param);

    protected:
        int pin;
        int high_event;
        int low_event;

};

#endif
