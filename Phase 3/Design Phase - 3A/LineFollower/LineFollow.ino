#include <Arduino.h>
#include "src/ColorSensor.h"
#include "src/DriveSystem.h"

JD::DriveSystem ds(11, 9, 12, 10);
JD::ColorSensor cs(53, 51, A15);
void            setup() {
        Serial.begin(115200);
        ds.forwards(1);
}

JD::Peripheral* peripherals[] = {&cs};

template <class T>
const T& minimum(const T& a) {
        return a;
}
template <class T, class... Args>
const T& minimum(const T& a, const T& b, const Args&... args) {
        return minimum(b < a ? b : a, args...);
}

// template <class... T>
// bool lineFollow(T... args) {
//         constexpr double ki = 0.5;
//         constexpr double kp = 0.5;
//         constexpr double kd = 0.5;

//         static unsigned long lastTime           = 0;
//         static double        outputSum          = 0;
//         static double        output             = 0.0;
//         static double        lastInput          = 0;
//         static double        lastOutput         = 0;
//         static double        lastlastOutput     = 0;
//         static double        lastlastlastOutput = 0;
//         unsigned long        now                = millis();
//         unsigned long        sampleTime         = 10;
//         unsigned long        timeChange         = (now - lastTime);
//         double               setpoint           = 8.0;
//         double               outMin             = setpoint;
//         double               outMax             = 1e3;

//         if (timeChange >= sampleTime) {
//                 double input  = minimum(cs.error(args)...);
//                 double error  = setpoint - input;
//                 double dInput = (input - lastInput);
//                 // Serial.println(dInput);
//                 outputSum += (ki * error);
//                 outputSum -= kp * dInput;
//                 // Serial.println(outputSum);

//                 if (outputSum > outMax)
//                         outputSum = outMax;
//                 else if (outputSum < outMin)
//                         outputSum = outMin;

//                 output = kp * error;

//                 output += outputSum - kd * dInput;
//                 // Serial.println(output);

//                 // if (output > outMax)
//                 //         output = outMax;
//                 // else if (output < outMin)
//                 //         output = outMin;

//                 lastInput = input;
//                 lastTime  = now;
//         }

//         double        speed        = 0.5;
//         static bool   left         = false;
//         static double timenegative = 0;
//         static double multiplier   = 1;
//         if (output < 0) {
//                 // double outer = speed - output / 300;
//                 // double inner = speed + output / 300;
//                 double outer = 1;
//                 if (timenegative == 0.0)
//                         timenegative = millis();

//                 Serial.println(millis() - timenegative);

//                 // If color can't be found for x amount of time, abort
//                 if ((millis() - timenegative) > 400)
//                         // if (multiplier > 1 << 3)
//                         return false;

//                 if (millis() - timenegative >
//                     (multiplier * 200 + 200 * (multiplier - 1))) {
//                         timenegative = 0;
//                         multiplier *= 2;
//                         left = !left;
//                 }

//                 if (outer < 0.2)
//                         outer = 0.2;

//                 if (left)
//                         ds.rotate(JD::DriveSystem::LEFT, outer);
//                 else
//                         ds.rotate(JD::DriveSystem::RIGHT, outer);
//         } else {
//                 timenegative = 0;
//                 multiplier   = 1;
//                 ds.forwards(speed);
//         }
//         lastOutput         = output;
//         lastlastOutput     = lastOutput;
//         lastlastlastOutput = lastlastOutput;
//         return true;
// }

template <class... T>
bool lineFollow(T... args) {
        constexpr double ki = 0.5;
        constexpr double kp = 0.5;
        constexpr double kd = 0.5;

        constexpr double speed      = 1;
        constexpr double stallSpeed = 0.2;

        static unsigned long lastTime           = 0;
        static double        outputSum          = 0;
        static double        output             = 0.0;
        static double        lastInput          = 0;
        static double        lastOutput         = 0;
        static double        lastlastOutput     = 0;
        static double        lastlastlastOutput = 0;
        unsigned long        now                = millis();
        unsigned long        sampleTime         = 10;
        unsigned long        timeChange         = (now - lastTime);
        double               setpoint           = 8.0;
        double               outMin             = 0;
        double               outMax             = speed - stallSpeed;

        if (timeChange >= sampleTime) {
                double input  = minimum(cs.error(args)...);
                double error  = setpoint - input;
                double dInput = (input - lastInput);
                // Serial.println(dInput);
                outputSum += (ki * error);
                outputSum -= kp * dInput;
                // Serial.println(outputSum);

                if (outputSum > outMax)
                        outputSum = outMax;
                else if (outputSum < outMin)
                        outputSum = outMin;

                output = kp * error;

                output += outputSum - kd * dInput;

                if (output > outMax)
                        output = outMax;
                else if (output < outMin)
                        output = outMin;

                // Serial.println(output);
                lastInput = input;
                lastTime  = now;
        }

        double inner = speed - output;
        double outer = speed;

        return true;
}

void loop() {
        static unsigned long lc = 0;
        for (auto p : peripherals)
                p->update();

        // ds.forwards(1);

        ds.forwards(0);
        Serial.println();
        Serial.print("start\n");
        switch (lc) {
                case 0:
                case __LINE__:
                        lc = __LINE__;
                        Serial.println(lc);
                        if (lineFollow(JD::ColorSensor::Blue,
                                       JD::ColorSensor::Red))
                                return;
                default: lc = 0;
        }
        ds.forwards(0);
        Serial.println("exiting");
        Serial.flush();
        exit(1);
        Serial.println();
        delay(10);
}