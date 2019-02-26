#ifndef JD_SENSOR_H
#define JD_SENSOR_H

#include "Peripheral.h"
namespace JD {

class Sensor : public Peripheral {
private:

        int val;
public:
        enum READ_BEHAVIOR {force, cache};
        constexpr Sensor(uint8_t pin) : Peripheral(pin), val(0) {}
        void update() override{
                val = analogRead(m_pin);
        }

        int read(READ_BEHAVIOR c = cache) {
                switch (c){
                        case force: update(); //fall through on purpose
                        case cache: return val;
                        default   : return -1;
                }
        }
};

}  // namespace JD
#endif
