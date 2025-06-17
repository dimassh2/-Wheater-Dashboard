# Panduan Setup ESP32 Weather Station

## Langkah 1: Persiapan Hardware

### Komponen yang Dibutuhkan:
- [x] ESP32 Development Board
- [x] DHT11 Temperature & Humidity Sensor
- [x] BMP280 Barometric Pressure Sensor  
- [x] Rain Drop Sensor Module
- [x] LCD 16x2 dengan I2C Module
- [x] Breadboard
- [x] Jumper Wires (Male-Male, Male-Female)
- [x] Resistor 10kΩ (untuk pull-up DHT11)
- [x] Power Supply 5V/1A (atau USB)

### Tools yang Dibutuhkan:
- [x] Arduino IDE (versi 1.8.x atau 2.x.x)
- [x] Kabel USB untuk ESP32
- [x] Multimeter (untuk testing, opsional)

## Langkah 2: Setup Arduino IDE

### Install ESP32 Board:
1. Buka Arduino IDE
2. File → Preferences
3. Tambahkan URL berikut di "Additional Board Manager URLs":
   ```
   https://dl.espressif.com/dl/package_esp32_index.json
   ```
4. Tools → Board → Boards Manager
5. Search "ESP32" → Install "ESP32 by Espressif Systems"

### Install Required Libraries:
Buka Tools → Manage Libraries, lalu install:
- [x] **DHT sensor library** by Adafruit
- [x] **Adafruit BMP280 Library** by Adafruit  
- [x] **Adafruit Unified Sensor** by Adafruit
- [x] **LiquidCrystal I2C** by Frank de Brabander
- [x] **ArduinoJson** by Benoit Blanchon (versi 6.x.x)

## Langkah 3: Wiring Hardware

Ikuti diagram wiring di file `wiring_diagram.md`:

### Checklist Koneksi:
- [x] ESP32 3.3V → VCC semua sensor
- [x] ESP32 GND → GND semua sensor  
- [x] GPIO 4 → DHT11 Data (dengan resistor 10kΩ pull-up)
- [x] GPIO 21 → SDA (BMP280 & LCD)
- [x] GPIO 22 → SCL (BMP280 & LCD)
- [x] GPIO 34 → Rain Sensor Analog Out

### Test Hardware:
1. Hubungkan ESP32 ke komputer via USB
2. Buka Serial Monitor (115200 baud)
3. Upload sketch sederhana untuk test I2C:
   ```cpp
   #include <Wire.h>
   void setup() {
     Serial.begin(115200);
     Wire.begin(21, 22);
     Serial.println("Scanning I2C devices...");
   }
   void loop() {
     // I2C scanner code
   }
   ```

## Langkah 4: Upload Code ke ESP32

### Konfigurasi Board:
- **Board**: ESP32 Dev Module
- **Upload Speed**: 921600
- **CPU Frequency**: 240MHz (WiFi/BT)
- **Flash Size**: 4MB (32Mb)
- **Port**: Pilih port COM yang sesuai

### Edit Konfigurasi WiFi:
1. Buka file `weather_station.ino`
2. Ganti kredensial WiFi:
   ```cpp
   const char* ssid = "NAMA_WIFI_ANDA";
   const char* password = "PASSWORD_WIFI_ANDA";
   ```

### Upload Process:
1. Compile code (Ctrl+R)
2. Upload ke ESP32 (Ctrl+U)
3. Buka Serial Monitor (Ctrl+Shift+M)
4. Set baud rate ke 115200
5. Tunggu hingga ESP32 terhubung ke WiFi
6. Catat IP address yang ditampilkan

## Langkah 5: Setup Website

### Update IP Address:
1. Buka file `api/sensor.js`
2. Ganti IP address ESP32:
   ```javascript
   const ESP32_IP = "192.168.1.XXX"; // IP dari Serial Monitor
   ```

### Test Koneksi:
1. Buka website di browser
2. Buka Developer Console (F12)
3. Lihat log koneksi ESP32
4. Test endpoint: `http://[IP_ESP32]/sensor`

## Langkah 6: Testing & Troubleshooting

### Test Checklist:
- [x] ESP32 terhubung ke WiFi
- [x] Serial Monitor menampilkan sensor readings
- [x] LCD menampilkan data sensor
- [x] Website bisa mengakses endpoint `/sensor`
- [x] Data real-time muncul di dashboard

### Common Issues:

#### ESP32 tidak terhubung WiFi:
- Periksa kredensial WiFi
- Pastikan WiFi 2.4GHz (bukan 5GHz)
- Cek jarak dari router

#### Sensor tidak terbaca:
- **DHT11**: Periksa pull-up resistor, tunggu 2 detik antar pembacaan
- **BMP280**: Cek alamat I2C (0x76 atau 0x77)
- **LCD**: Cek alamat I2C (0x27 atau 0x3F)

#### Website tidak dapat data:
- Pastikan ESP32 dan komputer dalam jaringan sama
- Cek CORS headers di ESP32 code
- Test endpoint langsung di browser

#### Compile Error:
- Pastikan semua library terinstall
- Gunakan ArduinoJson versi 6.x.x
- Restart Arduino IDE

## Langkah 7: Kalibrasi Sensor

### Rain Sensor:
1. Test dalam kondisi kering → catat nilai analog
2. Test dengan tetesan air → catat nilai analog  
3. Set threshold di code:
   ```cpp
   currentData.isRaining = rainValue < THRESHOLD_VALUE;
   ```

### BMP280 Altitude:
1. Cari ketinggian lokasi Anda (dari Google Maps)
2. Adjust sea level pressure untuk akurasi altitude

## Langkah 8: Deployment

### Untuk Development:
- Jalankan website dengan Live Server atau HTTP server lokal
- ESP32 dan komputer harus dalam jaringan sama

### Untuk Production:
- Deploy website ke hosting (Netlify, Vercel, dll)
- Setup port forwarding untuk ESP32 (jika diperlukan)
- Pertimbangkan keamanan (HTTPS, authentication)

## Tips & Best Practices:

1. **Power Management**: Gunakan deep sleep jika battery powered
2. **Error Handling**: Tambahkan retry logic untuk sensor readings
3. **Data Logging**: Simpan data ke SD card atau cloud
4. **Security**: Jangan expose ESP32 ke internet tanpa authentication
5. **Monitoring**: Setup watchdog timer untuk auto-restart
6. **Calibration**: Kalibrasi sensor secara berkala

## Support:

Jika mengalami masalah:
1. Cek Serial Monitor untuk error messages
2. Test setiap sensor secara individual
3. Gunakan I2C scanner untuk detect device addresses
4. Periksa wiring dengan multimeter
5. Konsultasi dokumentasi sensor masing-masing
```