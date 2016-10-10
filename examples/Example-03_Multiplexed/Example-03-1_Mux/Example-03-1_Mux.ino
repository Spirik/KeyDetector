/*
  Using Keydetector library to decode signals from PS3 controller multiplexed into single analog line. Listing 1.
  (Listing 2 can be found in "/examples/Example-03_Multiplexed/Example-03-2_Demux" directory).
  
  This part of the example demonstrates how to multiplex several digital signals from PS3 controller into single
  analog line using USB Host Shield Library 2.0 and R-2R resistor ladder DAC (with KeyDetector library on the other end
  to decodethe signal).

  This sketch is using USB Host Shield Library 2.0 to connect to PS3 controller via Bluetooth and
  should be uploaded to the Arduino equipped with ATmega328 (e.g. UNO, Pro Mini) with compatible
  USB Host Shield and supported Bluetooth dongle attached.

  Pressed button of the controller (D-pad keys, cross, or circle) is encoded into binary code and then converted
  into corresponding analog value using simple 3-bit DAC (based on R-2R resistor ladder).

  In addition, axis values from left stick of the PS3 DualShock Controller (or single available stick in case of
  PS3 Navigation Controller) output through two PWM pins, then converted to analog signals via simple RC low-pass filter.

  This example is partially based on the example code supplied with the USB Host Shield Library 2.0
  (see PS3BT sketch with the use of PS3 Bluetooth library developed by Kristian Lauszus).

  Note: get familiar with how to use USB Host Shield Library 2.0 and how to set it up to operate with
  Bluetooth connected PS3 controller before following this example.
  
  Additional info (including the breadboard view) available on GitHub:
  https://github.com/Spirik/KeyDetector

  Documentation for USB Host Shield Library 2.0 avaliable from its developers here:
  https://github.com/felis/USB_Host_Shield_2.0

  This example code is in the public domain.
 */

#include <PS3BT.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <SPI.h>
#endif

USB Usb; // Creating instance of the USB object
//USBHub Hub1(&Usb); // Some dongles have a hub inside

BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so
PS3BT PS3(&Btd, 0x00, 0x15, 0x83, 0xD3, 0x29, 0x55); // This will store the bluetooth address - this can be obtained from the dongle when running the sketch

// Tolerances to account for possible ripple on analog stick
const byte hatXLo = 117;
const byte hatXHi = 137;
const byte hatYLo = 117;
const byte hatYHi = 137;

// Pins PWM values from analog stick readings are written to
const byte xPinOut = 5;
const byte yPinOut = 6;

const int keyDelay = 200; // Delay to hold the output values so that receiving Arduino have chance to read them, ms

void setup() {
  Serial.begin(115200);
  
  // Set axis pins to output
  pinMode(xPinOut, OUTPUT);
  pinMode(yPinOut, OUTPUT);
  
  // Write default value to axis pins (meaning the stick is in neutral position)
  analogWrite(xPinOut, 127);
  analogWrite(yPinOut, 127);

  // ATmega168/328 port manipulation (more info here: https://www.arduino.cc/en/Reference/PortManipulation)
  DDRC = DDRC | B00000111; // Setting A0, A1, A2 pins as outputs through DDRC register of Port C
  PORTC = 0; // Setting A0, A1, A2 pins to default value of 0 (LOW)
  // Note: use expression PORTC & B11111000 if you don't want to change values of other bits (corresponding to pins A3, A4, A5)

  // Checking that USB Host Shield was initialized
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("\r\nPS3 Bluetooth Library Started"));
}
void loop() {
  Usb.Task();

  if (PS3.PS3Connected || PS3.PS3NavigationConnected) {
    
    // Reading stick values and writing them to Serial (for debug)
    if (PS3.getAnalogHat(LeftHatX) > hatXHi || PS3.getAnalogHat(LeftHatX) < hatXLo || PS3.getAnalogHat(LeftHatY) > hatYHi || PS3.getAnalogHat(LeftHatY) < hatYLo) {
      Serial.print(F("\r\nLeftHatX: "));
      Serial.print(PS3.getAnalogHat(LeftHatX));
      Serial.print(F("\tLeftHatY: "));
      Serial.print(PS3.getAnalogHat(LeftHatY));
    }
    
    // Reading stick values and writing them to corresponding output pins
    if ((PS3.getAnalogHat(LeftHatX) > hatXHi) || (PS3.getAnalogHat(LeftHatX) < hatXLo)) {
      analogWrite(xPinOut, PS3.getAnalogHat(LeftHatX));
    } else {
      analogWrite(xPinOut, 127);
    }
    if ((PS3.getAnalogHat(LeftHatY) > hatYHi) || (PS3.getAnalogHat(LeftHatY) < hatYLo)) {
      analogWrite(yPinOut, PS3.getAnalogHat(LeftHatY));
    } else {
      analogWrite(yPinOut, 127);
    }
    
    if (PS3.getButtonClick(PS)) {

      // Disconnect controller
      Serial.print(F("\r\nPS"));
      PS3.disconnect();

    }
    
    else {

      // Read buttons pressed and print corresponding binary codes to A0, A1, A2 pins
      if (PS3.getButtonClick(UP)) {
        Serial.print(F("\r\nUp"));
        PORTC = 1; // a.k.a. B000000001 - Setting A0 to 1, A1 to 0, A2 to 0 (will be read as 127 on receiving end)
        // Note: use expression (PORTC & (~7)) | 1 if you don't want to change values of other bits (corresponding to pins A3, A4, A5)
        delay(keyDelay);
      }
      else if (PS3.getButtonClick(RIGHT)) {
        Serial.print(F("\r\nRight"));
        PORTC = 2; // a.k.a. B000000010 - Setting A0 to 0, A1 to 1, A2 to 0 (will be read as to 255 on receiving end)
        // Note: use expression (PORTC & (~7)) | 2 if you don't want to change values of other bits (corresponding to pins A3, A4, A5)
        delay(keyDelay);
      }
      else if (PS3.getButtonClick(DOWN)) {
        Serial.print(F("\r\nDown"));
        PORTC = 3; // a.k.a. B000000011 - Setting A0 to 1, A1 to 1, A2 to 0 (will be read as 383 on receiving end)
        // Note: use expression (PORTC & (~7)) | 3 if you don't want to change values of other bits (corresponding to pins A3, A4, A5)
        delay(keyDelay);
      }
      else if (PS3.getButtonClick(LEFT)) {
        Serial.print(F("\r\nLeft"));
        PORTC = 4; // a.k.a. B000000100 - Setting A0 to 0, A1 to 0, A2 to 1 (will be read as 511 on receiving end)
        // Note: use expression (PORTC & (~7)) | 4 if you don't want to change values of other bits (corresponding to pins A3, A4, A5)
        delay(keyDelay);
      }
      else if (PS3.getButtonClick(CIRCLE)) {
        Serial.print(F("\r\nCircle"));
        PORTC = 5; // a.k.a. B000000101 - Setting A0 to 1, A1 to 0, A2 to 1 (will be read as 639 on receiving end)
        // Note: use expression (PORTC & (~7)) | 5 if you don't want to change values of other bits (corresponding to pins A3, A4, A5)
        delay(keyDelay);
      }
      else if (PS3.getButtonClick(CROSS)) {
        Serial.print(F("\r\nCross"));
        PORTC = 6; // a.k.a. B000000110 - Setting A0 to 0, A1 to 1, A2 to 1 (will be read as 767 on receiving end)
        // Note: use expression (PORTC & (~7)) | 6 if you don't want to change values of other bits (corresponding to pins A3, A4, A5)
        delay(keyDelay);
      }
      else {
        PORTC = 0; // Setting A0, A1, A2 pins to 0 (LOW)
      }

    }
  }
}
