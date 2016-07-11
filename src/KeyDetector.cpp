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

#include <Arduino.h>
#include "KeyDetector.h"

KeyDetector::KeyDetector(Key* keys, byte len, int analogThreshold, byte analogDelay) {
  _keys = keys;
  _len = len;
  _analogThreshold = analogThreshold;
  _analogDelay = analogDelay;
}

void KeyDetector::detect() {
  previous = current;
  boolean pressed = false;

  int val;
  byte i = 0;
  while (!pressed && i != _len) {
    if (_keys[i].level > -1) {
      
      // Detect Multiplexed keys (analog signal)
      val = analogRead(_keys[i].pin);
      if (_analogDelay > 0) {
        delay(_analogDelay);
        val = analogRead(_keys[i].pin);
      }
      if (val > _keys[i].level-_analogThreshold+1 && val < _keys[i].level+_analogThreshold-1) {
        current = _keys[i].code;
        pressed = true;
      }
    
    } else {
      
      // Detect Solo keys (digital signal)
      if (digitalRead(_keys[i].pin) == HIGH) {
        current = _keys[i].code;
        pressed = true;
      }
    
    }
    i++;
  }
  
  if (!pressed) {
    current = KEY_NONE;
  }
  trigger = (current != previous) ? current : KEY_NONE;
}
