
#include "src/ColorSensor.h"
#include <stdio.h>
#include "src/Led.h"
#include "src/Calibration.h"
#include <Arduino.h>

JD::LED indicator(LED_BUILTIN);

JD::ColorSensor cs(53, 51, A15);
JD::Sensor hall(A14);

JD::Peripheral *peripherals[] = {&cs, &hall};


void setup(){
        Serial.begin(115200);
        Serial.println(millis());
}

void loop(){
        static short lc = 0;
        static long int wait_until;
        for (auto p : peripherals)
                p->update();

        switch (lc){
                case 0:
                case __LINE__: lc = __LINE__;
                               wait_until = millis() + 2e3;
                               indicator.blink(1, 1e7);
                               Serial.println(millis());
                case __LINE__: lc = __LINE__;
                                JD::Calibration::set(JD::Calibration::ColorBlack, cs.raw());
                                if (millis() < wait_until)
                                        return;
                                wait_until = millis() + 2e3;
                                indicator.blink(2, 1e7);
                                Serial.println(millis());
                case __LINE__: lc = __LINE__;
                                JD::Calibration::set(JD::Calibration::ColorYellow, cs.raw());
                                if (millis() < wait_until)
                                        return;
                                wait_until = millis() + 2e3;
                                indicator.blink(2, 1e7);
                                Serial.println(millis());
                case __LINE__: lc = __LINE__;
                                JD::Calibration::set(JD::Calibration::ColorRed, cs.raw());
                                if (millis() < wait_until)
                                        return;
                                wait_until = millis() + 2e3;
                                indicator.blink(2, 1e7);
                                Serial.println(millis());
                case __LINE__: lc = __LINE__;
                                JD::Calibration::set(JD::Calibration::ColorBlue, cs.raw());
                                if (millis() < wait_until)
                                        return;
                                wait_until = millis() + 2e3;
                                indicator.blink(2, 1e7);
                                Serial.println(millis());
                default: lc = 0;
        }
        
        Serial.println("exiting");
        Serial.flush();
        exit(0);
}