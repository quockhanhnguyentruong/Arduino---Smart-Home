#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include "DHT.h"

#define DHTPIN D5
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

//Key = X5099CSA4TV6SVAM
#define ssid "abcd"
#define password "12345678969"
#define server "mqtt.thingspeak.com"
#define port 1883
char* topic = "channels/591053/publish/X5099CSA4TV6SVAM";
WiFiClient wifiClient;
PubSubClient client(server, port, wifiClient);
void setup() {
  Serial.begin(9600);
  delay(10);
  Serial.println("......................................................");
  Serial.print("Connecting");
  dht.begin();
  Serial.println(ssid);
  WiFi.begin("abcd", "12345678969");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  String clientName="ESP-Thingspeak";
  Serial.print("Connecting");
  Serial.print(server);
  Serial.print(" as ");
  Serial.println(clientName);
  
  if (client.connect((char*) clientName.c_str())) {
    Serial.println("Connected to MQTT broker");
    Serial.print("Topic");
    Serial.println(topic);
    
    if (client.publish(topic, "hello from ESP8266")) {
      Serial.println("Publish ok");
    }
    else {
      Serial.println("Publish failed");
    }
  }
  else {
    Serial.println("MQTT connect failed");
    Serial.println("Will reset and try again...");
    abort();
  }
}
void loop() {
  float temp = dht.readTemperature();
  float humi = dht.readHumidity();
  Serial.println(temp);
  Serial.println(humi);
  static int counter = 0;
  String payload="field1=";
  payload+=temp;
  payload+="&field2=";
  payload+=humi;
  payload+="&status=MQTTPUBLISH";
  
  if (client.connected()){
    Serial.print("Sendingoad: ");
    Serial.println(payload);
    
    if (client.publish(topic, (char*) payload.c_str())) {
      Serial.println("Publish ok");
    }
    else {
      Serial.println("Publish failed");
    }
  }
  else{
    Serial.println("Disconnect from server...!");  
  }
  delay(3000);
}
