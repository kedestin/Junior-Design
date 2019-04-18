// #include "src/Bumpers.h"
#include "src/ColorSensor.h"
// #include "src/Dashboard.h"
#include "src/DriveSystem.h"
#include "src/Led.h"
#include "src/Protothread.h"
#include "src/Receiver.h"
// #include "src/Switch.h"
#include "src/Timer.h"
#include "src/Transmitter.h"
// #include "src/Updateable.h"

JD::ColorSensor cs(53, 51, A15);
JD::DriveSystem ds(11, 12, 9, 10);
JD::Transmitter speaker(52);
JD::Receiver    mic(A14);
JD::LED         headlight1(50), headlight2(48), blinker1(44), tailight1(45),
    blinker2(47), tailight2(46);
// JD::Dashboard   dash(JD::LED(-1),
//                    JD::SevenSegment(-1, -1, -1, -1, -1, -1, -1, -1), ds);

JD::Updateable *peripherals[] = {&cs, &headlight1, &speaker, &mic};

void setup() {
        Serial.begin(115200);
        headlight1.off();
        headlight2.on();
        blinker1.on();
        tailight1.on();
        blinker2.on();
        tailight2.on();
        JD::setupPWM();
}

void loop() {
        for (auto p : peripherals)
                p->update();
        static JD::Timer timer;

        // Continuously
        switch (mic.receivedMsg()) {
                case JD::Receiver::msg200:
                        Serial.println("Received msg200\n");
                        headlight1.blink(2, 1000);
                        break;
                case JD::Receiver::msg300:
                        Serial.println("Received msg300\n");
                        headlight1.blink(3, 1000);
                        break;
                case JD::Receiver::msg400:
                        Serial.println("Received msg400\n");
                        headlight1.blink(4, 1000);
                        break;
                case JD::Receiver::msg500:
                        Serial.println("Received msg500\n");
                        headlight1.blink(5, 1000);
                        break;
                default: break;
        }

        PT_BEGIN();

        Serial.println("Sending 200");
        speaker.send(220, false);
        Serial.println("Forward");
        ds.forwards();
        timer.start(2000);
        PT_WAIT_UNTIL(timer.isFinished());

        Serial.println("Sending 300");
        speaker.send(320, false);
        Serial.println("Backwards");
        ds.backwards();
        timer.start(2000);
        PT_WAIT_UNTIL(timer.isFinished());

        Serial.println("Sending 400");
        speaker.send(420, false);
        Serial.println("Forward");
        ds.forwards();
        timer.start(2000);
        PT_WAIT_UNTIL(timer.isFinished());

        Serial.println("Sending 500");
        speaker.send(520, false);
        Serial.println("Backwards");
        ds.backwards();
        timer.start(2000);
        PT_WAIT_UNTIL(timer.isFinished());
        PT_END();
        PT_RESTART();
}
