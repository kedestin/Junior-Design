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
#include "src/Protothread.h"
#include "src/Timer.h"


JD::DriveSystem drivesys(8, 9, 10, 11);
JD::DriveFeedback driveFeed(45, 38, 25, 27, 24, 26, drivesys);
JD::ColorSensor colors(36, 37, A15);
JD::Receiver receiver(A7);
JD::Transmitter transmitter(22);
JD::Bumper bumpers(43, 42, 31);
JD::Sensor proximity(A5);
JD::LED blue(44);
const int HALL = 41;
const int HORN = 3;
unsigned long lastUpdate;
const int durations[9] = {5, 100, 200, 300, 400, 500, 1000, 2000, 3000};
char op = '\0', dur = '\0';
unsigned long startedOp, duration;
bool inProgress = false;

JD::Updateable *peripherals[] = {&drivesys, &driveFeed, &colors,
                                 &receiver, &transmitter, &bumpers, 
                                 &proximity};



void setup() {
    Serial.begin(9600);
    while(!Serial);
    JD::setupPWM();
    pinMode(HALL, INPUT);
    pinMode(HORN, OUTPUT);
    lastUpdate = 0;
}

void loop() {
    unsigned long time = millis();
    for (auto p : peripherals) {
        p->update();
    }

    if (time - lastUpdate > 2000) {
        sendUpdates();
        lastUpdate = time;
        blue.toggle();
    }


    if (Serial.available() >= 2 && !inProgress) {
        op = Serial.read();
        dur = Serial.read();
    }

    time = millis();
    switch (op) {
            case 'f':
                if (!inProgress) {
                    inProgress = true;
                    drivesys.forwards();
                    startedOp = time;
                }
                if (time - startedOp >= durations[dur - '0']) {
                    drivesys.stop();
                    inProgress = false;
                    op = '\0';
                    dur = '\0';
                }
                break;
            case 'r':
                if (!inProgress) {
                    inProgress = true;
                    drivesys.backwards();
                    startedOp = time;
                }
                if (time - startedOp >= durations[dur - '0']) {
                    drivesys.stop();
                    inProgress = false;
                    op = '\0';
                    dur = '\0';
                }
                break;
            case 'l':
                if (!inProgress) {
                    inProgress = true;
                    drivesys.turn(JD::DriveSystem::RIGHT);
                    startedOp = time;
                }
                if (time - startedOp >= durations[dur - '0']) {
                    drivesys.stop();
                    inProgress = false;
                    op = '\0';
                    dur = '\0';
                }
                break;
            case 't':
                if (!inProgress) {
                    inProgress = true;
                    drivesys.turn(JD::DriveSystem::LEFT);
                    startedOp = time;
                }
                if (time - startedOp >= durations[dur - '0']) {
                    drivesys.stop();
                    inProgress = false;
                    op = '\0';
                    dur = '\0';
                }
                break;
            case 'h':
                if (!inProgress) {
                    inProgress = true;
                    analogWrite(HORN, 128);
                    startedOp = time;
                }
                if (time - startedOp >= durations[dur - '0']) {
                    digitalWrite(HORN, LOW);
                    inProgress = false;
                    op = '\0';
                    dur = '\0';                   
                }
                break;
            case 's':
                transmitter.send(durations[dur - '0'], false);
                String msg = "mt" + getFmtdString(durations[dur - '0']);
                Serial.print(msg);
                break;
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

    switch (receiver.receivedMsg()) {
        case 2:
            Serial.write("mr200");
            break;
        case 3:
            Serial.write("mr300");
            break;
        case 4:
            Serial.write("mr400");
            break;
        case 5:
            Serial.write("mr500");
            break;
    }

    switch (bumpers.read()) {
        case 0:
            Serial.write("mc010");
            break;
        case 1:
            Serial.write("mc100");
            break;
        case 2:
            Serial.write("mc001");
            break;
    }
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