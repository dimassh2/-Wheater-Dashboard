#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <LiquidCrystal_I2C.h>

// Pin definitions for ESP32 Shield
#define DHT_PIN 4
#define DHT_TYPE DHT11
#define RAIN_SENSOR_PIN 34
#define SDA_PIN 21
#define SCL_PIN 22

// Sensor objects
DHT dht(DHT_PIN, DHT_TYPE);
Adafruit_BMP280 bmp;
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 columns, 2 rows

// WiFi credentials - GANTI DENGAN KREDENSIAL WIFI ANDA
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Web server
WebServer server(80);

// Sensor data structure
struct SensorData {
  float temperature;
  float humidity;
  float pressure;
  float altitude;
  bool isRaining;
  String timestamp;
};

// Global variables
SensorData currentData;
unsigned long lastSensorRead = 0;
unsigned long lastLCDUpdate = 0;
const unsigned long SENSOR_INTERVAL = 2000; // Read sensors every 2 seconds
const unsigned long LCD_INTERVAL = 3000;    // Update LCD every 3 seconds

void setup() {
  Serial.begin(115200);
  
  // Initialize I2C with ESP32 Shield default pins
  Wire.begin(SDA_PIN, SCL_PIN);
  
  // Initialize sensors
  dht.begin();
  
  // Try both common BMP280 I2C addresses
  if (!bmp.begin(0x76)) { 
    if (!bmp.begin(0x77)) { 
      Serial.println("Could not find BMP280 sensor!");
      Serial.println("Check wiring and I2C address!");
      while (1);
    }
  }
  
  // Configure BMP280 for optimal readings
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,
                  Adafruit_BMP280::SAMPLING_X2,
                  Adafruit_BMP280::SAMPLING_X16,
                  Adafruit_BMP280::FILTER_X16,
                  Adafruit_BMP280::STANDBY_MS_500);
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Weather Station");
  lcd.setCursor(0, 1);
  lcd.print("ESP32 Shield");
  delay(2000);
  
  // Initialize rain sensor pin (no external resistor needed)
  pinMode(RAIN_SENSOR_PIN, INPUT);
  
  // Connect to WiFi
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi");
  
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  
  int wifiAttempts = 0;
  while (WiFi.status() != WL_CONNECTED && wifiAttempts < 20) {
    delay(500);
    Serial.print(".");
    lcd.setCursor(wifiAttempts % 16, 1);
    lcd.print(".");
    wifiAttempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.println("WiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    
    // Display IP on LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WiFi Connected!");
    lcd.setCursor(0, 1);
    lcd.print(WiFi.localIP());
    delay(3000);
  } else {
    Serial.println("WiFi connection failed!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WiFi Failed!");
    lcd.setCursor(0, 1);
    lcd.print("Check Settings");
  }
  
  // Setup web server routes
  setupWebServer();
  
  // Start server
  server.begin();
  Serial.println("HTTP server started");
  Serial.println("Ready to serve sensor data!");
}

void loop() {
  // Handle web server requests
  server.handleClient();
  
  // Read sensors periodically
  if (millis() - lastSensorRead >= SENSOR_INTERVAL) {
    readSensors();
    lastSensorRead = millis();
  }
  
  // Update LCD periodically
  if (millis() - lastLCDUpdate >= LCD_INTERVAL) {
    updateLCD();
    lastLCDUpdate = millis();
  }
}

void readSensors() {
  // Read DHT11 (built-in pull-up, no external resistor needed)
  currentData.temperature = dht.readTemperature();
  currentData.humidity = dht.readHumidity();
  
  // Check if DHT readings are valid
  if (isnan(currentData.temperature) || isnan(currentData.humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    // Keep previous values or set defaults
    if (isnan(currentData.temperature)) currentData.temperature = 25.0;
    if (isnan(currentData.humidity)) currentData.humidity = 60.0;
  }
  
  // Read BMP280
  currentData.pressure = bmp.readPressure() / 100.0F; // Convert Pa to hPa
  currentData.altitude = bmp.readAltitude(1013.25); // Sea level pressure in hPa
  
  // Read rain sensor (analog reading, lower value = more water)
  int rainValue = analogRead(RAIN_SENSOR_PIN);
  currentData.isRaining = rainValue < 2000; // Adjust threshold as needed
  
  // Create timestamp
  currentData.timestamp = String(millis());
  
  // Print to serial for debugging
  Serial.println("=== Sensor Readings ===");
  Serial.println("Temperature: " + String(currentData.temperature, 1) + "°C");
  Serial.println("Humidity: " + String(currentData.humidity, 1) + "%");
  Serial.println("Pressure: " + String(currentData.pressure, 1) + " hPa");
  Serial.println("Altitude: " + String(currentData.altitude, 1) + " m");
  Serial.println("Rain Value: " + String(rainValue) + " (Raining: " + String(currentData.isRaining ? "Yes" : "No") + ")");
  Serial.println("Free Heap: " + String(ESP.getFreeHeap()) + " bytes");
  Serial.println("========================");
}

void updateLCD() {
  static int displayMode = 0;
  
  lcd.clear();
  
  switch (displayMode) {
    case 0: // Temperature and Humidity
      lcd.setCursor(0, 0);
      lcd.print("T:" + String(currentData.temperature, 1) + "C");
      lcd.setCursor(9, 0);
      lcd.print("H:" + String(currentData.humidity, 1) + "%");
      lcd.setCursor(0, 1);
      if (currentData.isRaining) {
        lcd.print("Status: HUJAN");
      } else if (currentData.temperature > 29) {
        lcd.print("Status: CERAH");
      } else {
        lcd.print("Status: BERAWAN");
      }
      break;
      
    case 1: // Pressure and Altitude
      lcd.setCursor(0, 0);
      lcd.print("P:" + String(currentData.pressure, 0) + "hPa");
      lcd.setCursor(0, 1);
      lcd.print("Alt:" + String(currentData.altitude, 0) + "m");
      break;
      
    case 2: // Network Info
      lcd.setCursor(0, 0);
      lcd.print("IP Address:");
      lcd.setCursor(0, 1);
      if (WiFi.status() == WL_CONNECTED) {
        lcd.print(WiFi.localIP());
      } else {
        lcd.print("Not Connected");
      }
      break;
      
    case 3: // System Info
      lcd.setCursor(0, 0);
      lcd.print("Uptime:" + String(millis()/1000) + "s");
      lcd.setCursor(0, 1);
      lcd.print("Heap:" + String(ESP.getFreeHeap()/1024) + "KB");
      break;
  }
  
  displayMode = (displayMode + 1) % 4; // Cycle through 4 display modes
}

void setupWebServer() {
  // Enable CORS for all routes
  server.onNotFound([]() {
    if (server.method() == HTTP_OPTIONS) {
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
      server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
      server.send(200);
    } else {
      server.send(404, "text/plain", "Not Found");
    }
  });
  
  // Main sensor data endpoint
  server.on("/sensor", HTTP_GET, []() {
    // Create JSON response
    StaticJsonDocument<400> doc;
    
    doc["temperature"] = String(currentData.temperature, 1);
    doc["humidity"] = String(currentData.humidity, 1);
    doc["pressure"] = String(currentData.pressure, 1);
    doc["altitude"] = String(currentData.altitude, 1);
    doc["isRaining"] = currentData.isRaining;
    doc["timestamp"] = currentData.timestamp;
    doc["uptime"] = millis();
    doc["free_heap"] = ESP.getFreeHeap();
    doc["wifi_rssi"] = WiFi.RSSI();
    
    String jsonString;
    serializeJson(doc, jsonString);
    
    // Send response with CORS headers
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    server.send(200, "application/json", jsonString);
    
    Serial.println("Sensor data requested via HTTP");
  });
  
  // Root endpoint for testing
  server.on("/", HTTP_GET, []() {
    String html = "<!DOCTYPE html><html><head><title>ESP32 Weather Station</title>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
    html += "<style>body{font-family:Arial;margin:40px;background:#f0f0f0;}";
    html += ".card{background:white;padding:20px;margin:10px 0;border-radius:8px;box-shadow:0 2px 4px rgba(0,0,0,0.1);}";
    html += ".value{font-size:2em;color:#2196F3;font-weight:bold;}";
    html += ".label{color:#666;margin-bottom:5px;}</style></head><body>";
    html += "<h1>🌤️ ESP32 Weather Station</h1>";
    html += "<div class='card'><div class='label'>Temperature</div><div class='value'>" + String(currentData.temperature, 1) + "°C</div></div>";
    html += "<div class='card'><div class='label'>Humidity</div><div class='value'>" + String(currentData.humidity, 1) + "%</div></div>";
    html += "<div class='card'><div class='label'>Pressure</div><div class='value'>" + String(currentData.pressure, 1) + " hPa</div></div>";
    html += "<div class='card'><div class='label'>Altitude</div><div class='value'>" + String(currentData.altitude, 1) + " m</div></div>";
    html += "<div class='card'><div class='label'>Rain Status</div><div class='value'>" + String(currentData.isRaining ? "🌧️ Raining" : "☀️ Dry") + "</div></div>";
    html += "<br><p><a href='/sensor'>📊 JSON Data</a> | <a href='/status'>ℹ️ System Status</a></p>";
    html += "<p><small>ESP32 Shield Weather Station | Uptime: " + String(millis()/1000) + "s</small></p>";
    html += "</body></html>";
    
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "text/html", html);
  });
  
  // Status endpoint
  server.on("/status", HTTP_GET, []() {
    StaticJsonDocument<300> doc;
    doc["status"] = "online";
    doc["device"] = "ESP32 Shield Weather Station";
    doc["uptime"] = millis();
    doc["free_heap"] = ESP.getFreeHeap();
    doc["wifi_ssid"] = WiFi.SSID();
    doc["wifi_rssi"] = WiFi.RSSI();
    doc["ip_address"] = WiFi.localIP().toString();
    doc["mac_address"] = WiFi.macAddress();
    
    String jsonString;
    serializeJson(doc, jsonString);
    
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "application/json", jsonString);
  });
}