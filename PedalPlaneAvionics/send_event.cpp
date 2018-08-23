#include "send_event.h"
#
#include <EventQueue.h>

extern EventQueue q;

void send_event(int event_code) 
{
    q.enqueueEvent(event_code, 0);
}
