#include "src/Calibration.h"
#include "src/ColorSensor.h"
#include "src/DriveSystem.h"
#include "src/Protothread.h"
#include "src/Updateable.h"
constexpr unsigned long rampUpTime_ms = 20;
constexpr uint8_t       RED_LED       = 36;
constexpr uint8_t       BLUE_LED      = 37;

JD::DriveSystem ds({10, 8, 255, 255}, {11, 9, 255, 255}, rampUpTime_ms);
JD::ColorSensor cs(RED_LED, BLUE_LED, A15);
JD::Updateable* peripherals[] = {&ds, &cs};

constexpr unsigned      testDistance_in  = 10;
constexpr double        targetSpeed_in_s = 1.25;
constexpr unsigned long target_ms =
    (double)testDistance_in * 1000 / targetSpeed_in_s;
double ssss;
void setup() {
        Serial.begin(115200);
        JD::Calibration::get(JD::Calibration::driveAt_1_25_inch_s, ssss);
        Serial.println(ssss);
        ds.forwards(.21);
        while(1)
                ds.update();
}

/**
 * @brief Test fixture is a straight board, with a 10in section delimited by
 *        blue and red markings
 *
 */
void loop() {
        return;
        for (auto p : peripherals)
                p->update();

        // Verify calibration
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
        static double        currSpeed = 1, rangeLow = 0, rangeHigh = 1;
        static unsigned long start = 0, end = 0;
        unsigned long        currTime;
        PT_BEGIN();
        PT_WAIT_UNTIL(cs.read() == JD::ColorSensor::Blue);
        ds.forwards(currSpeed);
        PT_WAIT_UNTIL(cs.read() != JD::ColorSensor::Blue);
        start = millis();
        PT_WAIT_UNTIL(cs.read() == JD::ColorSensor::Red);
        end = millis();
        ds.stop();
        currTime = end - start;

        if (0.9 * target_ms <= currTime && currTime <= 1.1 * target_ms) {
                JD::Calibration::set(JD::Calibration::driveAt_1_25_inch_s,
                                     currSpeed);
                exit(EXIT_SUCCESS);
        } else if (currTime > target_ms)
                rangeLow = currSpeed;
        else if (currTime < target_ms)
                rangeHigh = currSpeed;

        currSpeed = (rangeLow + rangeHigh) / 2;
        Serial.print(rangeLow);
        Serial.print(' ');
        Serial.print(currSpeed);
        Serial.print(' ');
        Serial.print(rangeHigh);
        Serial.print('\n');
        Serial.print(target_ms);
        Serial.print(' ');
        Serial.print(currTime);
        Serial.print('\n');
        PT_RESTART();

        PT_END();
}