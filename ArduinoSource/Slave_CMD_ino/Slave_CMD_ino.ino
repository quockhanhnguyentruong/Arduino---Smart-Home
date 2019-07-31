#include <Servo.h>
#include <Wire.h>

int led = 2;
int air = 3;
int fan = 4;
int isOpen = 0;
String state = "";

// ------------------ Repair for Servo -------------------
Servo myServo;
int pinServo = 5;
int pos = 150;
int count = 0;
int x = 0; // For backdoor

// ----------- Read device and MODE from PI --------------
String getDevice(String data){
  int pos = data.indexOf(":");
  return data.substring(0,pos);
}
int mode(String data){
  int pos = data.indexOf(":");
  return data.substring(pos+1).toInt();
}


// -------------- Turn on/off device ------------------------
void command(String device,int mode){
  if(device == "led"){
      if(mode != digitalRead(led)){
        digitalWrite(led,mode);
       
      }
  }
  else if (device == "air"){
    if(mode != digitalRead(air)){
        digitalWrite(air,mode);
    }
  }
  else if (device == "fan"){
    FanMode(mode);
  }
  else if (device == "door"){
    if(mode == 1 && isOpen == 0){
      OpenDoor();
      isOpen = 1;
    }
    else if (mode == 0 && isOpen == 1){
      CloseDoor();
      isOpen = 0;
    }
  }
}


// ---------------------- Set Up --------------------
void setup() {
  // put your setup code here, to run once:
  myServo.attach(pinServo);
  
  Wire.begin(69);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  
  Serial.begin(9600);
  pinMode(led,OUTPUT);
  pinMode(air,OUTPUT);
  pinMode(fan,OUTPUT);
  digitalWrite(led,0);
  digitalWrite(air,0);
  FanMode(0);
  CloseDoor();
}

// ----------------------- Loop ---------------------
void loop() {
  // put your main code here, to run repeatedly:
  //state = GetDeviceState();

  String data = "";
  while(Serial.available() > 0){
       data+= Serial.readString();
   }
   Split(data);
   state = GetDeviceState()
}

// ------- split device,mode and execute command -------
void Split(String data){
  if(data == "state"){
    GetDeviceState();
  }
  else if(data.length() > 0)
  {  
    String device = getDevice(data);
    int m = mode(data);
    command(device,m);
  }
}

// ------------------- Get Device State --------------------------
String GetDeviceState(){
    String light = String(digitalRead(led));
    String airCon = String(digitalRead(air));
    String Fan = String(digitalRead(fan));
    String ret =light+":"+airCon+":"+Fan+":"+String(isOpen);
    return ret;
}

// ------------------ Door process --------------------
void CloseDoor(){
  int pos = 110;
    for(int i = pos; i >= 0 ; i--){
      myServo.write(i);
      delay(15);
    }
}
void OpenDoor(){
  int pos = 110;
  for(int i = 0; i <= pos ; i++){
      myServo.write(i);
      delay(15);
   }
}
// ------------- Repair for Salve Mode -------------
void receiveEvent(int howMany) {
  String data = "";
  while (0 <Wire.available()) {
    char c = Wire.read();     
    data += c;
  }
  Serial.println(data);
  Split(data);
}

// function that executes whenever data is requested from master
void requestEvent() {
  char d[100] = "";
  state = GetDeviceState();
  int len = state.length()+1;  
  state.toCharArray(d,len);
  Wire.write(d,len);
}

// ------------------ Fan mode ---------------------------

void FanMode(int mode)
{
    if(mode >= 0 && mode <= 4)
    {
      analogWrite(fan, map(mode,0,4,0,255));
    }
}
