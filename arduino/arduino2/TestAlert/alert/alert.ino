
int piezoPin = 5;

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  tone(piezoPin, 1000, 500);
  delay(1000);
}
