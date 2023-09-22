/*
  Rotary encoder readings using KeyDetector library.

  Demonstrates how to use KeyDetector to trigger action based on digital signal readings from rotary encoder with button.
  Rotating the knob and pressing button at the same time will print corresponding message.
  
  Additional info (including the breadboard view) available on GitHub:
  https://github.com/Spirik/KeyDetector
  
  This example code is in the public domain.
*/

 #include <KeyDetector.h>

// Define signal identifiers for three outputs of encoder (channel A, channel B and a push-button)
#define KEY_A 1
#define KEY_B 2
#define KEY_C 3

// Pins encoder is connected to
const byte channelA = 2;
const byte channelB = 3;
const byte buttonPin = 4;

// Array of Key objects that will link GEM key identifiers with dedicated pins
// (it is only necessary to detect signal change on a single channel of the encoder, either A or B;
// order of the channel and push-button Key objects in an array is not important)
Key keys[] = {{KEY_A, channelA}, {KEY_C, buttonPin}};
//Key keys[] = {{KEY_C, buttonPin}, {KEY_A, channelA}};

// Create KeyDetector object
// KeyDetector myKeyDetector(keys, sizeof(keys)/sizeof(Key));
// To account for switch bounce effect of the buttons (if occur) you may want to specify debounceDelay
// as the third argument to KeyDetector constructor.
// Make sure to adjust debounce delay to better fit your rotary encoder.
// Also it is possible to enable pull-up mode when buttons wired with pull-up resistors (as in this case).
// Analog threshold is not necessary for this example and is set to default value 16.
KeyDetector myKeyDetector(keys, sizeof(keys)/sizeof(Key), /* debounceDelay= */ 5, /* analogThreshold= */ 16, /* pullup= */ true);

void setup() {
  // Serial communications setup
  Serial.begin(115200);

  // Pin modes set to INPUT_PULLUP with internal pullup resistors activated
  // (alternatively it is possible to connect terminals using external pullup 10kOhm resistors and INPUT mode)
  pinMode(channelA, INPUT_PULLUP);
  pinMode(channelB, INPUT_PULLUP);
  pinMode(buttonPin, INPUT_PULLUP);

  Serial.println("Sketch loaded. Interact with encoder.");
}

void loop() {
  myKeyDetector.detect();
  
  // Press event (e.g. when button was pressed or channel A signal chaged to HIGH)
  switch (myKeyDetector.trigger) {
    case KEY_A:
      //Signal from Channel A of encoder was detected
      if (digitalRead(channelB) == LOW) {
        // If channel B is LOW then the knob was rotated CW
        if (myKeyDetector.current == KEY_C) {
          Serial.println("Rotation CW with button pressed");
        } else {
          Serial.println("Rotation CW");
        }
      } else {
        // If channel B is HIGH then the knob was rotated CCW
        if (myKeyDetector.current == KEY_C) {
          Serial.println("Rotation CCW with button pressed");
        } else {
          Serial.println("Rotation CCW");
        }
      }
      break;
    case KEY_C:
      //Button was pressed
      Serial.println("Button pressed");
      break;
  }
  
  // Release event (e.g. when button was released or channel A signal changed to LOW)
  switch (myKeyDetector.triggerRelease) {
    case KEY_A:
      //Signal from Channel A of encoder was detected
      if (digitalRead(channelB) == LOW) {
        // If channel B is LOW then the knob was rotated CCW
        if (myKeyDetector.current == KEY_C) {
          Serial.println("Rotation CCW with button pressed (release)");
        } else {
          Serial.println("Rotation CCW (release)");
        }
      } else {
        // If channel B is HIGH then the knob was rotated CW
        if (myKeyDetector.current == KEY_C) {
          Serial.println("Rotation CW with button pressed (release)");
        } else {
          Serial.println("Rotation CW (release)");
        }
      }
      break;
    case KEY_C:
      // Button was released
      Serial.println("Button released");
      break;
  }
}