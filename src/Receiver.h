#ifndef JD_RECEIVER_H
#define JD_RECEIVER_H

#include "Filter.h"
#include "Sensor.h"
namespace JD {

class Receiver : public Sensor {
public:
        enum Message { msg200 = 2, msg300, msg400, msg500, msgNone };

        enum class Status { receiving, start, ending, end, none };
        Receiver(uint8_t pin) : Sensor(pin), filter(0.8, analogRead(m_pin)) {}

        Status getStatus() {
                static Status toReturn = Status::none;
                // If message has just started, set to receiving
                if (toReturn == Status::start)
                        toReturn = Status::receiving;
                // If message has just ended, set to none
                if (toReturn == Status::end)
                        toReturn = Status::none;

                int prev = filter.current();
                filter.update(analogRead(m_pin));
                int curr = filter.current();

                double slope = (static_cast<double>(curr) - prev) /
                               (millis() - lastUpdated);

                auto absVal   = [](double a) { return (a < 0) ? -a : a; };
                auto approxEq = [absVal](double a, double b) {
                        // 4 is an arbitrary threshold that seems to work well
                        return absVal(a - b) < 4;
                };
                // Serial.print(millis());
                // Serial.print(' ');
                // Serial.print(analogRead(m_pin));
                // Serial.print(' ');
                // Serial.print(curr);
                // Serial.print(' ');
                // Serial.println(slope);
                if (not approxEq(slope, 0.0)) {
                        // Message starts on rising edge
                        if (slope > 0) {
                                toReturn = Status::start;
                        }
                        // Message ends on falling edge
                        else if (slope < 0) {
                                toReturn = Status::ending;
                        }
                } else {
                        if (toReturn == Status::ending)
                                toReturn = Status::end;
                }

                return toReturn;
        }

        void update() override {
                if (millis() - lastUpdated > samplePeriod) {
                        Status currStatus = getStatus();
                        // switch(currStatus){
                        //         case Status::receiving:
                        //         Serial.println("receiving");
                        //                              break;
                        //                 case Status::ending:
                        //                 Serial.println("ending");
                        //                              break;
                        //                 case Status::end:
                        //                 Serial.println("end");
                        //                              break;
                        //                 case Status::start:
                        //                 Serial.println("start");
                        //                               break;
                        //                 default: Serial.println("none");
                        // }
                        if (currStatus == Status::receiving ||
                            currStatus == Status::start) {
                                count += millis() - lastUpdated;
                                // Serial.println(count);
                        } else if (currStatus == Status::end) {
                                // Serial.println(count);
                                // Serial.println(round((double)count / 100));
                                Message msg{
                                    (Message)round((double)count / 100)};
                                switch (msg) {
                                        case msg200:
                                        case msg300:
                                        case msg400:
                                        case msg500: currMsg = msg; break;
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
        ExponentialFilter<decltype(analogRead(m_pin))> filter;
        long unsigned                                  count       = 0;
        long unsigned                                  lastUpdated = 0;
        long unsigned samplePeriod                                 = 10;  // ms
        Message       currMsg                                      = msgNone;
};

}  // namespace JD

#endif