/*
  KeyDetector - Arduino library for detecting state change on analog and digital pins.
  Listens for specified values on the analog input and sets KeyDetector object state accordingly.
  Detects digital pin state change as well.

  Can be used to detect key press events that were assigned distinctive levels of the single
  analog signal ("multiplexed" to analog signal), e.g. by usig DAC to "encode" multiple digital
  signals to a single analog line.

  For documentation visit:
  https://github.com/Spirik/KeyDetector

  Copyright (c) 2016-2021 Alexander 'Spirik' Spiridonov

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

KeyDetector::KeyDetector(Key* keys_, byte len_, byte debounceDelay_, int analogThreshold_, bool pullup_)
  : _keys(keys_)
  , _len(len_)
  , _debounceDelay(debounceDelay_)
  , _analogThreshold(analogThreshold_)
  , _pullup(pullup_)
{ }

void KeyDetector::detect() {
  previous = current;
  boolean pressed = false;

  int val;
  byte i = 0;
  while (!pressed && i != _len) {
    if (_keys[i].level > -1) {
      
      // Detect multiplexed keys (analog signal)
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
      
      // Detect single keys (digital signal),
      // works with buttons (e.g. momentary switches) wired either with pulldown resistor (so the HIGH means that button is pressed),
      // or with pullup resistor (so the LOW means that button is pressed)
      if (_debounceDelay > 0 && digitalRead(_keys[i].pin) == (_pullup ? LOW : HIGH)) {
        delay(_debounceDelay);
      }
      if (digitalRead(_keys[i].pin) == (_pullup ? LOW : HIGH)) {
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
