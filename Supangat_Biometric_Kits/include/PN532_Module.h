#pragma once
#include <Arduino.h>

class PN532_Module {
public:
  static void begin();
  static bool scanUID();                        // Tambahkan
  static const uint8_t* getUID();               // Tambahkan
  static uint8_t getUIDLength();                // Tambahkan
};
