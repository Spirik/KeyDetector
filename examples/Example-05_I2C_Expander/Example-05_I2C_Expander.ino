/*
  I2C Expander signal readings using KeyDetector library with SparkFun Qwiic Directional Pad.

  Demonstrates how to use KeyDetector to trigger action based on digital signal readings from
  a SparkFun Qwiic Directional Pad which uses a PCA9554 8-bit I2C GPIO expander.
  
  The SparkFun Qwiic Directional Pad has 5 buttons:
  - Up (pin 4)
  - Down (pin 5)
  - Left (pin 6)
  - Right (pin 7)
  - Center/Select (pin 3)
  
  Connection:
  - Connect the Qwiic Directional Pad to your Arduino/ESP32/etc using a Qwiic cable
    or connect directly to the I2C pins (SDA/SCL)
  
  Additional info about KeyDetector library available on GitHub:
  https://github.com/Spirik/KeyDetector
  
  This example code is in the public domain.
*/

#include <Arduino.h>
#include <KeyDetector.h>
#include <SparkFun_I2C_Expander_Arduino_Library.h>

// Define signal identifiers for five buttons
#define KEY_UP 1
#define KEY_DOWN 2
#define KEY_LEFT 3
#define KEY_RIGHT 4
#define KEY_SELECT 5

// D-Pad pins on the PCA9554
const byte pinSelect = 3;
const byte pinUp = 4;
const byte pinDown = 5;
const byte pinLeft = 6;
const byte pinRight = 7;

const int keyPressDelay = 500; // Delay after key press event triggered and before continuous press is detected, ms
const int keyPressRepeatDelay = 250; // Delay between "remains pressed" message is printed, ms

long keyPressTime = 0; // Variable to hold time of the key press event
long now; // Variable to hold current time taken with millis() function at the beginning of loop()

// Implementation of the I2CExpander interface for PCA9554
class PCA9554Expander : public I2CExpander {
  private:
    SFE_PCA95XX io;
    
  public:
    PCA9554Expander() : io() {}
    
    bool begin() override {
      return io.begin();
    }
    
    void pinMode(uint8_t pin, uint8_t mode) override {
      io.pinMode(pin, mode);
    }
    
    void digitalWrite(uint8_t pin, uint8_t val) override {
      io.digitalWrite(pin, val);
    }
    
    int digitalRead(uint8_t pin) override {
      return io.isConnected() ? io.digitalRead(pin) == LOW : LOW;
    }
};

// Create PCA9554 expander instance
PCA9554Expander dpadExpander;

// Create array of Key objects that will link defined key identifiers with pins on the expander
// Note: We set ioType to KEY_IO_TYPE_I2C_EXPANDER and pass our expander object
// Also set pullup to true since the D-Pad buttons are active LOW
Key keys[] = {
  {KEY_UP, pinUp, -1, KEY_IO_TYPE_I2C_EXPANDER, &dpadExpander},
  {KEY_DOWN, pinDown, -1, KEY_IO_TYPE_I2C_EXPANDER, &dpadExpander},
  {KEY_LEFT, pinLeft, -1, KEY_IO_TYPE_I2C_EXPANDER, &dpadExpander},
  {KEY_RIGHT, pinRight, -1, KEY_IO_TYPE_I2C_EXPANDER, &dpadExpander},
  {KEY_SELECT, pinSelect, -1, KEY_IO_TYPE_I2C_EXPANDER, &dpadExpander}
};

// Create KeyDetector object with 10ms debounce and pullup set to true
KeyDetector dpad(keys, sizeof(keys)/sizeof(Key), 10, 16, true);

void setup() {
  // Serial communications setup
  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for serial port to connect (needed for native USB port only)
  }
  
  Serial.println("SparkFun Qwiic Directional Pad Example with KeyDetector");
  
  // Initialize the I2C expander
  if (!dpadExpander.begin()) {
    Serial.println("Failed to initialize PCA9554 expander! Check connections.");
    while (1); // Halt if we can't communicate with the expander
  }

  dpadExpander.pinMode(pinUp, INPUT);
  dpadExpander.pinMode(pinDown, INPUT);
  dpadExpander.pinMode(pinLeft, INPUT);
  dpadExpander.pinMode(pinRight, INPUT);
  dpadExpander.pinMode(pinSelect, INPUT);
  
  Serial.println("PCA9554 expander initialized successfully.");
  Serial.println("Press any button on the D-Pad...");
}

void loop() {
  // Get current time to use later on
  now = millis();
  
  // Check the current state of input signal
  dpad.detect();

  // When button press is detected ("triggered"), print corresponding message
  // and save current time as a time of the key press event
  switch (dpad.trigger) {
    case KEY_UP:
      Serial.println("UP pressed!");
      keyPressTime = now;
      break;
    case KEY_DOWN:
      Serial.println("DOWN pressed!");
      keyPressTime = now;
      break;
    case KEY_LEFT:
      Serial.println("LEFT pressed!");
      keyPressTime = now;
      break;
    case KEY_RIGHT:
      Serial.println("RIGHT pressed!");
      keyPressTime = now;
      break;
    case KEY_SELECT:
      Serial.println("SELECT pressed!");
      keyPressTime = now;
      break;
  }
  
  // When button release is detected, print message
  switch (dpad.triggerRelease) {
    case KEY_UP:
      Serial.println("UP released.");
      break;
    case KEY_DOWN:
      Serial.println("DOWN released.");
      break;
    case KEY_LEFT:
      Serial.println("LEFT released.");
      break;
    case KEY_RIGHT:
      Serial.println("RIGHT released.");
      break;
    case KEY_SELECT:
      Serial.println("SELECT released.");
      break;
  }

  // After keyPressDelay passed since keyPressTime, handle continuous press
  if (now > keyPressTime + keyPressDelay) {
    // Determine currently pressed button and print message with repeat delay
    switch (dpad.current) {
      case KEY_UP:
        Serial.println("UP held down...");
        delay(keyPressRepeatDelay);
        break;
      case KEY_DOWN:
        Serial.println("DOWN held down...");
        delay(keyPressRepeatDelay);
        break;
      case KEY_LEFT:
        Serial.println("LEFT held down...");
        delay(keyPressRepeatDelay);
        break;
      case KEY_RIGHT:
        Serial.println("RIGHT held down...");
        delay(keyPressRepeatDelay);
        break;
      case KEY_SELECT:
        Serial.println("SELECT held down...");
        delay(keyPressRepeatDelay);
        break;
    }
  }

  // Also demonstrate detection of simultaneous key presses (if secondary key detected)
  if (dpad.secondary != KEY_NONE) {
    String primaryKey, secondaryKey;
    
    // Determine name of primary key
    switch (dpad.current) {
      case KEY_UP: primaryKey = "UP"; break;
      case KEY_DOWN: primaryKey = "DOWN"; break;
      case KEY_LEFT: primaryKey = "LEFT"; break;
      case KEY_RIGHT: primaryKey = "RIGHT"; break;
      case KEY_SELECT: primaryKey = "SELECT"; break;
    }
    
    // Determine name of secondary key
    switch (dpad.secondary) {
      case KEY_UP: secondaryKey = "UP"; break;
      case KEY_DOWN: secondaryKey = "DOWN"; break;
      case KEY_LEFT: secondaryKey = "LEFT"; break;
      case KEY_RIGHT: secondaryKey = "RIGHT"; break;
      case KEY_SELECT: secondaryKey = "SELECT"; break;
    }
    
    if (dpad.previousSecondary != dpad.secondary) {
      Serial.print("Multiple buttons pressed: ");
      Serial.print(primaryKey);
      Serial.print(" + ");
      Serial.println(secondaryKey);
    }
  }
} 