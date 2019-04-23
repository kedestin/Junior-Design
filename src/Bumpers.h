#ifndef JD_BUMPERS_H
#define JD_BUMPERS_H
#include "Peripheral.h"

namespace JD {
class Bumper : public Peripheral<3> {
public:
        enum Collision : uint8_t { FrontLeft, FrontRight, Back, Front, None };
        uint8_t val = 0;
        Bumper(uint32_t frontLeft, uint32_t frontRight, uint32_t back)
            : Peripheral(frontLeft, frontRight, back) {}

        void update() override {
                // Serial.print(!digitalRead(m_pin[FrontLeft]));
                // Serial.print(' ');
                // Serial.print(!digitalRead(m_pin[FrontRight]));
                // Serial.print(' ');
                // Serial.print(!digitalRead(m_pin[Back]));
                // Serial.print('\n');
                val = 0;
                val |= (!digitalRead(m_pin[FrontLeft])) << FrontLeft;
                val |= (!digitalRead(m_pin[FrontRight])) << FrontRight;
                val |= (!digitalRead(m_pin[Back])) << Back;
        }

        Collision read() {
                // uint8_t temp         = val;
                // val                  = 0;
                constexpr uint8_t FL = 1 << FrontLeft, FR = 1 << FrontRight,
                                  B = 1 << Back;
                switch (val) {
                        case FL | FR: return Collision::Front;
                        case FL: return Collision::FrontLeft;
                        case FR: return Collision::FrontRight;
                        case B: return Collision::Back;
                        default: return Collision::None;
                }
        }
};
}  // namespace JD

#endif
