#include "PN532_Module.h"
#include <Adafruit_PN532.h>

#define PN532_SCK  (18)
#define PN532_MOSI (23)
#define PN532_SS   (27)
#define PN532_MISO (19)

Adafruit_PN532 nfc(PN532_SS);

uint8_t uid[7];
uint8_t uidLength = 0;

void PN532_Module::begin() {
  nfc.begin();
  nfc.SAMConfig();
}

bool PN532_Module::scanUID() {
  if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 50)) {
    return true;
  }
  return false;
}

const uint8_t* PN532_Module::getUID() {
  return uid;
}

uint8_t PN532_Module::getUIDLength() {
  return uidLength;
}
