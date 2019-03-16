#include <Arduino.h>
#include "src/Led.h"
#include "src/PWM.h"
#include "src/Protothread.h"
#include "src/Receiver.h"
#include "src/Transmitter.h"
#include "src/Timer.h"

JD::LED         green(LED_BUILTIN);
JD::Receiver    mic(A14);
JD::Transmitter speaker(4);

JD::Peripheral* peripherals[] = {&green, &mic, &speaker};

void setup() {
        // JD::setupPWM();
        Serial.begin(115200);
}

void loop() {
        for (auto p : peripherals)
                p->update();

        static JD::Timer timer;
        constexpr unsigned     times[]   = {200, 300, 400, 500, 100, 600};
        constexpr unsigned     num_times = sizeof(times) / sizeof(times[0]);
        unsigned               msgDuration;

        // Continuously 
        switch (mic.receivedMsg()) {
                case JD::Receiver::msg200:
                        Serial.println("Received msg200\n");
                        green.blink(2, 2000);
                        break;
                case JD::Receiver::msg300:
                        Serial.println("Received msg300\n");
                        green.blink(3, 2000);
                        break;
                case JD::Receiver::msg400:
                        Serial.println("Received msg400\n");
                        green.blink(4, 2000);
                        break;
                case JD::Receiver::msg500:
                        Serial.println("Received msg500\n");
                        green.blink(5, 2000);
                        break;
                default: break;
        }

        PT_BEGIN();

        msgDuration = times[micros() % num_times];
        switch (msgDuration) {
                case 200: Serial.println("Sending 200"); break;
                case 300: Serial.println("Sending 300"); break;
                case 400: Serial.println("Sending 400"); break;
                case 500: Serial.println("Sending 500"); break;
                default: Serial.println("Sending none"); break;
        }

        speaker.send(msgDuration, true); // Simulate a noisy signal
        
        timer.start(2000);

        PT_WAIT_UNTIL(timer.isFinished());

        PT_END();



        PT_RESTART();
}
