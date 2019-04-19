#include <Arduino.h>
#include "src/Led.h"
#include "src/PWM.h"
#include "src/Protothread.h"
#include "src/Receiver.h"
#include "src/Transmitter.h"
#include "src/Timer.h"

JD::LED         green(44);
JD::Receiver    mic(A1);
JD::Transmitter speaker(30);

JD::Updateable* peripherals[] = {&green, &mic, &speaker};

void setup() {
        JD::setupPWM();
        Serial.begin(115200);
        green.off();
}

void loop() {

        for (auto p : peripherals)
                p->update();

        static JD::Timer timer;

        // Continuously 
        switch (mic.receivedMsg()) {
                case JD::Receiver::msg200:
                        Serial.println("Received msg200\n");
                        green.blink(2, 1000);
                        break;
                case JD::Receiver::msg300:
                        Serial.println("Received msg300\n");
                        green.blink(3, 1000);
                        break;
                case JD::Receiver::msg400:
                        Serial.println("Received msg400\n");
                        green.blink(4, 1000);
                        break;
                case JD::Receiver::msg500:
                        Serial.println("Received msg500\n");
                        green.blink(5, 1000);
                        break;
                default: break;
        }
 
        PT_BEGIN();


        Serial.println("Sending 200"); 
        speaker.send(220, false);
        timer.start(2000);
        PT_WAIT_UNTIL(timer.isFinished());
        
        Serial.println("Sending 300"); 
        speaker.send(320, false);
        timer.start(2000);
        PT_WAIT_UNTIL(timer.isFinished());
        
        Serial.println("Sending 400"); 
        speaker.send(420, false);
        timer.start(2000);
        PT_WAIT_UNTIL(timer.isFinished());
        
        Serial.println("Sending 500"); 
        speaker.send(520, false);
        timer.start(2000);
        PT_WAIT_UNTIL(timer.isFinished());
        PT_END();



        PT_RESTART();
}