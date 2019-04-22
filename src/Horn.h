#ifndef JD_HORN_H
#define JD_HORN_H

#include "Peripheral.h"
#include "Updateable.h"
namespace JD {

class Horn : public Peripheral<1> {
private:
        double        freq = 500;
        unsigned long lengths[32];
        unsigned long currLength = (sizeof(lengths) / sizeof(lengths[0]) + 1),
                      numLengths = 0;
        unsigned long startedAt  = 0;

public:
        Horn(uint8_t pin) : Peripheral(pin) {}
        void setFrequency(double newFreq) { freq = newFreq; }
        bool isSending() {
                return currLength ==
                       (sizeof(lengths) / sizeof(lengths[0]) + 1);
        }
        template <class... T>
        void sendSequence(T... args) {
                static_assert(sizeof...(args), "Only supports 32 lengths");
                currLength           = 0;
                numLengths           = sizeof...(args);
                unsigned long vals[] = {static_cast<unsigned long>(args)...};
                for (unsigned long i = 0; i < sizeof...(args); i++)
                        lengths[i] = vals[i];
                startedAt = millis();
                tone(m_pin, freq);
        }

        void update() override {
                if (not isSending())
                        return;

                if (millis() - startedAt > lengths[currLength]) {
                        startedAt = millis();
                        currLength++;
                        if (not isSending())
                                return;
                        if (currLength % 2 == 1)
                                noTone(m_pin);
                        else
                                tone(m_pin, freq);
                }
        }
};
}  // namespace JD

#endif