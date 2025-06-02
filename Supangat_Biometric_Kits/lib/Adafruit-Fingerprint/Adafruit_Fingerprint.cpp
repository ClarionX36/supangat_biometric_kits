#include "Adafruit_Fingerprint.h"

Adafruit_Fingerprint::Adafruit_Fingerprint(HardwareSerial *hs, uint32_t password) {
  hwSerial = hs;
  mySerial = hs;
  thePassword = password;
  theAddress = 0xFFFFFFFF;
}

Adafruit_Fingerprint::Adafruit_Fingerprint(Stream *serial, uint32_t password) {
  mySerial = serial;
  hwSerial = nullptr;
  thePassword = password;
  theAddress = 0xFFFFFFFF;
}

void Adafruit_Fingerprint::begin(uint32_t baud) {
  if (hwSerial)
    hwSerial->begin(baud);
}

boolean Adafruit_Fingerprint::verifyPassword() {
  uint8_t packet[] = {
    FINGERPRINT_VERIFYPASSWORD,
    (uint8_t)(thePassword >> 24),
    (uint8_t)(thePassword >> 16),
    (uint8_t)(thePassword >> 8),
    (uint8_t)(thePassword)
  };
  writeStructuredPacket(Adafruit_Fingerprint_Packet(FINGERPRINT_COMMANDPACKET, 5, packet));
  Adafruit_Fingerprint_Packet reply(FINGERPRINT_ACKPACKET, 0, nullptr);
  if (getStructuredPacket(&reply) != FINGERPRINT_OK) return false;
  return reply.data[0] == FINGERPRINT_OK;
}

uint8_t Adafruit_Fingerprint::getImage() {
  uint8_t packet[] = {FINGERPRINT_GETIMAGE};
  writeStructuredPacket(Adafruit_Fingerprint_Packet(FINGERPRINT_COMMANDPACKET, 1, packet));
  Adafruit_Fingerprint_Packet reply(FINGERPRINT_ACKPACKET, 0, nullptr);
  if (getStructuredPacket(&reply) != FINGERPRINT_OK) return FINGERPRINT_PACKETRECIEVEERR;
  return reply.data[0];
}

uint8_t Adafruit_Fingerprint::image2Tz(uint8_t slot) {
  uint8_t packet[] = {FINGERPRINT_IMAGE2TZ, slot};
  writeStructuredPacket(Adafruit_Fingerprint_Packet(FINGERPRINT_COMMANDPACKET, 2, packet));
  Adafruit_Fingerprint_Packet reply(FINGERPRINT_ACKPACKET, 0, nullptr);
  if (getStructuredPacket(&reply) != FINGERPRINT_OK) return FINGERPRINT_PACKETRECIEVEERR;
  return reply.data[0];
}

int Adafruit_Fingerprint::uploadCharFromBuffer(uint8_t bufferID) {
  uint8_t packet[] = {FINGERPRINT_UPLOAD, bufferID};
  writeStructuredPacket(Adafruit_Fingerprint_Packet(FINGERPRINT_COMMANDPACKET, 2, packet));

  uint16_t offset = 0;
  downloadedTemplateLength = 0;
  free(downloadedTemplate);
  downloadedTemplate = (uint8_t *)malloc(1024);
  if (!downloadedTemplate) return FINGERPRINT_UPLOADFAIL;

  while (true) {
    Adafruit_Fingerprint_Packet reply(FINGERPRINT_DATAPACKET, 0, nullptr);
    uint8_t r = getStructuredPacket(&reply);
    if (r != FINGERPRINT_OK) {
      return r;
    }

    if ((offset + reply.length - 2) > 1024) {
      free(downloadedTemplate);
      downloadedTemplate = nullptr;
      return FINGERPRINT_PACKETRESPONSEFAIL;
    }

    memcpy(downloadedTemplate + offset, reply.data, reply.length - 2);
    offset += reply.length - 2;

    if (reply.type == FINGERPRINT_ENDDATAPACKET) break;
  }

  downloadedTemplateLength = offset;
  return FINGERPRINT_OK;
}

void Adafruit_Fingerprint::writeStructuredPacket(const Adafruit_Fingerprint_Packet &packet) {
  mySerial->write((uint8_t)(packet.start_code >> 8));
  mySerial->write((uint8_t)(packet.start_code & 0xFF));
  mySerial->write(packet.address[0]);
  mySerial->write(packet.address[1]);
  mySerial->write(packet.address[2]);
  mySerial->write(packet.address[3]);
  mySerial->write(packet.type);
  uint16_t wire_length = packet.length + 2;
  mySerial->write((uint8_t)(wire_length >> 8));
  mySerial->write((uint8_t)(wire_length & 0xFF));
  uint16_t sum = ((wire_length) >> 8) + ((wire_length) & 0xFF) + packet.type;
  for (uint8_t i = 0; i < packet.length; i++) {
    mySerial->write(packet.data[i]);
    sum += packet.data[i];
  }
  mySerial->write((uint8_t)(sum >> 8));
  mySerial->write((uint8_t)(sum & 0xFF));
}

uint8_t Adafruit_Fingerprint::getStructuredPacket(Adafruit_Fingerprint_Packet *packet, uint16_t timeout) {
  uint8_t byte;
  uint16_t idx = 0, timer = 0;

  while (true) {
    while (!mySerial->available()) {
      delay(1);
      timer++;
      if (timer >= timeout) return FINGERPRINT_TIMEOUT;
    }

    byte = mySerial->read();

    switch (idx) {
      case 0:
        if (byte != (FINGERPRINT_STARTCODE >> 8)) continue;
        packet->start_code = (uint16_t)byte << 8;
        break;
      case 1:
        packet->start_code |= byte;
        if (packet->start_code != FINGERPRINT_STARTCODE) return FINGERPRINT_BADPACKET;
        break;
      case 2:
      case 3:
      case 4:
      case 5:
        packet->address[idx - 2] = byte;
        break;
      case 6:
        packet->type = byte;
        break;
      case 7:
        packet->length = (uint16_t)byte << 8;
        break;
      case 8:
        packet->length |= byte;
        if (packet->length > sizeof(packet->data) + 2) {
          return FINGERPRINT_BADPACKET;
        }
        break;
      default:
        if ((idx - 9) >= sizeof(packet->data)) {
          return FINGERPRINT_BADPACKET;
        }
        packet->data[idx - 9] = byte;
        if ((idx - 8) == packet->length) return FINGERPRINT_OK;
        break;
    }
    idx++;
  }

  return FINGERPRINT_BADPACKET;
}
