---
output: pdf_document
---
EE31 Junior Design 
Spring 2019 
 	Project Design Phase 02B  
Pre-Studio 
 
Assignment Date and Due Date: check Assignment/Due Date Listing Pre-studio
assignments are to be recorded in your Bound Lab Notebook and reviewed by the
instructor at the start of class before starting your design work. 
	
 
If you are not sure what to do on this assignment, ask your instructor or TAs. 
Pre-studio questions:  Motion control 
 
Objective:  Design and construct a breadboard electrical circuit platform that
is able to demonstration motion control with two motors using software from the
Arduino microcontroller for forward, reverse, turn left, turn right, turn 180°,
turn an arbitrary number of degrees, and vary the speed using the L293D
H-Bridge integrated circuit. 
 
Think: 
1. **What problem are you trying to solve?**
Design a drive system that can drive two motors both forwards in backwards

2. **What does success look like for this design problem?  How do you know when
   you are done?  What are the specifications on the deliverable?  The
   tolerances?**


Fixture that can drive in a straight line at 1.5 inches/second for 5 inches
without deviating more that 10 degrees from the straight line. We know when we
are done once we have 


3. **Define pulse width modulation (PWM)**.

Modifying the duty cycle of a periodic digital signal.

4. **Who is the customer?  What is the customer requirement that impacts motion
control?  In the last phase there was only one motor, now there are two motors.
How does your design specification change from last time?  What customer
requirement does this reference?** 

The customer is the course staff. The customer requirement impacts motion
control is the requirement to drive in a straight line for 5 inches at 1.5
inches per second without devaiting more 10 degrees from the straight. The
design specificatoin change from last time is that we need twice as much
current, two control signals and callibration between the two motors. They all
reference straight line requirement.

5. **What are the pin assignments for the L293D H-Bridge integrated circuit?**   

 * PIN 1  = ENABLE1
 * PIN 2  = IN1
 * PIN 3  = OUT1
 * PIN 4  = GND
 * PIN 5  = GND
 * PIN 6  = OUT2
 * PIN 7  = IN2
 * PIN 8  = $v_s$
 * PIN 9  = ENABLE2
 * PIN 10 = IN3
 * PIN 11 = OUT3
 * PIN 12 = GND
 * PIN 13 = GND
 * PIN 14 = OUT4
 * PIN 15 = IN4
 * PIN 16 = $V_{ss}$

6. **What are the voltage and current limits of this H-Bridge?** 


| Parameter                  | Value | Unit |
| -------------------------- | ----- | ---- |
| Supply Voltage             | 36    | V    |
| Logic Supply Voltage       | 36    | V    |
| Input Voltage              | 7     | V    |
| Enable Peak Output Current | 1.2   | V    |

7. **You know how a CMOS inverter is constructed from MOSFETs:  How is the buffer
shown on the L293D schematic constructed?  How is this similar to your discrete
FET controller last week?** 

The buffer shown on the L293D schematic is probably constructed with two CMOS
inverters in series. Our discrete FET was an inverter. 

8. **Draw out the internal circuit schematic of the H-Bridge with an applied
voltage of 5V such that two motors are controlled.  What voltages are applied
to each of the pins to make two motors move forward?  Reverse? Rotate?  How is
this related to the truth table for the H-Bridge?  How would you modify these
modes to include speed control? **

![](https://i.imgur.com/1RyC8fl.png) 

Forwards 

IN1 = HIGH
IN2 = LO

IN3 = LO
IN4 = HIGH

Reverse

IN1 = LO
IN2 = HIGH

IN3 = HIGH
IN4 = LO

Rotate

IN1 = LO
IN2 = HIGH

IN3 = LO
IN4 = HIGH

9. **What is the estimated current draw?  Does this change your battery
considerations or power subsystem? **


 Estimated current draw is 1.2 A. (Max for motor is 800mA but we aren't near.
 Per channel for H-Bridge is 600mA)

10. **Sketch how the Arduino connects to the H-Bridge to turn the two motors on
and off.** 

![](https://itp.nyu.edu/physcomp/wp-content/uploads/LabDCMotorH-Bridge9V_bb.png)

11. **How fast can the Arduino be switched from one motor to the other?** 

The H-Bridge has a limit of 5kHz

12. **In lab you may observe that the Arduino occasionally resets when the motor
turns on.  Speculate on why this happens.  How is this prevented? **

Overdrawing current.

13. **The bot is moving in a straight line.  One of the sensors detects a problem
requiring the bot to turn left.  Write out the pseudo-code that make the bot
change direction. **

```c++
  void turn_left(){
      // Increase right motor's forwards speed
      forwards(RIGHT, LEFT_SPEED + CONSTANT);
  }
```
14. **What states are required to provide all desired motion control for the bot?
Which pins need to be defined in the setup() function?  Which pins are global?** 

We can define the following states $\{forwards, backwards, stopped\}$ for each
of the motors.  All of the desire motion control for the bot can be generated
using combinations of the above. Any 4 pins that handle PWM. All of them

15. **The loop() function runs forever.  How do you make the Arduino do forward,
reverse, turn left, turn right as required by the current ‘state’ of the bot
(i.e., input from the bot’s sensors)?**

Keep track of state in a global variable. Use sensors to modify state. Perform
action based on state.

16. **How does the bot turn 60°? (Hint: what is the specified shaft speed and
wheel diameter?) What is the error tolerance on turning a desired angle?**

Callibrate for a specific angular velocity. Run motor for required time to turn
60°.

Plan: 

 
1. **Design, sketch and analyze a schematic of the circuit that you will
construct during class to meet the posted specification.** 

![](https://upload.wikimedia.org/wikipedia/commons/thumb/1/10/Fritzing-breadboard.svg/2000px-Fritzing-breadboard.svg.png)

2. **What parts will you need to build a motor control circuit with an
   H-Bridge?** 

   Motor, Arduino, Wires, Battery, Capacitors, Resistors

3. **Design in pseudo-code the Arduino program that you will implement during
class to meet the posted specification.** 

```c++
#include <arduino/io.h>

enum STATE {FORWARDS, TURN_RIGHT, TURN_LEFT REVERSE, STOPPED}
STATE currState = STOPPED;

void setup(){

}

void loop(){
    state = getStateFromSensors();

    switch(state){
      case STOPPED:    stop();
                       break;
      case TURN_RIGHT: turn_right();
                       break; 
      case TURN_LEFT:  turn_left();
                       break;
      case FORWARDS:   forwards();
                       break;
      case REVERSE:    reverse();
                       break;
      default: abort;
    }
}
```
4. **Write out the pseudo-code for moving in a straight line?** 

```c++
  MOTOR_LEFT = 1;
  MOTOR_RIGHT = 2;

  void forwards(){
     speed(MOTOR_LEFT, FORWARDS);
     speed(MOTOR_RIGHT, REVERSE);
  }
```
5. **Write out the pseudo-code for turning left?** 
```c++
  //Assuming a pivot
  void turn_left(){
    speed(MOTOR_LEFT, OFF);
    speed(MOTOR_RIGHT, ON);
  }
```
 
Do: 

1. **Are you following your task list?  Who is responsible for what? When will
it be completed? What and how did you get a commitment? Do you believe the task
will be complete by the promised delivery date?  Do you trust your teammate to
follow through on their commitment? Why or why not?  If not, what are you going
to do about it?** 

We have for previous stages, but don't have one currently. 

2. **What is the fixture required to get the design constructed? What makes a
fixture so important for this phase?  What is the critical functionality of the
design fixture?** 
 

It isn't possible to test without having a fixture; you need something for the
motor to drive. To provide structural support for the bot and its components.

Test: 

1. **What is your test plan?** 

  Calibrate motors, run motor see if it goes in a straight line. Try get it to
  straight, if it doesn't change how fast we're running the motors.

2. **What equipment is required?** 

Our bot, a tape mesure, a stop watch, a protractor

3. **How do you test the speed and direction of the bot?** 

With a stopwatch and a meter stick.

4. **For what reasons do you need a special test bed?**   

The reason to facilate testing

Reflect: 

1. **What was the most unexpected failure? How did it happen?** 

We had no unexpected failures.

2. **What did you learn?** 

I'm really shit at circuit

3. **Why myth in your head was dispelled?** 

Task Questions: 

1. **How well did you complete the team design tasks you took
responsibility for from the last design phase?  What did you do well? What
unexpected problems did you have?  How did you overcome them?  What did you
learn? **

   Great. The final product was satifactory. I was out of town. Dave picked up
   the slack. 

2. **What part of the team design tasks for this phase are you planning to take
responsibility for this assignment?** 

  Code and hope to be more helpful with circuit design 

3. **What part of the team building and testing tasks for this phase are you
planning to take responsibility for this assignment?** 

  I plan to split responsibility for everything equally with my partner David

4. **What will you need to learn to complete the team tasks for this phase for
which you have taken responsibility?**  

  How to work with an HBRIDGE To control motors and with an arduino.
  
Word to the Wise: The best design approach is to assembly four mosFET ICs as a
discrete H-bridge to understand how an H-bridge works and how it controls the
motors – forward, backward, and turns – and at various speeds.  When you have
completed a discrete H-bridge AND have software on the Arduino working for
motion control, THEN AND ONLY THEN duplicate the circuit with the H-bridge IC
and keeping the discrete bridge intact to make your debugging efficient.  Make
sure the two circuits work the same.  If they do not, understand the changes
you need to make to make your design work.  Typical issues are bypass caps and
diodes needed versus non-needed, voltage levels on the PWM, ground loop, power
source not wired correctly or at the incorrect voltage, voltage not being
stable enough, and a host of other items.  This is the reason you want to start
with a discrete Hbridge as you will understand what is happening, determine how
to fix the problems, and when it happens with the Hbridge IC when built into
your bot, you understand the flaky behavior, recognize it, and know how to fix
it.  
 
You can do anything you want, as it is your design and your design approach.
However, there is a reason this section is in a large bold font. What might
that reason be? 
