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
                speedometer.on(abs(drivesystem.getSpeed()));

                if (drivesystem.getSpeed() > 0)
                        gearshifter.display('d');
                else if (drivesystem.getSpeed() < 0)
                        gearshifter.display('r');
                else
                        gearshifter.display('n');
        }
};
}  // namespace JD
#endif