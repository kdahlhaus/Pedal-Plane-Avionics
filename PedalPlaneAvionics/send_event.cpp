#include "send_event.h"

#include <EventQueue.h>
#include <ArduinoLog.h>

EventQueue q;

void send_event(int event_code, void *param) 
{
    bool ok = q.enqueueEvent(event_code, param);
    Log.trace("send_event(%d,) = %b\n", event_code, ok);
}
