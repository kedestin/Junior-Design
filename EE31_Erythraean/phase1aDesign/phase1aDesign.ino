const int redPin = 2;
const int greenPin = 3;
const int bluePin = 4;
const int potPin1 = 54;
const int potPin2 = 55;
const int switch1 = 18;
const int switch2 = 19;
const int switch3 = 20;
const int switch4 = 21;
const double HALFINPUT = 511;
const double MAXINPUT = 1023;
const double MAXBRIGHT = 255;
bool redOn = false, greenOn = false, blueOn = false;
volatile double blueDelay = 500;
volatile bool int1 = false;
volatile bool int2 = false;
volatile bool diag1 = false;
volatile bool diag2 = false;
long lastChange;
volatile long lastAction; //sleep checker
bool sleep = false;
  int prevInput1 = -1, prevInput2 = -1;

void setup() {
  // put your setup code here, to run once:
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(potPin1, INPUT);
  pinMode(potPin2, INPUT);
  pinMode(switch1, INPUT);
  attachInterrupt(digitalPinToInterrupt(switch1), changeBlueFreq, CHANGE);
  pinMode(switch2, INPUT);
  attachInterrupt(digitalPinToInterrupt(switch2), turnOnRed, CHANGE);
  pinMode(switch3, INPUT);
  attachInterrupt(digitalPinToInterrupt(switch3), intDiag1, CHANGE);
  pinMode(switch4, INPUT);
  attachInterrupt(digitalPinToInterrupt(switch4), intDiag2, CHANGE);
  lastChange = -1;

  Serial.begin(9600);
  while (!Serial) {
  }

  interrupts();

  onState();
}

void loop() {
  // put your main code here, to run repeatedly

  digitalWrite(redPin, LOW);
  redOn = false;

  //fadeBlueIn();
  greenRunState();

  if (sleep) {
    sleepState();
  }
}

void onState() {
  long startTime = millis();
  long currTime;

  do {
    currTime = millis();
    
    if (lastChange == -1 || currTime - lastChange >= 50) {
      lastChange = currTime;
      if (redOn == false) {
        digitalWrite(redPin, HIGH);
        redOn = true;
      } else {
        digitalWrite(redPin, LOW);
        redOn = false;
      }
    }
  } while (currTime - startTime < 3000);

  return;
}

void greenRunState() {
  long brightnessG = 255;
  double scaledBlue;
  const double FADE = 23.4375;
  const double greenFlash = 125;
  double scaledFade = FADE;
  long greenChange = 0, blueChange = 0;
  long currTime;
  int flash = 0;


  blueDelay = 500;
  int1 = false;
  int2 = false;
  digitalWrite(redPin, LOW);
 
  while (1) {
    int input = analogRead(potPin1);
    if (prevInput1 == -1 || abs(prevInput1 - input) >= 20) {
      prevInput1 = input;
      lastAction = millis();
    }
    scaledFade = input / HALFINPUT * FADE;

    input = analogRead(potPin2);
    if (prevInput2 == -1 || abs(prevInput2 - input) >= 20) {
      prevInput2 = input;
      lastAction = millis();
    }
    scaledBlue = input / MAXINPUT * MAXBRIGHT * 1.4;
    
    currTime = millis();
    if (brightnessG > 1) {
      if (greenChange == 0 || currTime - greenChange >= scaledFade) {
        flash = 0;
        greenOn = false;
        analogWrite(greenPin, brightnessG);
        brightnessG--;
        greenChange = currTime;
      }
    } else {
      if (currTime - greenChange >= greenFlash) {
        if (greenOn == true) {
          
          digitalWrite(greenPin, LOW);
          greenOn = false;
        } else {
          digitalWrite(greenPin, HIGH);
          greenOn = true;
          if (flash == 2) {
            brightnessG = 255;
          } else {
            flash++;
          }
        }
        greenChange = currTime;
      }
    }

    if (blueChange == 0 || currTime - blueChange >= blueDelay) {
      if (blueOn) {
        digitalWrite(bluePin, LOW);
        blueOn = false;
      } else {
        analogWrite(bluePin, scaledBlue);
        blueOn = true;
      }
      blueChange = currTime;
    }

    if (diag1) {
      diagnostic1();
      return;
    } else if (diag2) {
      diagnostic2();
      return;
    }

    if (currTime - lastAction >= 90000) {
      sleep = true;
      return;
    }
  }
  return;
}

void changeBlueFreq() {
  lastAction = millis();
  if (digitalRead(switch1) == LOW) return;
 
  if (!int1) {
    int1 = true;
    blueDelay = 50;
  }
  return;
}

void turnOnRed() {
  lastAction = millis();
  if (digitalRead(switch2) == HIGH) return;
  
  if (int1 && !int2) {
    int2 = true;
    digitalWrite(redPin, HIGH);
  }
  return;
}

void intDiag1() {
  lastAction = millis();
  Serial.println("why");
  if (digitalRead(switch3) == LOW) return;
  diag1 = true;
  return;
}

void intDiag2() {
  lastAction = millis();
  if (digitalRead(switch4) == LOW) return;
  diag2 = true;
  return;
}

void diagnostic1() {
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, LOW);

  for (int i = 0; i < 10; i++) {
    if (redOn == false) {
      digitalWrite(redPin, HIGH);
      redOn = true;
    } else {
      digitalWrite(redPin, LOW);
      redOn = false;
    }
    delay(250);
  }

  diag1 = false;
}

void diagnostic2() {
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, LOW);

  for (int i = 0; i < 16; i++) {
    if (redOn == false) {
      digitalWrite(redPin, HIGH);
      redOn = true;
    } else {
      digitalWrite(redPin, LOW);
      redOn = false;
    }
    delay(250);
  }

  diag2 = false;
}

void sleepState() {
  long entryTime = lastAction;
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, HIGH);
  long lastSwitch = 0;
  long currTime;

  int flashes = 0;
  int brightness = 255;

  
  do {
    currTime = millis();
    if (flashes < 4) {
      if (lastSwitch == 0 || currTime - lastSwitch >= 125) {
        if (blueOn) {
          digitalWrite(bluePin, LOW);
          blueOn = false;
          flashes++;
          lastSwitch = currTime;
        } else {
          digitalWrite(bluePin, HIGH);
          blueOn = true;
          lastSwitch = currTime;
        }
      }
    } else if (brightness > 0) {
      if (currTime - lastSwitch > 3.91) {
        analogWrite(bluePin, brightness);
        lastSwitch = currTime;
        brightness--;
      }
    } else {
      digitalWrite(bluePin, LOW);
    }
    
    int input = analogRead(potPin1);
    if (abs(prevInput1 - input) >= 20) {
      prevInput1 = input;
      lastAction = millis();
    }

    input = analogRead(potPin2);
    if (abs(prevInput2 - input) >= 20) {
      prevInput2 = input;
      lastAction = millis();
    }
  } while (lastAction == entryTime);

  sleep = false;
}
