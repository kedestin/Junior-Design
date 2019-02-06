#ifndef MOTOR_INTERFACE
#define MOTOR_INTERFACE

// Junior Design Namespace
namespace JD {
enum Pin : unsigned { Motor1f = 2, Motor1b = 3, Motor2b = 4, Motor2f = 5 };

template <Pin forward, Pin backward, int maxForwards, int maxBackwards>
struct MotorConfig {
        enum {Max = 255};
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
        void drive(double val, int maxVolt, bool forwards) const {
                val = (val > 1) ? 1 : (val < 0) ? 0 : val;

                analogWrite((forwards) ? forward : backward, maxVolt * val);
                analogWrite((forwards) ? backward : forward, 0);
        }
};

// struct DriveSystem {
//         constexpr JD::MotorConfig<JD::Pin::Motor1f, JD::Pin::Motor1b, 255, 255>
//             left;
//         constexpr JD::MotorConfig<JD::Pin::Motor2f, JD::Pin::Motor2b, 255, 255>
//             right;

//         static void forwards(double val = 1){
//                 left.forwards()
//         }
// };
}  // namespace JD
#endif