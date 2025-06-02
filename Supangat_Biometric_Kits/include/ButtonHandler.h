#ifndef BUTTON_HANDLER_H
#define BUTTON_HANDLER_H

#include <Arduino.h>

#define BTN_MODE 26
#define BTN_OK   33
#define BTN_BACK 25

class ButtonHandler {
  public:
    static void begin();
    static void update();

    static bool isModePressed();
    static bool isOKPressed();
    static bool isBackPressed();

    static uint8_t getSelectedOption();

  private:
    static bool modePressed;
    static bool okPressed;
    static bool backPressed;
    static uint8_t currentOption;
};

#endif
