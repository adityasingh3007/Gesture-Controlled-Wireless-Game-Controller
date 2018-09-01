# Wireless Gesture Controlled Game Controller


## Challenges:
+ Controlling can be further improvised.
+ To make sure that during workshop, no signal should interfere with the other TSOP present over there.

## Progress till date:
+ IR Transmission code is complete.
+ TSOP decoding is complete.
+ Accelerometer interfacing is done.
+ Interfacing with Python is done.
+ Code for Transmitter, Receiver microcontroller is done.
+ Transmission can be done from distance up to 3.5 meters(approx).
+ Asphalt, Star Defender, Motor racing game can be played using these codes.
```` 
NOTE: Basic objective of the prototype has been achived. Now this need a 
	little bit of modification to make it as much as user freindly as we can.
````
## Things to be Done
+ In NEC protocol we have the address to target which TSOP module we want to send the data. So to prevent interference of different IR Signals with different TSOP, we have to make atleast 90(this will varry as per the number of registrations) different address (*8-bit binary form*). 

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
+ PCBs



## Repository Organized.
