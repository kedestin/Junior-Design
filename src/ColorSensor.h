#ifndef JD_COLOR_SENSOR_H
#define JD_COLOR_SENSOR_H

#include "Calibration.h"
#include "Led.h"
#include "Peripheral.h"
#include "Sensor.h"
namespace JD {

class ColorSensor : public Peripheral {
public:
        enum Color { Black, Blue, Red, Yellow };
        using RawData = Vector3;
private:
        LED    red;
        LED    blue;
        Sensor phototransistor;



        RawData config[4];

        RawData val;
        Color   curr;

public:
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

        ColorSensor() = delete;

        void update() override {
                unsigned b, r, br;
                blue.off();
                red.off();

                blue.on();
                delay(3);
                b = phototransistor.read(Sensor::force);

                blue.off();
                red.on();
                delay(3);
                r = phototransistor.read(Sensor::force);

                blue.on();
                delay(3);
                br = phototransistor.read(Sensor::force);

                val = {static_cast<double>(b), static_cast<double>(r),
                       static_cast<double>(br)};
        }

        Color read() {
                double max_confidence = -1e10;
                int    max_i          = -1;
                int    i;
                for (i = 0; i < 4; i++) {
                        double curr_confidence = (val - config[i]).norm();
                        if (max_confidence < curr_confidence) {
                                max_i          = i;
                                max_confidence = curr_confidence;
                        }
                }
                curr = (Color)i;
                return curr;
        }

        RawData raw() {
                return val;
        }
        double r() { return val.data[0]; }
        double b() { return val.data[1]; }
        double br() { return val.data[2]; }

        double confidence() {}
        void   toggleBlue() {
                if (blue.isOn())
                        blue.off();
                else
                        blue.on();
        }

        void toggleRed() {
                if (red.isOn())
                        red.off();
                else
                        red.on();
        }
};

}  // namespace JD

#endif