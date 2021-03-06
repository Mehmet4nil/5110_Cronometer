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

extern uint8_t TinyFont[];
extern uint8_t SmallFont[];
extern uint8_t BigNumbers[];
const int buttonPin = 2, buttonPinP = 3;
int buttonState = 0, buttonStateP = 0;
bool buttonPress = true, buttonPressP = true, pause = true;

int realSec, secondes = 0, minutes, min, sec = 0;

volatile unsigned long start_sec, mili_sec;

void setup()
{
  Serial.begin(9600);
  myGLCD.InitLCD();
  myGLCD.setFont(SmallFont);
  randomSeed(analogRead(7));
  pinMode(buttonPin, INPUT);
  pinMode(buttonPinP, INPUT);

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
  buttonStateP = digitalRead(buttonPinP);

  myGLCD.clrScr();

  //-------------------------------------------------
  myGLCD.setFont(TinyFont);
  myGLCD.print(String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second()), 0, 0);
  myGLCD.print("- " + String(now.day()) + "/" + String(now.month()) + "- " + String(rtc.getTemperature()).substring(0, 2) + "'C", 32, 0);

  //-------------------------------------------------
  myGLCD.setFont(SmallFont);
  myGLCD.print(":", 37, 18);

  myGLCD.setFont(BigNumbers);
  //-------------------------------------------------
  if (minutes < 10) {
    myGLCD.print("0", 9, 8);
    myGLCD.print(String(minutes), 22, 8);
  }
  else {
    myGLCD.print(String(minutes), 8, 8);
  }
  //-------------------------------------------------
  if (secondes < 10) {
    myGLCD.print("0", 47, 8);
    myGLCD.print(String(secondes), 60, 8);
  }
  else {
    myGLCD.print(String(secondes), 46, 8);
  }
  //-------------------------------------------------


  if (pause == false) { //_Crono second play, pause
    mili_sec = millis();
    if (realSec == (now.second() - 1) || realSec - 60 == (now.second() - 1)) {
      secondes ++;
      start_sec = millis();
    }
    realSec = now.second();

    if (secondes == 60)
    {
      secondes = 0;
      minutes ++;
    }
  }

  myGLCD.setFont(SmallFont);

  if (buttonState == HIGH) { //_Crono reset button
    if (buttonPress) {
      sec = secondes;
      min = minutes;
      buttonPress = false;
    }
    secondes = 0;
    minutes = 0;
    //buttonPress = 0;
  } else { //_Crono write the last counted time on the screen
    buttonPress = true;
    myGLCD.print(":", 37, 34);
    //-------------------------------------------------
    if (min < 10) {
      myGLCD.print("0", 22, 34);
      myGLCD.print(String(min), 28, 34);
    }
    else {
      myGLCD.print(String(min), 22, 34);
    }
    //-------------------------------------------------
    if (sec < 10) {
      myGLCD.print("0", 47, 34);
      myGLCD.print(String(sec), 53, 34);
    }
    else {
      myGLCD.print(String(sec), 47, 34);
    }
    //-------------------------------------------------
    myGLCD.print(String(mili_sec - start_sec), 47, 41);
    myGLCD.update();
  }

  if (buttonStateP == HIGH) { //_Crono second play, pause button
    if (buttonPressP) {
      if (pause == true) {
        pause = false;
      } else {
        pause = true;
      }
      buttonPressP = false;
    }
  } else {
    buttonPressP = true;
  }
}
