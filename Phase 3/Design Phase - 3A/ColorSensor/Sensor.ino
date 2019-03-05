#include "src/Led.h"
#include "src/Sensor.h"

#include "src/ColorSensor.h"
// JD::LED red_led;
// JD::LED blue_led(12);
JD::ColorSensor cs(53, 51, A15);

JD::Peripheral* peripherals[] = {&cs};  // &phototransistor};

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

        // printVec(cs.raw());
        // Serial.println(cs.raw().norm());
        // Serial.println(cs.confidence(), 20);
        // Serial.println();
        // delay(100);
        // return;

        Serial.print("Color is ");
        switch (cs.read()) {
                case JD::ColorSensor::Red: Serial.print("Red."); break;
                case JD::ColorSensor::Blue: Serial.print("Blue."); break;
                case JD::ColorSensor::Black: Serial.print("Black."); break;
                case JD::ColorSensor::Yellow: Serial.print("Yellow."); break;
                default: Serial.print("Unknown: "); break;
        }

        // Serial.println(cs.confidence(), 7);
        // cs.read();

        Serial.println();
        Serial.println();
        // delay(1000);

}