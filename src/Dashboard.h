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
        bool               isParked = false;

public:
        Dashboard(LED l, SevenSegment gs, const DriveSystem& ds)
            : speedometer(l), gearshifter(gs), drivesystem(ds) {}

        void update() {
                speedometer.on(drivesystem.getSpeed());

                if (drivesystem.isForwards())
                        gearshifter.display('d');
                else if (drivesystem.isBackwards())
                        gearshifter.display('r');
                else if (drivesystem.isStopped() && not isParked)
                        gearshifter.display('n');
                else if (drivesystem.isStopped() && isParked)
                        gearshifter.display('p');
        }

        void gearshifterPark() { isParked = true; }
};
}  // namespace JD
#endif