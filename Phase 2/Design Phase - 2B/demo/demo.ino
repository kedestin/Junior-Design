
#include "src/DriveSystem.h"
#include "src/Protothread.h"
#include "src/Timer.h"
// JD::DriveSystem ds;
// JD::DriveSystem(4,6,5,7);
JD::MotorConfig m(4, 6, 255, 255, 2000);
void            setup() {
        Serial.begin(115200);
}

void loop() {
        m.update();

        static JD::Timer timer;
        PT_BEGIN();
        m.forwards();
        timer.start(2000);
        Serial.println("Forwards");
        PT_WAIT_UNTIL(timer.isFinished());
        m.backwards();
        timer.start(2000);
        Serial.println("Backwards");
        PT_WAIT_UNTIL(timer.isFinished());
        delay(2000);
        PT_END();
}

// void loop(){
//        constexpr double val = 0.3;
//
//        ds.forwards(val);
//        Serial.println("forwards");
//        delay(2000);
//
//        ds.backwards(val);
//        Serial.println("backwards");
//        delay(2000);
//
//        ds.pivot(JD::DriveSystem::LEFT, val);
//        Serial.println("pivot left");
//        delay(2000);
//
//        ds.rotate(JD::DriveSystem::LEFT, val);
//        Serial.println("rotate left");
//        delay(2000);
//
//        ds.turn(JD::DriveSystem::LEFT, val);
//        Serial.println("turn left");
//        delay(2000);
//
//        ds.pivot(JD::DriveSystem::RIGHT, val);
//        Serial.println("pivot right");
//        delay(2000);
//
//        ds.rotate(JD::DriveSystem::RIGHT, val);
//        Serial.println("rotate right");
//        delay(2000);
//
//        ds.turn(JD::DriveSystem::RIGHT, val);
//        Serial.println("turn right");
//        delay(2000);
//
//}
