#include "rm_btn.h"
#include "rm_cfg.h"

#include <Arduino.h>

using namespace robot_mitya;

static const int NUMBER_OF_BUTTONS = 5;
static const int buttonValues[NUMBER_OF_BUTTONS] = { 30, 150, 360, 535, 760 };
static int currentButton = -1;
static int previousButton = -1;
static int pressedButton = -1;

static boolean waiting = false;
static int pressedMillis = 0;

void RomeoButtons::initialize()
{
  pinMode(Cfg::BUTTONS_PIN, INPUT);
}

void RomeoButtons::refresh()
{
  if (!waiting)
  {
    currentButton = getButton(analogRead(Cfg::BUTTONS_PIN));
    waiting = true;
    pressedMillis = millis();
  }
  if (currentButton != previousButton)
  {
    if (millis() - pressedMillis >= 50) waiting = false;
    else return;
      
    currentButton = getButton(analogRead(Cfg::BUTTONS_PIN));
    if (currentButton != previousButton)
    {
      previousButton = currentButton;
      if (currentButton != pressedButton)
      {
        if (pressedButton >= 0)
        {
          Serial.println("released: s" + String(pressedButton + 1));
        }
      }
      pressedButton = currentButton;
      if (currentButton >= 0)
      {
        Serial.println("pressed: s" + String(currentButton + 1));
      }
    }
  }
}

int RomeoButtons::getButton(int analogValue)
{
  for (int i = 0; i < NUMBER_OF_BUTTONS; i++)
  {
    if (analogValue < buttonValues[i])
    {
      return i;  
    }
  }
  return -1;
}

