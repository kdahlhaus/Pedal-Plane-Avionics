// Copyright 2018 by Kevin Dahlhausen

#include "tachometer.h"

#include <ArduinoLog.h>
#include "avionics_events.h"
#include "send_event.h"


#define IOPIN 5
#define MILLIS_SAME_TO_CONSIDER_STOPPED 1500
#define CHANGES_PER_ROTATION 4
#define TIME_BETWEEN_RPM_UPDATE_EVENTS_MS 500


MeanFilter<int> rpm_filter3(3);
MeanFilter<int> rpm_filter7(7);
MeanFilter<int> rpm_filter9(9);
 

Tachometer::Tachometer() :
    rpm_filter(5)
{
    pinMode(IOPIN, INPUT_PULLUP);
    last_state = digitalRead(IOPIN);
    time_of_last_state_change = millis();
    is_moving = false;
    rpm = 0;
    time_of_last_rpm_event = 0;
    rpm_filter.setAllTo(0);
    rpm_filter3.setAllTo(0);
    rpm_filter7.setAllTo(0);
    rpm_filter9.setAllTo(0);
}

void Tachometer::update()
{
    int current_state = digitalRead(IOPIN);
    unsigned long current_time = millis();

    if (current_state != last_state) {
        // saw input state change

        if (is_moving) {
            rpm = 60000 / ( CHANGES_PER_ROTATION * (current_time - time_of_last_state_change));            
            rpm_filter.add(rpm);
            rpm_filter3.add(rpm);
            rpm_filter7.add(rpm);
            rpm_filter9.add(rpm);
        }

        if (!is_moving) {
            // started moving
            send_event(TACH_STARTED_MOVING);
            Log.trace("started moving\n");
        }

        is_moving = true;
        time_of_last_state_change = current_time;
        last_state = current_state;

    }
    else {
        // check for stopped
        if (is_moving && (current_time - time_of_last_state_change >= MILLIS_SAME_TO_CONSIDER_STOPPED)) {
            // stopped moving
            send_event(TACH_STOPPED_MOVING);
            Log.trace("stopped moving\n");
            is_moving = false;
            rpm = 0;
            rpm_filter.setAllTo(0);
            rpm_filter3.setAllTo(0);
            rpm_filter7.setAllTo(0);
            rpm_filter9.setAllTo(0);
        }
    }

#if 0
    // send RPM every TIME_BETWEEN_RPM_UPDATE_EVENTS_MS ms
    if (current_time - time_of_last_rpm_event >= TIME_BETWEEN_RPM_UPDATE_EVENTS_MS) {
        //send_event(TACH_RPM, (void *)rpm);
        time_of_last_rpm_event = current_time;
        Log.trace("  rpm: %d f3: %d, f5: %d, f7: %d, f9: %d\n", rpm, rpm_filter3.mean(), rpm_filter.mean(), rpm_filter7.mean(),rpm_filter9.mean() );
    }
#endif
}
