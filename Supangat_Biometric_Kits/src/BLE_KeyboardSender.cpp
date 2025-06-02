#include <Arduino.h>
#include <BleKeyboard.h>
#include "BLE_KeyboardSender.h"

BleKeyboard bleKeyboard("Supangat Kit 01", "Filo", 100);

void BLE_KeyboardSender::begin() {
  bleKeyboard.begin();
}

bool BLE_KeyboardSender::isConnected() {
  return bleKeyboard.isConnected();
}

void BLE_KeyboardSender::sendTemplateAsHex(const uint8_t* data, size_t len) {
  if (!bleKeyboard.isConnected()) {
    Serial.println("⚠️ BLE keyboard belum terhubung.");
    return;
  }

  char hex[3];
  for (size_t i = 0; i < len; i++) {
    sprintf(hex, "%02X", data[i]);
    bleKeyboard.print(hex);
  }
  Serial.println("📤 Template fingerprint dikirim via BLE HID (HEX).");
  bleKeyboard.write(KEY_TAB);
}

void BLE_KeyboardSender::sendUID(const uint8_t* uid, uint8_t uidLength) {
  if (!bleKeyboard.isConnected()) {
    Serial.println("⚠️ BLE keyboard belum terhubung.");
    return;
  }

  char hex[3];
  for (uint8_t i = 0; i < uidLength; i++) {
    sprintf(hex, "%02X", uid[i]);
    bleKeyboard.print(hex);
  }
  bleKeyboard.write(KEY_TAB);
  Serial.println("📤 UID eKTP dikirim via BLE HID (HEX).");
}
