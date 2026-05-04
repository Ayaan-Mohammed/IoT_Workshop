#define LED1 D1
#define LED2 D2
void setup() 
{
 pinMode(LED1, OUTPUT);
 pinMode(LED2, OUTPUT);
}
void loop() {
 digitalWrite(LED1, HIGH);
 digitalWrite(LED2, LOW);
 delay(1000);
 digitalWrite(LED1, LOW);
 digitalWrite(LED2, HIGH);
 delay(1000);
}
