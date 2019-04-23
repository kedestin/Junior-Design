#ifndef JD_COLOR_SENSOR_H
#define JD_COLOR_SENSOR_H

#include <Arduino.h>
#include "Calibration.h"
#include "Led.h"
#include "Peripheral.h"
#include "Sensor.h"
#include "Vector.h"

namespace JD {

static void printVec(const JD::Vector<3>& d);

class ColorSensor : public Peripheral<1> {
public:
        enum Color { Black, Blue, Red, Yellow };
        using RawData = Vector<3>;

private:
        LED    red;
        LED    blue;
        Sensor phototransistor;

        // Array of reference data
        RawData config[4];

        RawData val;
        Color   curr;

        double blueBright = 1, redBright = 1;

public:
        /**
         * @brief Construct a new Color Sensor object, and loads reference
         *        colors  from EEPROM
         */
        ColorSensor(uint8_t red_pin, uint8_t blue_pin, uint8_t photo_pin)
            : Peripheral(photo_pin),
              phototransistor(photo_pin),
              red(red_pin),
              blue(blue_pin),
              val(),
              curr(Black) {
                Calibration::get(Calibration::ColorBlack, config[Black]);
                Calibration::get(Calibration::ColorYellow, config[Yellow]);
                Calibration::get(Calibration::ColorRed, config[Red]);
                Calibration::get(Calibration::ColorBlue, config[Blue]);
        }
        ColorSensor(uint8_t red_pin, uint8_t blue_pin, uint8_t photo_pin,
                    double red_brightness, double blue_brightness)
            : ColorSensor(red_pin, blue_pin, photo_pin) {
                redBright  = red_brightness;
                blueBright = blue_brightness;
        }

        // Forbid default construction
        ColorSensor() = delete;

        /**
         * @brief Reads a triplet of data from sensor
         *
         */
        void update() override {
                unsigned b, r, br;
                blue.off();
                red.off();

                blue.on(blueBright);
                delay(3);
                b = phototransistor.read(Sensor::force);

                blue.off();
                red.on(redBright);
                delay(3);
                r = phototransistor.read(Sensor::force);

                blue.on(blueBright);
                delay(3);
                br  = phototransistor.read(Sensor::force);
                val = {r, b, br};

                // Added after confirmed working
                blue.off();
                red.off();
                // Serial.print("r: ");
                // Serial.print(r);
                // Serial.print(" b: ");
                // Serial.print(b);
                // Serial.print(" br: ");
                // Serial.print(br);
        }

        /**
         * @brief Determines which reference color the current raw value best
         *        matches
         *
         * @return Color the best match
         */
        Color read() {
                double min_error = 1e10;
                Color  max_i     = (Color)-1;
                Color  i;
                for (i = Color::Black; i <= Color::Yellow; i = (Color)(i + 1)) {
                        // switch (i) {
                        //         case Black: Serial.print("Black: "); break;
                        //         case Red: Serial.print("Red: "); break;
                        //         case Yellow: Serial.print("Yellow: ");
                        //         break; case Blue: Serial.print("Blue: ");
                        //         break; default: break;
                        // }
                        double curr_error = error(i);
                        // Serial.println(curr_error, 10);
                        if (min_error > curr_error) {
                                max_i     = i;
                                min_error = curr_error;
                        }
                }
                curr = max_i;
                return curr;
        }

        /**
         * @brief  Computes the error between the requested reference color and
         *         the current raw data
         *
         * @param c
         *
         * @return double Norm of the difference of the two vectors
         */
        double error(Color c) { return (config[c] - val).norm(); }
        
        const RawData& raw() { return val; }

        double r() { return val.data[0]; }
        double b() { return val.data[1]; }
        double br() { return val.data[2]; }

        // void toggleBlue() {
        //         if (blue.isOn())
        //                 blue.off();
        //         else
        //                 blue.on();
        // }

        // void toggleRed() {
        //         if (red.isOn())
        //                 red.off();
        //         else
        //                 red.on();
        // }

        /*_____________________________________________________________________

        Debugging

        _____________________________________________________________________*/
        void printConfig() {
                for (auto c : config)
                        printVec(c);
        }
};

static void printVec(const JD::Vector<3>& d) {
        Serial.print("r: ");
        Serial.print(d[0], 5);
        Serial.print(" b: ");
        Serial.print(d[1], 5);
        Serial.print(" br: ");
        Serial.println(d[2], 5);
}

}  // namespace JD

#endif