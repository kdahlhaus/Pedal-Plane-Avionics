#include "register_event_listener.h"

#include <EventQueue.h>
#include <EventDispatcher.h>

extern EventQueue q;
EventDispatcher event_dispatcher(&q);

bool register_event_listener(int ev_code, EventListener f) 
{
    event_dispatcher.addEventListener(ev_code, f);
}

