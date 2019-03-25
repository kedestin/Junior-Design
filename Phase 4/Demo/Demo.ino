#include <Arduino.h>
// #include "src/Led.h"
// #include "src/PWM.h"
// #include "src/Protothread.h"
// #include "src/Receiver.h"
// #include "src/Transmitter.h"
// #include "src/Timer.h"

// JD::LED         green(LED_BUILTIN);
// JD::Receiver    mic(A14);
// JD::Transmitter speaker(22);

// JD::Peripheral* peripherals[] = {&green, &mic, &speaker};

void setup() {
  pinMode(13, OUTPUT); // Set this pin as output

  pinMode(5, OUTPUT);pinMode(2, OUTPUT);

  Serial.begin(9600);

  //Serial1.begin(1200);

  TCCR3A = _BV(COM3A0) | _BV(COM3B0) | _BV(WGM30) | _BV(WGM31);
 
  // sets COM Output Mode to FastPWM with toggle of OC3A on compare match with OCR3A

  // also sets WGM to mode 15: FastPWM with top set by OCR3A

  TCCR3B = _BV(WGM32) | _BV(WGM33) |  _BV(CS31);

  // sets WGM as stated above; sets clock scaling to "divide by 8"

  OCR3A = 53;

  // above sets the counter value at which register resets to 0x0000;

  // generate 18.523 kHz when OCR3A=53 on Mega pin 5

  // Serial.println(TCCR3A, BIN);Serial.println(TCCR3B, BIN);

        // JD::setupPWM();
        // Serial.begin(115200);
}

void loop() {
        digitalWrite(22, HIGH);
        delay(350);
        digitalWrite(22, LOW);
        delay(1000);
        // for (auto p : peripherals)
        //         p->update();

//         static JD::Timer timer;
//         // constexpr unsigned     times[]   = {200, 300, 400, 500, 100, 600};
//         constexpr unsigned     times[]   = {500, 100, 50};
//         constexpr unsigned     num_times = sizeof(times) / sizeof(times[0]);
//         unsigned               msgDuration;

//         // Continuously 
//         switch (mic.receivedMsg()) {
//                 case JD::Receiver::msg200:
//                         Serial.println("Received msg200\n");
//                         green.blink(1, 2000);
//                         break;
//                 case JD::Receiver::msg300:
//                         Serial.println("Received msg300\n");
//                         green.blink(2, 2000);
//                         break;
//                 case JD::Receiver::msg400:
//                         Serial.println("Received msg400\n");
//                         green.blink(3, 2000);
//                         break;
//                 case JD::Receiver::msg500:
//                         Serial.println("Received msg500\n");
//                         green.blink(4, 2000);
//                         break;
//                 default: break;
//         }

//         PT_BEGIN();

//         msgDuration = times[micros() % num_times];
//         switch (msgDuration) {
//                 case 200: Serial.println("Sending 200"); break;
//                 case 300: Serial.println("Sending 300"); break;
//                 case 400: Serial.println("Sending 400"); break;
//                 case 500: Serial.println("Sending 500"); break;
//                 default: Serial.println("Sending none"); break;
//         }

//         // speaker.send(msgDuration, false); // Simulate a noisy signal
        
//         // timer.start(2000);
//         digitalWrite(22, HIGH);
//         delay(msgDuration);
//         digitalWrite(22, LOW);
//         // PT_WAIT_UNTIL(timer.isFinished());
//         delay(2000);
//         PT_END();



//         PT_RESTART();
}