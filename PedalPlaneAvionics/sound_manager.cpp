// Copyright 2018 by Kevin Dahlhausen

#include "sound_manager.h"
#include <ArduinoLog.h>

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioPlaySdWav           playSoundWav3;     //xy=191,300
AudioPlaySdWav           playSoundWav1;     //xy=193,137
AudioPlaySdWav           playSoundWav2;     //xy=194,224
AudioMixer4              mixer1;         //xy=387,184
AudioMixer4              mixer2;         //xy=395,250
AudioOutputI2S           i2s1;           //xy=570,219
AudioConnection          patchCord1(playSoundWav3, 0, mixer1, 3);
AudioConnection          patchCord2(playSoundWav3, 1, mixer2, 2);
AudioConnection          patchCord3(playSoundWav1, 0, mixer1, 0);
AudioConnection          patchCord4(playSoundWav1, 1, mixer2, 1);
AudioConnection          patchCord5(playSoundWav2, 0, mixer1, 2);
AudioConnection          patchCord6(playSoundWav2, 1, mixer2, 3);
AudioConnection          patchCord7(mixer1, 0, i2s1, 0);
AudioConnection          patchCord8(mixer2, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=570,334
// GUItool: end automatically generated code


// Use these with the Teensy Audio Shield
#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14
 
// Use these with the Teensy 3.5 & 3.6 SD card
//#define SDCARD_CS_PIN    BUILTIN_SDCARD
//#define SDCARD_MOSI_PIN  11  // not actually used
//#define SDCARD_SCK_PIN   13  // not actually used

// Use these for the SD+Wiz820 or other adaptors
//#define SDCARD_CS_PIN    4
//#define SDCARD_MOSI_PIN  11
//#define SDCARD_SCK_PIN   13


// # of sounds that can play at once
#define NUMBER_OF_CHANNELS 3 

#define NO_CHANNEL -1
#define NO_PRIORITY -1

#define HANDLE_TO_INDEX(h) ((int)(h)-1)

typedef struct ChannelInfo_s {
    const char *filename;
    int priority; 
    bool loop;
    AudioPlaySdWav& sdWav;
    ChannelInfo_s(AudioPlaySdWav& s) : priority(NO_PRIORITY), loop(false), sdWav(s) {}
} ChannelInfo;

ChannelInfo channels[] = {
    ChannelInfo(playSoundWav1),
    ChannelInfo(playSoundWav2),
    ChannelInfo(playSoundWav3)
};



void *SoundManager::play(const char *filename, int priority, bool loop)
{
    Log.trace(F("play(%s, %d, %b)\n"), filename, priority, loop);

    //find min priority
    int min_priority = 999;
    int min_priority_channel = NO_CHANNEL;
    int channel_of_equal_priority = NO_CHANNEL;

    // find candidate channel
    for (int i=0; i<= NUMBER_OF_CHANNELS; i++)
    {
        // first not playing channel
        if (!channels[i].sdWav.isPlaying())
        {
            min_priority_channel = i;
            break;
        }
    }
    if (min_priority_channel == NO_CHANNEL)
    {
        for (int i=0; i<= NUMBER_OF_CHANNELS; i++)
        {
            // determine channel w/minium priority that is less than new sound priority
            int channel_priority = channels[i].priority;
            if (channel_priority < min_priority && channel_priority < priority)
            {
                min_priority = channel_priority;
                min_priority_channel = i;
            }
        }
    }
    // new sounds of equal priority replace old sounds of that same priority
    if (min_priority_channel == NO_CHANNEL)
    {
        // find channel of equal priority
        for (int i=0; i<= NUMBER_OF_CHANNELS; i++)
        {
            if (channels[i].priority == priority)
            {
                min_priority = channels[i].priority;
                min_priority_channel = i;
                break;
            }
        } 

    }
    if (min_priority_channel == NO_CHANNEL)
    {
        // not able to play sound at this time
        Log.trace("no channels available for sound\n");
        return (void *)0;
    }

    channels[min_priority_channel].priority = priority;
    channels[min_priority_channel].sdWav.play(filename);
    channels[min_priority_channel].loop = loop;
    channels[min_priority_channel].filename = filename;

    // wait for sound to start playing
    for (int i=0; i<20; i++)
    {
        if (channels[min_priority_channel].sdWav.isPlaying())
        {
            break;
        }
        delay(5);
    }
    
    return (void *)(min_priority_channel + 1); // sound 'handle' is channel + 1
}


void SoundManager::update()
{
    // comment this if the optional volume
    // pot was not added to the audio board
    float vol = analogRead(15);
    vol = vol / 1024;
    sgtl5000_1.volume(vol); 

    // handle 'loop' sounds
    for (int i=0; i<NUMBER_OF_CHANNELS; i++)
    {
        if (channels[i].loop && !channels[i].sdWav.isPlaying())
        {
            channels[i].sdWav.play(channels[i].filename);
            // wait for sound to start playing
            for (int i=0; i<20; i++)
            {
                if (channels[i].sdWav.isPlaying())
                {
                    break;
                }
                delay(5);
            }
            Log.trace("restarted %s\n", channels[i].filename);
        }
    }

}


void SoundManager::stop(void *handle)
{
    int index = HANDLE_TO_INDEX(handle);
    if (handle && channels[index].sdWav.isPlaying())
    {
        channels[index].sdWav.stop();
        channels[index].loop = false;
        channels[index].priority = -1;
    }
}

bool SoundManager::is_playing(void *handle)
{
    bool ip = handle && channels[HANDLE_TO_INDEX(handle)].sdWav.isPlaying(); 
    //Log.trace("ip(%d)=%b\n", (int)(handle), ip);
    return ip;
}

void SoundManager::setup()
{
    AudioMemory(8);

    // Comment these out if not using the audio adaptor board.
    // This may wait forever if the SDA & SCL pins lack
    // pullup resistors
    sgtl5000_1.enable();
    sgtl5000_1.volume(0.5);

    SPI.setMOSI(SDCARD_MOSI_PIN);
    SPI.setSCK(SDCARD_SCK_PIN);
    if (!(SD.begin(SDCARD_CS_PIN))) {
        // stop here, but print a message repetitively
        while (1) {
          Log.error("Unable to access the SD card\n");
          delay(500);
        }
    } 
}






















#ifdef ARDUINO_AVR_NANO

// Arudino NANO prototyping using the TMRpcm library

#include <SD.h>
#define SD_ChipSelectPin 10
#include <TMRpcm.h>
#include <SPI.h>

TMRpcm tmrpcm;

int current_priority = -1;

void SoundManager::setup()
{
    tmrpcm.speakerPin = 9; //5,6,11 or 46 on Mega, 9 on Uno, Nano, etc
    if (!SD.begin(SD_ChipSelectPin)) 
    {  
        Log.trace("SD fail\n");
        while (true) {};
    }
    else
    {
        Log.trace("SD OK\n");
    }
}

void *SoundManager::play(const char *filename, int priority, bool loop)
{
    Log.trace("play\n");
    if (!tmrpcm.isPlaying() || (tmrpcm.isPlaying() && current_priority <= priority))
    {
        Log.trace(F("starting new sound %s\n"), filename);
        tmrpcm.stopPlayback();
        current_priority = priority;
        tmrpcm.play((char *)filename);
        return (void *)1;
    }
    Log.trace(F("requested but did not start sound %s\n"), filename);
    return (void *)0;
}

void SoundManager::stop(void *handle)
{
    tmrpcm.stopPlayback();
}

bool SoundManager::is_playing(void *handle)
{
    return tmrpcm.isPlaying();
}

#endif


SoundManager _theSoundManager;
SoundManager *theSoundManager = &_theSoundManager;
