#include <Arduino.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>

//#include "BluetoothSerial.h"

#include "boards.h"
#include "Config.h"
#include "FanControl.h"


/*#define BLACK 0x0000
#define RED   0xA150
#define WHITE 0xFFFF
#define CYAN  0x07FF
#define GRAY1 040404
#define GRAY2 505050*/ 

#ifdef Bluetooth
  #ifdef ESP32
    #include "BluetoothSerial.h"
    #endif
  BluetoothSerial SerialBT;
#endif

#ifdef TFT_1.8
  Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
#endif

int targetTemp = 26;
int fanSpeed = 100;
int minFanSpeed = 50;
int maxFanSpeed = 200;

//Stand vorher auf int. Wenn es Probleme gibt ändern!
int sensorwert;
int temperatur = 0;

bool y = 0;
bool x = 0;
int menu = 1;
int rotate = 1;

int redPrint;
int diaPrint1;
int diaPrint2;
int diaPrint3;

unsigned long lastMillis1;
unsigned long lastMillis2;
unsigned long lastMillis3;
unsigned long lastMillis4;
unsigned long lastMillis5;
unsigned long lastMillis6;

//0 Pin Nummer, 1 Aktuelle Temperatur, 2 Traget Temperatur Lüfter, 3 minimal Geschwindigkeit Lüfter, 4 maximaler Speed Lüfter
FanControl Fan0(FAN_PIN, targetTemp, minFanSpeed, maxFanSpeed);

void setup(){
  
  #ifdef DEBUG
    Serial.begin(DEBUG);
  #endif

  #ifdef Bluetooth
    SerialBT.begin("FanCon");
  #endif
  
  #ifdef TFT_1.8
    tft.initR(INITR_BLACKTAB);
  
    pinMode(UP_BUTTON, INPUT);
    pinMode(DOWN_BUTTON,INPUT);
    pinMode(SELECT_BUTTON,INPUT);
  #endif
  
  lastMillis1 = millis();
  lastMillis2 = millis();
  lastMillis3 = millis();
  lastMillis4 = millis();
  lastMillis5 = millis();
  lastMillis6 = millis();
}

void loop(){
  
  #ifdef TFT_1.8
    if(x == 0){
      tft.fillScreen(ST77XX_BLACK);
      updateMenu();
      tft.drawLine(0,25,160,25,GRAY2);
      x = 1;
    }
    else{
      if(digitalRead(DOWN_BUTTON) && (millis() - lastMillis5) >= 200){
        menu++;
        updateMenu();
        lastMillis5 = millis();
        while(digitalRead(DOWN_BUTTON)){
          #ifdef TMP36
          temperatur = tempReadTMP36(TEMP_SENSOR_PIN);
          #endif
        }
      }
      if(digitalRead(UP_BUTTON) && (millis() - lastMillis5) >= 200){
        menu--;
        updateMenu();
        lastMillis5 = millis();
        while(digitalRead(UP_BUTTON)){
          #ifdef TMP36
          temperatur = tempReadTMP36(TEMP_SENSOR_PIN);
          #endif
        }
      }
      if(digitalRead(SELECT_BUTTON) && (millis() - lastMillis5) >= 400){
        executeAction();
        updateMenu();
        lastMillis5 = millis();
        while(digitalRead(SELECT_BUTTON)){
          #ifdef TMP36
          temperatur = tempReadTMP36(TEMP_SENSOR_PIN);
          #endif
        }
      }
   }
    //Temperature reading
    if( ((millis()- lastMillis1) >= 1000) && (menu < 4)){
      tft.setRotation(rotate);
      tft.setTextSize(1);
      tft.setCursor(130, 10);
      tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
      tft.print(temperatur);
      tft.setCursor(145, 10);
      tft.println("C");
      tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
      tft.setCursor(130, 1);
      tft.setTextSize(1);
      tft.print(fanSpeed);

      // Letztes Zeichen löschen im Display wenn von drei auf zweistellig um springt.
      if((fanSpeed < 100) && (y == 0)){
        tft.setTextSize(1);
        tft.setCursor(142, 1);
        tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
        tft.println(" ");
        y = 1;
      }
      else{
        y = 0;
      }
    
    lastMillis1 = millis();
  }
  #endif

if((millis()- lastMillis6) >= 1000){
  #ifdef TMP36
    temperatur = tempReadTMP36(TEMP_SENSOR_PIN);
  #endif
    lastMillis6 = millis();
  }

  #ifdef DEBUG
    Serial.println(temperatur);
  #endif

  //Neue FanConrol Funktion* 
  if((millis()- lastMillis3) >= 100){
   fanSpeed = Fan0.fSpeed(temperatur);
   lastMillis3 = millis();
  }
  // Erster wert y zweiter x. +X >, +Y |
    pMeter(fanSpeed, 10, 35, "Speed = PWM");
    pMeter(temperatur, 10, 70, "Temp C");
}

// TFT Funktions
#ifdef TFT_1.8

void fillScreen(uint16_t color);

void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);

void setRotation(uint8_t rotation);

void drawRect(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t color);

void fillRect(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t color);

void setTextColor(uint16_t color, uint16_t backgroundcolor);

void setTextSize(uint8_t size);

void drawPixel(uint16_t x, uint16_t y, uint16_t color);

void setTextWrap(boolean w);

void drawChar(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg, uint8_t size);

//Zweiter Eintrag Speed% Für das zeichen vom Diagramm und dem Fan Speed. Noch im Test!
void printDia(){
  tft.drawPixel(150,110,RED);
}


//Menue Funktions
void updateMenu (){
  
  switch (menu){
    case 0:
      menu = 1;
      break;
      
    case 1:
      tft.setRotation(rotate);
      tft.setTextSize(2,3); // Hinweis: Adafruit_GFX nutzt normalerweise 1 Parameter
      tft.setCursor(0, 0);
      tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
      tft.println("Fan 1");
      tft.setTextSize(1);
      tft.setCursor(90, 0);
      tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
      tft.println("Speed:");
      tft.setCursor(95, 10);
      tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
      tft.println("Temp:");
      break;
      
    case 2:
      tft.setRotation(rotate);
      tft.setCursor(0, 0);
      tft.setTextSize(2,3);
      tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
      tft.println("Fan 2");
      tft.setTextSize(1);
      tft.setCursor(90, 0);
      tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
      tft.println("Speed:");
      tft.setCursor(95, 10);
      tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
      tft.println("Temp:");
      break;
      
    case 3:
      tft.setRotation(rotate);
      tft.setCursor(0, 0);
      tft.setTextSize(2,3);
      tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
      tft.println("Fan 3");
      tft.setTextSize(1);
      tft.setCursor(90, 0);
      tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
      tft.println("Speed:");
      tft.setCursor(95, 10);
      tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
      tft.println("Temp:");
      break;
      
    case 4:
      tft.setRotation(rotate);
      tft.setCursor(0, 0);
      tft.setTextSize(2,3);
      tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
      tft.println("Setup        ");
      break;
      
    case 5:
      menu = 4;
      break;
  }
}

// Switch für die Objekte im Hauptmenu und deren Untermenüs
void executeAction(){
  switch (menu){
    case 1:
      action1();
      break;
    case 2:
      action2();
      break;
    case 3:
      action3();
      break;
    case 4:
      action4();
      break;
  }
}

void action1(){
  tft.fillScreen(BLACK);
  tft.drawLine(0,25,160,25,GRAY2);
  tft.setRotation(rotate);
  tft.setTextSize(2,3);
  tft.setCursor(0, 0);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.println("Setup F1");
  do{
      
  }
  while (!digitalRead(SELECT_BUTTON) || (millis() - lastMillis5) <= 1500);
  lastMillis5 = millis();
  x = 0;
  }

void action2(){
  tft.fillScreen(BLACK);
  tft.drawLine(0,25,160,25,GRAY2);
  tft.setRotation(rotate);
  tft.setTextSize(2,3);
  tft.setCursor(0, 0);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.println("Setup F2");
  do{
    
  }
  while (!digitalRead(SELECT_BUTTON) || (millis() - lastMillis5) <= 1500);
    lastMillis5 = millis();
    x = 0;
  }

void action3(){
  tft.fillScreen(BLACK);
  tft.drawLine(0,25,160,25,GRAY2);
  tft.setRotation(rotate);
  tft.setTextSize(2,3);
  tft.setCursor(0, 0);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.println("Setup F3");
  do{
    
  }
  while (!digitalRead(SELECT_BUTTON) || (millis() - lastMillis5) <= 1500);
    lastMillis5 = millis();
    x = 0;
  }

void action4(){
  tft.fillScreen(BLACK);
  tft.drawLine(0,25,160,25,GRAY2);
  tft.setRotation(rotate);
  tft.setTextSize(2,3);
  tft.setCursor(0, 0);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.println("Setup");
  do{

    
    
    if(digitalRead(DOWN_BUTTON) && (millis() - lastMillis5) >= 200){
      menu++;
      
      lastMillis5 = millis();
      while(digitalRead(DOWN_BUTTON)){
        #ifdef TMP36
        temperatur = tempReadTMP36(TEMP_SENSOR_PIN);
        #endif       
      }
    }

     if(digitalRead(UP_BUTTON) && (millis() - lastMillis5) >= 200){
      menu--;
      
      lastMillis5 = millis();
      while(digitalRead(UP_BUTTON)){
        #ifdef TMP36
        temperatur = tempReadTMP36(TEMP_SENSOR_PIN);
        #endif
      }
    }
    
    //Neue FanFunktion* 
    if((millis()- lastMillis3) >= 100){
      fanSpeed = Fan0.fSpeed(temperatur);
      lastMillis3 = millis();
    }
  }
  
  while (!digitalRead(SELECT_BUTTON) || (millis() - lastMillis5) <= 1500);
    lastMillis5 = millis();
    x = 0;
    menu = 4;
}
#endif

#ifdef TMP36
int tempReadTMP36(int pin){
  sensorwert = analogRead(pin);
  //620
  int val = map(sensorwert, 0, 2300, -50, 150);
  return val;
}
#endif

//Neue Anzeige Funktion
//tft.drawLine(0,50,0,40,RED);
//Input für eingehendes Val. +X >, +Y |
void pMeter(int input, int x, int y, String text){
  tft.setRotation(rotate);

  int xMin = 1 + x;

  int xMax = 120 + xMin;

  //Variablen
  int x0p = map(input,0,255,xMin,xMax);

  //Roter Printer
  //tft.fillRect(x,y,w,h,color);
  tft.fillRect(x, y, x0p, 8, RED);

  int xPointer = x0p + 10;

  int breite = y + 12;

  // Zeigerlinie
  tft.drawLine(xPointer, y, xPointer, breite, WHITE);

  int xClearL = xPointer +1;

  // Löscher für Zeiger
  tft.drawLine(xClearL, y, xClearL, BLACK);

  //---------------------------
  //Ausgabe der Geschwindgkeit oder Temp usw
  int yPointerText = y + 15;

  int xPointerText;

  int clearText0;

  int clearText1;

  // Damit der Text der Zahl in der Mitte des Zeigers bleibt.
  //Bei 100 springt es eines nach Lins.
  if(input >= 100){
    xPointerText = xPointer - 9;
    clearText0 = xPointerText - 4;
  }
  else{
    xPointerText = xPointer - 6;
    clearText0 = xPointerText - 5;
    clearText1 = xPointerText + 12;

    // Zum Loschen von drei auf zwei Stellen
    tft.setTextSize(1);
    tft.setCursor(clearText1, yPointerText);
    tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    tft.print(" ");
  }

  tft.setTextSize(1);
  tft.setCursor(clearText0, yPointerText);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.print(" ");
  
  tft.setCursor(xPointerText, yPointerText);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.println(input);

  //----------------------------
  //Indifidueller Text
  int xText = x;

  int yText = y - 8;

  tft.setCursor(xText, yText);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.println(text);

  //----------------------------
  //Kleiner Streifen löschen mit mit schwarz.
  int xClear = x;

  int yClear = y + 8;

  // brc = Breite Rect clear
  int brc = 5;

  int xMinS = x + 1;

  int xMaxS = x + 121;

  int valRectS = map(input, 0, 255, xMinS, xMaxS);
  
  // Print des Balken
  tft.drawRect(xClear, yClear, valRectS, brc, BLACK);
}
