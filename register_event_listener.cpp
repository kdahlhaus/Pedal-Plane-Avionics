#include "register_event_listener.h"

#include <EventQueue.h>
#include <EventDispatcher.h>
#include <ArduinoLog.h>

extern EventQueue q;
EventDispatcher event_dispatcher(&q);

bool register_event_listener(int ev_code, EventListener f) 
{
    bool ok = event_dispatcher.addEventListener(ev_code, f);
    Log.trace("register event listener (%d,) = %b\n", ev_code, ok);
    return ok;
}
