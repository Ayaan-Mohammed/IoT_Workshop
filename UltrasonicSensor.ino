#define TRIG_PIN D1
#define ECHO_PIN D2
void setup() {
 Serial.begin(115200);
 pinMode(TRIG_PIN, OUTPUT);
 pinMode(ECHO_PIN, INPUT);
}
void loop() {
 long duration;
 float distance;
 digitalWrite(TRIG_PIN, LOW);
 delayMicroseconds(2);
 digitalWrite(TRIG_PIN, HIGH);
 delayMicroseconds(10);
 digitalWrite(TRIG_PIN, LOW);
 duration = pulseIn(ECHO_PIN, HIGH);
 distance = duration * 0.034 / 2;
 Serial.print("Distance: ");
 Serial.print(distance);
 Serial.println(" cm");
 delay(500);
}
