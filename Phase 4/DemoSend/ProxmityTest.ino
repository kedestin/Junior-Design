/*#include <Arduino.h>
#include "src/Sensor.h"
#include "src/LED.h"

JD::LED blue(44);
JD::Sensor proximity(A5);

JD::Updateable *peripherals[] = {&blue, &proximity};

void setup() {
    Serial.begin(9600);
    blue.off();
}

void loop() {
    for (auto p: peripherals) {
        p->update();
    }

    if (proximity.read() > 100) {
        blue.on();
    } else {
        blue.off();
    }

    Serial.println(proximity.read());
}*/