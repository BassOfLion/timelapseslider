//Buttons
//const int offButton = 12;
const int switchDirection = 12;
const int switchDownPin = 11;
const int switchUpPin = 10;
int offButtonState = HIGH;

//Start and stop LEDs
const int greenLed = 6;
const int redLed = 5;

//4-bit LED display
const int clock = 7;
const int latch = 8;
const int data = 9;
const int onDisplayTime = 5000;
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
const int inOne = 2;
const int inTwo = 4;
unsigned long onTime = 2000;
long offTime = 0;
const int speed = 255;
const int stop = 0;
int motorState = LOW;
boolean reverse = false;

//Time controls
unsigned long previousMillis = 0;
unsigned long previousMillisDisplayTime = 0;

void setup()
{
  pinMode(motorPin, OUTPUT);
  pinMode(inOne, OUTPUT);
  pinMode(inTwo, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(switchUpPin, INPUT_PULLUP);
  pinMode(switchDownPin, INPUT_PULLUP);
  //  pinMode(offButton, INPUT_PULLUP);
  pinMode(switchDirection, INPUT_PULLUP);
  pinMode(redLed, OUTPUT);
  pinMode(clock, OUTPUT);
  pinMode(latch, OUTPUT);
  pinMode(data, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
//  onOffToggle();

  //  if (offButtonState == LOW) {
  unsigned long currentTime = millis();

  if (digitalRead(switchUpPin) == LOW) {
    increaseButton();
    previousMillisDisplayTime = currentTime;
  }
  if (digitalRead(switchDownPin) == LOW) {
    decreaseButton();
    previousMillisDisplayTime = currentTime;
  }
  if (digitalRead(switchDirection) == LOW) {
    reverse = !reverse;
    delay(200);
  }

  motorToggle(currentTime);

  if (displayState == HIGH && (currentTime - previousMillisDisplayTime <= onDisplayTime)) {
    analyzeLedOutput(offTime / 1000); //Updating LED display during ON time
  } else {
    displayState = LOW;
    analyzeLedOutput(-1); //Turning LED display OFF until button is pressed
  }
  //  }
  //  else {
  //    turnOff(); //turning everything off
  //  }
}

void turnOff() {
  analogWrite(motorPin, stop); // Update
  digitalWrite(redLed, LOW); //Update Red LED
  digitalWrite(greenLed, LOW); //Update Green LED
  offTime = 0; //Reset OFF time
  analyzeLedOutput(-1);
}

//void onOffToggle() {
//  if (digitalRead(offButton) == LOW && offButtonState == HIGH) {
//    offButtonState = LOW;
//    delay(300);
//  } else if (digitalRead(offButton) == LOW && offButtonState == LOW) {
//    offButtonState = HIGH;
//    delay(300);
//  }
//}

void motorToggle (unsigned long currentMillis ) {
  if ((motorState == LOW) && (currentMillis - previousMillis >= offTime)) {
    previousMillis = currentMillis;  // Remember the time
    motorState = HIGH;
    analogWrite(motorPin, speed);    // Update motor
    digitalWrite(inOne, !reverse);
    digitalWrite(inTwo, reverse);
    digitalWrite(greenLed, HIGH);  //Update Green LED
    digitalWrite(redLed, LOW); //Update Red LED
  } else if ((motorState == HIGH) && (currentMillis - previousMillis >= onTime)) {
    previousMillis = currentMillis;  // Remember the time
    motorState = LOW;
    analogWrite(motorPin, stop);    // Update
    digitalWrite(greenLed, LOW); //Update Green LED
    digitalWrite(redLed, HIGH); //Update Red LED
  }
}

void increaseButton () {
  offTime += 5000;
  displayState = HIGH;
  Serial.println("FIRST IF and offTime: "); //debug
  Serial.println(offTime / 1000); //debug
  digitalWrite(switchUpPin, HIGH);
  delay(200);
}

void decreaseButton () {
  offTime -= 5000;
  displayState = HIGH;
  Serial.println("SECOND IF and offTime: "); //debug
  Serial.println(offTime / 1000); //debug
  digitalWrite(switchDownPin, HIGH);
  delay(200);
}

void analyzeLedOutput (int x) {
  if (x < 0) {
    updateLedDisplay(11, 4);
  } else if (x >= 0 && x <= 9) {
    updateLedDisplay(x, 0);
  } else if (x >= 10 && x <= 99) {
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
