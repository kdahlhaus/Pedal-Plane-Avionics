// Copyright 2018 Kevin Dahlhausen

#ifndef _output_h
#define _output_h

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

        void setHigh() { digitalWrite(pin, HIGH); } 
        void setLow() { digitalWrite(pin, LOW); }

        void onEvent(int event, int param)
        {
            if (event == high_event) {
                setHigh();
            }
            else if (event == low_event) {
                setLow();
            }
        }

    protected:
        int pin;
        int high_event;
        int low_event;

};

#endif
