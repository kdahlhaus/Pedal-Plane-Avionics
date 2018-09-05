#include "send_event.h"

#include <EventQueue.h>

EventQueue q;

void send_event(int event_code, void *param) 
{
    q.enqueueEvent(event_code, param);
}
