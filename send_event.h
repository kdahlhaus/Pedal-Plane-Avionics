#ifndef _send_event_h
#define _send_event_h

/*
    Send the given avionics event with an 
    optional parameter.

    For example:
        send_event(MACHINE_GUNS_STOP);
        send_event(MOTOR_SPEED, (void *)50);

*/
void send_event(int event_code, void *param=0);

#endif
