# ESP32 Shield Weather Station - Wiring Diagram

## Komponen yang Dibutuhkan:
- ESP32 Development Board
- ESP32 Extension Board/Shield (dengan terminal blocks)
- DHT11 Temperature & Humidity Sensor
- BMP280 Barometric Pressure Sensor
- Rain Drop Sensor Module
- LCD 16x2 dengan I2C Module
- Jumper Wires (Female-Female untuk sensor ke shield)

## Keuntungan Menggunakan ESP32 Shield:
✅ **Tidak perlu resistor eksternal** - Shield sudah memiliki pull-up resistor built-in
✅ **Koneksi lebih stabil** - Terminal blocks lebih kuat dari breadboard
✅ **Lebih rapi** - Tidak ada kabel berserakan
✅ **Mudah maintenance** - Sensor bisa dilepas-pasang dengan mudah
✅ **Lebih tahan lama** - Tidak ada koneksi loose

## Koneksi Pin ESP32 Shield:

### ESP32 Shield Terminal Blocks:
```
Terminal Block | ESP32 Pin | Komponen           | Keterangan
---------------|-----------|--------------------|-----------------
3.3V           | 3.3V      | VCC (semua sensor) | Power supply
GND            | GND       | GND (semua sensor) | Ground
D4             | GPIO 4    | DHT11 Data         | Data pin DHT11
D21            | GPIO 21   | SDA (I2C)          | I2C Data Line
D22            | GPIO 22   | SCL (I2C)          | I2C Clock Line
A0/D34         | GPIO 34   | Rain Sensor AO     | Analog Output Rain Sensor
```

### DHT11 Connections (3 Pin):
```
DHT11 Pin | Shield Terminal | Keterangan
----------|-----------------|------------
VCC       | 3.3V           | Power
GND       | GND            | Ground
DATA      | D4             | Data (pull-up sudah built-in di shield)
```

### BMP280 Connections (I2C - 4 Pin):
```
BMP280 Pin | Shield Terminal | Keterangan
-----------|-----------------|------------
VCC        | 3.3V           | Power
GND        | GND            | Ground
SDA        | D21            | I2C Data
SCL        | D22            | I2C Clock
```

### Rain Sensor Connections (4 Pin):
```
Rain Sensor Pin | Shield Terminal | Keterangan
----------------|-----------------|------------
VCC             | 3.3V           | Power
GND             | GND            | Ground
AO              | A0/D34         | Analog Output
DO              | -              | Digital Output (tidak digunakan)
```

### LCD 16x2 with I2C Module (4 Pin):
```
I2C LCD Pin | Shield Terminal | Keterangan
------------|-----------------|------------
VCC         | 5V atau 3.3V   | Power (cek spesifikasi LCD)
GND         | GND            | Ground
SDA         | D21            | I2C Data
SCL         | D22            | I2C Clock
```

## Diagram Koneksi Shield:

```
                    ESP32 SHIELD
                 ┌─────────────────┐
                 │  Terminal Blocks │
                 │                 │
    DHT11 ───────┤ D4   │   3.3V   │──── Power Rail
                 │      │          │
    BMP280 SDA ──┤ D21  │   GND    │──── Ground Rail
    BMP280 SCL ──┤ D22  │          │
    LCD SDA ─────┤      │          │
    LCD SCL ─────┤      │          │
                 │      │          │
    Rain AO ─────┤ A0   │          │
                 │      │          │
                 └─────────────────┘
```

## Langkah Assembly dengan Shield:

### 1. Persiapan Shield:
- Pasang ESP32 pada shield
- Pastikan semua pin terhubung dengan baik
- Cek apakah shield memiliki terminal blocks atau header pins

### 2. Koneksi Power:
- Hubungkan semua VCC sensor ke terminal 3.3V
- Hubungkan semua GND sensor ke terminal GND
- **Tidak perlu resistor pull-up** - sudah built-in di shield

### 3. Koneksi Data:
- DHT11 Data → Terminal D4
- BMP280 SDA → Terminal D21
- BMP280 SCL → Terminal D22
- LCD SDA → Terminal D21 (parallel dengan BMP280)
- LCD SCL → Terminal D22 (parallel dengan BMP280)
- Rain Sensor AO → Terminal A0/D34

### 4. Koneksi I2C Bus:
```
Shield D21 (SDA) ──┬── BMP280 SDA
                   └── LCD SDA

Shield D22 (SCL) ──┬── BMP280 SCL
                   └── LCD SCL
```

## Catatan Penting untuk Shield:

1. **Power Supply**: Shield biasanya memiliki regulator built-in
2. **Pull-up Resistors**: Sudah tersedia di shield untuk I2C dan DHT
3. **I2C Address**: 
   - BMP280: 0x76 atau 0x77
   - LCD I2C: 0x27 atau 0x3F
4. **Terminal Blocks**: Lebih kuat dan stabil dari breadboard
5. **Labeling**: Shield biasanya sudah berlabel pin dengan jelas

## Keunggulan Shield vs Breadboard:

| Aspek | Shield | Breadboard |
|-------|--------|------------|
| Stabilitas | ✅ Sangat stabil | ❌ Mudah loose |
| Resistor | ✅ Built-in | ❌ Perlu eksternal |
| Kerapian | ✅ Sangat rapi | ❌ Kabel berserakan |
| Maintenance | ✅ Mudah | ❌ Sulit troubleshoot |
| Portabilitas | ✅ Compact | ❌ Fragile |
| Harga | ❌ Lebih mahal | ✅ Murah |

## Testing dengan Shield:

1. **Power Test**: LED power pada shield harus menyala
2. **I2C Scan**: Jalankan I2C scanner untuk detect alamat
3. **Serial Monitor**: Cek output sensor readings
4. **LCD Test**: Pastikan LCD menampilkan data
5. **Web Test**: Akses IP ESP32 dari browser

## Troubleshooting Shield:

### Jika sensor tidak terbaca:
1. Cek koneksi terminal blocks
2. Pastikan power supply cukup (min 1A)
3. Test I2C address dengan scanner
4. Periksa kabel jumper female-female

### Jika WiFi tidak connect:
1. Cek kredensial WiFi di code
2. Pastikan WiFi 2.4GHz (bukan 5GHz)
3. Reset ESP32 dan coba lagi

### Jika LCD blank:
1. Cek alamat I2C LCD (0x27 atau 0x3F)
2. Pastikan backlight menyala
3. Adjust kontras jika ada potensiometer

## Rekomendasi Shield:

- **ESP32 DevKit Shield** dengan terminal blocks
- **ESP32 Expansion Board** dengan breadboard built-in
- **ESP32 Proto Shield** untuk prototyping
- **ESP32 Sensor Shield** khusus untuk sensor

Shield membuat proyek lebih profesional dan reliable untuk penggunaan jangka panjang!