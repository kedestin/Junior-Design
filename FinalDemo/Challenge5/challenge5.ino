#include "src/Calibration.h"
#include "src/DriveSystem.h"
#include "src/Protothread.h"
#include "src/Timer.h"
JD::DriveSystem ds;
void            setup() {}

void loop() {
        static JD::Timer timer;
        double           speed;
        JD::Calibration::get(JD::Calibration::driveAt_1_25_inch_s, speed);
        PT_BEGIN();
        ds.forwards(speed);
        timer.start(20000);
        PT_WAIT_UNTIL(timer.isFinished());
        ds.stop();
        PT_END();
}