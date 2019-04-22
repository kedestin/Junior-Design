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
JD::Horn          horn(3);
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

        // return;
        // static unsigned old  = halleffect.read();
        // unsigned        curr = halleffect.read();
        // if (old != curr) {
        //         old = curr;
        //         Serial.println(curr);
        // }
        // lineFollow(JD::ColorSensor::Blue);

        // return;
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
        constexpr double speed = 0.5;
        static JD::Timer timer;
        PT_BEGIN();
        // Serial.println(_lc);

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
        ds.forwards(speed);

        // The  bot  mimics  the  white  dotted  line until it collides with
        // the wall of the test track.
        Serial.println("Waiting until collision");
        PT_WAIT_UNTIL(bumper.read() != JD::Bumper::Collision::None);
        Serial.println("Hit something");
        // Rebounds from the wall
        ds.stop();
        timer.start(200);
        PT_WAIT_UNTIL(timer.isFinished());
        ds.backwards(speed);
        timer.start(750);
        PT_WAIT_UNTIL(timer.isFinished());

        ds.stop();
        timer.start(200);
        PT_WAIT_UNTIL(timer.isFinished());
        Serial.println("Rotating");
        // static_assert(false, "Must make a turn of X degrees off wall");
        ds.rotate(JD::DriveSystem::RIGHT);
        timer.start(375 * 25.0 / 90.0);
        PT_WAIT_UNTIL(timer.isFinished());

        // Then moves to find the bluepath on the far side
        ds.forwards(speed);
        PT_WAIT_UNTIL(cs.read() == JD::ColorSensor::Blue);

        // When it does so, Bot 1 illuminates a blueLED.
        blue.on();

        // Bot 1 must follow the bluepath until it detects the
        // pedestrian(magnetic field).
        // static_assert(false, "Must make an X Degree turn onto line");

        // static_assert(false, "Must linefollow on blue until pedestrian");
        Serial.println("Follow blue");
        PT_WAIT_WHILE((printColor(), (halleffect.read() == 1 &&
                                      lineFollow(JD::ColorSensor::Blue))));
        Serial.println("Done following blue");
        // Here it turns off  the blue LED  and  illuminates  a green LED
        blue.off();
        green.on();

        ds.forwards(speed);
        timer.start(500);
        PT_WAIT_UNTIL(timer.isFinished());
        // Bot  1  makes  a 90° right turn. The  bot will  be  judged  how
        // tight a  90° right turn  it  makes
        ds.rotate(JD::DriveSystem::RIGHT);
        timer.start(400);
        PT_WAIT_UNTIL(timer.isFinished());
        ds.stop();

        ds.forwards(speed);
        PT_WAIT_UNTIL(cs.read() == JD::ColorSensor::Yellow);
        // When  it  detects  the  yellow  path it beeps  it  horn  twiceand
        // turns  off  the  green  LED.
        // static_assert(false, "Honk twice");
        horn.sendSequence(300, 50, 300);
        green.off();
        // When Bot  1 makes the right turn it follows  the  yellow  path.

        timer.start(400);
        PT_WAIT_UNTIL(timer.isFinished());
        ds.rotate(JD::DriveSystem::RIGHT);
        timer.start(400);
        PT_WAIT_UNTIL(timer.isFinished());
        Serial.println("Follow yellow");
        // static_assert(false, "Follow yellow path until detects
        // pedestrian");
        PT_WAIT_WHILE(halleffect.read() == 1 &&
                      lineFollow(JD::ColorSensor::Yellow));
        Serial.println("Done following yellow");
        ds.stop();
        // // When Bot 1 detects  the  policeman  (magnetic  field) it  beeps
        // it
        // // horn twice and makes a 90° left turn.The bot will be judged how
        // // tight a 90° leftturn it makes
        // // static_assert(false, "Honk twice");
        // // horn.sendSequence(100, 20, 100);
        // // static_assert(false, "Make a 90 degree left turn");
        // ds.rotateDeg(JD::DriveSystem::LEFT, 90);

        // // When Bot 1 detects the redpath it illuminates a red LED, makes a
        // // 90° left turn traveling along the redpath until it detects the
        // // pedestrian.
        // ds.forwards(speed);
        // PT_WAIT_UNTIL(cs.read() == JD::ColorSensor::Yellow);
        // ds.rotateDeg(JD::DriveSystem::LEFT, 90);
        // // static_assert(false, "Follow red path until detects pedestrian");
        // PT_WAIT_WHILE(halleffect.read() == 1 &&
        //               lineFollow(JD::ColorSensor::Red));

        // // When it detects the pedestrian (magnetic field), it must
        // // flash a green LED, bringingthe bot to a complete and fullstop
        // green.blink(1);
        // ds.stop();

        // // communicate  with  the  TCC  via  a  200  ms  message_1
        // speaker.send(200);

        // // When  Bot  1  receives  a responding 200 ms message_1 from the
        // TCC,
        // // it illuminates its communication LED
        // PT_WAIT_UNTIL(mic.receivedMsg() == JD::Receiver::msg200);
        // communication.on();

        // // and continues along the redpath to the end of the path at the
        // wall.
        // // static_assert(false, "Follow red path until end of path");
        // timer.start(2000);
        // PT_WAIT_WHILE(not timer.isFinished() &&
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

template <class T>
const T& minimum(const T& a) {
        return a;
}
template <class T, class... Args>
const T& minimum(const T& a, const T& b, const Args&... args) {
        return minimum(b < a ? b : a, args...);
}

template <class... T>
bool lineFollow(T... args) {
        constexpr double ki = 0;
        constexpr double kp = .8;
        constexpr double kd = 1;
        // constexpr double ki = 0.5;
        // constexpr double kp = 0.5;
        // constexpr double kd = 0.5;

        static unsigned long lastTime           = 0;
        static double        outputSum          = 0;
        static double        output             = 0.0;
        static double        lastInput          = 0;
        static double        lastOutput         = 0;
        static double        lastlastOutput     = 0;
        static double        lastlastlastOutput = 0;
        unsigned long        now                = millis();
        unsigned long        sampleTime         = 0;
        unsigned long        timeChange         = (now - lastTime);
        double               setpoint           = 7;
        double               outMin             = setpoint;
        double               outMax             = 1e3;

        if (timeChange >= sampleTime) {
                double input = minimum(cs.error(args)...);
                // Serial.println(input);
                double error  = setpoint - input;
                double dInput = (input - lastInput);
                // Serial.println(dInput);
                outputSum += (ki * error);
                outputSum -= kp * dInput;
                // Serial.println(outputSum);

                if (outputSum > outMax)
                        outputSum = outMax;
                else if (outputSum < outMin)
                        outputSum = outMin;

                output = kp * error;

                output += outputSum - kd * dInput;
                // Serial.println(output);

                // if (output > outMax)
                //         output = outMax;
                // else if (output < outMin)
                //         output = outMin;

                lastInput = input;
                lastTime  = now;
        }

        double        speed        = 0.3;
        static bool   left         = false;
        static double timenegative = 0;
        static double multiplier   = 1;
        if (output < 0) {
                double outer = speed - output / 900;
                // double inner = speed + output / 300;
                // Serial.println(outputs);
                // double outer = 1;
                if (timenegative == 0.0)
                        timenegative = millis();

                // Serial.println(millis() - timenegative);

                // If color can't be found for x amount of time, abort
                if ((millis() - timenegative) > 400)
                        // if (multiplier > 1 << 3)
                        return false;

                if (millis() - timenegative >
                    (multiplier * 200 + 200 * (multiplier - 1))) {
                        timenegative = 0;
                        multiplier *= 2;
                        left = !left;
                }

                if (outer < 0.2)
                        outer = 0.2;

                if (left)
                        ds.rotate(JD::DriveSystem::LEFT, outer);
                else
                        ds.rotate(JD::DriveSystem::RIGHT, outer);
        } else {
                timenegative = 0;
                multiplier   = 1;
                ds.forwards(speed);
        }
        lastOutput         = output;
        lastlastOutput     = lastOutput;
        lastlastlastOutput = lastlastOutput;
        return true;
}
