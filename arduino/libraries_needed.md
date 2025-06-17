# Library yang Dibutuhkan untuk ESP32 Weather Station dengan Shield

## Cara Install Library di Arduino IDE:

1. Buka Arduino IDE
2. Pergi ke **Tools** > **Manage Libraries** atau tekan **Ctrl+Shift+I**
3. Search dan install library berikut:

## Required Libraries:

### 1. DHT sensor library
- **Nama**: DHT sensor library
- **Author**: Adafruit
- **Versi**: Latest (2.x.x)
- **Fungsi**: Untuk membaca sensor DHT11 (tanpa resistor eksternal)
- **Catatan**: Shield sudah memiliki pull-up resistor built-in

### 2. Adafruit BMP280 Library
- **Nama**: Adafruit BMP280 Library
- **Author**: Adafruit
- **Versi**: Latest (2.x.x)
- **Fungsi**: Untuk membaca sensor BMP280 via I2C
- **Catatan**: Mendukung alamat I2C 0x76 dan 0x77

### 3. Adafruit Unified Sensor
- **Nama**: Adafruit Unified Sensor
- **Author**: Adafruit
- **Versi**: Latest (1.x.x)
- **Fungsi**: Dependency untuk BMP280 dan sensor Adafruit lainnya
- **Catatan**: Wajib install sebelum BMP280 library

### 4. LiquidCrystal I2C
- **Nama**: LiquidCrystal I2C
- **Author**: Frank de Brabander
- **Versi**: Latest (1.x.x)
- **Fungsi**: Untuk mengontrol LCD 16x2 dengan I2C module
- **Catatan**: Mendukung alamat I2C 0x27 dan 0x3F

### 5. ArduinoJson
- **Nama**: ArduinoJson
- **Author**: Benoit Blanchon
- **Versi**: 6.x.x (PENTING: jangan gunakan versi 7.x.x)
- **Fungsi**: Untuk membuat response JSON untuk web API
- **Catatan**: Versi 6 lebih stabil untuk ESP32

## Library yang Sudah Built-in di ESP32:
- **WiFi.h** - WiFi connectivity
- **WebServer.h** - HTTP web server
- **Wire.h** - I2C communication
- **analogRead()** - ADC untuk rain sensor

## Instalasi ESP32 Board untuk Shield:

### Jika belum install ESP32 board:

1. Buka **File** > **Preferences**
2. Di "Additional Board Manager URLs", tambahkan:
   ```
   https://dl.espressif.com/dl/package_esp32_index.json
   ```
3. Pergi ke **Tools** > **Board** > **Boards Manager**
4. Search "ESP32" dan install "ESP32 by Espressif Systems"
5. Pilih versi terbaru (2.x.x atau 3.x.x)

## Board Settings untuk ESP32 dengan Shield:

- **Board**: ESP32 Dev Module
- **Upload Speed**: 921600 (atau 115200 jika error)
- **CPU Frequency**: 240MHz (WiFi/BT)
- **Flash Frequency**: 80MHz
- **Flash Mode**: QIO
- **Flash Size**: 4MB (32Mb)
- **Partition Scheme**: Default 4MB with spiffs
- **Core Debug Level**: None (atau Info untuk debugging)
- **PSRAM**: Disabled (kecuali shield mendukung PSRAM)

## Verifikasi Library Installation:

### Test I2C Scanner:
```cpp
#include <Wire.h>

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22); // SDA, SCL untuk ESP32 Shield
  Serial.println("I2C Scanner untuk ESP32 Shield");
}

void loop() {
  byte error, address;
  int nDevices = 0;
  
  for(address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
      nDevices++;
    }
  }
  
  if (nDevices == 0) {
    Serial.println("No I2C devices found");
  }
  
  delay(5000);
}
```

### Expected I2C Addresses:
- **BMP280**: 0x76 atau 0x77
- **LCD I2C**: 0x27 atau 0x3F

## Troubleshooting Library Issues:

### Error saat compile:

#### 1. "DHT.h: No such file or directory"
- Install "DHT sensor library" by Adafruit
- Restart Arduino IDE setelah install

#### 2. "Adafruit_BMP280.h: No such file or directory"  
- Install "Adafruit BMP280 Library"
- Install juga "Adafruit Unified Sensor" sebagai dependency

#### 3. "LiquidCrystal_I2C.h: No such file or directory"
- Install "LiquidCrystal I2C" by Frank de Brabander
- Jangan gunakan library "LiquidCrystal" biasa

#### 4. ArduinoJson compilation errors:
- Pastikan menggunakan ArduinoJson versi 6.x.x
- Uninstall versi 7.x.x jika terinstall
- Restart Arduino IDE

#### 5. "WiFi.h: No such file or directory"
- Pastikan ESP32 board sudah terinstall dengan benar
- Pilih board "ESP32 Dev Module" di Tools > Board

### Error saat upload:

#### 1. "Failed to connect to ESP32"
- Tekan dan tahan tombol BOOT saat upload
- Cek kabel USB dan driver ESP32
- Pastikan ESP32 terpasang benar di shield

#### 2. "Brownout detector was triggered"
- Power supply tidak cukup untuk shield + semua sensor
- Gunakan power supply minimal 2A
- Cek koneksi power di shield

#### 3. "Guru Meditation Error"
- Memory overflow atau stack overflow
- Reduce buffer sizes dalam code
- Monitor free heap memory

## Library Versions Compatibility:

| Library | Recommended Version | ESP32 Core |
|---------|-------------------|------------|
| DHT sensor library | 1.4.x | 2.x.x |
| Adafruit BMP280 | 2.6.x | 2.x.x |
| Adafruit Unified Sensor | 1.1.x | 2.x.x |
| LiquidCrystal I2C | 1.1.x | 2.x.x |
| ArduinoJson | 6.21.x | 2.x.x |

## Shield-Specific Notes:

1. **Pull-up Resistors**: Shield sudah menyediakan, tidak perlu eksternal
2. **Power Distribution**: Shield mengelola power untuk semua sensor
3. **I2C Bus**: Shield menyediakan I2C bus yang stabil
4. **Terminal Blocks**: Koneksi lebih kuat dari breadboard
5. **Pin Mapping**: Ikuti labeling di shield, bukan pin fisik ESP32

## Performance Optimization:

### Untuk Shield dengan banyak sensor:
```cpp
// Increase I2C clock speed
Wire.setClock(400000); // 400kHz instead of 100kHz

// Optimize sensor reading intervals
#define SENSOR_INTERVAL 2000 // 2 seconds
#define LCD_INTERVAL 3000    // 3 seconds

// Monitor memory usage
Serial.println("Free Heap: " + String(ESP.getFreeHeap()));
```

Dengan shield, setup menjadi lebih mudah dan reliable karena tidak perlu khawatir dengan resistor pull-up dan koneksi yang loose!