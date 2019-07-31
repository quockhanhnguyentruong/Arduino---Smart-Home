
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

//--------- Repair for pad passwd -------
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
String passwd = "6996";
String test = "";
int Dem = 1;
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {13,12,11,10}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {9,8,7,6}; //connect to the column pinouts of the keypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

//---------------------------------------
// ----------------- Move sensor ------------
int pirPin = 3;
int ledPin = 4;
int pirState = LOW;


//----------------Repair for LCD ---------------
LiquidCrystal_I2C lcd(0x27,16,2);

// -------------- Repair for Master -------------

void Send2Slave(String data){
    char d[] = "";
    int len = data.length()+1;
    data.toCharArray(d,len);
    Wire.beginTransmission(69);
    Wire.write(d,len);
    Wire.endTransmission(69);
}
void GetFromSlave(){
    Wire.requestFrom(69, 7);
    String data = "";
    while(Wire.available()){
      char c = Wire.read();
      data += c;
    }
    if(data.length() > 0){
      Serial.println(data);
    }
}



// ----------------- Set Up ---------------

void setup() {
  // put your setup code here, to run once:
  lcd.init();
  lcd.backlight();
  pinMode(ledPin, OUTPUT);
  pinMode(pirPin, INPUT);
  Wire.begin(69);
  Serial.begin(9600);
  PrintData(" B3bieSecTeam => Big Project2",0);
}

// ------------------- LOOP ----------------
void loop() {
  GetFromSlave();
  OpenLedOutDoor();
  PasswdTouch();
  delay(250);
}

// ------------------- Password Touch ----------------
void PasswdTouch(){
  char customKey = customKeypad.getKey();
  if(customKey == '#'){
    if(passwd.equals(test)){
      PrintData("The door is opened!",0);
      Send2Slave("door:1");
      Dem=0;
      test = "";
    }
    else{
      PrintData("Wrong Password!"+String(Dem),1);
      test = "";
      Dem +=1;
    } 
  }
  else if(customKey == '*'){
    test = "";
    PrintData("The door is closed!",0);
    Send2Slave("door:0");
    Dem=0;
  }
  else if(test.length() > 4){
    test = "";
    PrintData("Wrong Password!"+String(Dem),1);
    Dem+=1;
  }
  else if(customKey == 'A'){
      test = "";
      clearRowX(0);
  }
  else if(customKey == 'B'){
      lcd.clear();
  }
  else if (customKey){
    lcd.setCursor(test.length(),0);
    lcd.print("*");
    test += customKey;
  }
  if(Dem == 4){
    PrintData("Warning !!!",1);
    digitalWrite(pirPin, HIGH);
    delayMicroseconds(5000);
    digitalWrite(pirPin, LOW);
    delayMicroseconds(5000);
  }
}

//-------------------------- Move Detect --------------------
void OpenLedOutDoor(){
    pirState = digitalRead(pirPin);
    if(pirState == 1){
      digitalWrite(ledPin,HIGH);
      pirState = 0;
    }
    else{
      digitalWrite(ledPin,LOW);
    }
}

// --------------------- LCD mode ----------------------
void PrintData(String data,int row)
{
  if(row == 0){
    lcd.clear();  
  }
  else{
    clearRowX(1);
  }
  if(data.length() > 16){
    String part1 = data.substring(0,16);
    String part2 = data.substring(16,data.length());
    lcd.setCursor(0,0);
    lcd.print(part1);
    lcd.setCursor(0,1);
    lcd.print(part2);
  }
  else{
    lcd.setCursor(0,row);
    lcd.print(data);
  }
}
void clearRowX(int row){
  for(int i =0;i<16;i++){
    lcd.setCursor(i,row);
    lcd.print(" ");
  }
}


