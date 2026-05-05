#define LED_PIN D1   // GPIO5
void setup() {
 pinMode(LED_PIN, OUTPUT);
}
void loop() {
 digitalWrite(LED_PIN, HIGH); // LED ON
 delay(1000);
 digitalWrite(LED_PIN, LOW);  // LED OFF
 delay(1000);
}
