# StripDimmer
This is a simple project to re-create an incandescant lightbulb effect with an RGB light strip. 

## Features:
- Automatically fades to 25% warm glow when you enter the room, turns off when you leave.
- Touch sensor to switch to full brightness (and off again).
- At low brightness, the LED strip will have a warmer (orange/red) color. 
- At full brightness, the LED strip will have a bright cool white color.
- Measures light; When it's dark, it will not (automatically) turn on.

## Requirements
To build this project, you need:
- An Arduino Nano v3.0
- 3x BUK555 Field Effect Transistor
- HC-SR501 PIR sensor (or compatible)
- Light Dependant Resistor (LDR)
- A 12V RGB LED strip with common anode (common +)
- A couple of resistors, LEDs, wires and an Electrolytic Capacitor, see schematic.

## How to upload this program to the Arduino
1. Copy the libraries in this project into the libraries folder of your Arduino installation.
2. Open the `StripDimmer.ino` file with the Arduino IDE.
3. Select the Nano as board, and the AVRISP as programmer (or the one you normally use)
4. Upload.

## How to build the project
Plug the Arduino Nano into a breadboard, and build the circuit according to the schematic below, also found as eagle file in the schematics folder. Find a value for the 1~10k resistor so that a dark LDR will make A0 reach at most 5 Volts. Hook up your LED strip and test the software.

![Schematic](schematics/StripDimmer.png?raw=true)

Schematics are edited in [Eagle](http://www.cadsoftusa.com/eagle-pcb-design-software/schematic-editor/).

### Calibrating colors
When the colors look off, check that you didn't mix up the red, green and blue LEDs of your led strip. Depending on the manufacturer or production run of your LED strip, the colors might not look like what you expected. Or maybe you want to dim it from green to purple. You can do just that by changing the `colorcurve` arrray in `colorCurve.ino`. 

You have two tools to do this:

1. In the documentation folder, there is an excel sheet which shows the different PWM values, plotted as a graph. In this graph you can see that in my software, I never give more than 50% energy to the blue LEDs, because these make the color too cold/blue. You can play with the values in the sheet and try to find a smooth "curve", and put that in the software. This requires a bit of guesswork, so it's better to use rool number 2:
2. In `testStuff.ino`, there is a function called `colorMixer()`. Call this method in the `loop()`. After the Arduino starts up, open the Serial Monitor of the Arduino IDE. You can now play with the RGB values as follows:
    - For more red, press `Shift-R`, for less red, press `r`.
    - For more green, press `Shift-G`, for less red, press `g`.
    - For more blue, press `Shift-B`, for less red, press `b`.
    
In the serial monitor, the software will print the current PWM values. Use this to judge the color and build your color curve.

### Calibrating the touch sensor
The touch sensor uses the [Capsense](http://playground.arduino.cc/Main/CapacitiveSensor?from=Main.CapSense) library, which pulses one pin and measures how long it takes for the other pin to follow. When you touch the sense pin, you become a capacitance to ground, which turns the resistor into a lowpass filter, which in turn makes the sense pin slower in following the output pin.

For the touch sensor to work reliably you need to connect the negative of the Arduino to earth. Ideally this would be the ground pin of the wall socket. If that is not available, connect it to the copper water pipe or heating pipes in your house. Not grounding your circuit will make the sensing very unreliable.

When the touch sensor does not react, decrease the `touchSensitivity` value in `StripDimmer.ino`. When the touch sensor randomly reacts, increase the value.

Uncomment the `#define DEBUG` line to enable debug logging. The StripDimmer will report what it is doing, and what the touch values are when you touch the sensor. The yellow led will also light up when a touch is detected.

### Calibrating the Light sensor
The Light Depending Resistor will not be accurate, and the light in your house is probably different from mine. Of course, when the LED strip is on, the LDR will see this, so we need to compensate for that. In my case, the resistance of the LDR is lower when light shines on it. This means that in the configuration in the schematic, a lower value at `A0` means more light.

There are two settings for the LDR sensitivity:

- `darkness`: The value at A0 at which the Arduino will consider the house to be "dark".
- `darknessWithDimmedLights`: When the LED strip is at 25%, this value will be used to consider the house to be "dark", apart from the lights from the LED.

The first thing to calibrate it the `darkness` value. Uncomment the `#define DEBUG` line in `StripDimmer.ino`. This will turn on logging, and also will reduce the reaction times so you can test quicker. Turn off the lights and make it dark in your house. Check the log output, and try to trigger the PIR sensor. Increase the value for `darkness` if the PIR sensor does not react.

Turn on the lights in the house (not the LED strip). Make sure you can not trigger the PIR sensor now. If you can still trigger it, decrease the value for `darkness`.

Now do the same checks, but for the situation where the LED strip is 25% dimmed. When it is dark in the house, and the LED strip turns on because you moved, make sure you can still trigger the LDR while the LED strip is 25% on. Adjust the `darknessWithDimmedLights` value the same way as you did `darkness`.

### Calibrating the PIR sensor
The PIR sensor has two mechanical potentiometers to set sensitivity and pulse time. Set the pulse time as short as possible so we can determine that in software. Set the sensitivity of the PIR sensor so that it just reliably detects you walking into the room, not higher. You don't want a cat, a fly, or a warm gust of air from the central heating to turn on the lights.

### Calibrating the LED on/off timing
Although the settings should be correct, you may want to make the StripDimmer slower or faster in turning off the lights when you leave. To do this you can change the following values, below the `define LOGLEVEL LOG_LEVEL_INFO` line:

- `keepBrightMs`: When the StripDimmer is at full brightness (the user touched the sensor), this is the time it takes before the lights are (very slowly) dimmed back to 25%. The default is 5 minutes. PIR activity resets the timer and turns the brightness back to 100% if it started dimming.
- `keepOnDimMs`: When the StripDimmer turned the lights from 100% to 25% because of lack of PIR activity, this is the time before it switches to 0%. The default is 2 hours. PIR activity within this time resets the timer and turns the lights back on. This is designed so that you can walk out of the kitchen, and when you walk back into the kitchen to resume kooking, the StripDimmer will go back to 100% brightness.
- `keepOffDimMs`: When the StripDimmer is at 25%, this is the time after which de StripDimmer decides to switch the lights off. The default is 1 minute. PIR activity within this time resets the timer and keeps the lights at 25%.

### Putting it in a housing
Be creative. A metal housing is best, both for radiated electromagnetic noise from the PWM, but also for being resistant to external noise which can interfere with the touch or PIR sensor. Tips to think about before picking up a drill:
- Pay attention to the cooling fins of the BUK555. They are connected to the DRAIN, so you should isolate them from ground. Check the [BUK555 datasheet](documentation/BUK555-100A%20N%20Channel%20PowerMOS%20TTL%20FET.pdf?raw=true).
- Make sure you place the PIR sensor in a smart place. The corned or a room is best, check the [datasheet of the PIR sensor](documentation/HC-SR501-PIR%20Sensor.pdf?raw=true) to see the viewing angle of the sensor.
- Polish an aluminum strip to double as a touch sensor and reflector to hide the LED strip.
