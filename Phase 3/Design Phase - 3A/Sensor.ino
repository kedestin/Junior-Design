#include "src/Sensor.h"
#include "src/Led.h"

#include "src/ColorSensor.h"
// JD::LED red_led;
// JD::LED blue_led(12);
// JD::Sensor phototransistor;
JD::ColorSensor colorsensor(12, 13, A0);


JD::Peripheral* peripherals[] = {&colorsensor};// &phototransistor};



void setup() {
        Serial.begin(115200);
        // blue_led.on(0.5);
        // red_led.blink(3, 4294967295);
        // blue_led.fade(255, 0, 5000);

        colorsensor.toggleBlue();

        colorsensor.toggleRed();
}

void loop() {
        for (auto p : peripherals)
                p->update();
        delayMicroseconds(500);
}