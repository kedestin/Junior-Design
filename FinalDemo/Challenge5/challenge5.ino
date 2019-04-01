#include "src/DriveSystem.h"
#include "src/Timer.h"
#include "src/Protothread.h"
JD::DriveSystem ds;
void            setup() {}

void loop() {
        static JD::Timer timer;
        PT_BEGIN();
        ds.forwards();
        static_assert(false, "Drive at specific speed");
        timer.start(20000);
        PT_WAIT_UNTIL(timer.isFinished());
        ds.stop();
        PT_END();
}