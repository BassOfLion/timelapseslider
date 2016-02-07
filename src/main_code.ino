//Buttons
const int switchUpPin = 7;
const int switchDownPin = 8;

//Start and stop LEDs
const int greenLed = 13;
const int redLed = 12;

//4-bit LED display
const int clock = 4;
const int latch = 9;
const int data = 2;
const int onDisplayTime = 3000;
int displayState = HIGH;
const byte value[] = { B11000000, // 0
                       B11111001, // 1
                       B10100100, // 2
                       B10110000, // 3
                       B10011001, // 4
                       B10010010, // 5
                       B10000010, // 6
                       B11111000, // 7
                       B10000000, // 8
                       B10010000, // 9
                       B01111111, // dot .
                       B11111111 // display nothing
                     };
const byte digit[] = { B00000001, // left segment
                       B00000010,
                       B00000100,
                       B00001000,
                       B11111111
                     }; // right segment

//Motor controls
const int motorPin = 3;
unsigned long onTime = 1000;
long offTime = 0;
const int speed = 40;
const int stop = 0;
int motorState = LOW;

//Time controls
unsigned long previousMillis = 0;
unsigned long previousMillisDisplayTime = 0;

void setup()
{
  pinMode(motorPin, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(switchUpPin, OUTPUT);
  pinMode(switchDownPin, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(clock, OUTPUT);
  pinMode(latch, OUTPUT);
  pinMode(data, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  unsigned long currentTime = millis();

  if (digitalRead(switchUpPin) == LOW) {
    increaseButton();
    displayState = HIGH;
  }

  if (digitalRead(switchDownPin) == LOW) {
    decreaseButton();
    displayState = HIGH;
  }

  motorToggle(currentTime);

  if ((displayState == HIGH) && (currentTime - previousMillisDisplayTime >= onDisplayTime)) {
    displayState = LOW;
    previousMillisDisplayTime = currentTime;  // Remember the time
    analyzeLedOutput(offTime / 1000);
  }
  else if ((displayState == LOW) && (currentTime - previousMillisDisplayTime >= onDisplayTime)) {
    updateLedDisplay(11, 4);
  }
}

void motorToggle (unsigned long currentMillis ) {
  if ((motorState == LOW) && (currentMillis - previousMillis >= offTime)) {
    previousMillis = currentMillis;  // Remember the time
    motorState = HIGH;
    analogWrite(motorPin, speed);    // Update motor
    digitalWrite(greenLed, HIGH);  //Update Green LED
    digitalWrite(redLed, LOW); //Update Red LED
  }
  else if ((motorState == HIGH) && (currentMillis - previousMillis >= onTime)) {
    previousMillis = currentMillis;  // Remember the time
    motorState = LOW;
    analogWrite(motorPin, stop);    // Update
    digitalWrite(greenLed, LOW); //Update Green LED
    digitalWrite(redLed, HIGH); //Update Red LED
  }
}

void increaseButton () {
  offTime += 5000;
  Serial.println("FIRST IF and offTime: "); //debug
  Serial.println(offTime / 1000); //debug
  digitalWrite(switchUpPin, HIGH);
  delay(200);
}

void decreaseButton () {
  offTime -= 5000;
  Serial.println("SECOND IF and offTime: "); //debug
  Serial.println(offTime / 1000); //debug
  digitalWrite(switchDownPin, HIGH);
  delay(200);
}

void analyzeLedOutput (int x) {
  if (x >= 0 && x <= 9) {
    updateLedDisplay(x, 0);
  }

  else if (x >= 10 && x <= 99) {
    updateLedDisplay(x % 10, 0);
    updateLedDisplay(x / 10, 1);
  }
}

void updateLedDisplay (int val, int dig) {
  digitalWrite(latch, LOW);
  shiftOut(data, clock, MSBFIRST, value[val]);
  shiftOut(data, clock, MSBFIRST, digit[dig]);
  digitalWrite(latch, HIGH);
}


