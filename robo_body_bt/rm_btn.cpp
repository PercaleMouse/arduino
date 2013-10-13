#include "rm_btn.h"
#include "rm_cfg.h"
#include "rm_msg.h"

#include <Arduino.h>

using namespace robot_mitya;

static const int NUMBER_OF_BUTTONS = 5;
static const int buttonValues[NUMBER_OF_BUTTONS] = { 30, 150, 360, 535, 760 };
static int previousButton = -1;

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
    delay(1000);      // wait for debounce time
    analogValue = analogRead(Cfg::BUTTONS_PIN);
    button = getButton(analogValue);
    if (button != previousButton)
    {
      previousButton = button;
      if (button >= 0)
      {
        Message::debugOutput("pressed: s" + String(button + 1));
      }
    }
  }
}

int RomeoButtons::getButton(int analogValue)
{
  for (int i = 0; i < NUMBER_OF_BUTTONS; i++)
  {
//    delay(100);
    if (analogValue < buttonValues[i])
    {
//      Message::debugOutput(String(analogValue));
      return i;  
    }
  }
  return -1;
}

