#ifndef JD_RECEIVER_H
#define JD_RECEIVER_H

#include "Sensor.h"

namespace JD {

class Receiver : public Sensor {
public:
        enum Message { msg200 = 2, msg300, msg400, msg500, msgNone };

        void update() override {
                if (millis() - lastUpdated > samplePeriod) {
                        if (digitalRead(m_pin))
                                count++;
                        else {
                                switch (round(count / 10)) {
                                        case msg200:
                                        case msg300:
                                        case msg400:
                                        case msg500:
                                                currMsg =
                                                    (Message)round(count / 10);
                                                break;
                                        default: currMsg = msgNone;
                                }

                                count = 0;
                        }
                        lastUpdated = millis();
                }
        }

        Message receivedMsg() {
                Message tmp = currMsg;
                currMsg     = msgNone;
                return tmp;
        }

private:
        short         count        = 0;
        long unsigned lastUpdated  = 0;
        long unsigned samplePeriod = 10;  // ms
        Message       currMsg;
};

}  // namespace JD

#endif