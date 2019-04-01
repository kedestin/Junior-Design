#ifndef JUNIOR_DESIGN_DRIVE_SYSTEM
#define JUNIOR_DESIGN_DRIVE_SYSTEM

#include "MotorInterface.h"
#include "Led.h"
#include "SevenSegment.h"

namespace JD {

/**
 * @brief Abstraction for a 2 wheel drive system
 *        Expects that motors must be told to spin in opposite directions to go
 *        forwards
 */
class DriveSystem {
private:
        MotorConfig left;
        MotorConfig right;

        // 0 if stopped
        // negative if going backwards
        // positive if going forwards
        double currVal = 0;

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
                currVal = val;
                // update speedometer
                // update gearshifter
        }
        void stop(){
                forwards(0);
                currVal = 0;
        }

        void backwards(double val = 1) {
                left.backwards(val);
                right.forwards(val);
                currVal = -val;
        }


        /**
         * @brief Will turn with one wheel being center of rotation
         * 
         * @param d   Direction to turn
         * @param val Speed to turn (0 to 1)
         */
        void pivot(Direction d, double val = 1) {
                (d == LEFT ? left : right).stop();
                (d == LEFT ? right : left).forwards(val);
        
                currVal = val;
        }

        /**
         * @brief Will turn in place
         * 
         * @param d   Direction to turn
         * @param val Speed to turn (0 to 1)
         */
        void rotate(Direction d, double val = 1) {
                if (d == RIGHT) {
                        left.forwards(val);
                        right.forwards(val);
                } else {
                        left.backwards(val);
                        right.backwards(val);
                }
                currVal = val;
        }
        
        /**
         * @brief Will turn using specified values for inner and outer motors
         * 
         * @param d     Direction to turn
         * @param inner Speed for inner wheel (0 to 1)
         * @param outer Speed for outer wheel (0 to 1)
         */
        void turn(Direction d, double inner, double outer){
                left.forwards((d == LEFT) ? inner : outer);
                right.backwards((d == LEFT) ? outer : inner);
                currVal = outer;
        }
        /**
         * @brief Will turn with a non zero radius, determined by speed
         * 
         * @param d   Direction to turn
         * @param val Speed to turn (0 to 1)
         */
        void turn(Direction d, double val = 1) {
                (d == LEFT ? left : right).forwards(val / 2);
                (d == LEFT ? right : left).forwards(val);
                currVal = val;
        }

        double getSpeed() const {
                return currVal;
        }
};
}  // namespace JD

#endif
