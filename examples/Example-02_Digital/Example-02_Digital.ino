/*
  Digital signal readings using KeyDetector library.

  Demonstrates how to use KeyDetector to trigger action based on digital signal readings from momentary push-buttons.
  Pressing button once or keeping it in a pressed state continuously will print corresponding message. Pressing another
  button in combination with the one already in a pressed state will be detected as well.
  
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
// as the third argument to KeyDetector constructor:
// KeyDetector myKeyDetector(keys, sizeof(keys)/sizeof(Key), /* debounceDelay= */ 10);

void setup() {
  // Serial communications setup
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
      // Determine whether button A was pressed in combination with another one already in a pressed state (B or C)
      switch (myKeyDetector.current) {
        case KEY_B:
          Serial.println("Button A pressed simultaneously with Button B!");
          break;
        case KEY_C:
          Serial.println("Button A pressed simultaneously with Button C!");
          break;
        default:
          Serial.println("Button A pressed!");
          break;
      }
      keyPressTime = now;
      break;

    case KEY_B:
      // Determine whether button B was pressed in combination with another one already in a pressed state (A or C)
      switch (myKeyDetector.current) {
        case KEY_A:
          Serial.println("Button B pressed simultaneously with Button A!");
          break;
        case KEY_C:
          Serial.println("Button B pressed simultaneously with Button C!");
          break;
        default:
          Serial.println("Button B pressed!");
          break;
      }
      keyPressTime = now;
      break;

    case KEY_C:
      // Determine whether button C was pressed in combination with another one already in a pressed state (A or B)
      switch (myKeyDetector.current) {
        case KEY_A:
          Serial.println("Button C pressed simultaneously with Button A!");
          break;
        case KEY_B:
          Serial.println("Button C pressed simultaneously with Button B!");
          break;
        default:
          Serial.println("Button C pressed!");
          break;
      }
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
