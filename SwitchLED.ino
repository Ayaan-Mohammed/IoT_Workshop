#define LED_PIN D1
#define SW1_PIN D2
void setup() {
 pinMode(LED_PIN, OUTPUT);
 pinMode(SW1_PIN, INPUT_PULLUP);
 Serial.begin(115200);
}
void loop() {
 int buttonState = digitalRead(SW1_PIN);
 if (buttonState == LOW) {
   digitalWrite(LED_PIN, HIGH);
   Serial.println("Switch ON");
 } else {
   digitalWrite(LED_PIN, LOW);
   Serial.println("Switch OFF");
 }
 delay(200);
}
