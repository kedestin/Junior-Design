#ifndef JD_TRANSMITTER_H
#define JD_TRANSMITTER_H

#include "Peripheral.h"

namespace JD {

/**
 * @brief Abstraction for transmitter
 * 
 */
class Transmitter : public Peripheral {
public:
        Transmitter(uint8_t pin) : Peripheral(pin) {}

        /**
         * @brief Will output signal if transmitter should be outputing a 
         *        signal
         * 
         *        Can be configured to output noisy signal
         * 
         */
        void update() override {
                if (sending == 0)
                        return;

                
                if (start + sending < millis()) {
                        digitalWrite(m_pin, LOW);
                        reset();
                }
        }

        /**
         * @brief Sends of signal of duration time_ms
         * 
         * @param time_ms Duration of signal
         * @param noise   Whether signal should be noisy
         */
        void send(long unsigned time_ms, bool noise = false) {
                start   = millis();
                sending = time_ms;
                noisy   = noise;
                digitalWrite(m_pin, HIGH);
        }

        /**
         * @brief Resets the tranmitter
         * 
         */
        void reset() {
                start   = 0;
                sending = 0;
                noisy   = false;
        }

private:
        long unsigned start   = 0;
        long unsigned sending = 0;
        bool          noisy   = false;
};

}  // namespace JD

#endif
