

#ifndef JD_TIMER_H
#define JD_TIMER_H

#include <Arduino.h>

namespace JD {
/**
 * @brief Simple non blocking timer
 *
 */
class Timer {
public:
        /**
         * @brief Starts the timer, will go for wait_ms
         *
         * @param wait_ms
         */
        void start(unsigned long wait_ms) {
                if (!isRunning) {
                        finishTime = millis() + wait_ms;
                        isRunning  = true;
                }
        }

        /**
         * @brief Checks whether timer is finished
         */
        bool isFinished() {
                if (isRunning && ((signed long)(millis() - finishTime) >= 0))
                        isRunning = false;
                return !isRunning;
        }

        bool          isRunning  = false;
        unsigned long finishTime = 0;
};

}  // namespace JD

#endif
