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
                val |= digitalRead(m_pin[FrontLeft]) << FrontLeft;
                val |= digitalRead(m_pin[FrontRight]) << FrontRight;
                val |= digitalRead(m_pin[Back]) << Back;
        }

        Collision read() {
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