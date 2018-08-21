# Pedal-Plane Avionics
Firmware for a pedal-plane special-effects controller.  It will do things like control the motor sound, machine gun lights and sound, and navigation lights.

This is very much a work-in-progress.

## Libraries
    1. cd to your Arduino libs directory.
        (The default is ~/Arduino/libraries on Linux
        or ~/Documents/Arduino/libraries on Windows)
    2. clone https://github.com/kdahlhaus/EventSystem 
    3. clone  https://github.com/kroimon/Arduino-SerialCommand (or see below)
        (note that this is not the one in the Arduino lib manager)
    4. Use the library manager to add:
        * ArduinoLog (or https://github.com/thijse/Arduino-Log/)
        * Tasker


* You can optionally clone the git repositories somewhere else and make a symbolic link to the library under the IDE's libraries directory.  For example:
    Windows:
        mklink /D C:\Users\kevin\Documents\Arduino\libraries\Arduino-SerialCommand C:\Users\kevin\prog\arduino\libs\Arduino-SerialCommand
* If you use UECIDE, replace the Arduino directory above with "UECIDE"
