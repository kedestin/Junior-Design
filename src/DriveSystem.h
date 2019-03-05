#ifndef JUNIOR_DESIGN_DRIVE_SYSTEM
#define JUNIOR_DESIGN_DRIVE_SYSTEM

#include "MotorInterface.h"

namespace JD {

class DriveSystem {
private:
        MotorConfig left;
        MotorConfig right;

        double state1 = 0.0;
        double state2 = 0.0;

public:
        // enum PINOUT { Motor1f = 2, Motor1b = 3, Motor2b = 4, Motor2f = 5 };
        enum Direction { LEFT, RIGHT };
        constexpr DriveSystem()
            : left{2, 3, 255, 255}, right{4, 5, 255, 255} {}

        constexpr DriveSystem(unsigned Motor1f, unsigned Motor1b,
                              unsigned Motor2f, unsigned Motor2b)
            : left{Motor1f, Motor1b, 255, 255},
              right{Motor2f, Motor2b, 255, 255} {}

        void forwards(double val = 1) {
                 left.forwards(val);
                right.backwards(val);
        }

        void backwards(double val = 1) {
                left.backwards(val);
                right.forwards(val);
        }

        void pivot(Direction d, double val = 1) {
                (d == LEFT ? left : right).stop();
                (d == LEFT ? right : left).forwards(val);
        }
        void rotate(Direction d, double val = 1) {
                if (d == RIGHT) {
                        left.forwards(val);
                        right.forwards(val);
                } else {
                        left.backwards(val);
                        right.backwards(val);
                }
        }

        void turn(Direction d, double inner, double outer){
                left.forwards((d == LEFT) ? inner : outer);
                right.backwards((d == LEFT) ? outer : inner);

        }
        void turn(Direction d, double val = 1) {
                (d == LEFT ? left : right).forwards(val / 2);
                (d == LEFT ? right : left).forwards(val);
        }
};
}  // namespace JD

#endif
