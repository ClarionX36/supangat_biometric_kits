# Supangat Biometric Kits

Proyek ini menggunakan ESP32 dengan fingerprint sensor (AS608), pembaca e-KTP (PN532 via SPI), OLED display, dan BLE HID untuk mengirim data ke laptop/PC secara wireless.

## 🔧 Hardware yang Digunakan

| Komponen     | Koneksi ke ESP32 | VCC  | Keterangan    |
| ------------ | ---------------- | ---- | ------------- |
| OLED Display | SDA = GPIO21     | 5V   | I2C Data      |
|              | SCL = GPIO22     |      | I2C Clock     |
| PN532        | SCK = GPIO18     | 5V   | SPI Clock     |
|              | MISO = GPIO19    |      | SPI MISO      |
|              | MOSI = GPIO23    |      | SPI MOSI      |
|              | SS = GPIO27      |      | Chip Select   |
| AS608        | TX = GPIO16      | 3.3V | RX dari ESP32 |
|              | RX = GPIO17      |      | TX dari ESP32 |
| Tombol MODE  | GPIO26           | 3.3V | Navigasi Menu |
| Tombol OK    | GPIO33           |      | Pilih Menu    |
| Tombol BACK  | GPIO25           |      | Kembali       |

## 🧠 Fitur Utama

* ✅ Splash screen 3 detik, BLE nonaktif
* ✅ BLE aktif → tampil "Mencari Koneksi"
* ✅ Tampil "Berhasil Terhubung" saat koneksi sukses
* ✅ Menu navigasi dengan 3 tombol
* ✅ Mode fingerprint (AS608)
* ✅ Mode e-KTP (PN532 via SPI)
* ✅ Data dikirim ke laptop via BLE keyboard
* ✅ Deteksi koneksi terputus di semua mode
* ✅ Responsif dan non-blocking

## 📦 Struktur Project (PlatformIO)

```text
.
├── include/
│   └── (Header files)
├── src/
│   ├── main.cpp
│   ├── BLE_KeyboardSender.cpp
│   └── (modul lainnya)
├── platformio.ini
```
