
#include <Arduino.h>
#include <pins_arduino.h>
#include <stdio.h>
#include "src/Calibration.h"
#include "src/ColorSensor.h"
#include "src/Led.h"
#include "src/Protothread.h"
#include "src/Timer.h"

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
        for (auto p : peripherals)
                p->update();

        long unsigned wait_for = 5e3;
        // https://en.wikipedia.org/wiki/Duff%27s_device
        // https://en.wikipedia.org/wiki/Protothread
        // Serial.println(analogRead(A15));
        // printVec(cs.raw());
        // Serial.println(cs.raw().norm());
        // return;

        PT_BEGIN();
        Serial.println(millis());
        Serial.println("Black");
        // indicator.blink(1, 1e7);
        timer.start(wait_for);
        PT_WAIT_UNTIL(timer.isFinished());
        JD::Calibration::set(JD::Calibration::ColorBlack, cs.raw());
        printVec(cs.raw());
        // indicator.blink(2, 1e7);
        Serial.println(millis());
        Serial.println("Yellow");
        timer.start(wait_for);
        PT_WAIT_UNTIL(timer.isFinished());
        JD::Calibration::set(JD::Calibration::ColorYellow, cs.raw());
        printVec(cs.raw());
        // indicator.blink(3, 1e7);
        Serial.println(millis());
        Serial.println("Red");
        timer.start(wait_for);
        PT_WAIT_UNTIL(timer.isFinished());
        JD::Calibration::set(JD::Calibration::ColorRed, cs.raw());
        printVec(cs.raw());
        // indicator.blink(4, 1e7);
        Serial.println(millis());
        Serial.println("Blue");
        timer.start(wait_for);
        PT_WAIT_UNTIL(timer.isFinished());
        JD::Calibration::set(JD::Calibration::ColorBlue, cs.raw());
        printVec(cs.raw());
        // indicator.blink(5, 1e7);

        Serial.println(millis());
        PT_END();

        Serial.println("exiting");
        Serial.flush();
        exit(0);
}