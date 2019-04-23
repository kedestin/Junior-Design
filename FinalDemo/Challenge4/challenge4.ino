#include "src/DriveFeedback.h"
#include "src/DriveSystem.h"
#include "src/Updateable.h"
#include "src/Timer.h"
#include "src/Sensor.h"

JD::DriveSystem ds(10, 8, 11, 12, 20);
JD::DriveFeedback df(45, 38, 25, 24, 27, 26, ds);
JD::Sensor proximity(A5);

JD::Updateable* peripherals[] = {&ds, /*&df,*/ &proximity};


void setup() {
//        Serial.begin(115200);
//        while(!Serial);
//        Serial.println("Starting");
        df.headLightOn();
}

void loop() {
        // The  swarmbot  must  drive atnight  (dimroomlighting  conditions)
        // using only headlights  (mum  two  white  LEDs) toward  each
        // other and  avoid a  collision.  The  test  will  be  to  place  the
        // bots  at  opposite  ends of  the  longer dimension  of  the  track.
        // The  two  white  diodes represented  the  headlightsallowed  as
        // sources  of  illumination  for this challenge.    A  photodiode,
        // phototransistor,  or  photo  resistor  is  to be  used  as  the
        // detector.  When the two bots become close enough to detect each
        // other’s presence, theymust  stop,  communicate,  flash  their
        // headlights  twice,  and  illuminate  their  rear  yellow turn
        // signals.

        for (auto p : peripherals)
                p->update();

        static JD::Timer timer;

        PT_BEGIN();

//        Serial.println("drive?");
        ds.forwards(0.2);
        PT_WAIT_UNTIL(proximity.read() > 100);

        // PT_WAIT_UNTIL(static_assert(false, "Detect light"), false);

        // When the two bots become close enough to detect each other’s
        // presence, they must  stop,  communicate,  flash  their  headlights
        // twice,  and  illuminate  their  rear  yellow turn signals.
        ds.stop();
        //coMmuNiCAte
        PT_WAIT_UNTIL(timer.hasElapsed(1000));
        df.headLightOff();
        PT_WAIT_UNTIL(timer.hasElapsed(500));
        df.headLightOn();
        PT_WAIT_UNTIL(timer.hasElapsed(500));
        df.headLightOff();
        PT_WAIT_UNTIL(timer.hasElapsed(500));
        df.headLightOn();
        PT_WAIT_UNTIL(timer.hasElapsed(500));
        df.turnLightsOn();

        while(1);

        
        // static_assert(false, "Flash headlights twice");
        // static_assert(false, "Illuminate rear yellow turn signals");
        PT_END();
}
