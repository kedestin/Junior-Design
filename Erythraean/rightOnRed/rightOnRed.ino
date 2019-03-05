//Erythraean - Junior Design Project
//Benjamin Santaus and Kelsey Foster
//Right on Red, Left on Blue
//February 12, 2019

//Declare Color Input Threshold Constants
const int BLACKMIN = 0, BLACKMAX = 125, REDMIN = 320, REDMAX = 430,
          BLUEMIN = 240, BLUEMAX = 300, YELLOWMIN = 630, YELLOWMAX = 690;
//Declare color input pin (A13)
const int COLORPIN = 67;
//Declare motor output PWM pins
const int leftReverse = 11, leftForward = 10, rightReverse = 8, rightForward = 9;
//Color Constants
const int BLACK = 0, BLUE = 1, RED = 2, YELLOW = 3, OTHER = 4;
int currColor = -1, newColor = -1;
bool hitRed = false;
const int RINGSIZE = 5;
int input[RINGSIZE];
int ctr = 0;


void setup() {
  pinMode(leftForward, OUTPUT); //tell Arduino what pin is used for
  pinMode(leftReverse, OUTPUT);
  pinMode(rightForward, OUTPUT);
  pinMode(rightReverse, OUTPUT);
  pinMode(COLORPIN, INPUT);  
  interrupts(); //enable interrupts
  Serial.begin(9600); //enable serial port (communication with laptop)
  while (!Serial) {}
  Serial.println("Starting");
  clearRing();
  resetPins(); //set all pins to LOW, just in case
  startForward(); //start bot moving forward
}

void loop() {
  input[ctr] = analogRead(COLORPIN);
  int avg = avgColor();
  Serial.println(input[ctr]);
  Serial.print("avg: ");
  Serial.print(avg);
  Serial.println();
  if (avg > REDMIN && avg < REDMAX && !hitRed) {
    Serial.println("Found Red");
    startRightTurn();
    delay(1200);
    startForward();
    clearRing();
    hitRed = true;
  } else if (avg > BLUEMIN && avg < BLUEMAX && hitRed) {
    Serial.println("found Blue");
    startLeftTurn();
    delay(1200);
    startForward();
    delay(200);
    resetPins();
    while(1);
  }
  delay(50);
  ctr = (ctr + 1) % RINGSIZE;
}

void clearRing() {
  for (int i = 0; i < RINGSIZE; ++i) input[i] = 0;
}

int avgColor() {
  int sum = 0;
  int ct = 0;
  for (int i = 0; i < RINGSIZE; ++i) {
    sum += input[i];
    if (input[i] != 0) ct++;
  }
  return (sum / ct);
}

void startForward() {
  resetPins(); //set to LOW, for sanity purposes
  Serial.println("forward");
  analogWrite(leftForward, 85); //PWM to motors, left slower than right
  analogWrite(rightForward, 120);
}

void resetPins() {
  digitalWrite(leftReverse, LOW); //set all pins to LOW - stop motors.
  digitalWrite(rightReverse, LOW);
  digitalWrite(leftForward, LOW);
  digitalWrite(rightForward, LOW);
}

void startLeftTurn() {
  resetPins();
  analogWrite(rightForward, 85); //right forward pin to PWM
}

void startRightTurn() {
  resetPins();
  analogWrite(leftForward, 100); //left forward pin to PWM
}

void startSpinLeft() {
  resetPins();
  analogWrite(leftReverse, 75); //left reverse, right forward
  analogWrite(rightForward, 70); //produces spin left
}

void startSpinRight() {
  resetPins();
  analogWrite(leftForward, 70); //right reverse, left forward
  analogWrite(rightReverse, 70); //produces spin right
}
