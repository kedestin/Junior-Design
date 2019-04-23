#ifndef JUNIOR_DESIGN_DRIVE_FEEDBACK
#define JUNIOR_DESIGN_DRIVE_FEEDBACK

#include "DriveSystem.h"
#include "Led.h"
#include "Protothread.h"
#include "Updateable.h"
namespace JD {

/**
 * @brief Abstraction for human perceivable feedback for various systems on the
 * bot
 * 
 *      * Headlights
 *      * Brakelights
 *      * TurnSignals
 *       
 */
class DriveFeedback : public Updateable {
private:
        struct PairLights : public Updateable {
                LED left, right;
                PairLights(uint8_t l, uint8_t r) : left(l), right(r) {}
                void on() {
                        off();
                        left.on();
                        right.on();
                }
                void off() {
                        left.off();
                        right.off();
                }
                void update() override {
                        left.update();
                        right.update();
                }
        };

        struct Headlights : public PairLights {
                Headlights(uint8_t l, uint8_t r) : PairLights(l, r) {}
        };
        struct Brakelights : public PairLights {
                Brakelights(uint8_t l, uint8_t r) : PairLights(l, r) {}
        };
        struct TurnSignal : public PairLights {
                TurnSignal(uint8_t l, uint8_t r) : PairLights(l, r) {}
                double frequency_hz = 2;

                void blinkRight() {
                        // left.off();
                        right.blink(2, 0 - 1);
                }
                void blinkLeft() {
                        // right.off();
                        left.blink(2, 0 - 1);
                }
        };

        Headlights         head;
        Brakelights        brake;
        TurnSignal         turn;
        const DriveSystem &ds;

public:
        DriveFeedback(uint8_t head_l, uint8_t head_r, uint8_t brake_l,
                      uint8_t brake_r, uint8_t turn_l, uint8_t turn_r,
                      const DriveSystem &newDs)
            : head(head_l, head_r),
              brake(brake_l, brake_r),
              turn(turn_l, turn_r),
              ds(newDs) {}

        void update() override {
                // head.update();
                brake.update();
                turn.update();

                if (ds.isBackwards())
                        brake.on();
                else
                        brake.off();

                if (ds.getSpeed() == 0)
                        headLightOff();
                else    
                        headLightOn();

                PT_BEGIN();
                if (ds.isLeft()) {
                        // Serial.println("blinkleft");
                        // turn.blinkLeft();
                        turn.right.off();
                        // Only call blink once, otherwise stays on
                        turn.left.blink(2, 10000);
                        PT_WAIT_UNTIL(not ds.isLeft());
                } else if (ds.isRight()) {
                        // Serial.println("blinkright");
                        // turn.blinkRight();
                        // Only call blink once, otherwise stays on
                        turn.right.blink(2, 10000);
                        turn.left.off();
                        PT_WAIT_UNTIL(not ds.isRight());
                } else {
                        turn.off();
                }
                PT_END();
                PT_RESTART();
        }

        void headLightOn() { head.on(); }

        void headLightOff() { head.off(); }

        void turnLightsOn() {turn.on();}

        void turnLightsOff() {turn.off();}
};
}  // namespace JD

#endif
