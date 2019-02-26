void setup() {
  pinMode(66, INPUT);
  pinMode(51, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int mag = analogRead(66);
  if (mag < 900) {
    digitalWrite(51, HIGH);
  } else {
    digitalWrite(51, LOW);
  }
}
