#ifndef JD_LED_H
#define JD_LED_H
#include "Peripheral.h"
namespace JD {

/**
 * @brief An LED abstraction
 * 
 */
class LED : public Peripheral<1> {
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

        /**
         * @brief Fades LEDs
         * 
         * @param led  LED to fade
         */
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

        /**
         * @brief Blinks LED
         * 
         * @param led LED to fade
         */
        static void blink_impl(LED& led) {
                int           period = 1000 * (1 / led.blink_frequency);
                long unsigned start  = led.subscribedAt;
                long unsigned finish = led.blink_end;
                // Assumes duty cycle of 50%
                long unsigned half_period = period / 2;
                long unsigned curr        = millis();
                if (curr > led.blink_end) {
                        led.off();
                        // led.reset(); Remove if something breaks
                        return;
                }

                if (((curr - start) / half_period) % 2 == 0) {
                        led.on();
                } else{
                        led.on(0);
                }
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
                curr = 0;
                analogWrite(m_pin, curr);
                reset(); // Remove if somethig breaks
        }
        void reset(){
                unsubscribe();
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

        void toggle(){
                if (isOff())
                        on();
                else
                        off();
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
        void blink(double frequency, long unsigned duration_ms = 1000) {
                blink_frequency = frequency;

                auto start =
                    subscribe(reinterpret_cast<UpdateFunction>(blink_impl));

                blink_end = start + duration_ms;
        }
};
}  // namespace JD
#endif
