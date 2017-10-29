/*
  KeyDetector - Arduino library for detecting state change on analog and digital pins.
  Listens for specified values on the analog input and sets KeyDetector object state accordingly.
  Detects digital pin state change as well.

  Can be used to detect key press events that were assigned distinctive levels of the single
  analog signal ("multiplexed" to analog signal), e.g. by usig DAC to "encode" multiple digital
  signals to a single analog line.

  For documentation visit:
  https://github.com/Spirik/KeyDetector

  Created by Alexander 'Spirik' Spiridonov, 11 Jul 2016
  Updated 29 Oct 2017
  
  This is free software. You can redistribute it and/or modify it under
  the terms of Creative Commons Attribution-ShareAlike 4.0 International License.
  To view a copy of this license, visit https://creativecommons.org/licenses/by-sa/4.0/
*/

#include <Arduino.h>
#include "KeyDetector.h"

KeyDetector::KeyDetector(Key* keys, byte len, byte debounceDelay, int analogThreshold) {
  _keys = keys;
  _len = len;
  _debounceDelay = debounceDelay;
  _analogThreshold = analogThreshold;
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
      if (_debounceDelay > 0 && (val > _keys[i].level-_analogThreshold && val < _keys[i].level+_analogThreshold)) {
        delay(_debounceDelay);
        val = analogRead(_keys[i].pin);
      }
      if (val > _keys[i].level-_analogThreshold && val < _keys[i].level+_analogThreshold) {
        current = _keys[i].code;
        pressed = true;
      }
    
    } else {
      
      // Detect Solo keys (digital signal),
      // currently works with buttons (e.g. momentary switches) wired with pulldown resistor only (so the HIGH means that button is pressed)
      if (_debounceDelay > 0 && digitalRead(_keys[i].pin) == HIGH) {
        delay(_debounceDelay);
      }
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
