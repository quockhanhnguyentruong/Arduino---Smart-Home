#include "DHT.h"

#define DHTPIN 2

#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
int alert = 0;
void setup() {
  Serial.begin(9600);
  dht.begin();
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
  if(alert == 0){
    ReadHumiAndTemp();
  }
  else{
    Serial.println("SOS");
  }
  Flame();
  delay(250);
}
void ReadHumiAndTemp(){
  float h = dht.readHumidity();  
  float t = dht.readTemperature();
  Serial.print(t);
  Serial.print(":");
  Serial.print(h);
  Serial.println();
}
void Flame(){
  int sensorReading = analogRead(A0);
  if(sensorReading < 100){
    alert = 1;
  }
}
