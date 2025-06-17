# ESP32 Shield Weather Station - Fritzing Wiring Guide

## Komponen untuk Fritzing Diagram

### Parts List untuk Fritzing:
1. **ESP32 DevKit v1** (atau ESP32 WROOM-32)
2. **ESP32 Extension Shield/Proto Board**
3. **DHT11 Temperature & Humidity Sensor**
4. **BMP280 Barometric Pressure Sensor**
5. **Rain Drop Sensor Module**
6. **LCD 16x2 with I2C Backpack**
7. **Jumper Wires (Female-Female)**
8. **Power Supply 5V/2A**

## Fritzing Layout Diagram (ASCII Style)

```
                    ESP32 DEVELOPMENT SHIELD
    ┌─────────────────────────────────────────────────────────────┐
    │                                                             │
    │  ┌─────────────────────────────────────────────────────┐    │
    │  │                ESP32 DEVKIT                         │    │
    │  │  ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ │    │
    │  │  │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │    │
    │  │  └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ │    │
    │  └─────────────────────────────────────────────────────┘    │
    │                                                             │
    │  Terminal Blocks / Headers:                                 │
    │  ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐     │
    │  │3V3│GND│D4 │D21│D22│D34│5V │GND│   │   │   │   │   │     │
    │  └───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘     │
    │    │   │   │   │   │   │   │   │                           │
    │    │   │   │   │   │   │   │   └─── Power Rail (+5V)      │
    │    │   │   │   │   │   │   └─────── Ground Rail           │
    │    │   │   │   │   │   └─────────── Rain Sensor (A0)     │
    │    │   │   │   │   └─────────────── I2C SCL (Clock)      │
    │    │   │   │   └─────────────────── I2C SDA (Data)       │
    │    │   │   └─────────────────────── DHT11 Data           │
    │    │   └─────────────────────────── Ground Rail          │
    │    └─────────────────────────────── Power Rail (+3.3V)   │
    └─────────────────────────────────────────────────────────────┘
```

## Sensor Connection Details

### 1. DHT11 Temperature & Humidity Sensor
```
DHT11 Pinout:        Shield Connection:
┌─────────────┐      ┌─────────────┐
│  1  2  3    │      │             │
│ VCC DATA GND│ ──── │ 3V3 D4  GND │
└─────────────┘      └─────────────┘
     │   │   │            │   │   │
     │   │   └────────────┼───┼───┘
     │   └────────────────┼───┘
     └────────────────────┘

Wiring:
- DHT11 Pin 1 (VCC) → Shield 3.3V
- DHT11 Pin 2 (DATA) → Shield D4
- DHT11 Pin 3 (GND) → Shield GND
```

### 2. BMP280 Barometric Pressure Sensor
```
BMP280 Pinout:       Shield Connection:
┌─────────────────┐  ┌─────────────────┐
│VCC GND SCL SDA  │  │3V3 GND D22 D21  │
└─────────────────┘  └─────────────────┘
  │   │   │   │        │   │   │   │
  │   │   │   └────────┼───┼───┼───┘
  │   │   └────────────┼───┼───┘
  │   └────────────────┼───┘
  └────────────────────┘

Wiring:
- BMP280 VCC → Shield 3.3V
- BMP280 GND → Shield GND
- BMP280 SCL → Shield D22 (I2C Clock)
- BMP280 SDA → Shield D21 (I2C Data)
```

### 3. Rain Drop Sensor Module
```
Rain Sensor Pinout:  Shield Connection:
┌─────────────────┐  ┌─────────────────┐
│VCC GND DO AO    │  │3V3 GND -- D34   │
└─────────────────┘  └─────────────────┘
  │   │   │  │        │   │      │
  │   │   │  └────────┼───┼──────┘
  │   │   └───────────┼───┼─── (Not Used)
  │   └───────────────┼───┘
  └───────────────────┘

Wiring:
- Rain Sensor VCC → Shield 3.3V
- Rain Sensor GND → Shield GND
- Rain Sensor AO → Shield D34 (Analog Input)
- Rain Sensor DO → Not Connected
```

### 4. LCD 16x2 with I2C Module
```
LCD I2C Pinout:      Shield Connection:
┌─────────────────┐  ┌─────────────────┐
│VCC GND SDA SCL  │  │5V  GND D21 D22  │
└─────────────────┘  └─────────────────┘
  │   │   │   │        │   │   │   │
  │   │   │   └────────┼───┼───┼───┘
  │   │   └────────────┼───┼───┘
  │   └────────────────┼───┘
  └────────────────────┘

Wiring:
- LCD VCC → Shield 5V (atau 3.3V jika mendukung)
- LCD GND → Shield GND
- LCD SDA → Shield D21 (I2C Data)
- LCD SCL → Shield D22 (I2C Clock)
```

## Complete Fritzing Connection Table

| Component | Pin | Shield Terminal | Wire Color | Notes |
|-----------|-----|-----------------|------------|-------|
| **DHT11** | VCC | 3.3V | Red | Power |
| | DATA | D4 | Yellow | Digital Data |
| | GND | GND | Black | Ground |
| **BMP280** | VCC | 3.3V | Red | Power |
| | GND | GND | Black | Ground |
| | SDA | D21 | Blue | I2C Data |
| | SCL | D22 | Green | I2C Clock |
| **Rain Sensor** | VCC | 3.3V | Red | Power |
| | GND | GND | Black | Ground |
| | AO | D34 | Orange | Analog Output |
| **LCD I2C** | VCC | 5V | Red | Power (5V preferred) |
| | GND | GND | Black | Ground |
| | SDA | D21 | Blue | I2C Data (shared) |
| | SCL | D22 | Green | I2C Clock (shared) |

## Fritzing Breadboard View Layout

```
                    BREADBOARD VIEW
    ┌─────────────────────────────────────────────────────┐
    │                                                     │
    │  ESP32 SHIELD                                       │
    │  ┌─────────────────────────────────────────────┐    │
    │  │ [ESP32 DEVKIT MOUNTED ON SHIELD]            │    │
    │  └─────────────────────────────────────────────┘    │
    │                                                     │
    │  DHT11          BMP280         Rain Sensor          │
    │  ┌─────┐        ┌───────┐      ┌─────────┐          │
    │  │ ┌─┐ │        │ ┌───┐ │      │ ┌─────┐ │          │
    │  │ └─┘ │        │ └───┘ │      │ └─────┘ │          │
    │  └─────┘        └───────┘      └─────────┘          │
    │     │               │              │                │
    │     │               │              │                │
    │  ┌──▼───────────────▼──────────────▼──┐             │
    │  │  Terminal Blocks / Headers        │             │
    │  └───────────────────────────────────────┘          │
    │                                                     │
    │  LCD 16x2 with I2C                                  │
    │  ┌─────────────────────────────────────┐            │
    │  │ ████████████████████████████████████ │            │
    │  │ ████████████████████████████████████ │            │
    │  └─────────────────────────────────────┘            │
    │                    │                                │
    │                    │                                │
    │  ┌─────────────────▼─────────────────┐              │
    │  │     I2C Connection to Shield      │              │
    │  └───────────────────────────────────┘              │
    └─────────────────────────────────────────────────────┘
```

## Fritzing Schematic View

```
                    SCHEMATIC VIEW

ESP32 (GPIO Pins)                    Sensors
┌─────────────┐                     ┌─────────────┐
│             │                     │    DHT11    │
│    GPIO 4   │────────────────────▶│    DATA     │
│             │                     │             │
│   GPIO 21   │──┬─────────────────▶│   BMP280    │
│    (SDA)    │  │                  │    SDA      │
│             │  │                  │             │
│   GPIO 22   │──┼──┬──────────────▶│   BMP280    │
│    (SCL)    │  │  │               │    SCL      │
│             │  │  │               │             │
│   GPIO 34   │  │  │               │ Rain Sensor │
│    (ADC)    │──┼──┼──────────────▶│     AO      │
│             │  │  │               │             │
│    3.3V     │──┼──┼──┬───────────▶│   VCC (All) │
│             │  │  │  │            │             │
│     GND     │──┼──┼──┼───────────▶│   GND (All) │
│             │  │  │  │            │             │
│     5V      │──┼──┼──┼──┬────────▶│  LCD VCC    │
└─────────────┘  │  │  │  │         │             │
                 │  │  │  │         │    LCD      │
                 └──┼──┼──┼────────▶│    SDA      │
                    │  │  │         │             │
                    └──┼──┼────────▶│    LCD      │
                       │  │         │    SCL      │
                       │  │         └─────────────┘
                       │  │
                    Power Rails
                    ┌──▼──▼──┐
                    │ Shield │
                    │Terminal│
                    │ Blocks │
                    └────────┘
```

## Power Distribution Diagram

```
                    POWER DISTRIBUTION

External Power Supply (5V/2A)
            │
            ▼
    ┌───────────────┐
    │  ESP32 SHIELD │
    │   ┌─────────┐ │
    │   │ 5V Rail │ │ ──────────▶ LCD I2C Module
    │   └─────────┘ │
    │   ┌─────────┐ │
    │   │3.3V Rail│ │ ──┬──────▶ DHT11
    │   └─────────┘ │   │
    │   ┌─────────┐ │   ├──────▶ BMP280
    │   │GND Rail │ │   │
    │   └─────────┘ │   └──────▶ Rain Sensor
    └───────────────┘
            │
            ▼
    All GND connections
```

## I2C Bus Diagram

```
                    I2C BUS TOPOLOGY

ESP32 Shield
┌─────────────┐
│   GPIO 21   │ ──── SDA Bus ────┬──▶ BMP280 (0x76/0x77)
│    (SDA)    │                  │
│             │                  └──▶ LCD I2C (0x27/0x3F)
│   GPIO 22   │ ──── SCL Bus ────┬──▶ BMP280
│    (SCL)    │                  │
└─────────────┘                  └──▶ LCD I2C

Note: Both devices share the same I2C bus
      Each device has unique I2C address
```

## Fritzing Parts to Search For:

1. **ESP32 DevKit v1** - Search: "ESP32 WROOM-32"
2. **ESP32 Shield** - Search: "ESP32 expansion board" or "proto shield"
3. **DHT11** - Search: "DHT11 temperature humidity"
4. **BMP280** - Search: "BMP280 pressure sensor"
5. **Rain Sensor** - Search: "rain drop sensor module"
6. **LCD I2C** - Search: "LCD 16x2 I2C" or "LCD with I2C backpack"
7. **Jumper Wires** - Search: "jumper wire female"

## Step-by-Step Fritzing Creation:

### 1. Add Components:
- Drag ESP32 DevKit to breadboard view
- Add ESP32 shield/expansion board
- Add all sensors (DHT11, BMP280, Rain, LCD)

### 2. Position Components:
- Place ESP32 on shield
- Arrange sensors around the shield
- Position LCD separately

### 3. Add Connections:
- Use colored wires as per the table above
- Red for VCC/Power
- Black for GND
- Blue for SDA
- Green for SCL
- Yellow for DHT11 data
- Orange for Rain sensor analog

### 4. Label Connections:
- Add text labels for pin numbers
- Mark I2C addresses
- Add voltage levels (3.3V, 5V)

### 5. Create Schematic:
- Switch to schematic view
- Connect all components logically
- Add power symbols and ground symbols

This guide provides all the information needed to create a professional Fritzing diagram for your ESP32 Shield Weather Station project!