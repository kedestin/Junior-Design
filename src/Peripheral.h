#ifndef JD_PERIPHERAL_H
#define JD_PERIPHERAL_H

namespace JD {

class Peripheral {
protected:
        typedef void (*UpdateFunction)(void *);
        const uint8_t  m_pin;
        UpdateFunction onUpdate;
        long unsigned  subscribedAt;

        constexpr Peripheral(uint8_t pin)
            : m_pin(pin), onUpdate(nullptr), subscribedAt(0) {}

        long unsigned subscribe(UpdateFunction f) {
                subscribedAt = millis();
                onUpdate     = f;
                return subscribedAt;
        }

        void unsubscribe() {
                onUpdate     = nullptr;
                subscribedAt = 0;
        }

public:
        virtual void update() = 0;
};

}  // namespace JD

#endif
