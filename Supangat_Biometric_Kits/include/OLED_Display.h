#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H

#include <Adafruit_SSD1306.h>

class OLED_Display {
public:
  static void begin();

  static void showSplashScreen();
  static void showConnecting();
  static void showConnected();
  static void showDisconnected();

  static void showMainMenu();

  static void showFingerprintPrompt();
  static void showFingerprintSuccess();
  static void showFingerprintFailed();

  static void showKTPPrompt();
  static void showKTPSuccess();
  static void showKTPFailed();

private:
  static Adafruit_SSD1306 display;
};

#endif // OLED_DISPLAY_H
