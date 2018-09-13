// Copyright 2018 by Kevin Dahlhausen
#include <stdio.h>
#include "sound_manager.h"
#include <ArduinoLog.h>

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioPlaySdWav           motorSdWav2;     //xy=172,314
AudioPlaySdWav           playSdWav1;     //xy=173,175
AudioPlaySdWav           playSdWav0;     //xy=174,118
AudioPlaySdWav           motorSdWav1;     //xy=174,253
AudioEffectFade          motorFade1;          //xy=347,252
AudioEffectFade          motorFade2;          //xy=348,314
AudioMixer4              sfxMixer1;         //xy=368,143
AudioMixer4              finalMixer;         //xy=754,283
AudioOutputI2S           i2s1;           //xy=900,284
AudioConnection          patchCord1(motorSdWav2, 0, motorFade2, 0);
AudioConnection          patchCord2(motorSdWav2, 1, motorFade2, 0);
AudioConnection          patchCord3(playSdWav1, 0, sfxMixer1, 2);
AudioConnection          patchCord4(playSdWav1, 1, sfxMixer1, 3);
AudioConnection          patchCord5(playSdWav0, 0, sfxMixer1, 0);
AudioConnection          patchCord6(playSdWav0, 1, sfxMixer1, 1);
AudioConnection          patchCord7(motorSdWav1, 0, motorFade1, 0);
AudioConnection          patchCord8(motorSdWav1, 1, motorFade1, 0);
AudioConnection          patchCord9(motorFade1, 0, finalMixer, 1);
AudioConnection          patchCord10(motorFade2, 0, finalMixer, 2);
AudioConnection          patchCord11(sfxMixer1, 0, finalMixer, 0);
AudioConnection          patchCord12(finalMixer, 0, i2s1, 0);
AudioConnection          patchCord13(finalMixer, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=905,361
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


// # of SFX sounds that can play at once
#define NUMBER_OF_CHANNELS 2 

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
    ChannelInfo(playSdWav0),
    ChannelInfo(playSdWav1)
};


void *SoundManager::play(const char *filename, int priority, bool loop, float gain)
{
    char fbuf[10];
    sprintf(fbuf, "%f", gain);
    dtostrf(gain, 4, 2, fbuf);
    Log.trace(F("play(%s, pri=%d, loop=%b, gain=%s)\n"), filename, priority, loop, fbuf);

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

    sfxMixer1.gain(min_priority_channel*2, gain);
    sfxMixer1.gain(min_priority_channel*2+1, gain);

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

void SoundManager::setGain(void *handle, float gain)
{
    if (is_playing(handle))
    {
        Log.trace(F("SM.setGain(%d, %F)\n"), HANDLE_TO_INDEX(handle), gain);
        sfxMixer1.gain(HANDLE_TO_INDEX(handle)*2, gain);
        sfxMixer1.gain(HANDLE_TO_INDEX(handle)*2+1, gain);
    }
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
