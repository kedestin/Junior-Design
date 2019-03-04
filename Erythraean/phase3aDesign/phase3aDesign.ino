const int BLACKMIN = 0, BLACKMAX = 125, REDMIN = 166, REDMAX = 313,
          BLUEMIN = 520, BLUEMAX = 677, YELLOWMIN = 729, YELLOWMAX = 938;
const int INPIN = 67, BLUEPIN = 51, REDPIN = 53;

void setup() {
  // put your setup code here, to run once:
  pinMode(INPIN, INPUT);
  pinMode(BLUEPIN, OUTPUT);
  pinMode(REDPIN, OUTPUT);
  //digitalWrite(BLUEPIN, HIGH);
  digitalWrite(REDPIN, HIGH);
  Serial.begin(9600);
  while (!Serial) {}
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(500);
  int input = analogRead(INPIN);
  Serial.println(input);
  /*if (input < BLACKMAX) {
    Serial.println("Black");
  } else if (input > REDMIN && input < REDMAX) {
    Serial.println("Red");
  } else if (input > BLUEMIN && input < BLUEMAX) {
    Serial.println("Blue");
  } else if (input > YELLOWMIN && input < YELLOWMAX) {
    Serial.println("Yellow");
  } else {
    Serial.println("Recalibrate.");
  }*/
}
