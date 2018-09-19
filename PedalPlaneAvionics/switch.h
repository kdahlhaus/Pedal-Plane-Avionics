// Copyright 2018 by Kevin Dahlhausen

#ifndef switch_h
#define switch_h

#include <Bounce2.h>

/*
    A de-bounced digital input that sends events on transitions.
*/

class Switch
{
    public:

        Switch(int pin, int pin_mode, int high_to_low_event, int low_to_high_event=0);
        // NOTE: Switches must either be new'd from within setup() or 
        //       have an external pinMode(pin #, mode) call within setup.
        // pin:  digital input to read
        // pin_mode: mode to configure that digital input
        // high_to_low_event:  event to send on input high to low transition, 0 to disable
        // low_to_high_event:  event to send on input low to high transition, 0 to disable

        void update();
        // Update and debounce the switch and send events as appropriate.

    protected:
        int pin;
        int high_to_low_event;
        int low_to_high_event;
        Bounce debouncer;
};

#endif
