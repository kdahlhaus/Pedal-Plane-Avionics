#ifndef _register_event_listener_h_
#define _register_event_listener_h_

#include <EventDispatcher.h>
#include <Functor.h>

/*
    Registers a Functor to be called when the given event code is sent.

    Required Signature:
        void Function( int event, void *param)

*/
bool register_event_listener(int ev_code, EventListener f);

#endif
