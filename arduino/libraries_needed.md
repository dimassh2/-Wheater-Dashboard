# Library yang Dibutuhkan untuk ESP32 Weather Station

## Cara Install Library di Arduino IDE:

1. Buka Arduino IDE
2. Pergi ke **Tools** > **Manage Libraries** atau tekan **Ctrl+Shift+I**
3. Search dan install library berikut:

## Required Libraries:

### 1. DHT sensor library
- **Nama**: DHT sensor library
- **Author**: Adafruit
- **Versi**: Latest
- **Fungsi**: Untuk membaca sensor DHT11

### 2. Adafruit BMP280 Library
- **Nama**: Adafruit BMP280 Library
- **Author**: Adafruit
- **Versi**: Latest
- **Fungsi**: Untuk membaca sensor BMP280

### 3. Adafruit Unified Sensor
- **Nama**: Adafruit Unified Sensor
- **Author**: Adafruit
- **Versi**: Latest
- **Fungsi**: Dependency untuk BMP280

### 4. LiquidCrystal I2C
- **Nama**: LiquidCrystal I2C
- **Author**: Frank de Brabander
- **Versi**: Latest
- **Fungsi**: Untuk mengontrol LCD dengan I2C

### 5. ArduinoJson
- **Nama**: ArduinoJson
- **Author**: Benoit Blanchon
- **Versi**: 6.x.x (versi 6, bukan 7)
- **Fungsi**: Untuk membuat response JSON

## Library yang Sudah Built-in:
- **WiFi.h** - Sudah tersedia di ESP32 core
- **WebServer.h** - Sudah tersedia di ESP32 core
- **Wire.h** - Sudah tersedia di Arduino core

## Instalasi ESP32 Board:

Jika belum install ESP32 board:

1. Buka **File** > **Preferences**
2. Di "Additional Board Manager URLs", tambahkan:
   ```
   https://dl.espressif.com/dl/package_esp32_index.json
   ```
3. Pergi ke **Tools** > **Board** > **Boards Manager**
4. Search "ESP32" dan install "ESP32 by Espressif Systems"

## Board Settings untuk ESP32:

- **Board**: ESP32 Dev Module
- **Upload Speed**: 921600
- **CPU Frequency**: 240MHz (WiFi/BT)
- **Flash Frequency**: 80MHz
- **Flash Mode**: QIO
- **Flash Size**: 4MB (32Mb)
- **Partition Scheme**: Default 4MB with spiffs
- **Core Debug Level**: None
- **PSRAM**: Disabled

## Troubleshooting Library:

Jika ada error saat compile:

1. **Error BMP280**: Pastikan install "Adafruit Unified Sensor" juga
2. **Error LiquidCrystal**: Gunakan library "LiquidCrystal I2C" bukan "LiquidCrystal"
3. **Error ArduinoJson**: Pastikan menggunakan versi 6.x.x, bukan 7.x.x
4. **Error WiFi**: Pastikan board ESP32 sudah terinstall dengan benar
```