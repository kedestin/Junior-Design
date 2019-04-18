#include "src/DriveSystem.h"
#include "src/Protothread.h"
#include "src/Receiver.h"
#include "src/Switch.h"
#include "src/Timer.h"
#include "src/Transmitter.h"
#include "src/Updateable.h"
JD::Transmitter speaker(-1);
JD::Receiver    mic(-1);
JD::DriveSystem ds(-1, -1, -1, -1);
JD::Switch<1>   sw(-1);
JD::Updateable* peripherals[] = {&mic, &speaker, &sw, &ds};

void setup() {}

void runTest() {
        // The two bots communicate with each othersending commands with a 500
        // msmessage to have one of thebotsmove forwardtwelve inches, stop,
        // turn around 180°, stop,move backwardsthree inches, stop, turn left,
        // turn right, turn right, and turn right to end upas close as
        // possibleto the bot’sstartingposition. A 500 msmessage is sent to the
        // other bot to perform the same test.This test is ninety seconds.Each
        // bot has a designated mark  on  its  exterior  surface. At  the
        // start,  the  two  bots  have  their  marks  lined  up  with each
        // other.  At  the  end  of  the  challenge,  the  objective  is  to
        // have  the  marks  line  up.    The distance between the marks will
        // be measured;the closer they are to each other the better the
        // evaluation score.The objective is to make this distance as closeto
        // zero as possible.

        double _1_25_in_per_s;
        double distance_in;
        JD::Calibration::get(JD::Calibration::driveAt_1_25_inch_s,
                             _1_25_in_per_s);
        static JD::Timer timer;
        
        PT_BEGIN();
        
        // Move forwards 12 inches
        double distance_in = 12;
        timer.start(distance_in / 1.25 * 1000);
        ds.forwards(_1_25_in_per_s);
        PT_WAIT_UNTIL(timer.isFinished());

        // Rotate 180 degrees
        ds.rotateDeg(JD::DriveSystem::RIGHT, 180);
        
        // Move backwards 3 inches
        double distance_in = 12;
        timer.start(distance_in / 1.25 * 1000);
        ds.forwards(_1_25_in_per_s);
        PT_WAIT_UNTIL(timer.isFinished());

        // Rotate 90 degrees left
        ds.rotateDeg(JD::DriveSystem::LEFT, 90);
        
        // Rotate 90 degrees right
        ds.rotateDeg(JD::DriveSystem::RIGHT, 90);
        
        // Rotate 90 degrees right
        ds.rotateDeg(JD::DriveSystem::RIGHT, 90);
        
        // Rotate 90 degrees right
        ds.rotateDeg(JD::DriveSystem::RIGHT, 90);

        PT_END();
}

void loop() {
        for (auto p : peripherals)
                p->update();

        if (sw.read() == 0) {
                PT_BEGIN();
                runTest();
                speaker.send(500);
                PT_END();
        } else {
                PT_BEGIN();
                PT_WAIT_UNTIL(mic.receivedMsg() == JD::Receiver::msg500);
                runTest();
                PT_END();
        }
}