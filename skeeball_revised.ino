#include <LiquidCrystal.h>
#include <EEPROM.h>

const int[] _scorePin = {
  A0, // 10
  A1, // 20
  A2, // 30
  A3, // 40
  A4  // 50
};

const int[] _scorePressed = {
  0,
  0,
  0,
  0,
  0
};

const int _startPin = A5;
int _startPressed = 0;

const int _resetPin = A6;
int _resetPressed = 0;

int[] _lcdSize = {
  16,
  2
};

int _turnCount = 0;
int _currentScore = 0;

LiquidCrystal _lcd(2, 3, 4, 5, 6, 7);

void setup() {

  for (int i = 0; i <= 4; i++) {
    pinMode(_scorePin[i], INPUT);
  }

  pinMode(_startPin, INPUT);
  pinMode(_resetPin, INPUT);

  _lcd.begin(_lcdSize[0], _lcdSize[1]);
}

void loop() {
  // check if startButton is pressed
  if (digitalRead()startPin) == 1) {
    do {
      _startPressed = digitalRead(_startPin);
    } while (_startPressed == 1);
    runStart();
  }

  if (digitalRead(_resetPin) == 1) {
    do {
      _resetPressed = digitalRead(_resetPin);
    } while (_resetPressed == 1);
    runReset();
  }

  for (int i = 0; i <= 4; i++) {
    if (digitalRead(_scorePin[i]) == 1) {
      do {
        _scorePressed[i] = digitalRead(_scorePin[i]);
      } while (_scorePressed[i] == 1);
      runScore(i);
    }
  }
}

void runStart() {
  runReset();
  runHiScore();
}

void runReset() {
  _currentScore = 0;
  _turnCount = 0;
}

void runScore(int i) {
  int tempScore = _currentScore + i;
  _currentScore = tempScore;
}

void setHiScore() {
  int hiScore = getHiScore();
  if (_currentScore > hiScore) {
    int addr = _currentScore / 255;
    for (int i = 0; i < addr; i++) {
      EEPROM.write(i, 255);
    }
    EEPROM.write(addr, (_currentScore % 255));
  }
}

int getHiScore() {
  int score = 0;
  for (int i = 0; i < 512; i++) {
    tempScore = score + EEPROM.read(i);
    score = tempScore;
  }
  return score;
}
