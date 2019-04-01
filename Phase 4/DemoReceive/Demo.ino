#include <Arduino.h>
#include "src/Led.h"
#include "src/PWM.h"
#include "src/Protothread.h"
#include "src/Receiver.h"
#include "src/Transmitter.h"
#include "src/Timer.h"

JD::LED         green(LED_BUILTIN);
JD::Receiver    mic(A14);
JD::Transmitter speaker(22);

JD::Updateable* peripherals[] = {&green, &mic, &speaker};

void setup() {
        JD::setupPWM();
        Serial.begin(115200);
}

void loop() {
        for (auto p : peripherals)
                p->update();

        static JD::Timer timer;
        // constexpr unsigned     times[]   = {200, 300, 400, 500, 100, 600};
        constexpr unsigned     times[]   = {500, 50, 100};
        constexpr unsigned     num_times = sizeof(times) / sizeof(times[0]);
        unsigned               msgDuration;

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
}