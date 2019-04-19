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
        unsigned long endtime = 0, lastUpdated = 0;
        double        targetLeft = 0, targetRight = 0;
#ifdef INTERPOLATE
        unsigned long response_ms = 0;  // Response time to get to target speed
        void          rampUp(double left, double right) {
                targetLeft  = left;
                targetRight = right;
                lastUpdated = millis();
                endtime     = lastUpdated + response_ms;
        }
#else
        double maxSlope = 1;

        void rampUp(double left, double right) {
                targetLeft  = left;
                targetRight = right;
                lastUpdated = millis();
        }
#endif
        unsigned long timeToRotate90degAtFullSpeed;
        unsigned long rotateStopAt = 0;

public:
        // enum PINOUT { Motor1f = 2, Motor1b = 3, Motor2b = 4, Motor2f = 5 };
        enum Direction { LEFT, RIGHT };

        DriveSystem() : left{2, 3, 255, 255}, right{4, 5, 255, 255} {
                JD::Calibration::get(JD::Calibration::rotate90AtFull_ms,
                                     timeToRotate90degAtFullSpeed);
        }

#ifdef INTERPOLATE
        DriveSystem(unsigned Motor1f, unsigned Motor1b, unsigned Motor2f,
                    unsigned Motor2b, unsigned long r_time_ms = 0)
            : left{Motor1f, Motor1b, 255, 255},
              right{Motor2f, Motor2b, 255, 255},
              response_ms(r_time_ms) {
                JD::Calibration::get(JD::Calibration::rotate90AtFull_ms,
                                     timeToRotate90degAtFullSpeed);
        }
#else
        DriveSystem(unsigned Motor1f, unsigned Motor1b, unsigned Motor2f,
                    unsigned Motor2b, unsigned long zeroToOne = 1)
            : left{Motor1f, Motor1b, 255, 255},
              right{Motor2f, Motor2b, 255, 255},
              maxSlope(1.0 / zeroToOne) {
                JD::Calibration::get(JD::Calibration::rotate90AtFull_ms,
                                     timeToRotate90degAtFullSpeed);
        }
#endif
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

#ifdef INTERPOLATE
        void update() {
                // For rotation
                if (rotateStopAt != 0 && millis() > rotateStopAt) {
                        stop();
                        rotateStopAt = 0;
                }

                // For ramping up
                if (currRight == targetRight && currLeft == targetLeft)
                        return;

                double currTime = millis();
                double leftUpdate, rightUpdate;

                if (currTime > endtime || endtime == lastUpdated) {
                        // Force ending voltage to be target
                        leftUpdate  = targetLeft;
                        rightUpdate = targetRight;
                        endtime = currTime = 0;
                } else {
                        double slopeL =
                            (targetLeft - currLeft) / (endtime - lastUpdated);
                        double slopeR = (targetRight - currRight) /
                                        (endtime - lastUpdated);

                        // Linear Interpolation: y = mx + b
                        leftUpdate =
                            slopeL * (currTime - lastUpdated) + currLeft;
                        rightUpdate =
                            slopeR * (currTime - lastUpdated) + currRight;
                }

                if (leftUpdate >= 0) {
                        // Serial.print(leftUpdate);
                        // Serial.println(" left forwards");
                        left.forwards(leftUpdate);
                } else {
                        // Serial.print(leftUpdate);
                        // Serial.println(" left backwards");
                        left.backwards(abs(leftUpdate));
                }

                if (rightUpdate >= 0) {
                        // Serial.print(rightUpdate);
                        // Serial.println(" right forwards");
                        right.backwards(rightUpdate);
                } else {
                        // Serial.print(rightUpdate);
                        // Serial.println(" right backwards");
                        right.forwards(abs(rightUpdate));
                }
                // Serial.print(currLeft, 7);
                // Serial.print(' ');
                // Serial.print(leftUpdate, 7);
                // Serial.print(' ');
                // Serial.print(targetLeft, 7);
                // Serial.print(" | ");
                // Serial.print(currRight, 7);
                // Serial.print(' ');
                // Serial.print(rightUpdate, 7);
                // Serial.print(' ');
                // Serial.print(targetRight, 7);
                // Serial.println(' ');
                currLeft    = leftUpdate;
                currRight   = rightUpdate;
                lastUpdated = currTime;
        }
#else
        void update() {
                // For rotation
                if (rotateStopAt != 0 && millis() > rotateStopAt) {
                        stop();
                        rotateStopAt = 0;
                }

                // For ramping up
                if (currRight == targetRight && currLeft == targetLeft)
                        return;

                double currTime = millis();
                double leftUpdate, rightUpdate;
                double timeChange = currTime - lastUpdated;
                Serial.println(abs(targetLeft - currLeft), 8);
                Serial.println(abs(targetRight - currRight), 8);
                Serial.println(maxSlope, 8);
                if ((abs(targetLeft - currLeft)) < maxSlope)
                        leftUpdate = targetLeft;
                else {
                        leftUpdate = ((targetLeft >= currLeft) * 2 - 1) *
                                         maxSlope * timeChange +
                                     currLeft;
                }

                if ((abs(targetRight - currRight)) < maxSlope)
                        rightUpdate = targetRight;
                else {
                        rightUpdate = ((targetRight >= currRight) * 2 - 1) *
                                          maxSlope * timeChange +
                                      currRight;
                }
                if (leftUpdate >= 0) {
                        // Serial.print(leftUpdate);
                        // Serial.println(" left forwards");
                        left.forwards(leftUpdate);
                } else {
                        // Serial.print(leftUpdate);
                        // Serial.println(" left backwards");
                        left.backwards(abs(leftUpdate));
                }

                if (rightUpdate >= 0) {
                        // Serial.print(rightUpdate);
                        // Serial.println(" right forwards");
                        right.backwards(rightUpdate);
                } else {
                        // Serial.print(rightUpdate);
                        // Serial.println(" right backwards");
                        right.forwards(abs(rightUpdate));
                }
                Serial.println(leftUpdate);
                Serial.println(rightUpdate);

                currLeft    = leftUpdate;
                currRight   = rightUpdate;
                lastUpdated = currTime;
                Serial.println();
        }
#endif
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
                rotateStopAt = millis() + abs(angle / callibrationAngle) *
                                              timeToRotate90degAtFullSpeed;
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
