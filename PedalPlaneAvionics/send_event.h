#ifndef _send_event_h
#define _send_event_h

/*
    Send the given avionics event with an 
    optional parameter.
*/
void send_event(int event_code, void *param=0);

#endif
