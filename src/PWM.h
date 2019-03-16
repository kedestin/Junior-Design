#ifndef JD_PWM_H
#define JD_PWM_H
namespace JD {
/**
 * @brief Starter code given to output ~18.5kHz out of pin 5
 *
 */
void setupPWM() {
        pinMode(5, OUTPUT);

        TCCR3A = _BV(COM3A0) | _BV(COM3B0) | _BV(WGM30) | _BV(WGM31);

        // sets COM Output Mode to FastPWM with toggle of OC3A on compare match
        // with OCR3A

        // also sets WGM to mode 15: FastPWM with top set by OCR3A

        TCCR3B = _BV(WGM32) | _BV(WGM33) | _BV(CS31);

        // sets WGM as stated above; sets clock scaling to "divide by 8"

        OCR3A = 53;
}
}  // namespace JD
#endif
