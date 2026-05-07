#define FIREBASE_DISABLE_SD  // ← Add this line
#define FIREBASE_DISABLE_ONBOARD_WIFI

#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include <DHT.h>

// WiFi credentials
#define WIFI_SSID "wifi"
#define WIFI_PASSWORD "wifipassword"

// Firebase credentials
#define API_KEY "YOUR_API_KEY"
#define DATABASE_URL "YOUR_DB_URL"

// DHT settings
#define DHTPIN D2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

void setup()
{
  Serial.begin(115200);

  dht.begin();

  // Connect WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.println("WiFi Connected");

  // Firebase config
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop()
{
  if (Firebase.ready() && millis() - sendDataPrevMillis > 5000)
  {
    sendDataPrevMillis = millis();

    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    if (isnan(temperature) || isnan(humidity))
    {
      Serial.println("DHT read failed");
      return;
    }
 
    Serial.print("Temp: ");
    Serial.println(temperature);

    Serial.print("Humidity: ");
    Serial.println(humidity);

    // Send to Firebase
    Firebase.RTDB.setFloat(&fbdo, "/Sensor/Temperature", temperature);
    Firebase.RTDB.setFloat(&fbdo, "/Sensor/Humidity", humidity);

    Serial.println("Uploaded to Firebase");
  }
}
