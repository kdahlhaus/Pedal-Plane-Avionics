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
AudioControlSGTL5000     sgtl5000_1;     //xy=587,354
// GUItool: end automatically generated code

