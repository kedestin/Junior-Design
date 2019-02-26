#ifndef JD_LED_H
#define JD_LED_H
#include "Peripheral.h"
namespace JD {

class LED : public Peripheral {
private:
        typedef void (*LEDUpdateFunction)(LED&);

        static constexpr unsigned MAX = 255;
        unsigned                  curr;
        unsigned                  fade_from;
        unsigned                  fade_to;
        double                    fade_end;
        double                    blink_frequency;
        long unsigned             blink_end;

        float forceInRange(float val, float minim, float maxim) {
                return (val > maxim) ? maxim : (val < minim) ? minim : val;
        }

        static void fade_impl(LED& led) {
                double     currTime = millis();
                const long start    = led.subscribedAt;

                if (currTime > led.fade_end) {
                        // Force ending voltage to be target
                        led.curr = led.fade_to;
                        analogWrite(led.m_pin, led.curr);
                        led.unsubscribe();
                        return;
                }

                double slope = ((double)led.fade_to - led.fade_from) /
                               ((double)led.fade_end - start);

                // Linear Interpolation: y = mx + b
                int fadeValue = slope * (currTime - start) + led.fade_from;
                Serial.print(fadeValue);
                Serial.print(' ');
                Serial.print(start);
                Serial.print(' ');
                Serial.print(currTime);
                Serial.print(' ');
                Serial.print(led.fade_end);
                Serial.print(' ');
                Serial.print(led.fade_from);
                Serial.print(' ');
                Serial.println(led.fade_to);
                led.curr = fadeValue;
                analogWrite(led.m_pin, led.curr);
        }

        static void blink_impl(LED& led) {
                int           period = 1000 * (1 / led.blink_frequency);
                long unsigned start  = led.subscribedAt;
                long unsigned finish = led.blink_end;
                // Assumes duty cycle of 50%
                long unsigned half_period = period / 2;
                long unsigned curr        = millis();

                if (curr > led.blink_end) {
                        led.off();
                        led.unsubscribe();
                        return;
                }

                if (((curr - start) / half_period) % 2 == 0) {
                        led.on(0.2);
                } else
                        led.off();
        }

public:
        constexpr LED()
            : Peripheral(LED_BUILTIN),
              fade_from(0),
              fade_to(0),
              fade_end(0),
              blink_end(0),
              blink_frequency(0),
              curr(0) {}

        constexpr LED(unsigned pin)
            : Peripheral(pin),
              fade_from(0),
              fade_to(0),
              fade_end(0),
              blink_end(0),
              blink_frequency(0),
              curr(0) {}

        void off() {
                unsubscribe();
                curr = 0;
                analogWrite(m_pin, curr);
        }

        void on(float val = 1.0) {
                curr = MAX * forceInRange(val, 0.0, 1.0);
                analogWrite(m_pin, curr);
        }

        bool isOff(){
                return curr == 0;
        }

        bool isOn(){
                return not isOff();
        }

        void update() override {
                if (onUpdate != nullptr)
                        reinterpret_cast<LEDUpdateFunction>(onUpdate)(*this);
        }


        /**
         * @brief      Fades an LED over specified duration
         *
         * @details    Linearly interpolates the voltage sent to pin between
         *             starting and ending voltages
         *
         * @param[in]  from         The starting intensity
         * @param[in]  to           The ending intensity
         * @param[in]  duration_ms  The duration in milliseconds
         */
        void fade(unsigned from, unsigned to, unsigned duration_ms) {
                fade_from = from;
                fade_to   = to;

                auto start =
                    subscribe(reinterpret_cast<UpdateFunction>(fade_impl));

                fade_end = start + duration_ms;
        }
        /**
         * @brief      Blinks an led at given frequency for given durations
         *
         * @param[in]  frequency    The frequency
         * @param[in]  duration_ms  The duration milliseconds
         */
        void blink(double frequency, long unsigned duration_ms) {
                blink_frequency = frequency;

                auto start =
                    subscribe(reinterpret_cast<UpdateFunction>(blink_impl));

                blink_end = start + duration_ms;
        }
};
}  // namespace JD
#endif
