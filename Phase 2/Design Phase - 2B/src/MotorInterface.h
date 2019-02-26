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
        const Pin forward;
        const Pin backward;
        const int maxForwards;
        const int maxBackwards;

        void drive(double val, int maxVolt, bool forwards) const {
                val = (val > 1) ? 1 : (val < 0) ? 0 : val;

                analogWrite((forwards) ? forward : backward, maxVolt * val);
                analogWrite((forwards) ? backward : forward, 0);
        }
};

}  // namespace JD
#endif