#ifndef JD_RECEIVER_H
#define JD_RECEIVER_H

#include "Filter.h"
#include "Sensor.h"
namespace JD {

class Receiver : public Sensor {
public:
        enum Message { msg200 = 2, msg300, msg400, msg500, msgNone };

        enum Status { receiving, start, ending, end, none };

        Receiver(uint8_t pin) : Sensor(pin), filter(0.4, analogRead(m_pin)) {}
        // Experimental
        // Receiver(uint8_t pin) : Sensor(pin), filter(0.09, analogRead(m_pin))
        // {}

        /**
         * @brief Gets the Status
         *
         * @details A state machine that determines current state based
         *          on data trend and previous state
         *
         *          any state ---Rising Edge----------> start
         *          start     ---No change (0 slope)--> receiving
         *          receiving ---No change (0 slope)--> receiving
         *          any state ---Falling Edge---------> ending
         *          ending    ---No change (0 slope)--> end
         *          end       ---No change (0 slope)--> none
         *
         * @return Status
         */
        Status getStatus() {
                static Status toReturn = Status::none;
                // If message has just started, set to receiving
                if (toReturn == Status::start)
                        toReturn = Status::receiving;
                // If message has just ended, set to none
                if (toReturn == Status::end)
                        toReturn = Status::none;

                int prev = filter.current();
                int raw  = analogRead(m_pin);
                filter.update(raw);
                int curr = filter.current();
                // if (toReturn == Status::receiving){
                //         Serial.print(raw);
                //         Serial.print(' ');
                //         Serial.print(prev);
                //         Serial.print(' ');
                //         Serial.print(curr);
                //         Serial.print(' ');
                //         Serial.print(millis());
                //         Serial.println();
                // }
                double slope = (static_cast<double>(curr) - prev) /
                               (millis() - lastUpdated);

                auto absVal   = [](double a) { return (a < 0) ? -a : a; };
                auto approxEq = [absVal](double a, double b) {
                        // 5 is an arbitrary threshold that seems to work well
                        return absVal(a - b) < 5;
                        // return absVal(a - b) < 0.5;
                };
                // Serial.print(millis());
                // Serial.print(' ');

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
                // switch (toReturn) {
                //         case Status::start: Serial.print(" 50"); break;
                //         case Status::receiving: Serial.print(" 100"); break;
                //         case Status::ending: Serial.print(" 150"); break;
                //         case Status::end: Serial.print(" 200"); break;
                //         default: Serial.print(" 0"); break;
                // }
                return toReturn;
        }

        /**
         * @brief Sets current message if detects end of message
         *
         */
        void update() override {
                static unsigned long count = 0, lastUpdated = 0,
                                     threshold = 200;
                // Serial.print(analogRead(m_pin));
                // Serial.print(' ');
                // Serial.print(threshold);
                // Serial.print(' ');
                // Serial.print(500);
                if (analogRead(m_pin) > threshold) {
                        if (lastUpdated == 0)
                                lastUpdated = millis();
                        else
                                count += millis() - lastUpdated;
                        lastUpdated = millis();
                } else {
                        Message msg{(Message)round((double)count / 100)};
                        switch (msg) {
                                case msg200:
                                case msg300:
                                case msg400:
                                case msg500: currMsg = msg; break;
                                default: currMsg = msgNone;
                        }
                        lastUpdated = count = 0;
                }
                // Serial.print(' ');
                // Serial.print(count);
                // // Reading is more accurate with < 10ms samplePeriod
                // if (millis() - lastUpdated > samplePeriod) {
                //         Status currStatus = getStatus();
                //         // switch (currStatus) {
                //         //         case Status::end:
                //         Serial.println("end");
                //         //         break; case Status::ending:
                //         //                 Serial.println("ending");
                //         //                 break;
                //         //         case Status::start:
                //         //                 Serial.println("start");
                //         //                 break;
                //         //         case Status::receiving:
                //         // Serial.println("receiving");
                //         //                 break;
                //         //         case Status::none:
                //         //                 break;
                //         //                 Serial.println("none");
                //         //                 break;
                //         //         default: Serial.println("error");
                //         break;
                //         // }
                //         // Serial.print(' ');
                //         // Serial.print(count < 1000 ? count :
                //         1000); if (currStatus == Status::receiving)
                //         {
                //         // if (currStatus != Status::end &&
                //         //     currStatus != Status::none) {
                //                 count += millis() - lastUpdated;
                //         } else if (currStatus == Status::end) {
                //                 // Serial.println(count);
                //                 //
                //                 Serial.println(round((double)count /
                //                 100)); Message msg{
                //                     (Message)round((double)count /
                //                     100)};
                //                 switch (msg) {
                //                         case msg200:
                //                         case msg300:
                //                         case msg400:
                //                         case msg500: currMsg = msg;
                //                         break; default: currMsg =
                //                         msgNone;
                //                 }
                //         }

                //         if (currStatus == Status::end)
                //                 count = 0;
                //         // if (currStatus == Status::end ||
                //         //     currStatus == Status::start)
                //         //         count = 0;
                //         lastUpdated = millis();
                // }
        }

        /**
         * @brief Returns the received message and resets
         *
         * @Note  Messages that are unread will be overwritten by
         * subsequent messages
         *
         * @return Message that was read (msgNone if no message
         * detected)
         */
        Message receivedMsg() {
                Message tmp = currMsg;
                currMsg     = msgNone;
                return tmp;
        }

private:
        ExponentialFilter<decltype(analogRead(m_pin))> filter;
        long unsigned                                  count        = 0;
        long unsigned                                  lastUpdated  = 0;
        long unsigned                                  samplePeriod = 3;  // ms
        // long unsigned samplePeriod = 5;  // ms
        Message currMsg = msgNone;
};

}  // namespace JD

#endif