// Copyright 2018 by Kevin Dahlhausen

#include "sound_manager.h"
#include <ArduinoLog.h>

#define ARDUINO_AVR_NANO
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

SoundManager _theSoundManager;
SoundManager *theSoundManager = &_theSoundManager;

#endif
