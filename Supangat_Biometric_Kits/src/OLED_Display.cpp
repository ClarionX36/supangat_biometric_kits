#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include "OLED_Display.h"
#include "ButtonHandler.h"
#include "MenuManager.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 OLED_Display::display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void OLED_Display::begin() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED gagal diinisialisasi"));
    while (true);
  }
  display.clearDisplay();
  display.display();
}

void OLED_Display::showSplashScreen() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2);
  display.setCursor(10, 10);
  display.println("Supangat");
  display.setTextSize(1);
  display.setCursor(10, 35);
  display.println("Biometric Kit");
  display.setCursor(10, 50);
  display.println("v4.2");
  display.display();
}

void OLED_Display::showConnecting() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(10, 25);
  display.println("Mencari Koneksi...");
  display.display();
}

void OLED_Display::showConnected() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(10, 25);
  display.println("Berhasil Tersambung");
  display.display();
}

void OLED_Display::showDisconnected() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(10, 25);
  display.println("Koneksi Terputus");
  display.display();
}

void OLED_Display::showMainMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(10, 0);
  display.println("Pilih Mode");

  int selected = MenuManager::getCurrentOption();

  display.setCursor(10, 20);
  display.print(selected == 0 ? ">" : " ");
  display.println(" Fingerprint");

  display.setCursor(10, 35);
  display.print(selected == 1 ? ">" : " ");
  display.println(" eKTP");

  display.display();
}

void OLED_Display::showFingerprintPrompt() {
  display.clearDisplay();
  display.setCursor(10, 25);
  display.println("Tempelkan Jari");
  display.display();
}

void OLED_Display::showFingerprintSuccess() {
  display.clearDisplay();
  display.setCursor(10, 25);
  display.println("Berhasil");
  display.display();
}

void OLED_Display::showFingerprintFailed() {
  display.clearDisplay();
  display.setCursor(10, 25);
  display.println("Gagal");
  display.display();
}

void OLED_Display::showKTPPrompt() {
  display.clearDisplay();
  display.setCursor(10, 25);
  display.println("Tempelkan eKTP");
  display.display();
}

void OLED_Display::showKTPSuccess() {
  display.clearDisplay();
  display.setCursor(10, 25);
  display.println("Berhasil");
  display.display();
}

void OLED_Display::showKTPFailed() {
  display.clearDisplay();
  display.setCursor(10, 25);
  display.println("Gagal");
  display.display();
}
