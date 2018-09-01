#include "output.h"

void Output::onEvent(int event, int param)
{
    Log.trace(F("Output %d event: %d"), pin, event);
    if (event == this->high_event) {
        setHigh();
    }
    else if (event == this->low_event) {
        setLow();
    }
}
