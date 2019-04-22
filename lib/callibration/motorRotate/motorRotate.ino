#include <limits.h>
#include "src/Calibration.h"
#include "src/ColorSensor.h"
#include "src/DriveSystem.h"
#include "src/Protothread.h"
#include "src/Switch.h"
#include "src/Timer.h"
#include "src/Updateable.h"

constexpr unsigned long rampUpTime_ms = 20;
constexpr uint8_t       RED_LED       = 36;
constexpr uint8_t       BLUE_LED      = 37;

JD::DriveSystem ds(10, 8, 11, 9, rampUpTime_ms);
JD::ColorSensor cs(RED_LED, BLUE_LED, A15);
JD::Switch<1>   trigger(42);
JD::Updateable* peripherals[] = {&ds, &cs, &trigger};
unsigned long   turnTime;
using Color = JD::ColorSensor::Color;
void setup() {
        Serial.begin(115200);
        // JD::Calibration::get(JD::Calibration::rotate90AtFull_ms, turnTime);
        // Serial.println(turnTime);
}

void getNextColors(Color& start, Color& end) {}
void getTime(JD::PT_lc_t& _lc, Color startColor, Color endColor,
             unsigned long currGuess, long* result) {
        constexpr unsigned long inf       = ULONG_MAX;
        static unsigned long    startedAt = 0;
        static JD::Timer        timer;
        static bool             started = false;
        static unsigned long    startEND, endEND;
        constexpr double        resetSpeed = 0.24;
        // constexpr double        resetSpeed = 0.3;

        if (started && millis() - startedAt > currGuess) {
                Serial.print("Stopping ");
                Serial.println(millis());
                ds.stop();
                started = false;
        }

        PT_BEGIN_LC_ALREADY_DECLARED();
        Serial.print("currGuess ");
        Serial.println(currGuess);
        started = false;
        // Start turning on startColor
        PT_WAIT_UNTIL(cs.read() == startColor && ds.getSpeed() == 0);
        // PT_WAIT_UNTIL(trigger.read() == 0);

        Serial.println("Starting");
        // Turn slowly until on border of next color
        ds.rotate(JD::DriveSystem::RIGHT, resetSpeed);
        // Start test once bot leaves start color
        PT_WAIT_UNTIL(cs.read() != startColor);
        ds.stop();
        PT_WAIT_UNTIL(ds.getSpeed() == 0);
        timer.start(100);
        PT_WAIT_UNTIL(timer.isFinished());
        Serial.print("Starting timer: ");
        Serial.println(millis());
        startedAt = millis();
        started   = true;
        ds.rotate(JD::DriveSystem::RIGHT);
        PT_WAIT_UNTIL(ds.getSpeed() != 0);
        // Wait until bot stops or reaches ending color
        PT_WAIT_UNTIL(cs.read() == endColor || ds.getSpeed() == 0);

        // If bot has stopped before ending color
        // Update guess and try again
        if (ds.getSpeed() == 0 && cs.read() != endColor) {
                Serial.println("Not moving and not end");
                started = false;
                *result = -(millis() - startedAt);
                Serial.println("Resetting");
                ds.rotate(JD::DriveSystem::RIGHT, resetSpeed);
                PT_WAIT_UNTIL(cs.read() == endColor);
                ds.stop();
                Serial.println("Waiting to stop");
                PT_WAIT_UNTIL(ds.getSpeed() == 0);
                Serial.println("Done waiting to stop");
                timer.start(250);
                PT_WAIT_UNTIL(timer.isFinished());
                PT_STOP();
                return;
        }
        if (cs.read() == endColor) {
                // Start counting time since bot reached end zone
                Serial.println("Ending timer");
                startEND = millis();
                ds.stop();
                PT_WAIT_UNTIL(ds.getSpeed() == 0);
        }
        // End timer when on end zone and stopped
        endEND  = millis();
        *result = endEND - startEND;
        started = false;
        timer.start(250);
        PT_WAIT_UNTIL(timer.isFinished());
        PT_END();
}

/**
 * @brief Test fixture is a square board, with a red quadrant, blue quadrant
 *        and two white quadrants
 *
 *        Bot will binary search amount of time needed to turn exactly 90
 *        degrees;
 */

// #define NORMAL
// #define NEWNORMAL
void loop() {
#ifdef NORMAL

        for (auto p : peripherals)
                p->update();

        constexpr Color colors[] = {
            JD::ColorSensor::Blue, JD::ColorSensor::Black,
            JD::ColorSensor::Red, JD::ColorSensor::Black};
        constexpr unsigned numColors = sizeof(colors) / sizeof(colors[0]);
        // Golden ratio = (sqrt(5) - 1) / 2 = 0.61803398875
        constexpr double golden_ratio = 0.61803398875;

        static double        c, d;
        static unsigned long fc, fd;
        static double        a = 0, b = 2000;
        constexpr double     error_margin_ms = 2;
        static auto          thread          = JD::makeProtothread(getTime);
        // Golden Section Search
        PT_BEGIN();
        c = b - golden_ratio * (b - a);
        d = a + golden_ratio * (b - a);

        while (abs(c - d) > error_margin_ms) {
                Serial.println("c");
                PT_WAIT_WHILE(thread(colors[0], colors[2], c, &fc));
                Serial.println("restart");
                thread.restart();
                Serial.println("\nd");
                PT_WAIT_WHILE(thread(colors[2], colors[0], d, &fd));
                thread.restart();
                Serial.println("Done");
                if (fc < fd) {
                        b = d;
                        d = c;
                        c = b - golden_ratio * (b - a);
                } else {
                        a = c;
                        c = d;
                        d = a + golden_ratio * (b - a);
                }
                Serial.print(c);
                Serial.print(' ');
                Serial.print(d);
                Serial.print(' ');
                Serial.print(error_margin_ms);
                Serial.print(" | abs(c - d) > error_margin_ms: ");
                Serial.print((abs(c - d) > error_margin_ms) ? "True"
                                                            : "False");
                Serial.print('\n');
        }
        Serial.println("end");
        ds.stop();
        Serial.flush();
        // exit(0);
        JD::Calibration::set(JD::Calibration::rotate90AtFull_ms,
                             static_cast<unsigned long>((b + a) / 2));
        static JD::Timer timer;
        Serial.println(static_cast<unsigned long>((b + a) / 2));
        delay(10000);
        ds.rotateDeg(JD::DriveSystem::RIGHT);
        timer.start((b + a) / 2 + 20);
        PT_WAIT_UNTIL(timer.isFinished());
        ds.rotateDeg(JD::DriveSystem::RIGHT);
        timer.start((b + a) / 2 + 20);
        PT_WAIT_UNTIL(timer.isFinished());
        ds.rotateDeg(JD::DriveSystem::RIGHT);
        timer.start((b + a) / 2 + 20);
        PT_WAIT_UNTIL(timer.isFinished());
        ds.rotateDeg(JD::DriveSystem::RIGHT);
        timer.start((b + a) / 2 + 20);
        PT_WAIT_UNTIL(timer.isFinished());
        PT_END();

#else
#ifdef NEWNORMAL
        for (auto p : peripherals)
                p->update();
        static unsigned long lo = 0, hi = 5000, currColor = 0,
                             tolerance_ms = 22;
        static long result                = ULONG_MAX;
        static auto thread                = JD::makeProtothread(getTime);

        constexpr Color colors[] = {
            JD::ColorSensor::Blue, JD::ColorSensor::Black,
            JD::ColorSensor::Red, JD::ColorSensor::Black};
        constexpr unsigned long numColors = sizeof(colors) / sizeof(colors[0]);
        PT_BEGIN();
        while (not(0 <= result && result <= tolerance_ms)) {
                Serial.print("lo: ");
                Serial.print(lo);
                Serial.print(" hi: ");
                Serial.print(hi);
                Serial.print("\n");
                PT_WAIT_WHILE(thread(colors[currColor],
                                     colors[(currColor + 2) % numColors],
                                     (lo + hi) / 2, &result));
                thread.restart();
                if (result < 0) {
                        lo = (lo + hi) / 2;
                } else {
                        hi = (lo + hi) / 2;
                }
                currColor = (currColor + 2) % numColors;
                Serial.print("not(0 <= result && result <= tolerance_ms): ");
                Serial.print((not(0 <= result && result <= tolerance_ms))
                                 ? "True"
                                 : "False");
                Serial.print(", where result is ");
                Serial.print(result);
                Serial.print('\n');

                Serial.print('\n');
        }
        Serial.print("Done\n");
        JD::Calibration::set(JD::Calibration::rotate90AtFull_ms,
                             (lo + hi) / 2);
        exit(1);
        PT_END();

#else

        for (auto p : peripherals)
                p->update();

        static JD::Timer timer;
        PT_BEGIN();
        // JD::Calibration::get(JD::Calibration::rotate90AtFull_ms, turnTime);
        turnTime = 375;
        JD::Calibration::set(JD::Calibration::rotate90AtFull_ms, turnTime);
        Serial.println(turnTime);
        timer.start(turnTime);
        ds.rotate(JD::DriveSystem::RIGHT);
        PT_WAIT_UNTIL(timer.isFinished());
        ds.stop();
        PT_END();
#endif
#endif
}