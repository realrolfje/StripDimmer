# StripDimmer
This is a simple project to re-create an incandescant lightbulb effect with an RGB light strip. 

## Features:
- At lower brightness, the LED strip will have a warmer (orange/red) color. 
- At full brightness, the LED strip will have a bright cool white color.
- Automatically turns on when you enter the room, turns off when you leave.
- Touch sensor to switch ON/OFF
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
2. Open the "StripDimmer.ino" file with the Arduino IDE.
3. Select the Nano as board, and the AVRISP as programmer (or the one you normally use)
4. Upload.

