#include "src/Calibration.h"
#include "src/ColorSensor.h"
#include "src/DriveSystem.h"
#include "src/Protothread.h"
#include "src/Updateable.h"
JD::DriveSystem ds(-1, -1, -1, -1);
JD::ColorSensor cs(-1, -1, -1);
JD::Updateable* peripherals[] = {&ds, &cs};

constexpr unsigned testAngularDistance_deg = 90;

void setup() {}

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

        JD::ColorSensor::Color startColor = JD::ColorSensor::Blue,
                               endColor   = JD::ColorSensor::Red;
        unsigned long currGuess = 0 - 1, rangeHigh = 0 - 1, rangeLow = 0;
        unsigned long startedAt = 0, startEND, endEND;

        if (millis() - startedAt > currGuess)
                ds.stop();

        PT_BEGIN();
        // Start turning on startColor
        PT_WAIT_UNTIL(cs.read() == startColor);
        ds.rotate(JD::DriveSystem::RIGHT);

        // Start test once bot leaves start color
        PT_WAIT_UNTIL(cs.read() != startColor);
        startedAt = millis();

        // Wait until bot stops or reaches ending color
        PT_WAIT_UNTIL(cs.read() == endColor || ds.getSpeed() == 0);
        
        if (ds.getSpeed() == 0) {
                // If bot has stopped before ending color
                // Update guess and try again
                if (cs.read() != endColor) {
                        rangeLow  = currGuess;
                        currGuess = (rangeLow + rangeHigh) / 2;
                        PT_RESTART();
                        return;
                }
        } else if (cs.read() == endColor) {
                // Start counting time since bot reached end zone
                startEND = millis();
                ds.stop();
                PT_WAIT_UNTIL(ds.getSpeed() == 0);
        }
        // End timer when on end zone and stopped
        endEND = millis();

        // If bot isn't stopped on color within a certain threshold
        // Update guess and try again
        if (endEND - startEND > 100) {
                rangeHigh = currGuess;
                currGuess = (rangeLow + rangeHigh) / 2;
                PT_RESTART();
                return;
        }

        JD::Calibration::set(JD::Calibration::rotate90AtFull_ms, currGuess);
        PT_END();
}