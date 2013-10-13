#include "rm_btn.h"
#include "rm_cfg.h"

#include <Arduino.h>

using namespace robot_mitya;

static const int NUMBER_OF_BUTTONS = 5;
static const int buttonValues[NUMBER_OF_BUTTONS] = { 30, 150, 360, 535, 760 };
static int previousButton = -1;
static int pressedButton = -1;

//static boolean waiting = false;
//static int pressedMillis = 0;

void RomeoButtons::initialize()
{
  pinMode(Cfg::BUTTONS_PIN, INPUT);
}

void RomeoButtons::refresh()
{
  int analogValue = analogRead(Cfg::BUTTONS_PIN);
  int button = getButton(analogValue);
  if (button != previousButton)
  {
    delay(50);      // wait for debounce time
    analogValue = analogRead(Cfg::BUTTONS_PIN);
    button = getButton(analogValue);
    if (button != previousButton)
    {
      previousButton = button;
      if (button != pressedButton)
      {
        if (pressedButton >= 0)
        {
          Serial.println("released: s" + String(pressedButton + 1));
        }
      }
      pressedButton = button;
      if (button >= 0)
      {
        Serial.println("pressed: s" + String(button + 1));
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

