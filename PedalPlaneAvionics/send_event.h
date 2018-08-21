#ifndef _send_event_h
#define _send_event_h

#include <EventQueue.h>


extern EventQueue q;

void send_event(int event_code) { q.enqueueEvent(event_code, -1); }

#endif
