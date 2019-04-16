#ifndef JUNIOR_DESIGN_DRIVE_SYSTEM
#define JUNIOR_DESIGN_DRIVE_SYSTEM

#include "Calibration.h"
#include "Led.h"
#include "MotorInterface.h"
#include "Updateable.h"
namespace JD {

/**
 * @brief Abstraction for a 2 wheel drive system
 *        Expects that motors must be told to spin in opposite directions to go
 *        forwards
 */
class DriveSystem : public Updateable {
private:
        MotorConfig left;
        MotorConfig right;

        // 0 if stopped
        // negative if going backwards
        // positive if going forwards
        double        currLeft = 0, currRight = 0;
        unsigned long response_ms = 0;  // Response time to get to target speed
        unsigned long endtime = 0, lastUpdated = 0;
        double        targetLeft = 0, targetRight = 0;
        void          rampUp(double left, double right) {
                targetLeft  = left;
                targetRight = right;
                lastUpdated = millis();
                endtime     = lastUpdated + response_ms;
        }
        unsigned long timeToRotate90degAtFullSpeed;
        unsigned long rotateStopAt = 0;

public:
        // enum PINOUT { Motor1f = 2, Motor1b = 3, Motor2b = 4, Motor2f = 5 };
        enum Direction { LEFT, RIGHT };

        DriveSystem() : left{2, 3, 255, 255}, right{4, 5, 255, 255} {
                JD::Calibration::get(JD::Calibration::rotate90AtFull_ms,
                                     timeToRotate90degAtFullSpeed);
        }

        DriveSystem(unsigned Motor1f, unsigned Motor1b, unsigned Motor2f,
                    unsigned Motor2b, unsigned long r_time_ms = 0)
            : left{Motor1f, Motor1b, 255, 255},
              right{Motor2f, Motor2b, 255, 255},
              response_ms(r_time_ms) {
                JD::Calibration::get(JD::Calibration::rotate90AtFull_ms,
                                     timeToRotate90degAtFullSpeed);
        }

        void forwards(double val = 1) {
                // left.forwards(val);
                // right.backwards(val);
                // currLeft = currRight = val;
                rampUp(val, val);
        }
        void stop() {
                forwards(0);
                // currLeft = currRight = 0;
        }

        void backwards(double val = 1) {
                // left.backwards(val);
                // right.forwards(val);
                // currLeft = currRight = -val;
                rampUp(-val, -val);
        }

        void update() {
                // For rotation
                if (rotateStopAt != 0 && millis() > rotateStopAt) {
                        stop();
                        rotateStopAt = 0;
                }

                // For ramping up
                if (endtime == lastUpdated && lastUpdated == 0)
                        return;

                double currTime = millis();

                if (currTime > endtime) {
                        // Force ending voltage to be target
                        if (targetLeft >= 0)
                                left.backwards(targetLeft);
                        else
                                left.forwards(-targetLeft);
                        if (targetRight < 0)
                                right.backwards(-targetRight);
                        else
                                right.forwards(targetRight);
                        currLeft  = targetLeft;
                        currRight = targetRight;
                        endtime = lastUpdated = 0;
                        return;
                }

                double slopeL =
                    (targetLeft - currLeft) / (endtime - lastUpdated);
                double slopeR =
                    (targetRight - currRight) / (endtime - lastUpdated);
                // Linear Interpolation: y = mx + b
                double leftUpdate =
                    slopeL * (currTime - lastUpdated) + currLeft;
                double rightUpdate =
                    slopeR * (currTime - lastUpdated) + currRight;

                if (leftUpdate >= 0)
                        left.backwards(leftUpdate);
                else
                        left.forwards(-leftUpdate);
                if (rightUpdate < 0)
                        right.backwards(-rightUpdate);
                else
                        right.forwards(rightUpdate);

                currLeft    = leftUpdate;
                currRight   = rightUpdate;
                lastUpdated = currTime;
        }

        /**
         * @brief Will turn with one wheel being center of rotation
         *
         * @param d   Direction to turn
         * @param val Speed to turn (0 to 1)
         */
        void pivot(Direction d, double val = 1) {
                rampUp(d == LEFT ? 0 : val, d == LEFT ? val : 0);
                // (d == LEFT ? left : right).stop();
                // (d == LEFT ? right : left).forwards(val);

                // (d == LEFT ? currLeft : currRight) = 0;
                // (d == LEFT ? currRight : currLeft) = val;
        }

        /**
         * @brief Will turn in place
         *
         * @param d   Direction to turn
         * @param val Speed to turn (0 to 1)
         */
        void rotate(Direction d, double val = 1) {
                // if (d == RIGHT) {
                //         left.forwards(val);
                //         right.forwards(val);
                //         currLeft  = val;
                //         currRight = -val;  // Motors are backwards
                // } else {
                //         left.backwards(val);
                //         right.backwards(val);
                //         currLeft  = -val;
                //         currRight = val;
                // }

                rampUp(d == RIGHT ? val : -val, d == RIGHT ? -val : val);
        }

        void rotateDeg(Direction d, double angle = 90) { 
                constexpr double callibrationAngle = 90;
                rotateStopAt = millis() + abs(angle/callibrationAngle) * timeToRotate90degAtFullSpeed;
                rotate(d);
         }
        /**
         * @brief Will turn using specified values for inner and outer motors
         *
         * @param d     Direction to turn
         * @param inner Speed for inner wheel (0 to 1)
         * @param outer Speed for outer wheel (0 to 1)
         */
        void turn(Direction d, double inner, double outer) {
                rampUp((d == LEFT) ? inner : outer,
                       (d == LEFT) ? outer : inner);
                // left.forwards((d == LEFT) ? inner : outer);
                // right.backwards((d == LEFT) ? outer : inner);

                // currLeft  = (d == LEFT) ? inner : outer;
                // currRight = (d == LEFT) ? outer : inner;
        }
        /**
         * @brief Will turn with a non zero radius, determined by speed
         *
         * @param d   Direction to turn
         * @param val Speed to turn (0 to 1)
         */
        void turn(Direction d, double val = 1) {
                rampUp(d == LEFT ? (val / 2) : val,
                       d == LEFT ? val : (val / 2));
                // left.forwards(d == LEFT ? (val / 2) : val);
                // right.backwards(d == LEFT ? val : (val / 2));

                // currLeft  = (d == LEFT) ? (val / 2) : val;
                // currRight = (d == LEFT) ? val : (val / 2);
        }

        bool   isForwards() const { return currLeft > 0 && currRight > 0; }
        bool   isBackwards() const { return currLeft < 0 && currRight < 0; }
        bool   isStopped() const { return currLeft == 0 && currRight == 0; }
        bool   isLeft() const { return abs(currLeft) < abs(currRight); };
        bool   isRight() const { return abs(currRight) < abs(currLeft); };
        double getSpeed() const {
                return abs((currLeft < currRight) ? currRight : currLeft);
        }
};
}  // namespace JD

#endif
