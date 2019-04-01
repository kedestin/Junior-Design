
#include <Arduino.h>
#include <pins_arduino.h>
#include <stdio.h>
#include "src/Calibration.h"
#include "src/ColorSensor.h"
#include "src/Led.h"

JD::LED indicator(LED_BUILTIN);

JD::ColorSensor cs(53, 51, A15);
JD::Sensor      hall(A14);

JD::Updateable* peripherals[] = {&cs, &hall, &indicator};


void setup() {
        Serial.begin(115200);
        Serial.println(millis());
}

void loop() {
        static short    lc = 0;
        static long int wait_until;
        for (auto p : peripherals)
                p->update();

        long unsigned wait_for = 5e3;
        // https://en.wikipedia.org/wiki/Duff%27s_device
        // https://en.wikipedia.org/wiki/Protothread
        // Serial.println(analogRead(A15));
        // printVec(cs.raw());
        // Serial.println(cs.raw().norm());
        // return;
        switch (lc) {
                case 0:
                case __LINE__: lc         = __LINE__;
                        wait_until = millis() + wait_for;
                        indicator.blink(1, 1e7);
                        Serial.println(millis());
                        Serial.println("Black");
                case __LINE__: lc = __LINE__;
                        if (millis() < wait_until)
                                return;
                        JD::Calibration::set(JD::Calibration::ColorBlack,
                                             cs.raw());
                        printVec(cs.raw());
                        wait_until = millis() + wait_for;
                        indicator.blink(2, 1e7);
                        Serial.println(millis());
                        Serial.println("Yellow");
                case __LINE__: lc = __LINE__;
                        if (millis() < wait_until)
                                return;
                        JD::Calibration::set(JD::Calibration::ColorYellow,
                                             cs.raw());
                        printVec(cs.raw());
                        wait_until = millis() + wait_for;
                        indicator.blink(3, 1e7);
                        Serial.println(millis());
                        Serial.println("Red");
                case __LINE__: lc = __LINE__;
                        if (millis() < wait_until)
                                return;
                        JD::Calibration::set(JD::Calibration::ColorRed,
                                             cs.raw());
                        printVec(cs.raw());
                        wait_until = millis() + wait_for;
                        indicator.blink(4, 1e7);
                        Serial.println(millis());
                        Serial.println("Blue");
                case __LINE__: lc = __LINE__;
                        if (millis() < wait_until)
                                return;
                        JD::Calibration::set(JD::Calibration::ColorBlue,
                                             cs.raw());
                        printVec(cs.raw());
                        wait_until = millis() + wait_for;
                        indicator.blink(5, 1e7);
                        Serial.println(millis());
                default: lc = 0;
        }

        Serial.println("exiting");
        Serial.flush();
        exit(0);
}