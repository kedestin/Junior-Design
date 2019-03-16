#include <Arduino.h>
#include "src/Protothread.h"
#include "src/Receiver.h"
#include "src/Timer.h"
// #include <ArduinoCore-avr/blob/master/variants/mega/pins_arduino.h>
JD::Receiver uut(A14);
constexpr uint8_t emitter = 4;

void setup() {
        Serial.begin(115200);
        analogWrite(4, LOW);
}

void loop() {
        uut.update();

        const char *result;
        constexpr unsigned length_ms = 500;
        constexpr unsigned voltage = 255/4;

        
        receiverTest(emitter, length_ms, voltage, result);
        if (strcmp(result, "\0") == 0)
                return;

        Serial.println(result);
        // Serial.println(strlen(result));
        Serial.flush();

        delay(20000);
}

/**
 * @brief Test for the receiver
 *
 * @param pin      Pin to write output from
 * @param time_ms  Time to send message for (in ms)
 * @param voltage  Voltage to write to receiver pin (0-255)
 *
 * @return const char* Returns "\0" if test is not not done
 *                             detected message otherwise
 */
void receiverTest(uint8_t pin, unsigned long time_ms, unsigned long voltage,
                  const char *&result) {
        result = "\0";
        static JD::Timer timer;
        PT_BEGIN();

        analogWrite(pin, LOW);
        timer.start(300);
        // Serial.println(millis());
        PT_WAIT_UNTIL(timer.isFinished());

        // Serial.println(millis());
        analogWrite(pin, voltage);
        timer.start(time_ms);
        // Serial.println(millis());
        PT_WAIT_UNTIL(timer.isFinished());

        // Serial.println(millis());
        analogWrite(pin, LOW);
        timer.start(100);
        // Serial.println(millis());
        PT_WAIT_UNTIL(timer.isFinished());
        // Serial.println(millis());

        switch (uut.receivedMsg()) {
                case JD::Receiver::msg200: result = "msg200"; break;
                case JD::Receiver::msg300: result = "msg300"; break;
                case JD::Receiver::msg400: result = "msg400"; break;
                case JD::Receiver::msg500: result = "msg500"; break;
                default: result = "msgNone"; break;
        }
        PT_END();
        PT_RESTART();

        // static unsigned       lc        = 0;
        // static long unsigned  curr      = 0;
        // static constexpr char notDone[] = "\0";
        // switch (lc) {
        //         case 0: curr = millis();
        //         case __LINE__: lc = __LINE__;
        //                 // Baseline low
        //                 analogWrite(pin, LOW);
        //                 if (millis() < curr + 300) {
        //                         result = notDone;
        //                         return;
        //                 }

        //                 curr = millis();
        //                 // Send signal
        //                 analogWrite(pin, voltage);
        //         case __LINE__: lc = __LINE__;
        //                 // Wait for msg
        //                 if (millis() < curr + time_ms) {
        //                         result = notDone;
        //                         return;
        //                 }
        //                 curr = millis();
        //                 // Turn off
        //                 analogWrite(pin, LOW);
        //         case __LINE__: lc = __LINE__;
        //                 // Let sensor update
        //                 if (millis() < curr + 50) {
        //                         result = notDone;
        //                         return;
        //                 }
        //                 lc = __LINE__;
        //                 switch (uut.receivedMsg()) {
        //                         case JD::Receiver::msg200: return "msg200";
        //                         case JD::Receiver::msg300: return "msg300";
        //                         case JD::Receiver::msg400: return "msg400";
        //                         case JD::Receiver::msg500: return "msg500";
        //                         default: return "msgNone";
        //                 }
        //         default: lc = 0;
        // }
}