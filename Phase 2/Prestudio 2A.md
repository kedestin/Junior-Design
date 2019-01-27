---
output: pdf_document
geometry: 1 in
---

Think: 

 1. **Organize the questions that follow in a think, plan, do, test, reflect,**
    **demo iterative process. What thinking do you need to do?**

How to regular current with the microcontroller and how to callibrate the motor
based on the load.

 2. **What problem are you trying to solve? **

Figure out how to drive motors using a microcontroller

 3. **What is the specific customer requirement that impacts on motor control?
    **

Their specification on how fast it must go, and how precisely it turns

 4. **What does the customer care about motor control if the swarmbot is**
    **autonomous?** 

The motors need to work for it to be autonomous

 5. **Will a motor require more current when spinning freely or when propelling
    a swarmbot?** 

It needs more current when under load.


 6. **What is the minimum DC current required by the swarmbot’s motor if
    operated at 4.0 volts?**

The minimum is 80mA for no load

 7. **If you use a BS170 n-channel MOSFET to control your motor, what is the**
    **minimum gate-to-source voltage you need to apply to the gate terminal in
    order to guarantee that the transistor turns on?  (If you do not know,
    check the data sheet on trunk to determine the answer.)  What is the
    tolerance on this minimum?** 

2.1 (+- 1.3) V

 8. **What is tolerance on the maximum gate voltage that could be applied to
    the BS170 that still ensures the transistor will be in cutoff?**

 9. **What is the maximum current for the analog pins on the Arduino Mega? **

Digital is 40mA. Assume same for Analog

 10. **Review the various amplifier configurations that you learned in EE-21.
     Now, starting with a 6-volt power supply, design a circuit using the BS170
     MOSFET that allows the Arduino to switch the motor on and off.  The MOSFET
     gate voltage is constrained to the digital output voltage available from
     the Arduino.  As an initial estimate, assume that the operating current of
     the motor is 250 mA when it is actually propelling the swarmbot (i.e., the
     motor is delivering some power to the wheels).**


![Circuit](https://cdn2.bigcommerce.com/server800/a8995/product_images/uploaded_images/mosfet.png?t=1408498498)

 11. **Is it possible to energize the motor directly from the Arduino?  Why or
     why not? **

No it is not possible to energize the moror directly from the Arduino since the
Arduino has a maximum output current of 40mA. 

 12. **In item 5, how much power is being delivered to the motor? Approximately
     how much power is wasted in just turning the motor and gearbox?  How much
     power is delivered to the wheels and actually used to propel the
     swarmbot?**



 13. **What is the milliamp-hours capacity of a 9V battery if the current draw
     is 100 mA.  If the swarmbot draws 200 mA, how long can a swarmbot move in
     a straight line at full speed? Do calculations for the motor alone and
     with a one-pound mass swarmbot and two motors.  How does the second
     calculation degrade the battery compared to the first one?  You will need
     to review the datasheet in the file 9V Energizer Battery Datasheet.pdf on
     trunk.**

The milliamp-hours capacity of 9V battery is  500mAh if the current draw is 100
mA.
     
 $???$


Plan: 

 1. **Design, sketch and analyze a schematic of the circuit that you will
    construct during class to meet the posted specification.**
    
![](https://i.imgur.com/lOgAFWL.png)

 2. **Design, sketch, and plan out the fixture required to construct the design
    **

Protoboard, wires.

Do: 

 1. **Where do you need to do the design? **

In our notebook


 2. **What tasks are necessary to get from design to demo? **
   
   Think, Plan, Do

 3. **What questions are not yet answered?  Where do you intend to get the
    answers?**

  Motor specifics

Test: 

 3. **What tests (experiments) will you need to design?**
   
   Tests to verity that the motor can be driven backwards, forwards, can 
   dynamically switch between the two direction and tests to verify power
   draw.

 4. **Think about the test equipment required to validate and verify your
    design. What is the best test equipment for this phase of the project?**

   Our eyes, and a stopwatch


Reflect: 

 1. **How many things failed? **

    Nothing yet. Many to come.

 2. **Are you honest about your recording your and your team’s failures? What
    are you afraid of to not list your or your team’s failures along the way?
    What proficiency or skills are you are working to acquire? How do you know
    you have acquired them?**

    No. Not afraid, just lazy. 

    Better circuits skills. Once I notice that I'm good at circuits

 3. **One behavior Junior Design students have is they record only the final
    result in their design notebooks.  They are neat and unblemished. What do
    you think is the reason that Junior Design students tend to only show their
    correct answer?**

    Laziness

 4. **What is the value of having cross outs and mistakes documented along side
    corrections and explanations, or earlier design concepts that did not work
    along side the one that did work in your design notebook?**

    Having earlier iterations allows you to reflect on the journey, and earlier
    ideas can sometimes be useful in the future

 5. **Who is not telling the whole story?  For what reason? **

    Ron Lasser intentionally obfuscates details to to help us learn

 6. **How many questions did you ask today?  To Whom? **

    At least infinity to David. Some to Peter, Mateo and Emily.

Task Questions: 

 1. **How well did you complete the team design tasks you took responsibility
    for from the last design phase?  What did you do well? What unexpected
    problems did you have?  How did you overcome them?  What did you learn?**

    I completed my task well. The code works great. Unexpected  problems:
     * Integers are only 2 bytes on Arduino vs 4 bytes EVERYWHERE ELSE
  
   I learned that Arduino's use a subset of C++ compatible with AVR
   microcontrollers. It's compiled with
   [AVR-GCC](https://www.nongnu.org/avr-libc/). It supports most of C++11 
   except for new and delete and the standard library.

 2. **How well did you complete the team bot spec tasks you took
    responsibility for from the last phase?  What did you do well? What
    unexpected problems did you have?  How did you overcome them?  What did you
    learn?**

    It was fine. Write the spec. No problems. 

 3. **What part of the team design tasks for this phase are you planning to
    take responsibility for this assignment?** 

    I plan on 

 4. **What part of the team building and testing tasks for this phase are you
    planning to take responsibility for this assignment?** 



 5. **What will you need to learn to complete the team tasks for this phase
    for which you have taken responsibility?**

