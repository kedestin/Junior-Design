volatile bool forward = true;
volatile bool change = false;
int forwardBottom = 5, forwardTop = 2, reverseTop = 3, reverseBottom = 4;
int switchPin = 18;

void setup() {
  pinMode(forwardBottom, OUTPUT);
  pinMode(forwardTop, OUTPUT);
  pinMode(reverseBottom, OUTPUT);
  pinMode(reverseTop, OUTPUT);
  pinMode(switchPin, INPUT);
  interrupts();
  attachInterrupt(digitalPinToInterrupt(18), switchDir, CHANGE);
  Serial.begin(9600);
  while (!Serial) {}
  digitalWrite(reverseBottom, LOW);
  digitalWrite(reverseTop, LOW);
  startForward();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (forward && change) {
    digitalWrite(reverseBottom, LOW);
    digitalWrite(reverseTop, LOW);
    change = false;
    startForward();
  } else if (change) {
    digitalWrite(forwardBottom, LOW);
    digitalWrite(forwardTop, LOW);
    change = false;
    startReverse();
  }

}

void switchDir() {
  if (forward) {
    forward = false;
    change = true;
  } else {
    forward = true;
    change = true;
  }
}

void startForward() {
  delay(1000);
  digitalWrite(forwardBottom, HIGH);
  analogWrite(forwardTop, 255);
  int i = 0;
  while (i < 1000 && forward) {
    i++;
  }
  analogWrite(forwardTop, 255);
  Serial.println("forward");
}

void startReverse() {
  delay(1000);
  digitalWrite(reverseBottom, HIGH);
  analogWrite(reverseTop, 255);
  int i = 0;
  while (i < 1000 && !forward) {
    i++;
  }
  analogWrite(reverseTop, 200);
  Serial.println("reverse");
}
