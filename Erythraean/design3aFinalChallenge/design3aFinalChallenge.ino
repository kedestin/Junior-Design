//Erythraean - Junior Design Project
//Benjamin Santaus and Kelsey Foster
//Follow Blue, Turn on Red, Follow Red, Stop at Yellow
//February 12, 2019

//Declare Color Input Threshold Constants
const int BLACKMIN = 0, BLACKMAX = 125, REDMIN = 270, REDMAX = 410,
          BLUEMIN = 150, BLUEMAX = 240, YELLOWMIN = 420, YELLOWMAX = 570;
//Declare color input pin (A13)
const int COLORPIN = 67;
//Declare motor output PWM pins
const int leftReverse = 11, leftForward = 10, rightReverse = 8, rightForward = 9;
//Declare LED output pins
const int BLUEPIN = 51, REDPIN = 53;
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
  pinMode(BLUEPIN, OUTPUT);
  pinMode(REDPIN, OUTPUT);
  interrupts(); //enable interrupts
  Serial.begin(9600); //enable serial port (communication with laptop)
  while (!Serial) {}
  Serial.println("Starting");
  clearRing();
  resetPins(); //set all pins to LOW, just in case
  startForward(); //start bot moving forward
  followBlue();
  clearRing();
  startRightTurn();
  delay(1000);
  startForward();
  followRed();
}

void loop() {
  //unused - looping done in follow functions
}

void followBlue() {
  digitalWrite(BLUEPIN, HIGH);
  while(1) {
    delay(100); //wait 100ms
    input[ctr] = analogRead(COLORPIN);
    int avg = avgColor();
    //Serial.println(input[ctr]);
    //Serial.print("avg: ");
    //Serial.print(avg);
    //Serial.println();
    if (avg < BLUEMIN || avg > BLUEMAX) { //if not blue
      delay(100);
      resetPins();
      digitalWrite(BLUEPIN, LOW);
      digitalWrite(REDPIN, HIGH);
      for (int i = 0; i < RINGSIZE; ++i) {
        ctr = (ctr + 1) % RINGSIZE;
        input[ctr] = analogRead(COLORPIN);
        delay(100);
      }
      avg = avgColor();
      Serial.println("in red check");
      Serial.println(avg);
      if (avg > REDMIN && avg < REDMAX) {
        clearRing();
        return;
      }
      clearRing();
      digitalWrite(BLUEPIN, HIGH);
      digitalWrite(REDPIN, LOW);
      //Serial.println("not red");
      resetPins(); //stop
      int startSpin = millis();
      int lastRead = startSpin;
      bool leftSpin = false;
      int spinTime = 200;
      ctr = (ctr + 1) % RINGSIZE;
      input[ctr] = analogRead(COLORPIN);
      avg = avgColor();
      while (avg < BLUEMIN || avg > BLUEMAX) {
        int currTime = millis();
        if (currTime - startSpin > spinTime) {
          if (leftSpin) {
            startSpin = currTime;
            startSpinRight();
            leftSpin = false;
            spinTime += 300;
          } else {
            startSpin = currTime;
            startSpinLeft();
            leftSpin = true;
            spinTime += 300;
          }
        }
        if (currTime - lastRead > 20) {
          lastRead = currTime;
          ctr = (ctr + 1) % RINGSIZE;
          input[ctr] = analogRead(COLORPIN);
          avg = avgColor();
          //Serial.println(avg);
        }
      }
      resetPins();
      startForward();
      clearRing();
    } else {
      ctr = (ctr + 1) % RINGSIZE;
      //Serial.println("on blue"); //else print message
    }
  }
}

void followRed() {
  digitalWrite(REDPIN, HIGH);
  while(1) {
    delay(100); //wait 100ms
    input[ctr] = analogRead(COLORPIN);
    int avg = avgColor();
    Serial.println(input[ctr]);
    Serial.print("avg: ");
    Serial.print(avg);
    Serial.println();
    if (avg < REDMIN || avg > REDMAX) { //if not blue
      delay(50);
      resetPins();
      digitalWrite(BLUEPIN, HIGH);
      for (int i = 0; i < RINGSIZE; ++i) {
        ctr = (ctr + 1) % RINGSIZE;
        input[ctr] = analogRead(COLORPIN);
        delay(50);
      }
      ctr = (ctr + 1) % RINGSIZE;
      avg = avgColor();
      if (avg > YELLOWMIN && avg < YELLOWMAX) {
        Serial.println("done");
        resetPins();
        while(1);
      }
      clearRing();
      digitalWrite(BLUEPIN, LOW);
      Serial.println("else");
      resetPins(); //stop
      int startSpin = millis();
      int lastRead = startSpin;
      bool leftSpin = false;
      int spinTime = 200;
      ctr = (ctr + 1) % RINGSIZE;
      input[ctr] = analogRead(COLORPIN);
      while (avg < REDMIN || avg > REDMAX) {
        int currTime = millis();
        if (currTime - startSpin > spinTime) {
          if (leftSpin) {
            startSpin = currTime;
            startSpinRight();
            leftSpin = false;
            spinTime += 300;
          } else {
            startSpin = currTime;
            startSpinLeft();
            leftSpin = true;
            spinTime += 300;
          }
        }
        if (currTime - lastRead > 75) {
          lastRead = currTime;
          ctr = (ctr + 1) % RINGSIZE;
          input[ctr] = analogRead(COLORPIN);
          avg = avgColor();
          Serial.println(avg);
        }
      }
      resetPins();
      startForward();
      clearRing();
    } else {
      Serial.println("on RED"); //else print message
      ctr = (ctr + 1) % RINGSIZE;
    }
  }
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
    //Serial.println(ct);
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
  analogWrite(rightForward, 30);
}

void startSpinLeft() {
  resetPins();
  analogWrite(leftReverse, 85); //left reverse, right forward
  analogWrite(rightForward, 70); //produces spin left
}

void startSpinRight() {
  resetPins();
  analogWrite(leftForward, 70); //right reverse, left forward
  analogWrite(rightReverse, 70); //produces spin right
}
