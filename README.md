KeyDetector
===========

Arduino library for detecting state change on analog and digital pins. Listens for specified values on the analog input and sets `KeyDetector` object state accordingly. Detects digital pin state change as well.

Can be used to detect key press events that were assigned distinctive levels of the single analog signal ("multiplexed" to analog signal), e.g. by using DAC to "encode" multiple digital signals to a single analog line.

* [When to Use](#when-to-use)
* [How to Use](#how-to-use)
* [Reference](#reference)
* [Examples](#examples)
* [License](#license)
* [**Wiki**](https://github.com/Spirik/KeyDetector/wiki)

When to use
-----------
E.g. if you find yourself in a situation when you need to execute some code based on values passed through single analog input. You may want to do that in case if you are running low on a free pin budget, but still need a way to receive multiple control signals from another Arduino (and don’t want to use Serial or another interface for some reason). Then you use simplest DAC on the end of transmitting Arduino and multiplex the encoded control signals into single analog signal that will be decoded on the receiving Arduino. See [Example 3](https://github.com/Spirik/KeyDetector/wiki/Example-03:-Multiplexed-signal-readings) provided with the library to learn how to multiplex digital signals and demultiplex them back using KeyDetector.

And since nature of the analog signal isn’t important for the library, you may use it to detect specific values of any analog source you connect to Arduino, be it some sensor or simple potentiometer. That way you may execute some code based on, e.g., temperature readings of the sensor or rotation of the knob. See [Example 1](https://github.com/Spirik/KeyDetector/wiki/Example-01:-Analog-signal-readings) provided with the library to learn how to build  absolute rotary encoder using potentiometer and KeyDetector library.

Of course you may do this without any fancy library whatsoever, but KeyDetector will provide you with convenient way of tracking previous reading and executing desired code once per signal change (i.e. once per key press, ignoring the duration of it being in a pressed state).

And it will detect state changes on the digital pins as well. See [Example 2](https://github.com/Spirik/KeyDetector/wiki/Example-02:-Digital-signal-readings) provided with the library to learn how to detect momentary push-buttons single and continuous presses.

KeyDetector used as a means to detect push-buttons presses in order to navigate and interact with graphic multi-level menu in examples provided with the [GEM](https://github.com/Spirik/GEM) library.

How to use
-----------

### Install

Library format is compatible with Arduino IDE 1.5.x+. There are two ways to install the library:

- Download ZIP-archive directly from [Releases](https://github.com/Spirik/KeyDetector/releases) section (or Master branch) and extract it into KeyDetector folder inside your Library folder.
- Using Library Manager (since Arduino IDE 1.6.2): navigate to `Sketch > Include Library > Manage Libraries` inside your Arduino IDE and search for KeyDetector library, then click `Install`. (Alternatively you can add previously downloaded ZIP through `Sketch > Include Library > Add .ZIP Library` menu).

Whichever option you choose you may need to reload IDE afterwards.

### Import

To include KeyDetector library add the following line at the top of your sketch:

```cpp
#include <KeyDetector.h>
```

### Use

For example you want to detect several key press events multiplexed to a single analog pin (e.g. pressing *Up*, *Right*, *Down*, and *Left* control buttons) and one simple key press event on a separate digital pin (pressing *Enter* button).

Define macro constants (aliases) for the keys you want to detect. These are identifiers you will use later on in you sketch. (You may opt not to do that and go with plain numeric values as identifiers but that may cause your code harder to read and understand).

```cpp
#define KEY_ENTER 1
#define KEY_UP 2
#define KEY_RIGHT 3
#define KEY_DOWN 4
#define KEY_LEFT 5
```

> **Note:** there is one predefined alias for the state when no buttons were pressed (no signal detected): `KEY_NONE` that stands for `0`. Make sure not to use the value of `0` (zero) for any of your key identifiers.

Create constants for the pins you want to detect signals on:

```cpp
const byte enterPin = 2;
const byte navPin = A0;
```

Create array of `Key` objects. It will hold information about which button press event should be detected on which input pin and at which level of signal (for multiplexed signals only):

```cpp
Key keys[] = {{KEY_ENTER, enterPin}, {KEY_UP, navPin, 127}, {KEY_RIGHT, navPin, 255}, {KEY_DOWN, navPin, 383}, {KEY_LEFT, navPin, 511}};
```

Create `KeyDetector` object and supply its constructor with `keys` array created at the previous step and explicitly pass the size of the said array:

```cpp
KeyDetector myKeyDetector(keys, sizeof(keys)/sizeof(Key));
```

In the `setup()` section of the sketch set the input modes of the pins you're detecting data on:

```cpp
void setup() {
  pinMode(enterPin, INPUT);
  pinMode(navPin, INPUT);
}
```

In the `loop()` section of the sketch call `detect()` method of the `KeyDetector` instance. It will check the pins for the specific readings and populate `current`, `previous` and `trigger` properties of the object accordingly.

```cpp
void loop() {
  myKeyDetector.detect();
}
```

And finally add some action based on detected key press events. `trigger` property will be populated once per key press and hold the identifier of the button being pressed:

```cpp
void loop() {
  myKeyDetector.detect();
  
  switch (myKeyDetector.trigger) {
    case KEY_ENTER:
      //Enter button was pressed
      //Some action goes here...
      break;
    case KEY_UP:
      //Up button was pressed
      //Some action goes here...
      break;
    case KEY_RIGHT:
      //Right button was pressed
      //Some action goes here...
      break;
    case KEY_DOWN:
      //Down button was pressed
      //Some action goes here...
      break;
    case KEY_LEFT:
      //Left button was pressed
      //Some action goes here...
      break;
  }
}
```

Key currently being in pressed state is stored in the `current` property of the `KeyDetector` object. Unlike `trigger`, it will be populated with the key identifier not only once per button press but for the whole period of it being in this state (i.e. while signal maintains its value from the previous reading).

```cpp
myKeyDetector.current
```

Previously read signal (i.e. previously pressed button) stored in the `previous` property of the `KeyDetector` object:

```cpp
myKeyDetector.previous
```

> Each of these properties (`trigger`, `current`, `previous`) ends up storing value of `KEY_NONE` when no press events are detected for user defined buttons.

Reference
-----------

### Key

Data structure for linking key/signal identifier, input pin and (optionally) value of the signal that should trigger key press event. Object of type `Key` defines as follows:

```cpp
Key myKey = {KEY_ID, keyPin, signalValue}; //For analog pin with multiplexed signal
```

or

```cpp
Key myKeyDigital = {KEY_ID, keyPin}; //For digital pin
```

* **KEY_ID**  
  *Type*: `byte`  
  Identifier of the key/signal.

* **keyPin**  
  *Type*: `int`  
  Pin the key is attached to.

* **signalValue** [*optional*]  
  *Type*: `int`  
  *Default*: `-1`  
  Level of the analog signal at which press event of the multiplexed key is triggered. If not explicitly set by user, default value of `-1`  is assigned and key considered to be digital.

> **Note:** since KeyDetector ver. 1.1.2 key detection on digital pins works either with `pinMode` set to `INPUT` (so the `HIGH` level of signal means that button is pressed) or `INPUT_PULLUP` (so the `LOW` level of signal means that button is pressed). So buttons (e.g. momentary switches) should be wired accordingly, either with pulldown resistor or pullup resistor (external or internal).

### KeyDetector

Class responsible for watching for desired level of signal to occur on specified pin. Holds current and previously detected key/signal identifier. Object of class `KeyDetector` defines as follows:

```cpp
KeyDetector myKeyDetector(keysArray, keysArrayLength[, debounceDelay[, analogThreshold[, pullup]]]);
```

* **keysArray**  
  *Type*: `Key*` (pointer to array of type `Key`)  
  Holds the reference to an array of `Key` elements.

* **keysArrayLength**  
  *Type*: `byte`  
  Length of `keysArray`. Should be explicitly supplied because array is passed by reference. Easy way to provide array length is to calculate it using the following expression: `sizeof(keysArray)/sizeof(Key)`.

* **debounceDelay** [*optional*]  
  *Type*: `byte`  
  *Units*: ms  
  *Default*: `0`  
  Delay in ms to account for any signal ripple (e.g. switch bounce) when detecting digital signal, or transient process that may occur when adjusting the source level of analog signal on transmitting end. Try increasing this value if you are experiencing detection of undesired adjacent signals prior to the detection of the actual signal you're sending. For example, that may be the case when you are using low-pass RC filter on transmitting end to convert PWM signal into analog (due to the temporal nature of the process).

* **analogThreshold** [*optional*]  
  *Type*: `int`  
  *Default*: `16`  
  Used to form the range of values of analog signal at which press event of the multiplexed key is triggered. E.g. for the signal level of 127 the range of `[127 - analogThreshold + 1 .. 127 + analogThreshold - 1]` will be formed and any signal being in that range will trigger an event. Default value of `16` allows for up to 32 evenly spaced signal levels being multiplexed (e.g. via 5-bit DAC) into single analog line with 10-bit ADC on the receiving end.  
  Increasing this value when less then 32 different levels are required will cause signal detection (key press) event to happen earlier and be in the detection range longer. It may increase stability of detection for the signal with high level of ripple. On the contrary, decreasing `analogThreshold` may cause unreliable detection or multiple sequential detections of the unstable signal.  
  Not used for detection of keys attached to digital pins.

* **pullup** [*optional*]  
  *Type*: `bool`  
  *Default*: `false`  
  Boolean flag that determines whether digital pins are connected with pullup resistor (so the `LOW` means that button is pressed), or pulldown resistor (so the `HIGH` means that button is pressed). Default value `false` is for pulldown resistor configuration.

#### Constants

* **KEY_NONE**  
  *Type*: macro `#define KEY_NONE 0`  
  *Value*: `0`  
  Alias for the state when no buttons were pressed (no signal detected).

> Make sure not to use the value of `0` (zero) for any of your key identifiers.

#### Methods

* **detect()**  
  *Returns*: nothing  
  Checks input pins (supplied to `KeyDetector` constructor during initialisation) for the specific readings (supplied during initialisation as well) and populate `trigger`, `current` and `previous` properties of the object accordingly.

#### Properties

* **trigger**  
  *Type*: `byte`  
  *Initial value*: `KEY_NONE`, `0`  
  Populated once per key press and holds the identifier of the button being pressed. Use it to detect key press event.

* **current**  
  *Type*: `byte`  
  *Initial value*: `KEY_NONE`, `0`  
  Populated during key press and holds the identifier of the button currently being in pressed state. Unlike `trigger`, it will be populated with the key identifier not only once per button press but for the whole period of it being in this state (i.e. while signal maintains its value from the previous reading). Use it to detect key hold event.

* **previous**  
  *Type*: `byte`  
  *Initial value*: `KEY_NONE`, `0`  
  Holds the identifier of the button that was pressed when `detect()` method was previously called.

> Each of these properties (`trigger`, `current`, `previous`) ends up storing value of `KEY_NONE` when no press events are detected for user defined buttons.

Examples
-----------
KeyDetector library comes with several annotated examples that will help get familiar with it. More detailed info on the examples (including schematic and breadboard view) available in [Wiki](https://github.com/Spirik/KeyDetector/wiki).

License
-----------
This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library.  If not, see <http://www.gnu.org/licenses/>.