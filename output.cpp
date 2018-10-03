#include "output.h"
#include "register_event_listener.h"

Output::Output(int pin, int high_event, int low_event)
{
    Log.trace("Output(pin=%d...\n", pin);
    pinMode(pin, OUTPUT);
    this->pin = pin;
    this->high_event = high_event;
    this->low_event = low_event;

    register_event_listener(high_event, makeFunctor((EventListener *)0, (*this), &Output::onEvent));
    register_event_listener(low_event, makeFunctor((EventListener *)0, (*this), &Output::onEvent));
}

void Output::setHigh() { digitalWrite(pin, HIGH); Log.trace(F("setHigh pin %d\n"), pin); } 
void Output::setLow() { digitalWrite(pin, LOW); Log.trace(F("setLow pin %d\n"), pin); }

void Output::onEvent(int event, void *param)
{
    Log.trace(F("Output %d event: %d\n"), pin, event);
    if (event == this->high_event) {
        setHigh();
    }
    else if (event == this->low_event) {
        setLow();
    }
}
