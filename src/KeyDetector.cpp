/*
  KeyDetector - Arduino library for detecting state change on analog and digital pins.
  Listens for specified values on the analog input and sets KeyDetector object state accordingly.
  Detects digital pin state change as well.

  Can be used to detect key press events that were assigned distinctive levels of the single
  analog signal ("multiplexed" to analog signal), e.g. by usig DAC to "encode" multiple digital
  signals to a single analog line.

  Allows detection of up to two simultaneously pressed keys connected to digital pins.

  For documentation visit:
  https://github.com/Spirik/KeyDetector

  Copyright (c) 2016-2023 Alexander 'Spirik' Spiridonov

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
  previousSecondary = secondary;

  byte firstKey = KEY_NONE;
  byte secondKey = KEY_NONE;

  int val;
  byte i = 0;
  while (!secondKey && i != _len) {
    if (_keys[i].level > -1) {
      
      // Detect multiplexed keys (analog signal)
      val = analogRead(_keys[i].pin);
      if (_debounceDelay > 0 && (val > _keys[i].level-_analogThreshold && val < _keys[i].level+_analogThreshold)) {
        delay(_debounceDelay);
        val = analogRead(_keys[i].pin);
      }
      if (val > _keys[i].level-_analogThreshold && val < _keys[i].level+_analogThreshold) {
        // Support for simultaneous detection of analog readings may lead to unstable triggers when detection ranges
        // of analog signal are close or overlapping, hence disabling for now. As a result, analog readings are
        // detected as a primary key press (stored in 'current') if no other primary key presses were detected,
        // and can not be detected as a secondary key press (stored in 'secondary').
        // When digital Key objects present in the same Keys array as analog Key objects, they will be detected either
        // as primary (when placed before analog Key objects in constructor) or secondary (when placed after analog Key
        // objects in constructor).
        if (!firstKey) {
          firstKey = _keys[i].code;
        }/* else {
          secondKey = _keys[i].code;
        } */
      }
    
    } else {
      
      // Detect single keys (digital signal),
      // works with buttons (e.g. momentary switches) wired either with pulldown resistor (so the HIGH means that button is pressed),
      // or with pullup resistor (so the LOW means that button is pressed)
      if (_debounceDelay > 0 && digitalRead(_keys[i].pin) == (_pullup ? LOW : HIGH)) {
        delay(_debounceDelay);
      }
      if (digitalRead(_keys[i].pin) == (_pullup ? LOW : HIGH)) {
        if (!firstKey) {
          firstKey = _keys[i].code;
        } else {
          secondKey = _keys[i].code;
        }
      }
    
    }
    i++;
  }

  if (!firstKey) {
    current = KEY_NONE;
    secondary = KEY_NONE;
    triggerRelease = (previous != KEY_NONE) ? previous : KEY_NONE;
    trigger = KEY_NONE;
  } else if (secondKey) {
    if (secondKey != previous) {
      current = firstKey;
      secondary = secondKey;
    } else {
      current = secondKey;
      secondary = firstKey;
    }
    if (current != previous) {
      // If primary key changed while secondary being pressed (can happen when detecting analog and digital presses in the same object)
      trigger = current;
    } else {
      trigger = (secondary != previousSecondary) ? secondary : KEY_NONE;
    }
  } else {
    current = firstKey;
    secondary = KEY_NONE;
    if (previousSecondary != KEY_NONE) {
      // Make sure triggerRelease will hold correct value of released key (considering that it may be either primary or secondary one)
      triggerRelease = (previousSecondary == current) ? previous : previousSecondary;
      // If triggerRelease was fired avoid firing additional trigger
      trigger = KEY_NONE;
    } else {
      triggerRelease = KEY_NONE;
      trigger = (current != previous) ? current : KEY_NONE;
    }
  }
}
