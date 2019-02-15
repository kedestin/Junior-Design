#include "src/Sensor.h"
#include "src/Led.h"


JD::LED red_led;
JD::LED blue_led(12);
// JD::Sensor phototransistor;


JD::Peripheral* peripherals[] = {&red_led, &blue_led};// &phototransistor};



void setup() {
        Serial.begin(115200);
        blue_led.on(0.5);
        red_led.blink(3, 4294967295);
        blue_led.fade(255, 0, 5000);
}

void loop() {
        for (auto p : peripherals)
                p->update();
        delayMicroseconds(500);
}