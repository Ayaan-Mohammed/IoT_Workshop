#include <DHT.h>
#include <Adafruit_Sensor.h>
#define DHTPIN D4      
#define DHTTYPE DHT11    
DHT dht(DHTPIN, DHTTYPE);
void setup() {
 Serial.begin(115200);
  Serial.println("DHT11 Sensor with ESP8266");
  dht.begin();
}
void loop() 
{
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(2000);
    return;
  }
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %  |  ");

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  delay(2000);
}
