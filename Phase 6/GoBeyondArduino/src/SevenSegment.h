#ifndef JD_SEVEN_SEGMENT_H
#define JD_SEVEN_SEGMENT_H

#include "Peripheral.h"
namespace JD {

/**
 * @brief A SevenSegment Display abstraction
 *              _______
 *           F_|        |_A
 *           G_|        |_B
 *     cathode_|        |_cathode
 *             |        |
 *           E_|        |_DP
 *           D_|        |_C
 *             |________|
 *
 *
 *                A
 *              -----
 *           F |     | B
 *             |  G  |
 *              -----
 *           E |     | C
 *             |     |
 *              -----
 *                D
 */
class SevenSegment : public Peripheral<8> {
private:
        enum : uint8_t {
                A  = 0x1,
                B  = A << 1,
                C  = B << 1,
                D  = C << 1,
                E  = D << 1,
                F  = E << 1,
                G  = F << 1,
                DP = G << 1
        };

        using Peripheral<8>::m_pin;
        /**
         * @brief Lights up the segment display
         *
         * @param packed A bit-packed version of segments to turn on and off
         */
        void segments(uint8_t packed) {
                for (unsigned i = 0; i < sizeof(m_pin) / sizeof(m_pin[0]);
                     i++) {
                        digitalWrite(m_pin[i], packed & (1 << i) ? HIGH : LOW);
                }
        }

public:
        SevenSegment() : Peripheral(0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U) {}
        SevenSegment(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e,
                     uint8_t f, uint8_t g, uint8_t dp)
            : Peripheral(a, b, c, d, e, f, g, dp) {
                for (auto pin : m_pin)
                        pinMode(pin, OUTPUT);
        }

        void display(char toDisplay) {
                switch (toDisplay) {
                        // Final Stop
                        case 'p': segments(A | B | E | F | G); break;
                        // Reverse
                        case 'r': segments(E | G); break;
                        // Stopped, but will continue
                        case 'n': segments(C | E | G); break;
                        // Forwards
                        case 'd': segments(B | C | D | E | G); break;
                        case '1': segments(B | C); break;
                        case '2': segments(A | B | G | E | D); break;
                        default: segments(A | B | C | D | E | F | G); break;
                }
        }

        void update() {}
};
}  // namespace JD
#endif
