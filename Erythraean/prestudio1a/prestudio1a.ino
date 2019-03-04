#include <SoftwareSerial.h>
SoftwareSerial theSerialPort(10, 11); // RX, TX
void setup()
{
 // Use serial communications
 Serial.begin(9600);
 while (!Serial) {
 ; // port may need time to connect
 }
 Serial.println("Ready to go...");
 // set the data rate
Serial.begin(2400);
Serial.println("Hello, world?");
}
void loop() // continue until stopped
{
 if (Serial.available())
 Serial.write(Serial.read());
 if (Serial.available())
 Serial.write(Serial.read());
}
