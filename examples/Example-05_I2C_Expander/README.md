# Example-05: I2C Expander with SparkFun Qwiic Directional Pad

This example demonstrates how to use the KeyDetector library with the SparkFun Qwiic Directional Pad, which uses a PCA9554 8-bit I2C GPIO expander chip.

## Hardware Requirements

- Arduino board (or any other compatible board like ESP32, ESP8266, etc.)
- [SparkFun Qwiic Directional Pad](https://www.sparkfun.com/products/15316)
- Qwiic cable or I2C connection wires

## Wiring

The SparkFun Qwiic Directional Pad can be connected to your Arduino/board in one of two ways:

### Option 1: Using Qwiic Connector (Recommended)
If your board has a Qwiic connector or you have a Qwiic shield, simply connect the D-Pad to your board using a Qwiic cable.

### Option 2: Manual I2C Connection
If your board doesn't have a Qwiic connector, connect the D-Pad to your board using the following connections:

- D-Pad GND → Arduino GND
- D-Pad 3.3V → Arduino 3.3V
- D-Pad SDA → Arduino SDA (A4 on most Arduinos)
- D-Pad SCL → Arduino SCL (A5 on most Arduinos)

## Button Mapping

The SparkFun Qwiic Directional Pad has 5 buttons mapped to the following pins on the PCA9554 chip:

- Up: Pin 4
- Down: Pin 5
- Left: Pin 6
- Right: Pin 7
- Center/Select: Pin 3

## Active Low Logic

The buttons on the D-Pad are active LOW, meaning they are pulled up by default and pressing them connects the pin to ground. This example takes care of this by setting the `pullup` parameter to `true` in the KeyDetector constructor.

## Features Demonstrated

This example demonstrates:

1. Implementation of the `I2CExpander` interface for the PCA9554 chip
2. Reading button states from the I2C expander
3. Detecting button press, release, and held states
4. Handling simultaneous button presses
5. Sending button events to the Serial monitor

## Serial Output

Open the Serial Monitor at 115200 baud to see the button press, release, and held events. The example will print:
- When a button is pressed
- When a button is released
- When a button is held down
- When multiple buttons are pressed simultaneously

## Troubleshooting

If the example doesn't work:

1. Check your I2C connections
2. Verify that the I2C address is correct (0x27 by default)
3. Make sure the D-Pad is properly powered (3.3V)
4. Check if your Arduino can communicate with the D-Pad using an I2C scanner sketch

## Further Customization

You can modify the example to:
- Change the debounce delay (currently 10ms)
- Adjust the key press and repeat delays
- Add custom actions for different button combinations
- Integrate with other I2C devices using the same Wire interface 