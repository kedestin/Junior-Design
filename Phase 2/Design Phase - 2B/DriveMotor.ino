

enum Pin : unsigned { Motor = 2 };

template <Pin p, int maxForwards, int maxBackwards>
struct MotorConfig {
        /**
         * @brief Drives the motor in the defined forwards orientation. 
         *        Can scale speed from 0 to 1;
         * 
         * @param val Modifier for speed
         */
        void forwards(double val = 1) const  { drive(val, maxForwards); }


        /**
         * @brief Drives the motor in the defined backwards orientation. 
         *        Can scale speed from 0 to 1;
         * 
         * @param val Modifier for speed
         */
        void backwards(double val = 1) const  { drive(val, maxBackwards); }


        /**
         * @brief Stops the motor
         * 
         */
        void stop() const { drive(0, 0); }

private:
        drive(double val, int maxVolt) const {
                val = (val > 1) ? 1 : (val < 0) ? 0 : val;
                analogWrite(p, maxVolt * val);
        }
};

constexpr MotorConfig<Pin::Motor, 255, -255> left;

void setup() {
        Serial.begin(115200);
}

void loop() {
        double output[] = {5.0, 4.5, 4.0, 3.5, 3.0, 2.5, 2.0, 1.5, 1.0};
        for (auto &o : output)
                o /= 5.0;

        for (double val : output) {
                Serial.print("Sending ");
                Serial.print(val * 5.0);
                Serial.println(" V");
                left.forwards();
                delay(4000);
                Serial.println("Stopping");
                left.stop();
                delay(1000);
        }
}