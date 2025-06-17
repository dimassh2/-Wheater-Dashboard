# ESP32 Weather Station - Wiring Diagram

## Komponen yang Dibutuhkan:
- ESP32 Development Board
- DHT11 Temperature & Humidity Sensor
- BMP280 Barometric Pressure Sensor
- Rain Drop Sensor Module
- LCD 16x2 dengan I2C Module
- Breadboard dan Jumper Wires
- Resistor 10kΩ (untuk pull-up DHT11)

## Koneksi Pin:

### ESP32 Pinout:
```
ESP32 Pin    | Komponen           | Keterangan
-------------|--------------------|-----------------
3.3V         | VCC (semua sensor) | Power supply
GND          | GND (semua sensor) | Ground
GPIO 4       | DHT11 Data         | Data pin DHT11
GPIO 21      | SDA (I2C)          | I2C Data Line
GPIO 22      | SCL (I2C)          | I2C Clock Line
GPIO 34      | Rain Sensor AO     | Analog Output Rain Sensor
```

### DHT11 Connections:
```
DHT11 Pin | ESP32 Pin | Keterangan
----------|-----------|------------
VCC       | 3.3V      | Power
GND       | GND       | Ground
DATA      | GPIO 4    | Data (dengan pull-up 10kΩ ke 3.3V)
```

### BMP280 Connections (I2C):
```
BMP280 Pin | ESP32 Pin | Keterangan
-----------|-----------|------------
VCC        | 3.3V      | Power
GND        | GND       | Ground
SDA        | GPIO 21   | I2C Data
SCL        | GPIO 22   | I2C Clock
```

### Rain Sensor Connections:
```
Rain Sensor Pin | ESP32 Pin | Keterangan
----------------|-----------|------------
VCC             | 3.3V      | Power
GND             | GND       | Ground
AO              | GPIO 34   | Analog Output
DO              | -         | Digital Output (tidak digunakan)
```

### LCD 16x2 with I2C Module:
```
I2C LCD Pin | ESP32 Pin | Keterangan
------------|-----------|------------
VCC         | 5V        | Power (atau 3.3V jika modul mendukung)
GND         | GND       | Ground
SDA         | GPIO 21   | I2C Data
SCL         | GPIO 22   | I2C Clock
```

## Diagram Skematik:

```
                    ESP32
                 ┌─────────┐
                 │         │
    DHT11 ───────┤ GPIO 4  │
                 │         │
    BMP280 SDA ──┤ GPIO 21 │── LCD SDA
    BMP280 SCL ──┤ GPIO 22 │── LCD SCL
                 │         │
    Rain AO ─────┤ GPIO 34 │
                 │         │
    3.3V ────────┤ 3.3V    │
    GND ─────────┤ GND     │
                 └─────────┘
```

## Catatan Penting:

1. **Power Supply**: Pastikan ESP32 mendapat cukup daya (minimal 500mA)
2. **Pull-up Resistor**: DHT11 memerlukan resistor pull-up 10kΩ antara pin data dan VCC
3. **I2C Address**: 
   - BMP280 biasanya menggunakan address 0x76 atau 0x77
   - LCD I2C biasanya menggunakan address 0x27 atau 0x3F
4. **Rain Sensor**: Nilai analog akan berkurang saat terkena air
5. **Grounding**: Pastikan semua komponen terhubung ke ground yang sama

## Langkah Assembly:

1. Pasang ESP32 pada breadboard
2. Hubungkan semua VCC ke rail power 3.3V
3. Hubungkan semua GND ke rail ground
4. Pasang DHT11 dengan resistor pull-up
5. Hubungkan BMP280 ke I2C bus
6. Hubungkan LCD I2C ke I2C bus yang sama
7. Hubungkan rain sensor ke pin analog
8. Double-check semua koneksi sebelum power on

## Testing:

1. Upload code ke ESP32
2. Buka Serial Monitor (115200 baud)
3. Tunggu koneksi WiFi berhasil
4. Catat IP address yang ditampilkan
5. Test endpoint: http://[IP_ADDRESS]/sensor
```