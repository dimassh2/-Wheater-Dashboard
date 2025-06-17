# Panduan Setup ESP32 Weather Station dengan Shield

## Langkah 1: Persiapan Hardware dengan Shield

### Komponen yang Dibutuhkan:
- [x] ESP32 Development Board
- [x] **ESP32 Extension Board/Shield** (dengan terminal blocks)
- [x] DHT11 Temperature & Humidity Sensor
- [x] BMP280 Barometric Pressure Sensor  
- [x] Rain Drop Sensor Module
- [x] LCD 16x2 dengan I2C Module
- [x] Jumper Wires Female-Female (untuk sensor ke shield)
- [x] Power Supply 5V/2A (untuk shield dan semua sensor)

### Keuntungan Menggunakan Shield:
✅ **Tidak perlu resistor eksternal** - Pull-up resistor sudah built-in
✅ **Koneksi lebih stabil** - Terminal blocks lebih kuat
✅ **Setup lebih cepat** - Plug and play
✅ **Troubleshooting mudah** - Koneksi jelas dan berlabel
✅ **Portable** - Semua komponen terpasang rapi

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

## Langkah 3: Assembly Hardware dengan Shield

### Checklist Assembly:
- [x] Pasang ESP32 pada shield dengan benar
- [x] Pastikan semua pin ESP32 masuk ke socket shield
- [x] Cek apakah shield memiliki LED power indicator

### Koneksi Sensor ke Shield Terminal Blocks:
```
Sensor          | Shield Terminal | Kabel
----------------|-----------------|-------
DHT11 VCC       | 3.3V           | Merah
DHT11 GND       | GND            | Hitam  
DHT11 DATA      | D4             | Kuning

BMP280 VCC      | 3.3V           | Merah
BMP280 GND      | GND            | Hitam
BMP280 SDA      | D21            | Biru
BMP280 SCL      | D22            | Hijau

Rain VCC        | 3.3V           | Merah
Rain GND        | GND            | Hitam
Rain AO         | A0/D34         | Orange

LCD VCC         | 5V atau 3.3V   | Merah
LCD GND         | GND            | Hitam
LCD SDA         | D21            | Biru
LCD SCL         | D22            | Hijau
```

### Test Hardware dengan Shield:
1. Hubungkan power ke shield (USB atau DC jack)
2. LED power pada shield harus menyala
3. LCD backlight harus menyala
4. Tidak ada komponen yang panas berlebihan

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
1. Compile code (Ctrl+R) - pastikan tidak ada error
2. Upload ke ESP32 (Ctrl+U)
3. Buka Serial Monitor (Ctrl+Shift+M)
4. Set baud rate ke 115200
5. Tunggu hingga ESP32 terhubung ke WiFi
6. Catat IP address yang ditampilkan di Serial Monitor dan LCD

## Langkah 5: Setup Website

### Update IP Address:
1. Buka file `api/sensor.js`
2. Ganti IP address ESP32:
   ```javascript
   const ESP32_IP = "192.168.1.XXX"; // IP dari Serial Monitor/LCD
   ```

### Test Koneksi:
1. Buka website di browser
2. Buka Developer Console (F12)
3. Lihat log koneksi ESP32
4. Test endpoint langsung: `http://[IP_ESP32]/sensor`

## Langkah 6: Testing & Troubleshooting dengan Shield

### Test Checklist:
- [x] Shield power LED menyala
- [x] ESP32 terhubung ke WiFi (cek LCD dan Serial Monitor)
- [x] LCD menampilkan data sensor dengan benar
- [x] Serial Monitor menampilkan sensor readings
- [x] Website bisa mengakses endpoint `/sensor`
- [x] Data real-time muncul di dashboard

### Common Issues dengan Shield:

#### ESP32 tidak terdetect:
- Pastikan ESP32 terpasang dengan benar di shield
- Coba tekan tombol reset di ESP32
- Periksa kabel USB dan driver ESP32

#### Sensor tidak terbaca:
- **DHT11**: Cek koneksi terminal D4, tunggu 2 detik antar pembacaan
- **BMP280**: Cek alamat I2C (0x76 atau 0x77), pastikan SDA/SCL benar
- **LCD**: Cek alamat I2C (0x27 atau 0x3F), pastikan power 5V jika diperlukan
- **Rain Sensor**: Cek koneksi A0, test dengan menyentuh sensor

#### Shield tidak mendapat power:
- Pastikan power supply minimal 2A untuk semua sensor
- Cek koneksi DC jack atau USB power
- LED power pada shield harus menyala

#### I2C conflict:
- Jalankan I2C scanner untuk detect alamat yang bentrok
- Pastikan hanya satu device per alamat I2C
- Cek kabel SDA/SCL tidak tertukar

## Langkah 7: Kalibrasi Sensor dengan Shield

### Rain Sensor Calibration:
1. Buka Serial Monitor
2. Catat nilai analog saat kering (biasanya > 3000)
3. Teteskan air pada sensor, catat nilai (biasanya < 1000)
4. Set threshold di code:
   ```cpp
   currentData.isRaining = rainValue < 2000; // Adjust sesuai hasil test
   ```

### BMP280 Altitude Calibration:
1. Cari ketinggian lokasi Anda dari Google Maps
2. Adjust sea level pressure untuk akurasi altitude
3. Monitor pembacaan selama beberapa hari untuk stabilitas

## Langkah 8: Monitoring & Maintenance

### Daily Monitoring:
- Cek LCD display untuk data terkini
- Monitor Serial output untuk error messages
- Test website connectivity secara berkala

### Weekly Maintenance:
- Bersihkan sensor dari debu
- Cek koneksi terminal blocks
- Restart ESP32 untuk refresh memory

### Monthly Calibration:
- Re-kalibrasi rain sensor threshold
- Cek akurasi temperature dengan termometer referensi
- Update firmware jika ada perbaikan

## Tips Khusus untuk Shield:

1. **Terminal Blocks**: Kencangkan sekrup terminal secara berkala
2. **Heat Management**: Pastikan ventilasi cukup untuk shield
3. **Power Quality**: Gunakan power supply berkualitas untuk stabilitas
4. **Cable Management**: Gunakan cable ties untuk kerapian
5. **Backup**: Simpan konfigurasi dan code sebagai backup

## Troubleshooting Advanced:

### Jika website tidak dapat data:
```bash
# Test koneksi langsung
curl http://[IP_ESP32]/sensor

# Test dari browser
http://[IP_ESP32]/status
```

### Jika memory leak:
- Monitor free heap di Serial Monitor
- Restart ESP32 jika heap < 50KB
- Optimize code untuk mengurangi memory usage

### Jika WiFi sering disconnect:
- Pindah ESP32 lebih dekat ke router
- Gunakan WiFi 2.4GHz yang stabil
- Tambahkan WiFi reconnect logic di code

Shield membuat proyek weather station lebih profesional dan reliable. Dengan setup yang benar, sistem bisa berjalan 24/7 tanpa masalah!