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

#ifndef HEADER_KEYDETECTOR
#define HEADER_KEYDETECTOR

#include <Arduino.h>

#define KEY_NONE 0

// Declaration of Key element type
struct Key {
  Key(byte c, int p, int l = -1) : code(c), pin(p), level(l) {}
  byte code;  // Identifier of the key
  int pin;    // Pin the key is attached to
  int level;  // Level of the analog signal at which press event of the multiplexed key is triggered
              // (if not explicitly set - default value of -1 is assigned and key considered to be digital)
};

// Declaration of KeyDetector class
class KeyDetector {
  public:
    /* 
      @param 'keys_' - array of the Key elements
      @param 'len_' - length of the 'keys' array
      @param 'debounceDelay_' (optional) - delay in ms to account for any signal ripple (e.g. switch bounce) when detecting digital signal,
      or transient process that may occur when adjusting the source level of analog signal
      default 0
      @param 'analogThreshold_' (optional) - value added to and subtracted from the level value of the multiplexed Key,
      formed range [level - analogThreshold + 1 .. level + analogThreshold - 1] is then used to detect key press event
      default 16 (which allows for maximum of 32 keys multiplexed via 5-bit DAC into single analog line with 10-bit ADC on the Arduino end)
      @param 'pullup_' (optional) - whether digital pins are connected with pullup resistor (so the LOW means that button is pressed),
      or pulldown resistor (so the HIGH means that button is pressed), default value is for pulldown resistor configuration
      default false
    */
    KeyDetector(Key* keys_, byte len_, byte debounceDelay_ = 0, int analogThreshold_ = 16, bool pullup_ = false);
    byte trigger = KEY_NONE;    // Identifier of key being pressed (triggers ones at the beginning of press event)
    byte current = KEY_NONE;    // Identifier of key currently in pressed state
    byte previous = KEY_NONE;   // Identifier of previously pressed key
    void detect();
  private:
    Key* _keys;
    byte _len;
    byte _debounceDelay;
    int _analogThreshold;
    bool _pullup;
};
  
#endif
