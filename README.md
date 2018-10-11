        ____           __      __   ____  __                    ___        _             _          
       / __ \___  ____/ /___ _/ /  / __ \/ /___ _____  ___     /   |_   __(_)___  ____  (_)_________
      / /_/ / _ \/ __  / __ `/ /  / /_/ / / __ `/ __ \/ _ \   / /| | | / / / __ \/ __ \/ / ___/ ___/
     / ____/  __/ /_/ / /_/ / /  / ____/ / /_/ / / / /  __/  / ___ | |/ / / /_/ / / / / / /__(__  ) 
    /_/    \___/\__,_/\__,_/_/  /_/   /_/\__,_/_/ /_/\___/  /_/  |_|___/_/\____/_/ /_/_/\___/____/  

    

# Pedal-Plane Avionics
This is the design and firmware for a pedal-plane special-effects controller based on the Teensy 3.2 microcontroller.  Pedal planes are small pedal-powered airplanes for children aged about 3-7.   Some sources of pedal plane kits and plans are [PuddleJumpSquadron](https://www.etsy.com/shop/PuddlejumpSquadron?ref=l2-shopheader-name), [Experimental Aircraft Association](https://www.eaa.org/en/eaa-shop/kids-apparel-and-collectibles/pedal-airplane-plans),  and [Aviation Products, Inc.](http://www.pedalplanekits.com/).

![P-51 Pedal Plane](https://i.pinimg.com/originals/aa/81/22/aa812234470c6a0a591d3ea2acb93481.jpg "P-51 Pedal Plane")

See [examples/p-51/](https://github.com/kdahlhaus/Pedal-Plane-Avionics/blob/master/examples/p-51/p-51.ino) for a working example.

## Features
* Complex engine sound (starter, startup, idle, various RPMs)
* Machine gun sound
* Bomb-drop sound
* Navigation lights
* Gyro spool-up sound on master power up
* Bluetooth interface 
* Random radio 'chatter' e.g.: *Two 109's 12:00 high*
* Android app (prototyped)
* Landing lights (TODO)
* 'Zoom' flyby sound (needs tweaking)


## Hardware

The system is built around a Teensy 3.2 microcontroller.  The code should run fine or with minimal changes on other large Arduino-like controllers.  A [google docs spreadsheet](https://docs.google.com/spreadsheets/d/1WeCcuZLrt08O3oWe9VnZAMBnWO3cXGibphIcXRtGH-A/edit?usp=sharing) contains the [roadmap, bill of materials, and pin connections](https://docs.google.com/spreadsheets/d/1WeCcuZLrt08O3oWe9VnZAMBnWO3cXGibphIcXRtGH-A/edit?usp=sharing).


## Libraries
This project leverages a few Arduino libraries.  Please follow the instructions below to install them.

1. cd to your Arduino libs directory.
        (The default is ~/Arduino/libraries on Linux
        or ~/Documents/Arduino/libraries on Windows)
2. clone https://github.com/kdahlhaus/Pedal-Plane-Avionics/
3. clone https://github.com/kdahlhaus/EventSystem                
4. clone https://github.com/kdahlhaus/Arduino-SerialCommand
    (note that this is not the one in the Arduino lib manager)
5. clone https://github.com/thomasfredericks/Bounce2.git
6. clone https://github.com/adafruit/Adafruit_Sensor.git
7. clone https://github.com/adafruit/Adafruit_LIS3DH
8. clone https://github.com/janelia-arduino/Functor.git
9. clone https://github.com/jgillick/arduino-LEDFader.git LEDFader 
    (note that library must be in dir LEDFader due to the name of the include files it contains)
10. Use the library manager to add:
    * ArduinoLog (or https://github.com/thijse/Arduino-Log/)

### Note:
You can optionally clone the git repositories somewhere else and make a symbolic link to the library under the IDE's libraries directory.  For example:
 Windows: *mklink /D C:\Users\kevin\Documents\Arduino\libraries\Arduino-SerialCommand C:\Users\kevin\prog\arduino\libs\Arduino-SerialCommand*
Linux: *ln -s ~/prog/arduino/libs/Arduino-SerialCommand/ ~/apps/arduino-1.8.5/libraries/*

(If you use *UECIDE*, replace the Arduino directory above with "UECIDE")


## SD-Card
The SD is the limiting factor on the number of sounds that can be played at once.  Use a high-quality SD card, such as the
SanDisk Ultra Micro SD. The maximum size card supported by the system is 32 GB.

The card must be formatted using a FAT32 file system.


## Sounds                                                         

### Format
Sounds are 16-bit PCM WAV files at 44100 kHz sample rate.


### Layout on the SD Card
All sounds are located in the root directory of the SD card or in a directory off of the root.  These are the required names:

* bombdrop.wav
* startup.wav  # played once on avionics start (the gyro spool-up)

* crash.wav   # plane crash sound
* machguns.wav 

Motor sounds:

* starters.wav # motor starter sound start
* starterl.wav # motor starter sound loop
* starting.wav # motor starting
* idle.wav    # motor idling
* rpm1.wav
* rpm2.wav
* rpm3.wav
* stop.wav  # engine stopping


Radio chatter sounds can have any name and are located in:

* /radio/

Zoom sounds can have any name and are located in;
* /zoom/


### Locating Sounds
This is the biggest challenge.  I unfortunately do not have the license to distribute the sounds I use. I use the [Audacity Sound Editor](https://www.audacityteam.org/) for editing the sounds.  Note that with Audacity, you must "export" the sound as a wav file as the "Save" function saves the sound as an Audacity sound project. Most edits include cropping, amplification, fade-in, and fade-out.  Some of the youtube videos have a B-17 drone sound effect.  Applying a 151 Hz notch filter to the sound decreases that noise.  Here are some links to sources of sounds: 


* [P-51 Sounds for the Flight Gear Simulator](https://github.com/onox/p51d/tree/master/Sounds) TODO: These could actually be distributed as sample sounds.  
* [Gee-Bee Sounds for the Flight Gear Simulator](https://github.com/FGMEMBERS/Gee-Bee/tree/master/Sounds) TODO: These could also be distrubuted as sample sounds. 
* [IL-2 1946 B-17 Mission](https://www.youtube.com/watch?v=CHPc3t4F1Kk) This is good for radio chatter.  
* [P-51 Startup / Fly By](https://www.youtube.com/watch?v=UCyHCzWUgKI)
* [Radio on / off](https://www.zapsplat.com/music/radio-switch-on-receive-static-then-off/)

[Soundbible.com](http://soundbible.com/suggest.php?q=machine+gun&x=0&y=0) has many free sounds, many licensed so that they can be distrubuted if attriubtion if given to the author.

## Firmware Design
The main aspects of the code's architecture are:
1. Event-Driven
2. Object-Managed Setup
3. Code Conventions


### Event Driven
Components communicate through events.  An event is a combination of an event number (defined in avionics_events.h) and an optional parameter.  There can be an arbritary number of sources and responders for a given event.  Most of the time, the sources for an event are a switch and the serial interfaces.  The most common pattern of event usage within the program is that a given event is generated by a switch and the serial interfaces and handled by a single domain object.  DROP_BOMB is a good example of an event that follows this pattern. 

For example, the machine-gun switch sends a "MACHINEGUNS_START" event when it is pressed.  The machine-guns listen for this event and begin shooting the machine guns when it is received.  For now, shooting means playing the sound, but it could also mean synchronizing machine-gun flashes as well.  MACHINEGUNS_STOP is sent when the button is released and the machine-guns stop.
 
This allows events to come from the USB terminal or bluetooth connection and cause
the exact same effect as the physical switch.

### Object-managed Setup
The common pattern in Arduino sketches is to implement a function called 'setup' that contains hardware-related setup such as setting input / outuput pins.  I wanted objects in the system to control and setup the hardware they use as appropriate.  For example, a Switch object sets the pin it uses as an input.  I have handled this by dynamically allocating these objects within the 'setup' function and having the constructors of the objects set the hardware configuration as appropriate.  This means that these objects must be constructed within the context of the 'setup' call.  This is not enforced by code but creating objects outside of setup may not work as desired.   Memory fragmentation is not a concern because these objects live the entire lifetime of the program and are not deleted and recreated during execution.

This hides details (keeping the main sketch clean) and ensures correct configuration. 

The downside is that high-level objects must be dynamically allocated within the setup() call.


### Code Conventions
1. Hardware configuration in the constructor
    * Objects should handle their own hardware configuration in the constructor.
    * Objects are dynamically allocated within 'setup.'
2. Update
    * Objects that need to 'run' or be periodically updated implement a member function called 'update.'  This is added to the 'loop' function in PedalPlaneAvionics.ino.
3. Events
    * Domain objects communicate or are triggered by events defined in 'avionics_events.h'
    * Events should have a command defined in 'interpreter.cpp'
4. Coding style
    * I'll start off by apologizing for the mixture of camel-case and underscores separating the words.  I've been out of the C/C++ world for some time and so went back and forth a bit when starting this project.  My intention is to stay with camel-case and to slowly refactor any underscored variable or function names.
    * A similar case exists for brackets.


## Credits
* [Pedal Plane Group on Facebook](https://www.facebook.com/groups/PedalPlanes/) Lots of ideas on features and motivation to pursue this project.  I've used ideas from [Mike Badger](https://www.facebook.com/vadger/videos/10155459934356465/) and [Alex Romero](https://www.facebook.com/groups/PedalPlanes/permalink/10156602338668427/) in particular. Thanks!
* [Open Panzer](https://github.com/OpenPanzerProject/OP-Sound) is an open-source Teensy-based sound system for model tanks.  I've used ideas from it (no code) in designing the Pedal Plane Avionics. 


## Licensing
This project is licensed under the GNU GPL 3.0.   My intent is for it to be freely used and modified.  If you distribute modified code though you have to make your modifications available in source form.
