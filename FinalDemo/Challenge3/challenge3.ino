#include "src/Dashboard.h"
#include "src/DriveFeedback.h"
#include "src/DriveSystem.h"
#include "src/LED.h"
#include "src/Protothread.h"
#include "src/Timer.h"
// Ramp up time is 1 second
constexpr unsigned long rampUpTime_ms = 1000;
JD::DriveSystem         ds(8, 9, 10, 11, rampUpTime_ms);
JD::DriveFeedback       df(45, 38, 25, 24, 27, 26, ds);
JD::Dashboard           dash(JD::LED(2),
                   JD::SevenSegment(53U, 52U, 34U, 47U, 49U, 51U, 50U, -1U),
                   ds);
JD::LED                 blue(44);
JD::Updateable*         peripherals[] = {&ds, &df, &dash, &blue};
void                    setup() {
        Serial.begin(115200);
}
void loop() {
        for (auto p : peripherals)
                p->update();

        static JD ::Timer timer;
        blue.blink(2,0-1);

        PT_BEGIN();
        ds.forwards();
        Serial.println("forwards");
        timer.start(2000 + rampUpTime_ms);
        PT_WAIT_UNTIL(timer.isFinished());

        ds.backwards();
        Serial.println("backwards");
        timer.start(2000 + rampUpTime_ms);
        PT_WAIT_UNTIL(timer.isFinished());
        
        ds.stop();
        Serial.println("stop");
        timer.start(2000 + rampUpTime_ms);
        PT_WAIT_UNTIL(timer.isFinished());
        
        ds.turn(JD::DriveSystem::RIGHT);
        Serial.println("turn right");
        timer.start(2000 + rampUpTime_ms);
        PT_WAIT_UNTIL(timer.isFinished());
        
        ds.turn(JD::DriveSystem::LEFT);
        Serial.println("turn left");
        timer.start(2000 + rampUpTime_ms);
        PT_WAIT_UNTIL(timer.isFinished());
        Serial.println("Parked");
        dash.gearshifterPark();
        timer.start(2000);
        PT_WAIT_UNTIL(timer.isFinished());
        PT_END();
        
        PT_RESTART();
}