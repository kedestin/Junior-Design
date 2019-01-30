---
title: Bot Spec
author: David Janowsky & Kevin Destin
output: pdf_document
geometry: margin=1in
fontsize: 11pt
---

**Drive** **System**

 * **Power** (4.5 V 80-800mA)
 * **Output**: Rotation (Max 120 RPM)
 * **Functionality**: Provides mobility 
 * **Performance**:	
   *  90 deg (+- 5 deg) turn
   * 25 in (+- 5 in) / 20 seconds in a straight line
 * **Acceptable** **dimensions** (**length**, **width**, **and** **height**)
   **and** **weight**: 
   * Wheel 11g  x4 (60x60x8mm)
   * Motor  19g x2 (36.5x20x27.4mm)

**Power**

 * **Input**: 9V[Arduino] + 1.5V [Motor]  +  Control Signal
 * **Output**: (4.5 V 80-800mA) [Motor] + (7-12V ) [Arduino]
 * **Functionality**: Provides power to systems that require it 
 * **Performance**: (4.5 V 80-800mA) [Motor] + (7-12V ) [Arduino] for 90 seconds
   atleast
 * **Parameters:**
   * 9V 17.5x26.5x48.5mm
   * 1.5V 50x13x13mm

**Communication – Emission (Speaker), Reception (Microphone), Human
Interaction**

**Emission**

 * **Input**: Control Signal and Power (.5 W)
 * **Output**: pulses at 18.75 kHz
 * **Functionality**: Outputs messages in the form of pulses
 * **Performance**: 200, 300, 400, 500 ms pulse (+- 50ms)
 * **Paramters**: 
   * Speaker 3.8x20x20mm 
   

**Reception**

 * **Input**: Power and Pulse at 18.75kHz
 * **Output**: The decoded message
 * **Functionality**: Receives pulses at determined frequency and decodes it
 * **Performance**: Differenctiate between 200, 300, 400, and 500ms
 * **Parameters**:
   * Microphone: 9.7x9.7x4.5mm

**Human Interaction**

Brake Lights, Turn signals, Headlights, Horn, LED etc...

 * **Input**: Control Signal
 * **Output**: Appropriate feedback for humans (brake lights illuminate when
   breaking, etc...)
 * **Functionality**: Provides feedback to human onlookers as to what action is
   being taken by the bot
 * **Parameters**:
   * Speaker: 
     * 0.1 W (0.89 V)
     * 15x15x5.7mm


**Sensor – Magnetic Field, Accelerometer, Light Sensor**

**Hall Sensor (Collision Prevention)**

 * **Input**: Magnetic Field + Power 
 * **Output**: Voltage proportional to magnetic field strength
 * **Functionality**: Detect magnetic (Pedestrians)
 * **Performance**: Must output a strong enough signal before hitting
   pedestrian
 * **Parameters**:
   * 1.5x4.1x17.05mm


**Collision Detection**

 * **Input**: Collision  + Power
 * **Output**: Binary signal designating a collision
 * **Functionality**: Signals bot to collision
 * **Performance**: Minimize false positives and misses


**Light** **Sensor**

 * **Input**: Light + Power
 * **Output**:  Light  + Voltage
 * **Functionality**: Path Following + Bot Collision Detection
 * **Performance**: Must differentiate between black blue yellow and red. Must
   avoid collision with bot.
 * **Parameters**:
   * 35x5.8x5.8mm
  
**Microcontroller**

 * **Input**: (7-12V DC)
 * **Output**:  Control Signals for various systems 
 * **Functionality**: Runs all logic and code from running autonomously

**Miscellaneous**

 * **Cost**: $17,599
 * **Weight**: ~3 lbs.
 * **Size**: ~10 in. x 10 in. x 10 in.
