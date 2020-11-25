// Copyright (C)2020 @Mehmet4nil

//      SCK  - Pin 8
//      MOSI - Pin 9
//      DC   - Pin 10
//      RST  - Pin 11
//      CS   - Pin 12

// LCD5110_Graph
// library web: http://www.RinkyDinkElectronics.com/

#include <LCD5110_Graph.h>
#include <Wire.h>
#include "RTClib.h"

LCD5110 myGLCD(8, 9, 10, 11, 12);

RTC_DS3231 rtc;

extern uint8_t SmallFont[];
extern uint8_t BigNumbers[];
const int buttonPin = 2;
int buttonState = 0;
bool buttonPress = true;

int realSec, secondes = -1, minutes, min, sec;

void setup()
{
  Serial.begin(9600);
  myGLCD.InitLCD();
  myGLCD.setFont(SmallFont);
  randomSeed(analogRead(7));
  pinMode(buttonPin, INPUT);

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  delay(2000);
}

void loop()
{
  DateTime now = rtc.now();
  buttonState = digitalRead(buttonPin);

  myGLCD.clrScr();

  //-------------------------------------------------
  myGLCD.setFont(SmallFont);
  myGLCD.print(":", 37, 10);

  myGLCD.setFont(BigNumbers);
  //-------------------------------------------------
  if (minutes < 10) {
    myGLCD.print("0", 9, 0);
    myGLCD.print(String(minutes), 22, 0);
  }
  else {
    myGLCD.print(String(minutes), 8, 0);
  }
  //-------------------------------------------------
  if (secondes < 10) {
    myGLCD.print("0", 47, 0);
    myGLCD.print(String(secondes), 60, 0);
  }
  else {
    myGLCD.print(String(secondes), 46, 0);
  }
  //-------------------------------------------------

  if (realSec == (now.second() - 1) || realSec - 60 == (now.second() - 1)) {
    secondes ++;
  }
  realSec = now.second();

  if (secondes == 60)
  {
    secondes = 0;
    minutes ++;
  }

  myGLCD.setFont(SmallFont);

  if (buttonState == HIGH) {
    if (buttonPress) {
      sec = secondes;
      min = minutes;
      buttonPress = false;
    }
    secondes = 0;
    minutes = 0;
    //buttonPress = 0;
  } else {
    buttonPress = true;
    myGLCD.print(":", 37, 26);
    //-------------------------------------------------
    if (min < 10) {
      myGLCD.print("0", 22, 26);
      myGLCD.print(String(min), 28, 26);
    }
    else {
      myGLCD.print(String(min), 22, 26);
    }
    //-------------------------------------------------
    if (sec < 10) {
      myGLCD.print("0", 47, 26);
      myGLCD.print(String(sec), 53, 26);
    }
    else {
      myGLCD.print(String(sec), 47, 26);
    }
    //-------------------------------------------------
    myGLCD.update();
  }
}
