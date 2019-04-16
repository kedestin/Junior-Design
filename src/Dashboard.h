#ifndef JD_DASHBOARD_H
#define JD_DASHBOARD_H

#include "DriveSystem.h"
#include "Led.h"
#include "SevenSegment.h"
#include "Updateable.h"
namespace JD {

class Dashboard : public Updateable {
private:
        LED                speedometer;
        SevenSegment       gearshifter;
        const DriveSystem& drivesystem;

public:
        Dashboard(LED l, SevenSegment gs, const DriveSystem& ds)
            : speedometer(l), gearshifter(gs), drivesystem(ds) {}

        void update() {
                speedometer.on(drivesystem.getSpeed());

                if (drivesystem.isForwards())
                        gearshifter.display('d');
                else if (drivesystem.isBackwards())
                        gearshifter.display('r');
                else if (drivesystem.isStopped())
                        gearshifter.display('n');
        }

        void gearshifterPark(){
                gearshifter.display('p');
        }
};
}  // namespace JD
#endif