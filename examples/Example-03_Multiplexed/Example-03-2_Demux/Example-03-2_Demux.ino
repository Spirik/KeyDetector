/*
  Using Keydetector library to decode signals from PS3 controller multiplexed into single analog line. Listing 2.
  (Listing 1 can be found in "/examples/Example-03_Multiplexed/Example-03-1_Mux" directory).
  
  This part of the example demonstrates how to use KyDetector to decode signals from PS3 controller previously
  encoded into single analog line (as seen in Listing 1).

  Currently pressed button of the controller (D-pad keys, cross, or circle) will be printed.

  In addition, axis values from left stick of the PS3 DualShock Controller (or single available stick in case of
  PS3 Navigation Controller) will be printed as well. Analog values from the stick are transmitted through 
  separate wires.
  
  Additional info (including the breadboard view) available on GitHub:
  https://github.com/Spirik/KeyDetector
  
  This example code is in the public domain.
*/

#include <KeyDetector.h>

// Define signal identifiers for the buttons
#define KEY_UP 1
#define KEY_RIGHT 2
#define KEY_DOWN 3
#define KEY_LEFT 4
#define KEY_O 5
#define KEY_X 6

// Define variables to hold axis values
int x, y;

// Signal pins
const byte kPin = A2; // Pin for multiplexed signal
const byte xPin = A1; // Pin for X axis signal
const byte yPin = A0; // Pin for Y axis signal

// Create array of Key objects that will link defined key identifiers with dedicated pins
Key keys[] = {{KEY_UP, kPin, 127}, {KEY_RIGHT, kPin, 255}, {KEY_DOWN, kPin, 383}, {KEY_LEFT, kPin, 511}, {KEY_O, kPin, 639}, {KEY_X, kPin, 767}};

// Create KeyDetector object
KeyDetector key(keys, sizeof(keys)/sizeof(Key));
// Note that you can increase threshold value to achieve more accurate detection (e.g. in case of high signal ripple
// or inaccuracy of the DAC used to encode signals). To do so, use the following line instead (where 24 is the custom
// value of threshold):
// KeyDetector key(keys, sizeof(keys)/sizeof(Key), 0, 24);
// Additionally you can specify debounceDelay value to account for any transient process that may occur when adjusting
// the source level of analog signal:
// KeyDetector key(keys, sizeof(keys)/sizeof(Key), 5, 24);

void setup() {
  // Configure the reference voltage used for analog input (i.e. the value used as the top of the input range)
  analogReference(EXTERNAL);
  
  Serial.begin(115200);

  // Set signal pins to input
  pinMode(kPin, INPUT);
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
}

void loop() {
  // Get X value of the stick
  x = analogRead(xPin);
  delay(10);
  // Get Y value of the stick
  y = analogRead(yPin);
  
  // Print X and Y values. Thresholds (500 and 524) used here help prevent false readings near the mid-point of the range
  if (x > 524 || x < 500 || y > 524 || y < 500) {
    Serial.print("X: ");    Serial.print(x);
    Serial.print("\tY: ");  Serial.println(y);
  }

  // Check the current state of input signal
  key.detect();

  // When button press is detected ("triggered"), print corresponding message
  switch (key.trigger) {
    case KEY_UP:
      Serial.println("UP");
      break;
    case KEY_RIGHT:
      Serial.println("RIGHT");
      break;
    case KEY_DOWN:
      Serial.println("DOWN");
      break;
    case KEY_LEFT:
      Serial.println("LEFT");
      break;
    case KEY_O:
      Serial.println("O");
      break;
    case KEY_X:
      Serial.println("X");
      break;
  }

}
