
#include "src/DriveSystem.h"
#include "src/Protothread.h"
#include "src/Timer.h"
// JD::DriveSystem ds;
 JD::DriveSystem ds(8,9,10,11, 2000);
//JD::MotorConfig m(4, 6, 255, 255, 2000);
void            setup() {
        Serial.begin(115200);
}

void loop() {
        ds.update();

        static JD::Timer timer;
        PT_BEGIN();
        ds.forwards();
        timer.start(3000);
        Serial.println("Forwards");
        PT_WAIT_UNTIL(timer.isFinished());
        ds.backwards();
        Serial.println("Backwards");
        timer.start(3000);
        PT_WAIT_UNTIL(timer.isFinished());
        PT_END();
        PT_RESTART();
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
