#include "output.h"

void Output::setHigh() { digitalWrite(pin, HIGH); Log.trace(F("setHigh pin %d\n"), pin); } 
void Output::setLow() { digitalWrite(pin, LOW); Log.trace(F("setLow pin %d\n"), pin); }

void Output::onEvent(int event, int param)
{
    Log.trace(F("Output %d event: %d\n"), pin, event);
    if (event == this->high_event) {
        setHigh();
    }
    else if (event == this->low_event) {
        setLow();
    }
}
