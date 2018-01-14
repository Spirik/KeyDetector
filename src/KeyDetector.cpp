/*
  KeyDetector - Arduino library for detecting state change on analog and digital pins.
  Listens for specified values on the analog input and sets KeyDetector object state accordingly.
  Detects digital pin state change as well.

  Can be used to detect key press events that were assigned distinctive levels of the single
  analog signal ("multiplexed" to analog signal), e.g. by usig DAC to "encode" multiple digital
  signals to a single analog line.

  For documentation visit:
  https://github.com/Spirik/KeyDetector

  Copyright (c) 2016-2018 Alexander 'Spirik' Spiridonov

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 3 of the License, or (at your option) any later version.
  
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.
  
  You should have received a copy of the GNU Lesser General Public License
  along with this library.  If not, see <http://www.gnu.org/licenses/>.
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
