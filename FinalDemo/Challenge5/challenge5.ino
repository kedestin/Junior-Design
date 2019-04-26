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
  double valueToSetCalibration = .20;
  
  JD::Calibration::set(JD::Calibration::driveAt_1_25_inch_s, valueToSetCalibration);
}

void loop() {
      ds.update();
        static JD::Timer timer, stopTimer;
        static int init = 0;
        double           speed = 1;
        double  period = 100, ratio = 0.14;
        // JD::Calibration::get(JD::Calibration::driveAt_1_25_inch_s, speed);
      
        if (not init){
          stopTimer.start(20000);
          init = 1;
        }
        if (stopTimer.isFinished())
          exit(1);
        
        PT_BEGIN();
        ds.forwards(speed);
        PT_WAIT_UNTIL(timer.hasElapsed(ratio * period));
        ds.stop();
        PT_WAIT_UNTIL(timer.hasElapsed((1-ratio) * period));
        PT_END();
        PT_RESTART();


        // PT_BEGIN();
        // ds.forwards(speed);
        // PT_WAIT_UNTIL(timer.hasElapsed(20000));
        // ds.stop();
        // PT_END();
}
