//Erythraean - Junior Design Project
//Benjamin Santaus and Kelsey Foster
//Move Bot forward and Stop at Blue
//February 12, 2019

//Declare Color Input Threshold Constants
const int BLACKMIN = 0, BLACKMAX = 125, REDMIN = 400, REDMAX = 500,
          BLUEMIN = 240, BLUEMAX = 300, YELLOWMIN = 680, YELLOWMAX = 780;
//Declare color input pin (A13)
const int COLORPIN = 67;
//Declare motor output PWM pins
const int leftReverse = 8, leftForward = 9, rightReverse = 11, rightForward = 10;
//Color Constants
const int BLACK = 0, BLUE = 1, RED = 2, YELLOW = 3;
int currColor = -1, newColor = -1;
bool colorChange = false;


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
  resetPins(); //set all pins to LOW, just in case
  startForward(); //start bot moving forward
}

void loop() {
  delay(100); //wait 100ms
  int input = analogRead(COLORPIN); //read color input pin
  Serial.println(input);
  if (input > BLUEMIN && input < BLUEMAX) { //if blue
    if (!colorChange) colorChange = true;
    else {
      resetPins(); //stop
      colorChange = false;
    }
  } else {
    Serial.println("not blue"); //else print message
  }
}

void startForward() {
  resetPins(); //set to LOW, for sanity purposes
  analogWrite(leftForward, 100); //PWM to motors, left slower than right
  analogWrite(rightForward, 85);
}

void resetPins() {
  digitalWrite(leftReverse, LOW); //set all pins to LOW - stop motors.
  digitalWrite(rightReverse, LOW);
  digitalWrite(leftForward, LOW);
  digitalWrite(rightForward, LOW);
}
