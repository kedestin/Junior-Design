#include "src/ColorSensor.h"
#include "src/DriveSystem.h"
#include "src/Led.h"
#include "src/Protothread.h"
#include "src/Sensor.h"
#include "src/Switch.h"
#include "src/Updateable.h"
// JD::LED red_led;
// JD::LED blue_led(12);
constexpr uint8_t       RED_LED       = 36;
constexpr uint8_t       BLUE_LED      = 37;
constexpr unsigned long rampUpTime_ms = 200;

JD::Switch<1>   hall(41);
JD::ColorSensor cs(RED_LED, BLUE_LED, A15);
JD::DriveSystem ds(10, 8, 11, 9, rampUpTime_ms);
JD::Updateable* peripherals[] = {&cs, &ds, &hall};  // &phototransistor};

void setup() {
        Serial.begin(115200);
        // blue_led.on(0.5);
        // red_led.blink(3, 4294967295);
        // blue_led.fade(255, 0, 5000);

        // cs.toggleBlue();

        // cs.toggleRed();
        cs.printConfig();
}

void loop() {
        for (auto p : peripherals)
                p->update();

        // Serial.println(cs.raw().norm());
        // Serial.println(cs.confidence(), 20);
        // Serial.println();
        // delay(100);
        // return;
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
        // Serial.println(hall.read());
        // PT_BEGIN();
        // ds.forwards();
        // PT_WAIT_UNTIL(hall.read() == 0);
        // ds.backwards();
        // PT_END();
        // // Serial.println(cs.confidence(), 7);
        // // cs.read();

        // Serial.println();
        // Serial.println();
        // delay(1000);
}