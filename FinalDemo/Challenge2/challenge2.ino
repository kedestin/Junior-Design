#include "src/DriveSystem.h"
#include "src/Protothread.h"
#include "src/Receiver.h"
#include "src/Switch.h"
#include "src/Timer.h"
#include "src/Transmitter.h"
#include "src/Updateable.h"
#include "src/PWM.h"
JD::Transmitter speaker(30);
JD::Receiver    mic(A7);
//JD::DriveSystem ds({10, 8, 236, 255}, {11, 12, 236,255});  /* For Loki */
JD::DriveSystem ds({10, 8, 240, 230}, {11, 12, 235, 200}); /* For Thor */
JD::Switch<1>   sw(33);
JD::Updateable* peripherals[] = {&mic, &speaker, &sw, &ds};

double _1_25_in_per_s;
 double distance_in = 12;
void setup() {
  JD::setupPWM();
  Serial.begin(115200);
  //double valueToSetCalibration = .2;
  //JD::Calibration::set(JD::Calibration::driveAt_1_25_inch_s, valueToSetCalibration);
  //JD::Calibration::get(JD::Calibration::driveAt_1_25_inch_s, _1_25_in_per_s);
}

void runTest(bool& done) {
  ds.update();
        // The two bots communicate with each other sending commands with a 500
        // ms message to have one of the bots move forward twelve inches, stop,
        // turn around 180°, stop, move backwards three inches, stop, turn left,
        // turn right, turn right, and turn right to end up as close as
        // possible to the bot’s starting position. A 500 ms message is sent to the
        // other bot to perform the same test. This test is ninety seconds. Each
        // bot has a designated mark  on  its  exterior  surface. At  the
        // start,  the  two  bots  have  their  marks  lined  up  with each
        // other.  At  the  end  of  the  challenge,  the  objective  is  to
        // have  the  marks  line  up.    The distance between the marks will
        // be measured;the closer they are to each other the better the
        // evaluation score.The objective is to make this distance as closeto
        // zero as possible.

        static JD::Timer timer;
        PT_BEGIN();

        // Move forwards 12 inches
        distance_in = 12;
        ds.forwards(_1_25_in_per_s);


        
        /* Commands/Values for Loki */
        
//        PT_WAIT_UNTIL(timer.hasElapsed(distance_in / 3.5 * 1000));
        PT_WAIT_UNTIL(timer.hasElapsed(distance_in / 2 * 1000));
        ds.stop();
        PT_WAIT_UNTIL(timer.hasElapsed(200));
        
        // Rotate 180 degrees
        ds.rotate(JD::DriveSystem::RIGHT, 0.5);
        PT_WAIT_UNTIL(timer.hasElapsed(375 * 2.8));
        ds.stop();
        PT_WAIT_UNTIL(timer.hasElapsed(200));
        
        // Move backwards 3 inches
        distance_in = 3;
        ds.backwards(_1_25_in_per_s);
        PT_WAIT_UNTIL(timer.hasElapsed(distance_in / 2.3 * 1000));
        ds.stop();
        PT_WAIT_UNTIL(timer.hasElapsed(200));
        
        // Rotate 90 degrees left
        ds.rotate(JD::DriveSystem::LEFT, 0.5);
        PT_WAIT_UNTIL(timer.hasElapsed(375 * 1.5));
        ds.stop();
        PT_WAIT_UNTIL(timer.hasElapsed(800));
        
        // Rotate 90 degrees right
        ds.rotateDeg(JD::DriveSystem::RIGHT, 0.4);
        PT_WAIT_UNTIL(timer.hasElapsed(375 * 1.1));
        ds.stop();
        PT_WAIT_UNTIL(timer.hasElapsed(500));
        
        // Rotate 90 degrees right
        ds.rotate(JD::DriveSystem::RIGHT, 0.5);
        PT_WAIT_UNTIL(timer.hasElapsed(375* 1.2));
        ds.stop();
        PT_WAIT_UNTIL(timer.hasElapsed(500));
        
        // Rotate 90 degrees right
        ds.rotate(JD::DriveSystem::RIGHT, 0.5);
        PT_WAIT_UNTIL(timer.hasElapsed(375* 1.2));
        ds.stop();


        /* Commands/Values for Thor */
        
//        PT_WAIT_UNTIL(timer.hasElapsed(distance_in / 2.1 * 1000));
//        ds.stop();
//        PT_WAIT_UNTIL(timer.hasElapsed(200));
       
//        // Rotate 180 degrees
//        ds.rotate(JD::DriveSystem::RIGHT, 0.5);
//        PT_WAIT_UNTIL(timer.hasElapsed(375 * 3));
//        ds.stop();
//        PT_WAIT_UNTIL(timer.hasElapsed(200));
       
//        // Move backwards 3 inches
//        distance_in = 3;
//        ds.backwards(_1_25_in_per_s);
//        PT_WAIT_UNTIL(timer.hasElapsed(distance_in / 2 * 1000));
//        ds.stop();
//        PT_WAIT_UNTIL(timer.hasElapsed(200));
       
//        // Rotate 90 degrees left
//        ds.rotate(JD::DriveSystem::LEFT, 0.5);
//        PT_WAIT_UNTIL(timer.hasElapsed(375 * 1.7));
//        ds.stop();
//        PT_WAIT_UNTIL(timer.hasElapsed(800));
       
//        // Rotate 90 degrees right
//        ds.rotateDeg(JD::DriveSystem::RIGHT, 0.4);
//        PT_WAIT_UNTIL(timer.hasElapsed(375 * 1));
//        ds.stop();
//        PT_WAIT_UNTIL(timer.hasElapsed(500));
       
//        // Rotate 90 degrees right
//        ds.rotate(JD::DriveSystem::RIGHT, 0.5);
//        PT_WAIT_UNTIL(timer.hasElapsed(375* 1.25));
//        ds.stop();
//        PT_WAIT_UNTIL(timer.hasElapsed(500));
       
//        // Rotate 90 degrees right
//        ds.rotate(JD::DriveSystem::RIGHT, 0.5);
//        PT_WAIT_UNTIL(timer.hasElapsed(375* 1.25));
//        ds.stop();

        
         PT_END();
         done = true;
}

void loop() {
        JD::Timer timer;

        for (auto p : peripherals) {
                p->update();
        }

        if (sw.read() == 0) {
                bool isDone = false;
                while(mic.receivedMsg() == JD::Receiver::msgNone) {
                        mic.update();
                        Serial.println("still looking?");
                }
                while(!isDone){
                  runTest(isDone);
                }
                speaker.send(500);
                while (!timer.hasElapsed(800)) speaker.update();
                while(1);
        } else {
                bool isDone = false;
                speaker.send(500);
                unsigned long startTime = millis();
                while (!timer.hasElapsed(800)) {
                        speaker.update();
                }
                mic.receivedMsg();
                while(mic.receivedMsg() != JD::Receiver::msg500) mic.update();
                while(!isDone) {
                  runTest(isDone);
                }
                while(1);
        }
}
