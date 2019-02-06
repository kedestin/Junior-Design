
#include "src/MotorInterface.h"


constexpr JD::MotorConfig<JD::Pin::Motor1f, JD::Pin::Motor1b, 255, 255> left;
constexpr JD::MotorConfig<JD::Pin::Motor2f, JD::Pin::Motor2b, 255, 255> right;

void setup() {
        Serial.begin(115200);
}


void loop() {
        // double output[] = {5.0, 4.5, 4.0, 3.5}
        // double output[] = {3.0, 2.5, 2.0, 1.5, 1.0};
        double output[] = {0.65};
        for (auto &o : output)
                o /= 5.0;

        for (double val : output) {
                Serial.print("Sending ");
                Serial.print(val * 5.0);
                Serial.println(" V");
                
                right.forwards(val);
                left.backwards(val);
                delay(500);

                // Serial.println("Stopping");
                // left.stop();
                // right.stop();
                // delay(100);

                // left.backwards(val);
                // right.backwards(val);
                // delay(4000);

                // left.stop();
                // right.stop();
                // delay(1000);
        }
}