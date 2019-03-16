#ifndef JD_PERIPHERAL_H
#define JD_PERIPHERAL_H

#include <stdint.h>

namespace JD {

/**
 * @brief An abstraction for peripherals connected to the Arduino
 * 
 */
class Peripheral {
protected:
        typedef void (*UpdateFunction)(void *);
        const uint8_t  m_pin;
        UpdateFunction onUpdate;
        long unsigned  subscribedAt;

        constexpr Peripheral(uint8_t pin)
            : m_pin(pin), onUpdate(nullptr), subscribedAt(0) {}

        /**
         * @brief Sets onUpdate to requested update function
         * 
         * @param f 
         * @return long unsigned  The time that the subscription happened
         */
        long unsigned subscribe(UpdateFunction f) {
                subscribedAt = millis();
                onUpdate     = f;
                return subscribedAt;
        }

        /**
         * @brief Unsets current update function
         * 
         */
        void unsubscribe() {
                onUpdate     = nullptr;
                subscribedAt = 0;
        }

public:
        virtual void update() = 0;
};

}  // namespace JD

#endif
