#include "src/Bumpers.h"
#include "src/ColorSensor.h"
#include "src/Dashboard.h"
#include "src/DriveFeedback.h"
#include "src/DriveSystem.h"
#include "src/Horn.h"
#include "src/Led.h"
#include "src/Protothread.h"
#include "src/Receiver.h"
#include "src/Switch.h"
#include "src/Timer.h"
#include "src/Transmitter.h"

constexpr unsigned long rampUpTime_ms = 20;
constexpr uint8_t       RED_LED       = 36;
constexpr uint8_t       BLUE_LED      = 37;

JD::DriveSystem ds(10, 8, 11, 12, rampUpTime_ms);
JD::ColorSensor cs(RED_LED, BLUE_LED, A15);

JD::Switch<1>     sw(33), halleffect(39);
JD::Transmitter   speaker(30);
JD::Receiver      mic(A7);
JD::LED           blue(44), red(48), start(44), green(46), communication(46);
JD::Bumper        bumper(43, 42, 28);
JD::Horn          horn(13);
JD::Dashboard     dash(JD::LED(2),
                   JD::SevenSegment(53U, 52U, 34U, 47U, 49U, 51U, 50U, 255U),
                   ds);
JD::DriveFeedback feed(45, 38, 25, 24, 27, 26, ds);
JD::Updateable*   peripherals[] = {&sw,    &speaker, &mic,  &blue,      &red,
                                 &start, &bumper,  &cs,   &ds,        &sw,
                                 &dash,  &feed,    &horn, &halleffect};

void bot1();
void bot2() {}

template <class T>
const T& minimum(const T& a);

template <class T, class... Args>
const T& minimum(const T& a, const T& b, const Args&... args);

void printColor() {
        static JD::ColorSensor::Color old  = JD::ColorSensor::Color::Black;
        JD::ColorSensor::Color        curr = cs.read();

        if (curr != old) {
                Serial.print("Color is ");
                switch (curr) {
                        case JD::ColorSensor::Red: Serial.print("Red."); break;
                        case JD::ColorSensor::Blue:
                                Serial.print("Blue.");
                                break;
                        case JD::ColorSensor::Black:
                                Serial.print("Black.");
                                break;
                        case JD::ColorSensor::Yellow:
                                Serial.print("Yellow.");
                                break;
                        default: Serial.print("Unknown: "); break;
                }
                old = curr;
                Serial.println();
        }
}

template <class... T>
bool lineFollow(T... args);
void setup() {
        Serial.begin(115200);
        JD::setupPWM();
        feed.headLightOn();
        Serial.println("Starting");
        JD::Calibration::set(JD::Calibration::rotate90AtFull_ms, 375);
        horn.sendSequence(50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
                          50, 50, 50, 50, 50, 50, 50);
        // ds.forwards(0.5);
}

void loop() {
        for (auto p : peripherals)
                p->update();
        static JD::Timer timer;
        PT_BEGIN()
        speaker.send(200);
        PT_WAIT_UNTIL(timer.hasElapsed(400));
        PT_END()
        PT_RESTART();
        if (sw.read() == 0)
                bot1();
        else
                bot2();
}

// Note: It's functionally equivalent to check for any message when waiting for
// a message. We are never in a situation where the next action is ambiguous
// and depends on the message received.

void bot1() {
        constexpr double speed = 0.5;
        static JD::Timer timer;
        auto             timeForXDeg = [](double speed, double angle) {
                constexpr unsigned calibrate90 = 375;
                return (1 / speed) * (angle / 90) * 375;
        };
        PT_BEGIN();

        // Bot  1  flashes blue  and  red  LEDs  three  times
        red.blink(3, 1000);
        blue.blink(3, 1000);

        // timer.start(1050);
        PT_WAIT_UNTIL(timer.hasElapsed(1050));

        // signals  to  Bot  2  with  a  500  ms  message  that  it  is
        // starting.
        speaker.send(500);
        // timer.start(550);
        PT_WAIT_UNTIL(timer.hasElapsed(550));

        // Bot 1 illuminates  its  start  LED  and  then  starts to  move.
        start.on();
        ds.forwards(speed);

        // The  bot  mimics  the  white  dotted  line until it collides with
        // the wall of the test track.
        Serial.println("Waiting until collision");
        PT_WAIT_UNTIL(bumper.read() != JD::Bumper::Collision::None);
        Serial.println("Hit something");
        // Rebounds from the wall
        ds.stop();
        PT_WAIT_UNTIL(timer.hasElapsed(200));
        Serial.println("Backing Up");
        ds.backwards(speed);
        // timer.start(750);
        PT_WAIT_UNTIL(timer.hasElapsed(750));

        ds.stop();
        PT_WAIT_UNTIL(timer.hasElapsed(200));
        Serial.println("Rotating");
        // static_assert(false, "Must make a turn of X degrees off wall");
        ds.rotate(JD::DriveSystem::RIGHT, speed);
        // PT_WAIT_UNTIL(timer.hasElapsed(375 * 60.0 / 90.0));
        // ds.stop();
        // Then moves to find the bluepath on the far side
        // ds.forwards(speed);
        PT_WAIT_UNTIL(cs.read() == JD::ColorSensor::Blue);

        // PT_WAIT_UNTIL(cs.read() == JD::ColorSensor::Blue);

        // When it does so, Bot 1 illuminates a blueLED.
        blue.on();
        ds.stop();
        PT_WAIT_UNTIL(timer.hasElapsed(200));

        // Bot 1 must follow the bluepath until it detects the
        // pedestrian(magnetic field).
        // static_assert(false, "Must make an X Degree turn onto line");

        // static_assert(false, "Must linefollow on blue until pedestrian");
        Serial.println("Follow blue");
        ds.turn(JD::DriveSystem::LEFT, 0.5);
        PT_WAIT_WHILE(
            (followEdge(JD::ColorSensor::Blue, JD::DriveSystem::LEFT),
             (halleffect.read() == 1)));
        Serial.println("Stopping");
        ds.stop();
        PT_WAIT_UNTIL(timer.hasElapsed(1000));
        Serial.println("Done following blue");
        // Here it turns off  the blue LED  and  illuminates  a green LED
        blue.off();
        green.on();

        ds.forwards(speed);
        // timer.start(800);
        PT_WAIT_UNTIL(timer.hasElapsed(1500));
        // Bot  1  makes  a 90° right turn. The  bot will  be  judged  how
        // tight a  90° right turn  it  makes
        ds.rotate(JD::DriveSystem::RIGHT, speed);

        PT_WAIT_UNTIL(timer.hasElapsed(timeForXDeg(speed, 90)));

        ds.stop();

        ds.forwards(speed);
        Serial.println("Initial yellow");
        PT_WAIT_UNTIL(cs.read() == JD::ColorSensor::Yellow);
        // When  it  detects  the  yellow  path it beeps  it  horn  twiceand
        // turns  off  the  green  LED.
        // static_assert(false, "Honk twice");
        // horn.sendSequence(300, 50, 300);
        green.off();
        // When Bot  1 makes the right turn it follows  the  yellow  path.
        // Delay allows bot to actually start on path.
        ds.stop();
        PT_WAIT_UNTIL(timer.hasElapsed(100));
        ds.forwards();
        Serial.println(millis());
        PT_WAIT_UNTIL(timer.hasElapsed(500));
        Serial.println(millis());
        PT_WAIT_UNTIL(((printColor()), cs.read() == JD::ColorSensor::Black));

        Serial.println("Rotating");
        ds.rotate(JD::DriveSystem::RIGHT, speed);

        PT_WAIT_UNTIL(cs.read() == JD::ColorSensor::Yellow);
        Serial.println("On Yellow");

        ds.stop();
        PT_WAIT_UNTIL(timer.hasElapsed(200));
        Serial.println("Follow yellow");
        // static_assert(false, "Follow yellow path until detects
        // pedestrian");
        // ds.turn(JD::DriveSystem::LEFT, speed);
        // PT_WAIT_
        PT_WAIT_WHILE(
            (followEdge(JD::ColorSensor::Yellow, JD::DriveSystem::RIGHT),
             (halleffect.read() == 1)));
        ds.stop();
        PT_WAIT_UNTIL(timer.hasElapsed(1000));
        ds.forwards(speed);
        PT_WAIT_UNTIL(timer.hasElapsed(1500));
        ds.stop();
        PT_WAIT_UNTIL(timer.hasElapsed(200));
        Serial.println("Done following yellow");
        // When Bot 1 detects  the  policeman  (magnetic  field) it  beeps
        // horn twice and makes a 90° left turn.The bot will be judged how
        // tight a 90° leftturn it makes
        // static_assert(false, "Honk twice");
        // horn.sendSequence(2000, 1000, 2000);
        // static_assert(false, "Make a 90 degree left turn");
        ds.stop();
        PT_WAIT_UNTIL(timer.hasElapsed(200));
        ds.rotate(JD::DriveSystem::LEFT, speed);
        PT_WAIT_UNTIL(timer.hasElapsed(timeForXDeg(speed, 90)));
        // // When Bot 1 detects the redpath it illuminates a red LED, makes a
        // // 90° left turn traveling along the redpath until it detects the
        // // pedestrian.
        ds.forwards(speed);
        Serial.println("Looking for red");
        PT_WAIT_UNTIL(cs.read() == JD::ColorSensor::Red);
        Serial.println("Stopping");
        ds.stop();

        PT_WAIT_UNTIL(timer.hasElapsed(100));
        ds.forwards();
        Serial.println(millis());
        PT_WAIT_UNTIL(timer.hasElapsed(500));
        Serial.println(millis());
        PT_WAIT_UNTIL(((printColor()), cs.read() == JD::ColorSensor::Black));

        Serial.println("Rotating");
        ds.rotate(JD::DriveSystem::LEFT, speed);
        PT_WAIT_UNTIL(cs.read() == JD::ColorSensor::Red);

        // static_assert(false, "Follow red path until detects pedestrian");
        ds.stop();
        // PT_WAIT_UNTIL(timer.hasElapsed(200));

        Serial.println("Following red");
        ds.turn(JD::DriveSystem::RIGHT, speed);
        PT_WAIT_WHILE(
            (followEdge(JD::ColorSensor::Red, JD::DriveSystem::RIGHT),
             (halleffect.read() == 1)));
        Serial.println("Found hall sensor");
        ds.stop();
        PT_WAIT_UNTIL(timer.hasElapsed(1000));
        // When it detects the pedestrian (magnetic field), it must
        // flash a green LED, bringingthe bot to a complete and fullstop
        green.blink(1);

        Serial.println("Sending Message");
        // communicate  with  the  TCC  via  a  200  ms  message_1
        speaker.send(200);

        ds.rotate(JD::DriveSystem::RIGHT, speed);
        PT_WAIT_UNTIL(timer.hasElapsed(timeForXDeg(speed, 20)));
        // When  Bot  1  receives  a responding 200 ms message_1 from the
        // TCC,
        // it illuminates its communication LED
        ds.stop();
        Serial.println("Waiting for message");
        PT_WAIT_UNTIL(mic.receivedMsg() == JD::Receiver::msg200);
        communication.on();

        // // and continues along the redpath to the end of the path at the
        // wall.
        // // static_assert(false, "Follow red path until end of path");
        // PT_WAIT_WHILE(
        //               lineFollow(JD::ColorSensor::Red));

        // // When it detects the wall at the end of the redpath, it stops,
        // // blinking its redLED twice.
        // red.blink(2);

        // // Bot 1 rotates 90°to the left, stopping in the vicinity of the end
        // // red path and the spot on the wall where it collided
        // ds.rotateDeg(JD::DriveSystem::LEFT, 90);

        // // comes to a full stop and illuminates blue and red LEDs flashing
        // them
        // // three times
        // ds.stop();
        // blue.blink(3);
        // red.blink(3);

        // // Bot 1 then  signals  to  the  TCC  with  a  300  ms  unit  step
        // // function  “message_2”.
        // speaker.send(300);

        // //  Bot  2  signals a  500  ms message to Bot 1 that is it is
        // starting
        // //  to follow itspath.  Appropriate LEDs are flashed to indicate
        // this
        // //  exchange.
        // PT_WAIT_UNTIL(mic.receivedMsg() == JD::Receiver::msg500);

        // // Bot 2 signals Bot 1 with a 500 ms unit step function “message_4”
        // to
        // // start traversing an unmarked path in the black track,  toward
        // // eachotherfor  a  gentle  collision.
        // PT_WAIT_UNTIL(mic.receivedMsg() == JD::Receiver::msg500);
        // ds.forwards(0.4);

        // When  they  collide,  the  bots flash  their headlights, braking
        // lights,beep their hornsand turn signals ten times to indicate the
        // end of the challenge. The LEDs must finish flashing to indicate the
        // end of the test within the 90-second  time  limit.
        // static_assert(false, "Flash lights and horns 10 times");
        // horn.sendSequence(100, 50, 100, 50, 100, 50, 100, 50, 100, 50, 100,
        // 50,
        //   100, 50, 100, 50, 100, 50, 100, 50);

        PT_END();
}

bool followEdge(JD::ColorSensor::Color c, JD::DriveSystem::Direction edge) {
        const static unsigned wiggleFreq = 20;
        constexpr double      speed      = 0.5;
        static unsigned long  lastSwitch = millis();
        static unsigned long  weLostIt   = 3000;
        unsigned long         currTime   = millis();
        static bool           prevColor  = (cs.read() == c);
        bool                  onColor    = (cs.read() == c);
        using DS                         = JD::DriveSystem;

        if (onColor && currTime - lastSwitch > wiggleFreq) {
                // Serial.println("Blue to Black");
                ds.stop();
                ds.turn(DS::Direction(DS::LEFT ^ edge), speed);
                lastSwitch = currTime;
                prevColor  = onColor;
        } else if (!onColor && currTime - lastSwitch > wiggleFreq) {
                // Serial.println("Black to Blue");
                ds.stop();
                ds.turn(DS::Direction(DS::RIGHT ^ edge), speed);
                lastSwitch = currTime;
                prevColor  = onColor;
        } else if (currTime - lastSwitch > weLostIt) {
                // Serial.println("How");
                return false;
        }

        // Serial.println("In funct");
        return true;
}
