#include <Arduino.h>
#include "src/Bumpers.h"
#include "src/ColorSensor.h"
#include "src/Horn.h"
#include "src/Transmitter.h"
#include "src/Receiver.h"
#include "src/MotorInterface.h"
#include "src/PWM.h"
#include "src/Updateable.h"
#include "src/DriveSystem.h"
#include "src/DriveFeedback.h"


JD::DriveSystem drivesys(8, 10, 11, 9);
JD::DriveFeedback driveFeed(45, 38, 25, 27, 24, 26, drivesys);
JD::ColorSensor colors(36, 37, A15);
JD::Receiver receiver(A7);
JD::Transmitter transmitter(22);
JD::Bumper bumpers(43, 42, 31);
JD::Sensor proximity(A5);
const int HALL = 41;
int lastUpdate = -1000;

JD::Updateable *peripherals[] = {&drivesys, &driveFeed, &colors,
                                 &receiver, &transmitter, &bumpers, 
                                 &proximity};



void setup() {
    Serial.begin(9600);
    while(!Serial);
    JD::setupPWM();
    pinMode(HALL, INPUT);
}

void loop() {
    long time = millis();
    for (auto p : peripherals) {
        p->update();
    }

    if (time - lastUpdate > 2000) {
        sendUpdates();
        lastUpdate = time;
    }
}

void sendUpdates() {
    if (drivesys.isForwards() || drivesys.isLeft() || drivesys.isRight())
        Serial.write("sg010");
    else if (drivesys.isBackwards()) 
        Serial.write("sg100");
    else if (drivesys.isStopped()) 
        Serial.write("sg001");
    
    switch (colors.read()) {
        case 0:
            Serial.write("sc000");
        case 1:
            Serial.write("sc001");
        case 2:
            Serial.write("sc010");
        case 3:
            Serial.write("sc100");
    }

    String spd = getFmtdString((int) drivesys.getSpeed());
    spd = "sd" + spd;
    Serial.print(spd);

    if (digitalRead(HALL) == HIGH) Serial.write("sa000");
    else Serial.write("sa111");

    String prox = getFmtdString((int) proximity.read());
    prox = "sp" + prox;
    Serial.print(prox);
}

String getFmtdString(int val) {
    int ct = 1;
    if (val >= 1000) return "999";
    if (val < 0) return "000"; 
    String fmtd = "";
    while (val >= 10) {
        fmtd += (char) ('0' + val % 10);
        val = val / 10;
        ct++;
    }
    fmtd += (char) ('0' + val);
    for(int i = 3; i > ct; i--) {
        fmtd = '0' + fmtd;
    }
    return fmtd;
}