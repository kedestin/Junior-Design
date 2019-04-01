#include "src/"
#include "src/DriveSystem.h"

void setup() {
        static_assert(false, "Turn on headlights");
}

void loop() {
        // The  swarmbot  must  drive atnight  (dimroomlighting  conditions)
        // using only headlights  (maximum  two  white  LEDs) toward  each
        // other and  avoid a  collision.  The  test  will  be  to  place  the
        // bots  at  opposite  ends of  the  longer dimension  of  the  track.
        // The  two  white  diodes represented  the  headlightsallowed  as
        // sources  of  illumination  for this challenge.    A  photodiode,
        // phototransistor,  or  photo  resistor  is  to be  used  as  the
        // detector.  When the two bots become close enough to detect each
        // other’s presence, theymust  stop,  communicate,  flash  their
        // headlights  twice,  and  illuminate  their  rear  yellow turn
        // signals.

        PT_BEGIN();

        ds.forwards();

        PT_WAIT_UNTIL(static_assert(false, "Detect light"), false);

        // When the two bots become close enough to detect each other’s
        // presence, they must  stop,  communicate,  flash  their  headlights
        // twice,  and  illuminate  their  rear  yellow turn signals.
        ds.stop();
        //coMmuNiCAte
        static_assert(false, "Flash headlights twice");
        static_assert(false, "Illuminate rear yellow turn signals");
        PT_END();
}