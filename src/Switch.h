#ifndef JD_SWITCH_H
#define JD_SWITCH_H

#include "Peripheral.h"

namespace JD {
template <unsigned numPins>
class Switch : public Peripheral<numPins> {
private:
        using Peripheral<numPins>::m_pin;
        uint32_t val = 0;

public:
        template <class... T>
        constexpr Switch(T... args) : Peripheral<numPins>(args...), val(0) {
                static_assert(sizeof...(args) == numPins,
                              "Number of constructor arguments must match "
                              "number of pins\n");
        }

        void update() override {
                for (unsigned i = 0; i < sizeof(m_pin) / sizeof(m_pin[0]); i++)
                        val &= digitalRead(m_pin[0]) << i;
        }

        uint32_t read() { return val; }
};

template <>
void Switch<1>::update() {
        val = digitalRead(m_pin);
}
}  // namespace JD

#endif