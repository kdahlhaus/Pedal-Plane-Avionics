#include "output.h"

void Output::onEvent(int event, int param)
{
        {
            Log.trace(F("Output %d event: %d"), pin, event);
            if (event == high_event) {
                setHigh();
            }
            else if (event == low_event) {
                setLow();
            }
        }         
}
