# Pedal-Plane Avionics
Firmware for a pedal-plane special-effects controller.  It will do things like control the motor sound, machine gun lights and sound, and navigation lights.

This is very much a work-in-progress.

## Libraries
    1. cd to your Arduino libs directory.
        (The default is ~/Arduino/libraries on Linux
        or ~/Documents/Arduino/libraries on Windows)
    2. clone https://github.com/kdahlhaus/EventSystem                
    3. clone https://github.com/kdahlhaus/Arduino-SerialCommand
        (note that this is not the one in the Arduino lib manager)
    4. clone https://github.com/thomasfredericks/Bounce2.git
    5. clone https://github.com/adafruit/Adafruit_Sensor.git
    6. clone https://github.com/adafruit/Adafruit_LIS3DH
    7  clone https://github.com/janelia-arduino/Functor.git
    8. clone https://github.com/jgillick/arduino-LEDFader.git LEDFader 
        (note that library must be in dir LEDFader due to the name of the include files it contains)
    9. Use the library manager to add:
        * ArduinoLog (or https://github.com/thijse/Arduino-Log/)
        * Tasker


* You can optionally clone the git repositories somewhere else and make a symbolic link to the library under the IDE's libraries directory.  For example:
    Windows:
        mklink /D C:\Users\kevin\Documents\Arduino\libraries\Arduino-SerialCommand C:\Users\kevin\prog\arduino\libs\Arduino-SerialCommand
    Linux:
        ln -s ~/prog/arduino/libs/Arduino-SerialCommand/ ~/apps/arduino-1.8.5/libraries/

* If you use UECIDE, replace the Arduino directory above with "UECIDE"


## Sounds                                                         

    If using TMRpcm, sounds must be WAV files, 8 bit, 32000 Hz sample rate, mono.


    All sounds are located in the root directory of the SD card with the following names:

        "startup.wav" - played automatically once on power up.   I use the sound of gyro winding up.
        "machguns.wav" - machine guns
        "mostart.wav" - motor start sound
        "morun.wav" - motor running sound
        "mostop.wav" - motor stopping sound
        "bombdrop.wav" - bomb dropping sound

        bombdrop.wav
        crash.wav
        idle.wav
        machguns.wav
        morun.wav
        mostart.wav
        mostop.wav
        rpm1.wav
        starters.wav # starter start
        starterl.wav # starter loop
        starting.wav
        startup.wav
        stop.wav
        System Volume Information
        zoom1.wav
        zoom2.wav
        zoom3.wav



    Future:
        12345678.wav
        mostart1
        moidle1
        morrun11
        morrun12
        morrun13
        morrun13
        mostop1
