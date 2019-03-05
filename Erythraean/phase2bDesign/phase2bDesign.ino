//set pins for PWM
const int leftReverse = 8, leftForward = 9, rightReverse = 11, rightForward = 10;

void setup() {
  //init pins for output
  pinMode(leftForward, OUTPUT);
  pinMode(leftReverse, OUTPUT);
  pinMode(rightForward, OUTPUT);
  pinMode(rightReverse, OUTPUT);
  interrupts();
  Serial.begin(9600); //start serial connection
  while (!Serial) {}
  delay(5000);
  resetPins(); //set all to LOW
}

void loop() {
  // put your main code here, to run repeatedly:
  startForward(); //perform test sequence
  delay(1000);
  startReverse();
  delay(1000);
  startLeftTurn();
  delay(1000);
  startRightTurn();
  delay(1000);
  startSpinLeft();
  delay(1000);
  startSpinRight();
  delay(1000);
}

void resetPins() {
  digitalWrite(leftReverse, LOW); //set all to low (stop bot) 
  digitalWrite(rightReverse, LOW);
  digitalWrite(leftForward, LOW);
  digitalWrite(rightForward, LOW);
}

void startForward() {
  resetPins();
  delay(1000); //let motors and bot stop
  analogWrite(leftForward, 100); //set forward pins to PWM
  analogWrite(rightForward, 85); //reverse pins LOW
}

void startReverse() {
  resetPins();
  delay(1000);
  analogWrite(leftReverse, 95); //reverse pins to PWM
  analogWrite(rightReverse, 100); //forward pins LOW
}

void startLeftTurn() {
  resetPins();
  delay(1000);
  analogWrite(rightForward, 85); //right forward pin to PWM
}

void startRightTurn() {
  resetPins();
  delay(1000);
  analogWrite(leftForward, 100); //left forward pin to PWM
}

void startSpinLeft() {
  resetPins();
  delay(1000);
  analogWrite(leftReverse, 95); //left reverse, right forward
  analogWrite(rightForward, 85); //produces spin left
}

void startSpinRight() {
  resetPins();
  delay(1000);
  analogWrite(leftForward, 100); //right reverse, left forward
  analogWrite(rightReverse, 100); //produces spin right
}
