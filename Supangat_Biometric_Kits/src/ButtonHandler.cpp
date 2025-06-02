#include "ButtonHandler.h"

bool ButtonHandler::modePressed = false;
bool ButtonHandler::okPressed = false;
bool ButtonHandler::backPressed = false;
uint8_t ButtonHandler::currentOption = 0;

void ButtonHandler::begin() {
  pinMode(BTN_MODE, INPUT);
  pinMode(BTN_OK, INPUT);
  pinMode(BTN_BACK, INPUT);
}

void ButtonHandler::update() {
  static unsigned long lastDebounce = 0;
  if (millis() - lastDebounce < 200) return;

  if (digitalRead(BTN_MODE) == HIGH) {
    modePressed = true;
    currentOption = (currentOption + 1) % 2;
    lastDebounce = millis();
  }

  if (digitalRead(BTN_OK) == HIGH) {
    okPressed = true;
    lastDebounce = millis();
  }

  if (digitalRead(BTN_BACK) == HIGH) {
    backPressed = true;
    lastDebounce = millis();
  }
}

bool ButtonHandler::isModePressed() {
  bool temp = modePressed;
  modePressed = false;
  return temp;
}

bool ButtonHandler::isOKPressed() {
  bool temp = okPressed;
  okPressed = false;
  return temp;
}

bool ButtonHandler::isBackPressed() {
  bool temp = backPressed;
  backPressed = false;
  return temp;
}

uint8_t ButtonHandler::getSelectedOption() {
  return currentOption;
}
