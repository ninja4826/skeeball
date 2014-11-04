#include <LiquidCrystal.h>
#include <EEPROM.h>

const int _scorePin = A0;

const int _startPin = A1;

const int _muxPin[] = {A1, A2, A3};
const int _ticketPin = 9;

int _ballCount;
int _currentScore;
int _bonuses[] = {0, 0};
int _hiScore = hiScoreRead();
int _hiScoreAddress = hiScoreAddress();

LiquidCrystal _lcd(2, 3, 4, 5, 6, 7);

void setup() {
  
  _lcd.begin(16, 2);
  pinMode(_muxPin[0], OUTPUT);
  pinMode(_muxPin[1], OUTPUT);
  pinMode(_muxPin[2], OUTPUT);
  
}

void loop() {
  
  if (digitalRead(_startPin) == HIGH) {
    int startPressed;
    do {
      startPressed = digitalRead(_startPin);
    } while (startPressed == HIGH);
    
    _ballCount = 0;
    _currentScore = 0;
    _bonuses[0] = 0;
    _bonuses[1] = 0;
    ballControl(9);
  }
  int scorePressed = analogRead(_scorePin);
  if (scorePressed <= 900) {
    int tempScorePressed;
    do {
      tempScorePressed = analogRead(_scorePin);
    } while (tempScorePressed <= 900);
    addPoints(scorePressed);
    if (ballCheck()) {
      gameOver();
    }
  }
}

void addPoints(int scoreVal) {
  int points;
  
  if (scoreVal < 300) {
    points = 10;
  }
  if (scoreVal >= 330 && scoreVal <= 350) {
    points = 20;
  }
  if (scoreVal >= 500 && scoreVal <= 520) {
    points = 30;
  }
  if (scoreVal >= 600 && scoreVal <= 620) {
    points = 40;
  }
  if (scoreVal >= 670 && scoreVal <= 690) {
    points = 50;
  }
  if (scoreVal >= 725 && scoreVal <= 745) {
    points = 100;
  }
  int tempScore = _currentScore + points;
  _currentScore = tempScore;
  bonusCheck();
}

void bonusCheck() {
  if ((_currentScore / 100) > _bonuses[0]) {
    if ((_currentScore / 1000) > _bonuses[1]) {
      ballControl(5);
      _bonuses[1] = _currentScore / 1000;
    } else {
      ballControl(1);
      _bonuses[0] = _currentScore / 100;
    }
  }
}

int hiScoreRead() {
  int hiScore = 0;
  int tempScore = 0;
  for (int i = 0; i < 512; i++) {
    tempScore = hiScore + EEPROM.read(i);
    hiScore = tempScore;
  }
  return hiScore;
}

int hiScoreAddress() {
  int addr;
  for (int i = 0; i < 512; i++) {
    if (EEPROM.read(i) != 255) {
      addr = i;
    }
  }
  return addr;
}

void hiScoreWrite() {
  if (_currentScore > _hiScore) {
    int tempAddress = _currentScore / 255;
    int addr = tempAddress - _hiScoreAddress;
    for (int i = 0; i <= addr; i++) {
      EEPROM.write(i, 255);
    }
    addr++;
    EEPROM.write(addr, (_currentScore % 255));
    _hiScore = _currentScore;
    _hiScoreAddress = addr;
  }
}

void ballControl(int balls) {
  switch (balls) {
    case 1:
      registerStuff("ball", 1);
      delay(1000);
      registerStuff("ball", 0);
      break;
    case 5:
      registerStuff("ball", 1);
      delay(4000);
      registerStuff("ball", 0);
      break;
    case 9:
      registerStuff("ball", 1);
      delay(5000);
      registerStuff("ball", 0);
  }
  _ballCount = _ballCount + balls;
}

boolean ballCheck() {
  boolean check = false;
  if (_ballCount < 1) {
    check = true;
  }
  return check;
}

void gameOver() {
  // do gameover stuff.
}

void registerStuff(String obj, int state) {
  int pin;
  if (obj == "gameOver") {
    pin = 0;
  }
  if (obj == "winner") {
    pin = 1;
  }
  if (obj == "freeGame") {
    pin = 2;
  }
  if (obj == "flashing") {
    pin = 3;
  }
  if (obj == "ball") {
    pin = 4;
  }
  
  byte bitsToSend = 0;
  digitalWrite(_muxPin[0], LOW);
  
  bitWrite(bitsToSend, pin, state);
  
  shiftOut(_muxPin[2], _muxPin[1], MSBFIRST, bitsToSend);
  
  digitalWrite(_muxPin[0], HIGH);
}
