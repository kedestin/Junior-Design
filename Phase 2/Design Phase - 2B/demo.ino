
#include "src/DriveSystem.h"

JD::DriveSystem ds;

void setup(){
        Serial.begin(115200);
}


void loop(){
        constexpr double val = 0.3;

        ds.forwards(val);
        Serial.println("forwards");
        delay(2000);

        ds.backwards(val);
        Serial.println("backwards");
        delay(2000);

        ds.pivot(JD::DriveSystem::LEFT, val);
        Serial.println("pivot left");
        delay(2000);

        ds.rotate(JD::DriveSystem::LEFT, val);
        Serial.println("rotate left");
        delay(2000);

        ds.turn(JD::DriveSystem::LEFT, val);
        Serial.println("turn left");
        delay(2000);

        ds.pivot(JD::DriveSystem::RIGHT, val);
        Serial.println("pivot right");
        delay(2000);

        ds.rotate(JD::DriveSystem::RIGHT, val);
        Serial.println("rotate right");
        delay(2000);

        ds.turn(JD::DriveSystem::RIGHT, val);
        Serial.println("turn right");
        delay(2000);

}