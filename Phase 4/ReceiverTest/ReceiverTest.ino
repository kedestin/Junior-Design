#include <Arduino.h>
#include "src/Receiver.h"

JD::Receiver uut(A14);


void setup(){
        Serial.begin(115200);
        analogWrite(4, LOW);
}

void loop(){
        uut.update();


        const char * result = receiverTest(4, 200, 10);
        if (strcmp(result, "\0") == 0)
                return;
                
        Serial.println(result);
        // Serial.println(strlen(result));
        Serial.flush();
        exit(0);
        delay(10000);
}



const char * receiverTest(uint8_t pin, unsigned long time_ms, unsigned long voltage){
        static unsigned lc = 0;
        static long unsigned curr = 0;
        static constexpr char notDone[] = "\0";
        switch(lc){
                case 0: curr = millis();
                case __LINE__: lc = __LINE__;
                                // Baseline low
                                analogWrite(pin, LOW);
                                if (millis() < curr + 300)
                                        return notDone;
                                curr = millis();
                                // Send signal
                                analogWrite(pin, voltage);
                case __LINE__: lc = __LINE__;
                                // Wait for msg
                                if (millis() < curr + time_ms)
                                        return notDone;
                                curr = millis();
                                // Turn off
                                analogWrite(pin, LOW);
                case __LINE__: lc = __LINE__;
                                // Let sensor update
                                if (millis() < curr + 50)
                                        return notDone;
                                lc = __LINE__;
                                switch (uut.receivedMsg()) {
                                        case JD::Receiver::msg200: return "msg200";
                                        case JD::Receiver::msg300: return "msg300";
                                        case JD::Receiver::msg400: return "msg400";
                                        case JD::Receiver::msg500: return "msg500";
                                        default: return "msgNone";
                                }
                default: lc = 0;
        }
}