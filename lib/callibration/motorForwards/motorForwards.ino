#include "src/Calibration.h"
#include "src/ColorSensor.h"
#include "src/DriveSystem.h"
#include "src/Protothread.h"
#include "src/Updateable.h"
JD::DriveSystem ds(-1, -1, -1, -1);
JD::ColorSensor cs(-1, -1, -1);
JD::Updateable* peripherals[] = {&ds, &cs};

constexpr unsigned testDistance_in = 10;

void setup() {}

/**
 * @brief Test fixture is a straight board, with a 10in section delimited by
 *        blue and red markings
 *
 */
void loop() {
        for (auto p : peripherals)
                p->update;

        double currSpeed = 1, rangeLow = 0, rangeHigh = 1,
               targetSpeed_in_s = 1.25;
        unsigned long start = 0, end = 0,
                      target_ms =
                          (double)testDistance_in * 1000 / targetSpeed_in_s;

        PT_BEGIN();
        PT_WAIT_UNTIL(cs.read() == JD::ColorSensor::Blue);
        ds.forwards(currSpeed);
        PT_WAIT_UNTIL(cs.read() != JD::ColorSensor::Blue);
        start = millis();
        PT_WAIT_UNTIL(cs.read() == JD::ColorSensor::Red);
        end = millis();

        unsigned long currTime = end - start;

        if (0.9 * target_ms <= currTime && currTime <= 1.1 * target_ms) {
                JD::Calibration::set(JD::Calibration::driveAt_1_25_inch_s,
                                     currSpeed);
                exit(EXIT_SUCCESS);
        } else if (currTime > target_ms)
                rangeHigh = currSpeed;
        else if (currTime < target_ms)
                rangeLow = currSpeed;

        currSpeed = (rangeLow + rangeHigh) / 2;
        PT_RESTART();

        PT_END();
}