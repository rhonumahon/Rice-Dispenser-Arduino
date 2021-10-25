#include <HX711_ADC.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

HX711_ADC LoadCell(2, 3);
LiquidCrystal_I2C lcd(0x27, 16, 2);

int upButton = 10;
int downButton = 11;
int selectButton = 12;
int sinandomengActuate = 9;
int dinuradoActuate = 8;//blue
int forward = 7;
int reverse = 6;
int limitSwitch = 5;
bool limitS = false;
bool doneSelectType = false;
bool doneSelectKilo = false;
bool repeat = false;
int menu = 1;
int kilo = 1;
int proceed = 1;
String riceType;
String unitKilo;


void setup() {
  pinMode(sinandomengActuate, OUTPUT);
  pinMode(dinuradoActuate, OUTPUT);
  pinMode(forward, OUTPUT);
  pinMode(reverse, OUTPUT);
  pinMode(limitSwitch, INPUT_PULLUP);
  LoadCell.begin();
  LoadCell.start(2000);
  LoadCell.setCalFactor(2000.0);
  lcd.begin();
  lcd.backlight();
  pinMode(upButton, INPUT_PULLUP);
  pinMode(downButton, INPUT_PULLUP);
  pinMode(selectButton, INPUT_PULLUP);
  updateMenuLCD();
}

void loop() {
  if (!digitalRead(downButton)){
    if(doneSelectType == false){
      menu++;
      updateMenuLCD();
    } else if(doneSelectType == true & doneSelectKilo == false & repeat == false){
      kilo++;
      selectKiloLCD();
    } else if(doneSelectType == true & doneSelectKilo == true & repeat == false){
      proceed++;
      confirmationLCD();
    } else if (repeat == true){
      proceed++;
      confirmationLCD();
    }
    delay(100);
    while (!digitalRead(downButton));
  }
  if (!digitalRead(upButton)){
    if(doneSelectType == false){
      menu--;
      updateMenuLCD();
    } else if(doneSelectType == true & doneSelectKilo == false & repeat == false){
      kilo--;
      selectKiloLCD();
    }else if(doneSelectType == true & doneSelectKilo == true & repeat == false){
      proceed--;
      confirmationLCD();
    } else if (repeat == true){
      proceed--;
      confirmationLCD();
    }
    delay(100);
    while(!digitalRead(upButton));
  }
  //---------------------------------------------SELECTION
  if (!digitalRead(selectButton)){
    if(doneSelectType == false){
      updateMenu();
      delay(200);
    } else if(doneSelectType == true & doneSelectKilo == false & repeat == false){
      selectKilo();
      delay(200);
    } else if(doneSelectType == true & doneSelectKilo == true & repeat == false){
      confirmation();
      delay(200);
    } else if(repeat == true){
      LoadCell.tare();
      repeat = false;
      confirmation();
    }
    while (!digitalRead(selectButton));
  }
  
  if (!digitalRead(limitSwitch) & limitS == true){
      digitalWrite(reverse, LOW);
      limitS = false;
      repeat = true;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(riceType + unitKilo);
      lcd.setCursor(0, 1);
      lcd.print("Finished");
      delay(1000);
      confirmationLCD();
    //while (!digitalRead(limitSwitch));
  }
}

void updateMenuLCD() {
  switch (menu) {
    case 0:
      menu = 1;
      break;
    case 1:
      lcd.clear();
      lcd.print(">Sinandomeng");
      lcd.setCursor(0, 1);
      lcd.print(" Dinurado");
      break;
    case 2:
      lcd.clear();
      lcd.print(" Sinandomeng");
      lcd.setCursor(0, 1);
      lcd.print(">Dinurado");
      break;
    case 3:
      menu = 2;
      break;
  }
}

void updateMenu() {
  switch (menu) {
    case 1:
      lcd.clear();
      doneSelectType = true;
      menu = 1;
      selectKiloLCD();
      break;
    case 2:
      lcd.clear();
      doneSelectType = true;
      menu = 2;
      selectKiloLCD();
      break;
}
}


void selectKiloLCD(){
  switch (kilo) {
    case 0:
      kilo = 1;
      break;
    case 1:
      lcd.clear();
      lcd.print(">3kilo");
      lcd.setCursor(0, 1);
      lcd.print(" 5kilo");
      delay(200);
      break;
    case 2:
      lcd.clear();
      lcd.print(" 3kilo");
      lcd.setCursor(0, 1);
      lcd.print(">5kilo");
      delay(200);
      break;
    case 3:
      lcd.clear();
      lcd.print(">7kilo");
      lcd.setCursor(0, 1);
      lcd.print(" back");
      delay(200);
      break;
    case 4:
      lcd.clear();
      lcd.print(" 7kilo");
      lcd.setCursor(0, 1);
      lcd.print(">back");
      delay(200);
      break;
    case 5:
      kilo = 4;
      break;
  }
  
}

void selectKilo(){
  switch (kilo) {
    case 1:
    lcd.clear();
      kilo = 1;
      doneSelectKilo = true;
      confirmationLCD();
      break;
    case 2:
    lcd.clear();
      kilo = 2;
      doneSelectKilo = true;
      confirmationLCD();
      break;
    case 3:
    lcd.clear();
      kilo = 3;
      doneSelectKilo = true;
      confirmationLCD();
      break;
    case 4:
    lcd.clear();
      clearAllData();
      updateMenuLCD();
      break;
}
}

void confirmationLCD(){
  if(menu == 1){
    riceType = "Sinandomeng,";
  }
  else if(menu == 2){
    riceType = "Dinorado,";
  }
  if(kilo == 1){
    unitKilo = " 3kl";
  }
  else if(kilo == 2){
    unitKilo = " 5kl";
  }
  else if(kilo == 3){
    unitKilo = " 7kl";
  }
  switch (proceed) {
    case 0:
      proceed = 1;
      break;
    case 1:
      lcd.clear();
      lcd.print(riceType + unitKilo);
      lcd.setCursor(0, 1);
      lcd.print(">proceed  cancel");
      break;
    case 2:
      lcd.clear();
      lcd.print(riceType + unitKilo);
      lcd.setCursor(0, 1);
      lcd.print(" proceed >cancel");
      break;
    case 3:
      proceed = 2;
      break;
  }
}

void confirmation(){
  switch (proceed) {
    case 1:
      lcd.clear();
      lcd.print("Processing...");
      delay(1000);
      processing();
      break;
    case 2:
    lcd.clear();
    lcd.print("Cancelling...");
    delay(1000);
    clearAllData();
    updateMenuLCD();
      break;
  }
}

void clearAllData(){
  doneSelectType = false;
  doneSelectKilo = false;
  repeat = false;
  menu = 1;
  kilo = 1;
  proceed = 1;
  riceType = "";
  unitKilo = "";
}

void processing(){
  lcd.clear();
    String kg = "kg";
    float i = 0;
    int weight = 0;
    if(kilo == 1){
      weight = 30;
    } else if (kilo == 2){
      weight = 50;
    } else if(kilo == 3){
      weight = 70;
    }
    
    if(menu == 1){
    digitalWrite(sinandomengActuate, HIGH);
    while(i < weight){
    LoadCell.update();
    i = LoadCell.getData();
    //float result = i/1000;
    lcd.setCursor(0, 0);
    lcd.print("Weight[kilo]:");
    lcd.setCursor(0, 1);
    lcd.print(i + kg); //replace i by result
    }
    digitalWrite(sinandomengActuate, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Weight[kilo]:");
    lcd.setCursor(0, 1);
    lcd.print(i + kg);
    motor();
    } else if (menu == 2){
    digitalWrite(dinuradoActuate, HIGH);
    while(i < weight){
    LoadCell.update();
    i = LoadCell.getData();
    //float result = i/1000;
    lcd.setCursor(0, 0);
    lcd.print("Weight[kilo]:");
    lcd.setCursor(0, 1);
    lcd.print(i + kg); //replace i by result
    }
    digitalWrite(dinuradoActuate, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Weight[kilo]:");
    lcd.setCursor(0, 1);
    lcd.print(i + kg);
    delay(1000);
    motor();
    }   
}

void motor(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(riceType + unitKilo);
  lcd.setCursor(0, 1);
  lcd.print("Processing...");
  digitalWrite(forward, HIGH);
  delay(5000);//time of tilting
  digitalWrite(forward, LOW);
  delay(5000);//time of pour
  digitalWrite(reverse, HIGH);
  limitS = true;
}