
#include <Arduino.h>
#include <pins_arduino.h>
#include <stdio.h>
#include "src/Calibration.h"
#include "src/ColorSensor.h"
#include "src/Led.h"
#include "src/Protothread.h"
#include "src/Timer.h"
#include "src/Filter.h"
JD::LED           indicator(LED_BUILTIN);
constexpr uint8_t RED_LED  = 36;
constexpr uint8_t BLUE_LED = 37;
JD::ColorSensor   cs(RED_LED, BLUE_LED, A15, 1, 1);
// JD::Sensor        hall(A14);

// JD::Updateable* peripherals[] = {&cs, &indicator};
JD::Updateable* peripherals[] = {&cs};

void setup() {
        Serial.begin(115200);
        Serial.println(millis());
        analogWrite(RED_LED, 102);
}

void loop() {
        static JD::Timer timer;
        static ExponentialFilter<JD::ColorSensor::RawData> blue(1, cs.raw());
        static ExponentialFilter<JD::ColorSensor::RawData> red(1, cs.raw());
        static ExponentialFilter<JD::ColorSensor::RawData> yellow(1, cs.raw());
        static ExponentialFilter<JD::ColorSensor::RawData> black(1, cs.raw());
        for (auto p : peripherals)
                p->update();

        long unsigned wait_for = 5e3;

        PT_BEGIN();
        Serial.println(millis());
        Serial.println("Black");
        timer.start(wait_for);
        PT_WAIT_UNTIL((black.update(cs.raw()),timer.isFinished()));
        // JD::Calibration::set(JD::Calibration::ColorBlack, cs.raw());
        JD::Calibration::set(JD::Calibration::ColorBlack, black.current());
        printVec(cs.raw());

        Serial.println(millis());
        Serial.println("Yellow");
        timer.start(wait_for);
        PT_WAIT_UNTIL((yellow.update(cs.raw()),timer.isFinished()));
        JD::Calibration::set(JD::Calibration::ColorYellow, yellow.current());
        printVec(cs.raw());

        Serial.println(millis());
        Serial.println("Red");
        timer.start(wait_for);
        PT_WAIT_UNTIL((red.update(cs.raw()),timer.isFinished()));
        JD::Calibration::set(JD::Calibration::ColorRed, red.current());
        printVec(cs.raw());

        Serial.println(millis());
        Serial.println("Blue");
        timer.start(wait_for);
        PT_WAIT_UNTIL((blue.update(cs.raw()),timer.isFinished()));
        JD::Calibration::set(JD::Calibration::ColorBlue, blue.current());
        printVec(cs.raw());
        Serial.println(millis());

        PT_END();
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
                Serial.print(cs.error(curr));
                old = curr;
                Serial.println();
        }

}