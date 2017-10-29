/*
  Analog signal readings using KeyDetector library.

  Absolute rotary encoder (sort of) using potentiometer and KeyDetector library. Rotation of the knob will result in printing its
  current position and direction of rotation. The whole range of rotation is divided into several segments with the middle points
  at 25%, 50%, 75% and 100% of the range. Hence the segments: [12.5% .. 37.5%], [37.5% .. 62.5%], [62.5% .. 87.5%], [87.5% .. 100%].
  Once the bounds of segments are crossed the output will be printed.
  
  Additional info (including the breadboard view) available on GitHub:
  https://github.com/Spirik/KeyDetector
  
  This example code is in the public domain.
*/

#include <KeyDetector.h>

// Define signal identifiers; in this example, number in KEY_XX may be thought of as a percentage of pot rotation range,
// e.g. KEY_25 means 25% of the total pot rotation range. The actual value of defined identifier isn't that important most of the time,
// hovewer since we'll be comparing these identifier values later on to determine the direction of pot rotation, we should assume that
// they form increasing sequence, i.e. KEY_50 should be higher than KEY_25, but lower than KEY_75 and KEY_100, etc.
// For the sake of simplicity we'll use the same value for each identifier as specified in the name of identifier itself,
// so KEY_25 will stand for 25, KEY_50 for 50, and so on
#define KEY_25 25
#define KEY_50 50
#define KEY_75 75
#define KEY_100 100

const byte potPin = A0; // Pin the pot is connected to

char* state; // Variable to hold string describing direction of rotation

// Create array of Key objects that will link defined key/signal identifiers with pin and level of signal that should trigger detect
Key keys[] = {{KEY_25, potPin, 255}, {KEY_50, potPin, 511}, {KEY_75, potPin, 767}, {KEY_100, potPin, 1023}};

// Create KeyDetector object
KeyDetector myKeyDetector(keys, sizeof(keys)/sizeof(Key), 0, 129);
// Notes:
// Increasing of 'debounceDelay' (set to 0) can cause skip of detection of adjacent values if pot is rotated quickly enough, but helps
// to account for any transient processes or ripple that may occur during pot rotation and value readings, try value of 1
// as an alternative to the delay set at the end of the loop().
// Value of 'analogThreshold' is set to 129 so that there are no gaps between detection ranges of signals,
// i.e. [255-129+1 .. 255+129-1]=[127 .. 383] for KEY_25,
// and  [511-129+1 .. 511+129-1]=[383 .. 639] for KEY_50; note that common for both ranges value of 383 will, actually, trigger
// press event of KEY_25, because its Key object was placed before Key object of KEY_50 in the keys[] array

void setup() {
  Serial.begin(115200);
  
  // Set potPin to input
  pinMode(potPin, INPUT);
}

void loop() {
  // Check the current state of input signal
  myKeyDetector.detect();

  // Chekcking the direction of rotation of the pot: if previously detected value was lower than current one - that means
  // we're going up (i.e. increasing the value of the signal), and vice verse
  if (myKeyDetector.previous < myKeyDetector.current) {
    state = "Up to ";
  } else {
    state = "Down to ";
  }
  
  // Printing current state of the pot as soon as we jump from one range to the next (i.e. when trigger event happens)
  switch (myKeyDetector.trigger) {
    case KEY_25:
      Serial.print(state);
      Serial.println("25%");
      break;
    case KEY_50:
      Serial.print(state);
      Serial.println("50%");
      break;
    case KEY_75:
      Serial.print(state);
      Serial.println("75%");
      break;
    case KEY_100:
      Serial.print(state);
      Serial.println("100%");
      break;
  }

  // Small delay to account for any transient processes or ripple that may occur during pot rotation and value readings.
  // Alternatively can be adjusted through debounceDelay parameter supplied to KeyDetector constructor
  delay(2);
}
