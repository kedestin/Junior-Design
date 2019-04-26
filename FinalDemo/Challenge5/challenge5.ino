#include "src/Calibration.h"
#include "src/DriveSystem.h"
#include "src/Protothread.h"
#include "src/Timer.h"
/* For Loki */
JD::DriveSystem ds({10, 8, 238, 255}, {11, 12, 255, 248});

/* For Thor */
//JD::DriveSystem ds({10, 8, 240, 255}, {11, 12, 255, 210});

JD::Updateable* peripherals[] = {&ds};

void            setup() {
  /* For Loki */
  //double valueToSetCalibration = .1585;

  /* For Thor */
  double valueToSetCalibration = .23;
  
  JD::Calibration::set(JD::Calibration::driveAt_1_25_inch_s, valueToSetCalibration);
}

void loop() {
        static JD::Timer timer;
        double           speed;
        JD::Calibration::get(JD::Calibration::driveAt_1_25_inch_s, speed);
        PT_BEGIN();
        ds.forwards(speed);
        PT_WAIT_UNTIL(timer.hasElapsed(20000));
        ds.stop();
        PT_END();
}
