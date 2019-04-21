#include <float.h>
#include "src/Calibration.h"
#include "src/ColorSensor.h"
#include "src/DriveSystem.h"
#include "src/Protothread.h"
#include "src/Switch.h"
#include "src/Updateable.h"

constexpr unsigned long rampUpTime_ms = 0;
constexpr uint8_t       RED_LED       = 36;
constexpr uint8_t       BLUE_LED      = 37;

JD::DriveSystem ds(10, 8, 11, 9, rampUpTime_ms);
JD::ColorSensor cs(RED_LED, BLUE_LED, A15);
JD::Switch<1>   trigger(42);
JD::Updateable* peripherals[] = {&ds, &cs, &trigger};

using Color = JD::ColorSensor::Color;

void setup() {
        Serial.begin(115200);
}

void getTime(JD::PT_lc_t& _lc, Color startColor, Color endColor,
             unsigned long currGuess, unsigned long* result) {
        constexpr double     inf       = DBL_MAX;
        static unsigned long startedAt = 0;
        static bool          started   = false;
        unsigned long        startEND, endEND;

        if (started && millis() - startedAt > currGuess) {
                Serial.print("Stopping ");
                Serial.println(millis());
                ds.stop();
                started = false;
        }

        PT_BEGIN_LC_ALREADY_DECLARED();
        started = false;
        // Start turning on startColor
        PT_WAIT_UNTIL(cs.read() == startColor);
        // PT_WAIT_UNTIL(trigger.read() == 0);

        Serial.println("Starting");
        ds.rotate(JD::DriveSystem::RIGHT);
        // Start test once bot leaves start color
        PT_WAIT_UNTIL(cs.read() != startColor);

        Serial.print("Starting timer: ");
        Serial.println(millis());
        startedAt = millis();
        started   = true;

        // Wait until bot stops or reaches ending color
        PT_WAIT_UNTIL(cs.read() == endColor || ds.getSpeed() == 0);

        // If bot has stopped before ending color
        // Update guess and try again
        if (ds.getSpeed() == 0 && cs.read() != endColor) {
                Serial.println("Not moving and not end");
                started = false;
                PT_STOP();
                *result = inf;
                // Need to return something
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
        endEND = millis();
        *result = endEND - startEND;
        started = false;
        PT_END();
}

/**
 * @brief Test fixture is a square board, with a red quadrant, blue quadrant
 *        and two white quadrants
 *
 *        Bot will binary search amount of time needed to turn exactly 90
 *        degrees;
 */
void loop() {
        for (auto p : peripherals)
                p->update();

        constexpr Color colors[] = {
            JD::ColorSensor::Blue, JD::ColorSensor::Black,
            JD::ColorSensor::Red, JD::ColorSensor::Black};
        constexpr unsigned numColors = sizeof(colors) / sizeof(colors[0]);
        // Golden ratio = (sqrt(5) - 1) / 2 = 0.61803398875
        constexpr double golden_ratio = 0.61803398875;

        static double           c, d;
        static unsigned long    fc, fd;
        static double           a = 0, b = 55;
        constexpr unsigned long error_margin_ms = 10;
        static auto             thread          = JD::makeProtothread(getTime);
        // Golden Section Search
        PT_BEGIN();
        c = b - golden_ratio * (b - a);
        d = a + golden_ratio * (b - a);

        while (abs(c - d) > error_margin_ms) {
                PT_WAIT_WHILE(thread(colors[0], colors[1], c, &fc));
                thread.restart();
                PT_WAIT_WHILE(thread(colors[0], colors[1], d, &fd));
                if (fc < fd) {
                        b = d;
                        d = c;
                        c = b - golden_ratio * (b - a);
                } else {
                        a = c;
                        c = d;
                        d = a + golden_ratio * (b - a);
                }
        }
        JD::Calibration::set(JD::Calibration::rotate90AtFull_ms, (b + a) / 2);
        PT_END();
}