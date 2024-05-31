# simple-home-security
An Arduino-based machine that alerts of break-ins using proximity.

## Introduction
Oftentimes when living with multiple people in the same house, there are times when you may not want anyone else to enter your room—especially when you are not there. As such, my main motivation for this was for that purpose: to keep others out of my room. However, while developing the system, I found that it could be easily repurposed into a simple home security system.\
As such, I created a security system with the following goals and features in mind:
* Detect movement at a specified distance and alert the user in some way.
* Play a sound to alert and warn the intruder.
* Change the display of the on-board LED matrix to an appropriate symbol.
* Write a message to the Arduino Serial Monitor whenever movement is detected, _logging the time it happens._

## Circuit
![Schematic of Home Security Circuit](circuit.svg)\
Above is a schematic of the circuit required, which is simple and able to be freely modified. The following components are used:
* Arduino Uno R4 WiFi (Uno R3 shown in schematic)
* Ultrasonic Distance Sensor (HC-SR01)
* Piezo Buzzer
* 2x Resistors (Equivalent resistance)
There are three sub-circuits that comprise this system, each with specific functions.
### Ultrasonic Sensor Functionality
The purpose of the ultrasonic sensor is for the system to be able to calculate the distance of an object—or in this case, a person—near it. The Vcc and GND pins are straightforward and can just be plugged into the corresponding Arduino pins.\
"TRIG" or "Trigger" is our input pin. By sending a HIGH signal for at least 10µs, the sensor will emit the ultrasonic wave required for the next step. Since we only need to emit a simple HIGH/LOW signal, Trigger can be connected to any digital I/O pin on the Arduino.\
This is where our output pin, "ECHO", comes into play. When a wave is emitted, it will travel to the nearest object and be reflected off it, returning to the sensor. The Echo pin will be set to HIGH for an amount of time equivalent to 
