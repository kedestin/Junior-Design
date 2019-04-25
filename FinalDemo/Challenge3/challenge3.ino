#include "src/Dashboard.h"
#include "src/DriveFeedback.h"
#include "src/DriveSystem.h"
#include "src/LED.h"
#include "src/Protothread.h"
#include "src/Timer.h"
#include "src/Switch.h"
#include "src/Horn.h"
// Ramp up time is 1 second
// constexpr unsigned long rampUpTime_ms = 1000;
constexpr unsigned long rampUpTime_ms = 1000;
// JD::DriveSystem         ds(8, 9, 10, 11, rampUpTime_ms);
// JD::DriveSystem         ds( 9, 11, 8, 10, rampUpTime_ms);
JD::DriveSystem         ds( 10, 8, 11, 12, rampUpTime_ms);
JD::DriveFeedback       df(45, 38, 25, 24, 27, 26, ds);
JD::Dashboard           dash(JD::LED(2),
                   JD::SevenSegment(53U, 52U, 34U, 47U, 49U, 51U, 50U, 255U),
                   ds);
JD::Horn                horn(6);
JD::LED                 blue(44);
JD::Updateable*         peripherals[] = {&ds, &df, &dash, &blue};
void                    setup() {
        Serial.begin(115200);
        df.headLightOn();
        blue.blink(2,0-1);
        horn.sendSequence(500, 100, 500, 100, 500);
        while(horn.isSending()) horn.update();
}
JD::Switch<1> sw(0); 
void loop() {
        const double speed = 0.75;
        static JD ::Timer timer;
        for (auto p : peripherals)
                p->update();


        PT_BEGIN();
        ds.forwards(speed);
        Serial.println("forwards");
        timer.start(1000 + rampUpTime_ms);
        PT_WAIT_UNTIL(timer.isFinished());
        // delay(20000);
        ds.backwards(speed);
        Serial.println("backwards");
        timer.start(1000 + rampUpTime_ms);
        PT_WAIT_UNTIL(timer.isFinished()); 
        // delay(20000);
        
        ds.stop();
        Serial.println("stop");
        timer.start(500 + rampUpTime_ms);
        PT_WAIT_UNTIL(timer.isFinished());
        // delay(20000);
        
        ds.turn(JD::DriveSystem::RIGHT, speed);
        Serial.println("turn right");
        timer.start(500 + rampUpTime_ms);
        PT_WAIT_UNTIL(timer.isFinished());
        // // delay(20000);
        
        ds.turn(JD::DriveSystem::LEFT, speed);
        Serial.println("turn left");
        timer.start(500 + rampUpTime_ms);
        PT_WAIT_UNTIL(timer.isFinished());
        // // delay(20000);
        ds.stop();
        PT_WAIT_UNTIL(ds.getSpeed() == 0);
        Serial.println("Parked");
        dash.gearshifterPark();
        timer.start(1000);
        PT_WAIT_UNTIL(timer.isFinished());
        // delay(20000);
        PT_END();
}