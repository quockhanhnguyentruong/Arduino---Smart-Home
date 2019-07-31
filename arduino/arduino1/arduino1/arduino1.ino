#include "DHT.h"

#define DHTPIN 2

#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
int alert = 0;
int alertPin = 3;
String temp = "";
void setup() {
  Serial.begin(9600);
  dht.begin();
  // put your setup code here, to run once:
}

void loop() {
  
  String temp = ReadHumiAndTemp();
  // put your main code here, to run repeatedly:
  String data = "";
  while(Serial.available() > 0){
       data+= Serial.read();
  }
  if(data.length() > 1){
      if(alert == 0){
        Serial.println(temp);
      }
      else{
        Serial.println("SOS");
        
      }
  }
  
  Flame();
  if(alert == 1){
    Alert();  
  }
  delay(250);
}
String ReadHumiAndTemp(){
  String data = "";
  float h = dht.readHumidity();  
  float t = dht.readTemperature();
  data += String(t) + ":" + String(h);
  return data;
}
void Flame(){
  int sensorReading = analogRead(A0);
  if(sensorReading < 100){
    alert = 1;
  }
}
void Alert(){
  tone(alertPin, 1000, 500); 
}
