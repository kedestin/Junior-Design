#ifndef JD_PERIPHERAL_H
#define JD_PERIPHERAL_H

#include <stdint.h>
#include "Updateable.h"
namespace JD {

/**
 * @brief An abstraction for peripherals connected to the Arduino
 *
 */
template <unsigned numPins>
class Peripheral : public Updateable {
protected:
        typedef void (*UpdateFunction)(void *);
        const uint8_t  m_pin[numPins];
        UpdateFunction onUpdate;
        long unsigned  subscribedAt;

        template <class... T>
        constexpr Peripheral(T... args)
            : m_pin{args...}, onUpdate(nullptr), subscribedAt(0U) {
                static_assert(sizeof...(args) == numPins,
                              "Number of constructor arguments must match "
                              "number of pins\n");
        }

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
};

/**
 * @brief Specialization of peripherals for 1 pin
 */
template <>
class Peripheral<1> : public Updateable {
protected:
        typedef void (*UpdateFunction)(void *);
        const uint8_t  m_pin;
        UpdateFunction onUpdate;
        long unsigned  subscribedAt;

        constexpr Peripheral(uint8_t pin)
            : m_pin{pin}, onUpdate(nullptr), subscribedAt(0) {}

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
};

}  // namespace JD

#endif
