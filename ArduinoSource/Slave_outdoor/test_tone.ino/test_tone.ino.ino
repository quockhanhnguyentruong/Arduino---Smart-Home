int pinSpeaker = 11;

void setup() {
  // put your setup code here, to run once:
  pinMode(pinSpeaker, OUTPUT);
  Serial.begin(9600);
  
}
void loop() {
  // put your main code here, to run repeatedly:
  analogWrite(9, 20);
  delay(delayms);
  analogWrite(9, 0);
  delay(delayms);
  
  delay(1000);
}

void playTone()
{
  for(int i = 0; i < 1000; i++){
    digitalWrite(pinSpeaker,HIGH);
    delayMicroseconds(i);
    digitalWrite(pinSpeaker,LOW);
    delayMicroseconds(i);
  }
}
