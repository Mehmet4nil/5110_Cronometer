// Copyright (C)2020 @Mehmet4nil

//      SCK  - Pin 8
//      MOSI - Pin 9
//      DC   - Pin 10
//      RST  - Pin 11
//      CS   - Pin 12

// LCD5110_Graph
// library web: http://www.RinkyDinkElectronics.com/

#include <LCD5110_Graph.h>

LCD5110 myGLCD(8, 9, 10, 11, 12);

extern uint8_t SmallFont[];
extern uint8_t BigNumbers[];
const int buttonPin = 2;
int buttonState = 0;
static char buttonPress = 1;

signed short minutes, min, secondes, sec;

void setup()
{
  Serial.begin(9600);
  myGLCD.InitLCD();
  myGLCD.setFont(SmallFont);
  randomSeed(analogRead(7));
  pinMode(buttonPin, INPUT);
}

void loop()
{
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



  secondes ++;

  if (secondes == 60)
  {
    secondes = 0;
    minutes ++;
  }
  myGLCD.setFont(SmallFont);
  if (buttonState == HIGH) {
    sec = secondes;
    min = minutes;

    secondes = 0;
    minutes = 0;
    //buttonPress = 0;
  }
  myGLCD.print(":", 37, 26);
  myGLCD.print(String(min), 22, 26);
  myGLCD.print(String(sec), 47, 26);
  myGLCD.update();
  delay(1000);
}
