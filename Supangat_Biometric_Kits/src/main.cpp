#include <Arduino.h>
#include <Adafruit_Fingerprint.h>
#include <BLE_KeyboardSender.h>
#include <PN532_Module.h>
#include <OLED_Display.h>
#include <ButtonHandler.h>
#include <MenuManager.h>

HardwareSerial mySerial(2); // RX = GPIO16, TX = GPIO17
Adafruit_Fingerprint finger(&mySerial);

enum State {
  STATE_SPLASH,
  STATE_CONNECTING,
  STATE_MENU,
  STATE_FINGERPRINT,
  STATE_EKTP,
  STATE_DISCONNECTED
};

State currentState = STATE_SPLASH;
State previousState = STATE_MENU;
unsigned long splashStart = 0;

// Non-blocking scan eKTP
unsigned long lastScan = 0;
const unsigned long scanInterval = 200;
bool lastScanResult = false;

void setup() {
  Serial.begin(115200);
  delay(500);

  OLED_Display::begin();
  PN532_Module::begin();
  ButtonHandler::begin();
  finger.begin(57600);

  BLE_KeyboardSender::begin();  // BLE aktif sejak awal agar stabil

  splashStart = millis();
  OLED_Display::showSplashScreen();
}

void loop() {
  ButtonHandler::update();

  if (currentState != STATE_SPLASH && !BLE_KeyboardSender::isConnected()) {
    if (currentState != STATE_DISCONNECTED) {
      previousState = currentState;
      OLED_Display::showDisconnected();
      delay(2000);
      OLED_Display::showConnecting();
      currentState = STATE_DISCONNECTED;
    }
    return;
  }

  switch (currentState) {
    case STATE_SPLASH:
      if (millis() - splashStart > 3000) {
        OLED_Display::showConnecting();
        currentState = STATE_CONNECTING;
      }
      break;

    case STATE_CONNECTING:
      if (BLE_KeyboardSender::isConnected()) {
        OLED_Display::showConnected();
        delay(1500);
        OLED_Display::showMainMenu();
        currentState = STATE_MENU;
      }
      break;

    case STATE_DISCONNECTED:
      if (BLE_KeyboardSender::isConnected()) {
        OLED_Display::showConnected();
        delay(1500);
        if (previousState == STATE_EKTP) OLED_Display::showKTPPrompt();
        else if (previousState == STATE_FINGERPRINT) OLED_Display::showFingerprintPrompt();
        else OLED_Display::showMainMenu();
        currentState = previousState;
      }
      break;

    case STATE_MENU:
      if (ButtonHandler::isModePressed()) {
        MenuManager::nextOption();
        OLED_Display::showMainMenu();
      }

      if (ButtonHandler::isOKPressed()) {
        if (MenuManager::getCurrentOption() == 0) {
          OLED_Display::showFingerprintPrompt();
          currentState = STATE_FINGERPRINT;
        } else {
          OLED_Display::showKTPPrompt();
          currentState = STATE_EKTP;
        }
      }
      break;

    case STATE_FINGERPRINT:
      if (ButtonHandler::isBackPressed()) {
        OLED_Display::showMainMenu();
        currentState = STATE_MENU;
        return;
      }
      if (finger.getImage() != FINGERPRINT_OK) return;
      if (finger.image2Tz(1) != FINGERPRINT_OK) {
        OLED_Display::showFingerprintFailed();
        delay(1500);
        OLED_Display::showFingerprintPrompt();
        return;
      }
      if (finger.uploadCharFromBuffer(1) != FINGERPRINT_OK) {
        OLED_Display::showFingerprintFailed();
        delay(1500);
        OLED_Display::showFingerprintPrompt();
        return;
      }
      BLE_KeyboardSender::sendTemplateAsHex(finger.downloadedTemplate, finger.downloadedTemplateLength);
      OLED_Display::showFingerprintSuccess();
      delay(1500);
      OLED_Display::showFingerprintPrompt();
      break;

    case STATE_EKTP:
      if (ButtonHandler::isBackPressed()) {
        OLED_Display::showMainMenu();
        currentState = STATE_MENU;
      } else {
        if (millis() - lastScan > scanInterval) {
          lastScan = millis();
          lastScanResult = PN532_Module::scanUID();
        }

        if (lastScanResult && BLE_KeyboardSender::isConnected()) {
          BLE_KeyboardSender::sendUID(PN532_Module::getUID(), PN532_Module::getUIDLength());
          OLED_Display::showKTPSuccess();
          delay(1500);
          OLED_Display::showKTPPrompt();
          lastScanResult = false; // reset
        }
      }
      break;
  }
}
