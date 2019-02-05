#include <Arduino.h>
#include <HardwareSerial.h>
#include <avr/io.h>
#include <stdio.h>

#define SHOULD_USE_PRINTF
// http://playground.arduino.cc/Main/Printf
#ifdef SHOULD_USE_PRINTF

static FILE uartout = {0};
// create a output function
// This works because Serial.write, although of
// type virtual, already exists.
static int uart_putchar(char c, FILE *stream) {
        Serial.write(c);
        return 0;
}

#endif



constexpr unsigned POT_1 = 1;
constexpr unsigned POT_2 = 2;

void setup() {
        Serial.begin(115200);

#ifdef SHOULD_USE_PRINTF
        // fill in the UART file descriptor with pointer to writer.
        fdev_setup_stream(&uartout, uart_putchar, NULL, _FDEV_SETUP_WRITE);
        // The uart is the standard output device STDOUT.
        stdout = &uartout;
#endif
        auto ISR = [](){
                printf("POT_1 =%d POT_2 = %d\n", analogRead(POT_1),
                       analogRead(POT_2));
        })
        attachInterrupt(digitalPinToInterrupt(POT_1), ISR, CHANGE);
        attachInterrupt(digitalPinToInterrupt(POT_2), ISR, CHANGE);
}

void loop() {}