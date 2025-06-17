#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <LiquidCrystal_I2C.h>

// Pin definitions
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
  
  // Initialize I2C
  Wire.begin(SDA_PIN, SCL_PIN);
  
  // Initialize sensors
  dht.begin();
  
  if (!bmp.begin(0x76)) { // Try address 0x76 first
    if (!bmp.begin(0x77)) { // Then try 0x77
      Serial.println("Could not find BMP280 sensor!");
      while (1);
    }
  }
  
  // Configure BMP280
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
  lcd.print("Starting...");
  
  // Initialize rain sensor pin
  pinMode(RAIN_SENSOR_PIN, INPUT);
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  // Display IP on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("IP:");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  delay(3000);
  
  // Setup web server routes
  setupWebServer();
  
  // Start server
  server.begin();
  Serial.println("HTTP server started");
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
  // Read DHT11
  currentData.temperature = dht.readTemperature();
  currentData.humidity = dht.readHumidity();
  
  // Read BMP280
  currentData.pressure = bmp.readPressure() / 100.0F; // Convert to hPa
  currentData.altitude = bmp.readAltitude(1013.25); // Sea level pressure
  
  // Read rain sensor (digital reading)
  int rainValue = analogRead(RAIN_SENSOR_PIN);
  currentData.isRaining = rainValue < 2000; // Adjust threshold as needed
  
  // Create timestamp
  currentData.timestamp = String(millis());
  
  // Print to serial for debugging
  Serial.println("=== Sensor Readings ===");
  Serial.println("Temperature: " + String(currentData.temperature) + "°C");
  Serial.println("Humidity: " + String(currentData.humidity) + "%");
  Serial.println("Pressure: " + String(currentData.pressure) + " hPa");
  Serial.println("Altitude: " + String(currentData.altitude) + " m");
  Serial.println("Rain: " + String(currentData.isRaining ? "Yes" : "No"));
  Serial.println("========================");
}

void updateLCD() {
  static int displayMode = 0;
  
  lcd.clear();
  
  switch (displayMode) {
    case 0: // Temperature and Humidity
      lcd.setCursor(0, 0);
      lcd.print("T:" + String(currentData.temperature, 1) + "C");
      lcd.setCursor(8, 0);
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
      
    case 2: // IP Address
      lcd.setCursor(0, 0);
      lcd.print("IP Address:");
      lcd.setCursor(0, 1);
      lcd.print(WiFi.localIP());
      break;
  }
  
  displayMode = (displayMode + 1) % 3; // Cycle through 3 display modes
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
    StaticJsonDocument<300> doc;
    
    doc["temperature"] = String(currentData.temperature, 1);
    doc["humidity"] = String(currentData.humidity, 1);
    doc["pressure"] = String(currentData.pressure, 0);
    doc["altitude"] = String(currentData.altitude, 0);
    doc["isRaining"] = currentData.isRaining;
    doc["timestamp"] = currentData.timestamp;
    
    String jsonString;
    serializeJson(doc, jsonString);
    
    // Send response with CORS headers
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    server.send(200, "application/json", jsonString);
  });
  
  // Root endpoint for testing
  server.on("/", HTTP_GET, []() {
    String html = "<!DOCTYPE html><html><head><title>ESP32 Weather Station</title></head><body>";
    html += "<h1>ESP32 Weather Station</h1>";
    html += "<h2>Current Readings:</h2>";
    html += "<p>Temperature: " + String(currentData.temperature, 1) + "°C</p>";
    html += "<p>Humidity: " + String(currentData.humidity, 1) + "%</p>";
    html += "<p>Pressure: " + String(currentData.pressure, 0) + " hPa</p>";
    html += "<p>Altitude: " + String(currentData.altitude, 0) + " m</p>";
    html += "<p>Rain: " + String(currentData.isRaining ? "Yes" : "No") + "</p>";
    html += "<br><a href='/sensor'>JSON Data</a>";
    html += "</body></html>";
    
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "text/html", html);
  });
  
  // Status endpoint
  server.on("/status", HTTP_GET, []() {
    StaticJsonDocument<200> doc;
    doc["status"] = "online";
    doc["uptime"] = millis();
    doc["free_heap"] = ESP.getFreeHeap();
    doc["wifi_rssi"] = WiFi.RSSI();
    
    String jsonString;
    serializeJson(doc, jsonString);
    
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "application/json", jsonString);
  });
}