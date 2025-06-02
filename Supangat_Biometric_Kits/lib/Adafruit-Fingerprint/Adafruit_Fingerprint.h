#ifndef ADAFRUIT_FINGERPRINT_H
#define ADAFRUIT_FINGERPRINT_H

#include "Arduino.h"

// Return codes
#define FINGERPRINT_OK 0x00
#define FINGERPRINT_PACKETRECIEVEERR 0x01
#define FINGERPRINT_NOFINGER 0x02
#define FINGERPRINT_IMAGEFAIL 0x03
#define FINGERPRINT_IMAGEMESS 0x06
#define FINGERPRINT_FEATUREFAIL 0x07
#define FINGERPRINT_NOMATCH 0x08
#define FINGERPRINT_NOTFOUND 0x09
#define FINGERPRINT_ENROLLMISMATCH 0x0A
#define FINGERPRINT_BADLOCATION 0x0B
#define FINGERPRINT_DBRANGEFAIL 0x0C
#define FINGERPRINT_UPLOADFEATUREFAIL 0x0D
#define FINGERPRINT_PACKETRESPONSEFAIL 0x0E
#define FINGERPRINT_UPLOADFAIL 0x0F
#define FINGERPRINT_DELETEFAIL 0x10
#define FINGERPRINT_DBCLEARFAIL 0x11
#define FINGERPRINT_PASSFAIL 0x13
#define FINGERPRINT_INVALIDIMAGE 0x15
#define FINGERPRINT_FLASHERR 0x18
#define FINGERPRINT_INVALIDREG 0x1A
#define FINGERPRINT_ADDRCODE 0x20
#define FINGERPRINT_PASSVERIFY 0x21
#define FINGERPRINT_STARTCODE 0xEF01

// Packet types
#define FINGERPRINT_COMMANDPACKET 0x1
#define FINGERPRINT_DATAPACKET 0x2
#define FINGERPRINT_ACKPACKET 0x7
#define FINGERPRINT_ENDDATAPACKET 0x8

// Other error codes
#define FINGERPRINT_TIMEOUT 0xFF
#define FINGERPRINT_BADPACKET 0xFE

// Command set
#define FINGERPRINT_GETIMAGE 0x01
#define FINGERPRINT_IMAGE2TZ 0x02
#define FINGERPRINT_SEARCH 0x04
#define FINGERPRINT_REGMODEL 0x05
#define FINGERPRINT_STORE 0x06
#define FINGERPRINT_LOAD 0x07
#define FINGERPRINT_UPLOAD 0x08
#define FINGERPRINT_DELETE 0x0C
#define FINGERPRINT_EMPTY 0x0D
#define FINGERPRINT_READSYSPARAM 0x0F
#define FINGERPRINT_SETPASSWORD 0x12
#define FINGERPRINT_VERIFYPASSWORD 0x13
#define FINGERPRINT_HISPEEDSEARCH 0x1B
#define FINGERPRINT_TEMPLATECOUNT 0x1D
#define FINGERPRINT_AURALEDCONFIG 0x35
#define FINGERPRINT_LEDON 0x50
#define FINGERPRINT_LEDOFF 0x51
#define FINGERPRINT_WRITE_REG 0x0E

// Register addresses
#define FINGERPRINT_BAUD_REG_ADDR 0x04
#define FINGERPRINT_SECURITY_REG_ADDR 0x05
#define FINGERPRINT_PACKET_REG_ADDR 0x06

#define DEFAULTTIMEOUT 1000

struct Adafruit_Fingerprint_Packet {
  Adafruit_Fingerprint_Packet(uint8_t type, uint16_t length, uint8_t *data) {
    this->start_code = FINGERPRINT_STARTCODE;
    this->type = type;
    this->length = length;
    address[0] = 0xFF;
    address[1] = 0xFF;
    address[2] = 0xFF;
    address[3] = 0xFF;
    if (length < 64)
      memcpy(this->data, data, length);
    else
      memcpy(this->data, data, 64);
  }

  uint16_t start_code;
  uint8_t address[4];
  uint8_t type;
  uint16_t length;
  uint8_t data[512];
};

class Adafruit_Fingerprint {
public:
  Adafruit_Fingerprint(HardwareSerial *hs, uint32_t password = 0x0);
  Adafruit_Fingerprint(Stream *serial, uint32_t password = 0x0);

  void begin(uint32_t baud);
  boolean verifyPassword(void);
  uint8_t getImage(void);
  uint8_t image2Tz(uint8_t slot = 1);
  int uploadCharFromBuffer(uint8_t bufferID);
  void sendTemplateAsHash(); // <- Tambahkan ini

  void writeStructuredPacket(const Adafruit_Fingerprint_Packet &p);
  uint8_t getStructuredPacket(Adafruit_Fingerprint_Packet *p, uint16_t timeout = DEFAULTTIMEOUT);

  uint8_t *downloadedTemplate = nullptr;
  uint16_t downloadedTemplateLength = 0;

private:
  Stream *mySerial;
  HardwareSerial *hwSerial;
  uint32_t thePassword;
  uint32_t theAddress;
};

#endif
