
/*
  Project: Skeeball
  Author: Russell Hueske
  Email: hueske.russ690@gmail.com
*/

// Pin initalization

int clockPin = 7;
int latchPin = 8;
int dataPin = 9;

int buttonIndex = 0;

byte switchVar1 = 72; //01001000

void setup() {

  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, INPUT);

}

void loop() {

  digitalWrite(latchPin, 1);

  delayMicroseconds(20);

  digitalWrite(latchPin, 0);

  switchVar1 = shiftIn(dataPin, clockPin);
  getInput();

}

byte shiftIn(int myDataPin, int myClockPin) {
  int i;
  int temp = 0;
  int pinState;
  byte myDataIn = 0;

  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, INPUT);

  for (i = 7; i >= 0; i --) {
    digitalWrite(myClockPin, 0);
    delayMicroseconds(0.2);
    temp = digitalRead(myDataPin);
    if (temp) {
      pinState = 1;
      myDataIn = myDataIn | (1 << i);
    } else {
      pinState = 0;
    }

    digitalWrite(myClockPin, 1);

  }

  return myDataIn;
}

void getInput() {
  for (int i = 0; i <= 7; i++)
  {
    if (switchVar1 & (1 << i) ) {
      buttonIndex = i;
    }
  }
}
