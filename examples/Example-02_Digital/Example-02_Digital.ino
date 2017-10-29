/*
  Digital signal readings using KeyDetector library.

  Demonstrates how to use KeyDetector to trigger action based on digital signal readings from momentary push-buttons.
  Pressing button once or keeping it in pressed state continuously will print corresponding message.
  
  Additional info (including the breadboard view) available on GitHub:
  https://github.com/Spirik/KeyDetector
  
  This example code is in the public domain.
*/

#include <KeyDetector.h>

// Define signal identifiers for three buttons. The actual value of defined identifier isn't important for this example
#define KEY_A 1
#define KEY_B 2
#define KEY_C 3

// Pins the three buttons are connected to
const byte pinA = 2;
const byte pinB = 3;
const byte pinC = 4;

const int keyPressDelay = 1000; // Delay after key press event triggered and before continuous press is detected, ms
const int keyPressRepeatDelay = 500; // Delay between "remains pressed" message is printed, ms

long keyPressTime = 0; // Variable to hold time of the key press event
long now; // Variable to hold current time taken with millis() function at the beginning of loop()

// Create array of Key objects that will link defined key identifiers with dedicated pins
Key keys[] = {{KEY_A, pinA}, {KEY_B, pinB}, {KEY_C, pinC}};

// Create KeyDetector object
KeyDetector myKeyDetector(keys, sizeof(keys)/sizeof(Key));
// To account for switch bounce effect of the buttons (if occur) you may whant to specify debounceDelay
// as the second argument to KeyDetector constructor:
// KeyDetector myKeyDetector(keys, sizeof(keys)/sizeof(Key), 10);

void setup() {
  Serial.begin(115200);
  
  // Set button pins to input
  pinMode(pinA, INPUT);
  pinMode(pinB, INPUT);
  pinMode(pinC, INPUT);
}

void loop() {
  // Get current time to use later on
  now = millis();
  
  // Check the current state of input signal
  myKeyDetector.detect();

  // When button press is detected ("triggered"), print corresponding message
  // and save current time as a time of the key press event
  switch (myKeyDetector.trigger) {
    case KEY_A:
      Serial.println("Button A pressed!");
      keyPressTime = now;
      break;
    case KEY_B:
      Serial.println("Button B pressed!");
      keyPressTime = now;
      break;
    case KEY_C:
      Serial.println("Button C pressed!");
      keyPressTime = now;
      break;
  }

  // After keyPressDelay passed since keyPressTime...
  if (now > keyPressTime + keyPressDelay) {
  
    // ...determine currently pressed button (i.e. button being in a pressed state)
    // and print corresponding message, followed by keyPressRepeatDelay
    switch (myKeyDetector.current) {
      case KEY_A:
        Serial.println("Button A remains pressed.");
        delay(keyPressRepeatDelay);
        break;
      case KEY_B:
        Serial.println("Button B remains pressed.");
        delay(keyPressRepeatDelay);
        break;
      case KEY_C:
        Serial.println("Button C remains pressed.");
        delay(keyPressRepeatDelay);
        break;
    }

  }

}
