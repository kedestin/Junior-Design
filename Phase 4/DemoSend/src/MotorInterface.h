#ifndef MOTOR_INTERFACE
#define MOTOR_INTERFACE

// Junior Design Namespace
namespace JD {

typedef unsigned Pin;

struct MotorConfig {
        MotorConfig()                     = delete;
        MotorConfig(const MotorConfig &m) = delete;

        constexpr MotorConfig(Pin f, Pin b, int mF, int mB)
            : forward(f), backward(b), maxForwards(mF), maxBackwards(mB) {}

        enum { Max = 255 };
        /**
         * @brief Drives the motor in the defined forwards orientation.
         *        Can scale speed from 0 to 1;
         *
         * @param val Modifier for speed
         */
        void forwards(double val = 1) const { drive(val, maxForwards, true); }

        /**
         * @brief Drives the motor in the defined backwards orientation.
         *        Can scale speed from 0 to 1;
         *
         * @param val Modifier for speed
         */
        void backwards(double val = 1) const {
                drive(val, maxBackwards, false);
        }

        /**
         * @brief Stops the motor
         *
         */
        void stop() const { drive(0, 0, false); }

private:
        // Pin that, when high, drives motor in forwards direction
        const Pin forward;
        // Pin that, when high, drives motor in backwards direction
        const Pin backward;
        // Max voltage to apply forwards (0 to 255)
        const int maxForwards;
        // Max voltage to apply backwards (0 to 255)
        const int maxBackwards;


        /**
         * @brief Drives the motor in requested direction at requested speed
         * 
         * @param val      Speed (0 to 1)
         * @param maxVolt  Maximum voltage to apply to motor (0 to 255)
         * @param forwards Whether should go forwards
         */
        void drive(double val, int maxVolt, bool forwards) const {
                val = (val > 1) ? 1 : (val < 0) ? 0 : val;

                analogWrite((forwards) ? forward : backward, maxVolt * val);
                analogWrite((forwards) ? backward : forward, 0);
        }
};

}  // namespace JD
#endif