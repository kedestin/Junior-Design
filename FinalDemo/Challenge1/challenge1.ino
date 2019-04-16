#include "src/Bumpers.h"
#include "src/ColorSensor.h"
#include "src/DriveSystem.h"
#include "src/Led.h"
#include "src/Protothread.h"
#include "src/Receiver.h"
#include "src/Switch.h"
#include "src/Timer.h"
#include "src/Transmitter.h"

JD::Switch<1>   sw(-1), halleffect(-1);
JD::DriveSystem ds(-1, -1, -1, -1);
JD::Transmitter speaker(-1);
JD::Receiver    mic(-1);
JD::LED         blue(-1), red(-1), start(-1), green(-1), communication(-1);
JD::Bumper      bumper(-1, -1, -1);
JD::ColorSensor cs(-1, -1, -1);
JD::Updateable* peripherals[] = {&sw,  &speaker, &mic,    &blue,
                                 &red, &start,   &bumper, &cs};

void bot1();
void bot2();

void setup() {}

void loop() {
        for (auto p : peripherals)
                p->update();

        if (sw.read() == 0)
                bot1();
        else
                bot2();
}

// Note: It's functionally equivalent to check for any message when waiting for
// a message. We are never in a situation where the next action is ambiguous
// and depends on the message received.

void bot1() {
        // if collision
        // resolve

        static JD::Timer timer;
        PT_BEGIN();

        // Bot  1  flashes blue  and  red  LEDs  three  times
        red.blink(3, 1000);
        blue.blink(3, 1000);

        timer.start(1050);
        PT_WAIT_UNTIL(timer.isFinished());

        // signals  to  Bot  2  with  a  500  ms  message  that  it  is
        // starting.
        speaker.send(500);
        timer.start(550);
        PT_WAIT_UNTIL(timer.isFinished());

        // Bot 1 illuminates  its  start  LED  and  then  starts to  move.
        start.on();
        ds.forwards();

        // The  bot  mimics  the  white  dotted  line until it collides with
        // the wall of the test track.
        PT_WAIT_UNTIL(bumper.read() != JD::Bumper::Collision::None);

        // Rebounds from the wall
        ds.stop();
        static_assert(false, "Must make a turn of X degrees off wall");

        // Then moves to find the bluepath on the far side
        ds.forwards();
        PT_WAIT_UNTIL(cs.read() == JD::ColorSensor::Blue);

        // When it does so, Bot 1 illuminates a blueLED.
        blue.on();

        // Bot 1 must follow the bluepath until it detects the
        // pedestrian(magnetic field).
        static_assert(false, "Must make an X Degree turn onto line");
        static_assert(false, "Must linefollow on blue until pedestrian");

        // Here it turns off  the blue LED  and  illuminates  a green LED
        blue.off();
        green.on();

        // Bot  1  makes  a 90° right turn. The  bot will  be  judged  how
        // tight a  90° right turn  it  makes
        ds.rotateDeg(JD::DriveSystem::RIGHT, 90);
        ds.forwards();

        PT_WAIT_UNTIL(cs.read() == JD::ColorSensor::Yellow);
        // When  it  detects  the  yellow  path it beeps  it  horn  twiceand
        // turns  off  the  green  LED.
        static_assert(false, "Honk twice");
        green.off();
        // When Bot  1 makes the right turn it follows  the  yellow  path.

        ds.rotateDeg(JD::DriveSystem::RIGHT, 90);
        static_assert(false, "Follow yellow path until detects pedestrian");

        // When Bot 1 detects  the  policeman  (magnetic  field) it  beeps  it
        // horn twice and makes a 90° left turn.The bot will be judged how
        // tight a 90° leftturn it makes
        static_assert(false, "Honk twice");
        static_assert(false, "Make a 90 degree left turn");


        // When Bot 1 detects the redpath it illuminates a red LED, makes a
        // 90° left turn traveling along the redpath until it detects the
        // pedestrian.
        ds.forwards();
        PT_WAIT_UNTIL(cs.read() == JD::ColorSensor::Yellow);
        ds.rotateDeg(JD::DriveSystem::LEFT, 90);
        static_assert(false, "Follow red path until detects pedestrian");

        // When it detects the pedestrian (magnetic field), it must
        // flash a green LED, bringingthe bot to a complete and fullstop
        green.blink(1);
        ds.stop();

        // communicate  with  the  TCC  via  a  200  ms  message_1
        speaker.send(200);

        // When  Bot  1  receives  a responding 200 ms message_1 from the TCC,
        // it illuminates its communication LED
        PT_WAIT_UNTIL(mic.receivedMsg() == JD::Receiver::msg200);
        communication.on();

        // and continues along the redpath to the end of the path at the wall.
        static_assert(false, "Follow red path until end of path");

        // When it detects the wall at the end of the redpath, it stops,
        // blinking its redLED twice.
        red.blink(2);

        // Bot 1 rotates 90°to the left, stopping in the vicinity of the end
        // red path and the spot on the wall where it collided
        ds.rotateDeg(JD::DriveSystem::LEFT, 90);

        // comes to a full stop and illuminates blue and red LEDs flashing them
        // three times
        ds.stop();
        blue.blink(3);
        red.blink(3);

        // Bot 1 then  signals  to  the  TCC  with  a  300  ms  unit  step
        // function  “message_2”.
        speaker.send(300);

        //  Bot  2  signals a  500  ms message to Bot 1 that is it is starting
        //  to follow itspath.  Appropriate LEDs are flashed to indicate  this
        //  exchange.
        PT_WAIT_UNTIL(mic.receivedMsg() == JD::Receiver::msg500);

        // Bot 2 signals Bot 1 with a 500 ms unit step function “message_4” to
        // start traversing an unmarked path in the black track,  toward
        // eachotherfor  a  gentle  collision.
        PT_WAIT_UNTIL(mic.receivedMsg() == JD::Receiver::msg500);
        ds.forwards(0.4);

        // When  they  collide,  the  bots flash  their headlights, braking
        // lights,beep their hornsand turn signals ten times to indicate the
        // end of the challenge. The LEDs must finish flashing to indicate the
        // end of the test within the 90-second  time  limit.
        static_assert(false, "Flash lights and horns 10 times");

        PT_END();
}
