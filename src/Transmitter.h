#ifndef JD_TRANSMITTER_H
#define JD_TRANSMITTER_H

#include "Peripheral.h"

namespace JD {

class Transmitter : public Peripheral {
public:
        void update() override {
                if (sending == 0)
                        return;
                
                analogWrite(m_pin, 255);

                if (start + sending < millis()){
                        analogWrite(m_pin, LOW);
                        reset();
                }

        }

        void send(long unsigned time_ms){
                start = millis();
                sending = time_ms;
        }

        void reset() {
                start = 0;
                sending = 0;
        }

private:

        long unsigned start = 0;
        long unsigned sending = 0;
};

}  // namespace JD

#endif
