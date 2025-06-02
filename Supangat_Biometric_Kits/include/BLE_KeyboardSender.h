#pragma once
#include <BleKeyboard.h>

class BLE_KeyboardSender {
public:
  static void begin();
  static bool isConnected();
  static void sendTemplateAsHex(const uint8_t* data, size_t len);
  static void sendUID(const uint8_t* uid, uint8_t uidLength);
};
