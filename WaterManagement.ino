/****************************************************
 * Smart Flood System - FINAL VERSION
 * ESP8266 + ADS1115 + 3 Sensors + Firebase
 ****************************************************/

#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include <Wire.h>
#include <Adafruit_ADS1X15.h>

// ---------------- WIFI ----------------
#define WIFI_SSID "YOUR_WIFI"
#define WIFI_PASSWORD "YOUR_PASSWORD"

// ---------------- FIREBASE ----------------
#define API_KEY "YOUR_API_KEY"
#define DATABASE_URL "YOUR_DATABASE_URL"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// ---------------- ADS1115 ----------------
Adafruit_ADS1115 ads;

// ---------------- PINS ----------------
#define RELAY_PIN D6
#define FLOAT_PIN D5

// ---------------- THRESHOLDS ----------------
#define SOIL_THRESHOLD 15000
#define RAIN_THRESHOLD 15000
#define FLOAT_TRIGGERED LOW

// ---------------- VARIABLES ----------------
int soilValue, rainValue;
bool floatState;
bool flood = false;

// Stability control
bool lastFloodState = false;
unsigned long lastChangeTime = 0;
#define STABILITY_DELAY 3000   // ms

// ---------------- SETUP ----------------
void setup() {
  Serial.begin(115200);

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(FLOAT_PIN, INPUT_PULLUP);
  digitalWrite(RELAY_PIN, HIGH);

  if (!ads.begin()) {
    Serial.println("ADS1115 not detected!");
    while (1);
  }

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected!");

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

// ---------------- SENSOR FUNCTIONS ----------------
int readSoil() {
  return ads.readADC_SingleEnded(0);
}

int readRain() {
  return ads.readADC_SingleEnded(1);
}

bool readFloat() {
  return digitalRead(FLOAT_PIN) == FLOAT_TRIGGERED;
}

// ---------------- SMART LOGIC ----------------
bool evaluateFlood(int soil, int rain, bool floatState) {

  bool soilWet = soil < SOIL_THRESHOLD;
  bool heavyRain = rain < RAIN_THRESHOLD;

  int riskCount = 0;

  if (soilWet) riskCount++;
  if (heavyRain) riskCount++;
  if (floatState) riskCount++;

  return (riskCount >= 2);
}

// ---------------- STABILITY FILTER ----------------
bool stableFloodDecision(bool currentState) {

  if (currentState != lastFloodState) {
    lastChangeTime = millis();
  }

  if ((millis() - lastChangeTime) > STABILITY_DELAY) {
    lastFloodState = currentState;
  }

  return lastFloodState;
}

// ---------------- ACTUATION ----------------
void controlValve(bool state) {
  digitalWrite(RELAY_PIN, state ? LOW : HIGH);
}

// ---------------- FIREBASE ----------------
void updateFirebase() {
  Firebase.RTDB.setInt(&fbdo, "/flood/soil", soilValue);
  Firebase.RTDB.setInt(&fbdo, "/flood/rain", rainValue);
  Firebase.RTDB.setBool(&fbdo, "/flood/float", floatState);
  Firebase.RTDB.setBool(&fbdo, "/flood/status", flood);
}

// ---------------- LOOP ----------------
void loop() {

  // 1. Read sensors
  soilValue = readSoil();
  rainValue = readRain();
  floatState = readFloat();

  // 2. Intelligent decision
  bool rawFlood = evaluateFlood(soilValue, rainValue, floatState);

  // 3. Stability filtering (prevents flicker)
  flood = stableFloodDecision(rawFlood);

  // 4. Control valve
  controlValve(flood);

  // 5. Update Firebase
  updateFirebase();

  // 6. Debug
  Serial.print("Soil: "); Serial.print(soilValue);
  Serial.print(" | Rain: "); Serial.print(rainValue);
  Serial.print(" | Float: "); Serial.print(floatState);
  Serial.print(" | Flood: "); Serial.println(flood);

  delay(2000);
}
