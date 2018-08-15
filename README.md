# Wireless Gesture Controlled Game Controller


## Challenges:
+ To make the controlling as smooth as possible.
+ To make sure that during workshop, no signal should interfere with the other TSOP present over there.
+ Try with multiple games.

## Progress till date:
+ IR Transmission code is complete.
+ TSOP decoding is complete.
+ Accelerometer interfacing is done.
+ Interfacing with Python is done.
+ Code for Transmitter, Receiver microcontroller is done.
```` 
NOTE: Basic objective of the prototype has been achived. Now this need a little bit of modification to make it as much as user freindly as we can.
````
## Things to be Done
+ Instead of Accelerometer, we can think of using Joystick.
+ Also a keypad control can be added so, if the user wants to control it with the button he can
+ In NEC protocol we have the address to target which TSOP module we want to send the data. Spo to prevent interference of different IR Signals with different TSOP, we have to have atleast 90 address (*8-bit binary form*) in our hand.

## Components to be Used:
+ Arduino UNO
+ Arduino NANO
+ ADXL335 Accelerometer
+ 2 IR Led
+ TSOP 1738 Module
+ 330 ohm Resistor
+ Jumper Wires
+ Headers
+ Pref Boards
+ 9 V Battery
+ Breadboard



## Repository Organized.
