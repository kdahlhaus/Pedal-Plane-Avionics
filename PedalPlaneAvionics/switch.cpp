// Copyright 2018 by Kevin Dahlhausen

#include "switch.h"
#include "send_event.h"
#include <ArduinoLog.h>

Switch::Switch(int pin, int pin_mode, int high_to_low_event, int low_to_high_event)
{
    this->pin = pin;
    this->high_to_low_event = high_to_low_event;
    this->low_to_high_event = low_to_high_event;
    debouncer.attach(pin, pin_mode);

}

void Switch::update()
{
    debouncer.update();
    
    if (low_to_high_event && debouncer.rose())
    {
        Log.trace("switch %d l to h\n", pin);
        send_event(low_to_high_event);
    }
    else if (high_to_low_event && debouncer.fell())
    {
        Log.trace("switch %d h to l\n", pin);
        send_event(high_to_low_event);
    } 
}
