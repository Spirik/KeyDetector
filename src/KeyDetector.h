/*
  KeyDetector - Arduino library for detecting state change on analog and digital pins.
  Listens for specified values on the analog input and sets KeyDetector object state accordingly.
  Detects digital pin state change as well.

  Can be used to detect key press events that were assigned distinctive levels of the single
  analog signal ("multiplexed" to analog signal), i.e. by usig DAC to "encode" multiple digital
  signals to a single analog line.

  For documentation visit:
  https://github.com/spirik/KeyDetector

  Created by Alexander 'Spirik' Spiridonov, 11 July 2016
  
  This is free software. You can redistribute it and/or modify it under
  the terms of Creative Commons Attribution-ShareAlike 4.0 International License.
  To view a copy of this license, visit https://creativecommons.org/licenses/by-sa/4.0/
*/

#ifndef HEADER_KEYDETECTOR
#define HEADER_KEYDETECTOR

#include <Arduino.h>

#define KEY_NONE 0

// Declaration of Key element type
struct Key {
  Key(byte c, int p, int l = -1) : code(c), pin(p), level(l) {}
  byte code;  // Code (or 'alias') of the key
  int pin;    // Pin the key is attached to
  int level;  // Level of the analog signal at which press event of the multiplexed key is triggered (if left unset - key considered to be digital)
};

// Declaration of KeyDetector class
class KeyDetector {
  public:
    /* 
      @param 'keys' - array of the Key elements
      @param 'len' - length of the 'keys' array
      @param 'analogThreshold' (optional) - value added to and subtracted from the level value of the multiplexed Key,
      formed range [level - analogThreshold + 1 .. level + analogThreshold - 1] is then used to detect key press event;
      default 16 (which allows for maximum of 32 keys multiplexed via 5-bit DAC to single analog line with 10-bit ADC on the Arduino end)
      @param 'analogDelay' (optional) - delay in ms to account for any transient process that may occur when adjusting the source level of analog signal;
      default 0
    */
    KeyDetector(Key* keys, byte len, int analogThreshold = 16, byte analogDelay = 0);
    byte current = KEY_NONE;    // Code of key currently in pressed state
    byte previous = KEY_NONE;   // Code of previously pressed key
    byte trigger = KEY_NONE;    // Code of key being pressed (triggers ones at the beginning of press event)
    void detect();
  private:
    Key* _keys;
    byte _len;
    int _analogThreshold;
    byte _analogDelay;
};
  
#endif
