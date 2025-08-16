#pragma once
#include "Config.h"

#ifdef UNO
  //ST7735 Pins
  #define TFT_CS            10  //CS/SS
  #define TFT_RST           8 //RESET
  #define TFT_DC            9   //A0
#endif

//SDA Pin D51, A0 Pin D18
#ifdef MEGA
  #define TFT_CS            53  //CS/SS
  #define TFT_RST           -1  //RESET
  #define TFT_DC            18  //A0
//Buttons
  #define UP_BUTTON         3
  #define DOWN_BUTTON       5
  #define SELECT_BUTTON     6
//Temp Sensor Pin
  #ifdef TMP36
    #define TEMP_SENSOR_PIN   A0
  #endif
  #define FAN_PIN           13
#endif

//Nano
#ifdef NANO
  #define TFT_CS            53  //CS/SS
  #define TFT_RST           -1  //RESET
  #define TFT_DC            18  //A0
//Buttons
  #define UP_BUTTON         3
  #define DOWN_BUTTON       5
  #define SELECT_BUTTON     6
//Temp Sensor Pin
  #ifdef TMP36
    #define TEMP_SENSOR_PIN   A0
  #endif
  #define FAN_PIN           13
#endif

//ESP32
#ifdef ESP32
  #define TFT_CS            5
  #define TFT_RST           4
  #define TFT_DC            2
//Buttons
  #define UP_BUTTON         33
  #define DOWN_BUTTON       32
  #define SELECT_BUTTON     35
//Temp Sensor Pin
  #ifdef TMP36  // No testet!
    #define TEMP_SENSOR_PIN   26
  #endif
  #define FAN_PIN           13
#endif
