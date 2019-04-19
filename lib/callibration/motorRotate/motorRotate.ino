#include "src/Calibration.h"
#include "src/ColorSensor.h"
#include "src/DriveSystem.h"
#include "src/Protothread.h"
#include "src/Switch.h"
#include "src/Updateable.h"
constexpr unsigned long rampUpTime_ms = 200;
JD::DriveSystem         ds(10, 8, 11, 9, rampUpTime_ms);
constexpr uint8_t       RED_LED  = 36;
constexpr uint8_t       BLUE_LED = 37;
JD::ColorSensor         cs(RED_LED, BLUE_LED, A15);
JD::Switch<1>           trigger(42);
// JD::Switch<1>           trigger(42);
JD::Updateable*         peripherals[]           = {&ds, &cs, &trigger};
constexpr unsigned      testAngularDistance_deg = 90;

void setup() {
        Serial.begin(115200);
}
template <class T>
void swap(T& a, T& b) {
        T temp = a;
        a      = b;
        b      = temp;
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

        static JD::ColorSensor::Color startColor = JD::ColorSensor::Blue,
                                      endColor   = JD::ColorSensor::Red;
        static unsigned long currGuess = 300, rangeHigh = currGuess * 2,
                             rangeLow = 0, startedAt = 0;
        static bool   started = false;
        unsigned long startEND, endEND;

        if (started && millis() - startedAt > currGuess) {
                Serial.print("Stopping ");
                Serial.println(millis());
                ds.stop();
                started = false;
                // delay(10000);
        }
        // Serial.print("Color is ");
        // switch (cs.read()) {
        //         case JD::ColorSensor::Red: Serial.print("Red."); break;
        //         case JD::ColorSensor::Blue: Serial.print("Blue."); break;
        //         case JD::ColorSensor::Black: Serial.print("Black."); break;
        //         case JD::ColorSensor::Yellow: Serial.print("Yellow.");
        //         break; default: Serial.print("Unknown: "); break;
        // }
        PT_BEGIN();
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

        if (ds.getSpeed() == 0) {
                // If bot has stopped before ending color
                // Update guess and try again

                Serial.println("Not moving");
                if (cs.read() != endColor) {
                        Serial.println("Not red");
                        rangeLow  = currGuess;
                        currGuess = (rangeLow + rangeHigh) / 2;
                        started   = false;
                        Serial.print(rangeLow);
                        Serial.print(' ');
                        Serial.print(currGuess);
                        Serial.print(' ');
                        Serial.println(rangeHigh);
                        swap(startColor, endColor);
                        PT_RESTART();
                        return;
                }
        } else if (cs.read() == endColor) {
                // Start counting time since bot reached end zone
                Serial.println("Ending timer");
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
                Serial.print(endEND - startEND);
                Serial.print(' ');
                Serial.print(rangeLow);
                Serial.print(' ');
                Serial.print(currGuess);
                Serial.print(' ');
                Serial.println(rangeHigh);

                started = false;
                swap(startColor, endColor);
                PT_RESTART();
                return;
        }

        JD::Calibration::set(JD::Calibration::rotate90AtFull_ms, currGuess);
        PT_END();
}