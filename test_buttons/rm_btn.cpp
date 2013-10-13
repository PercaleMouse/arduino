#include "rm_btn.h"
#include "rm_cfg.h"

#include <Arduino.h>

using namespace robot_mitya;

static int buttonsPin;
static const int NUMBER_OF_BUTTONS = 5;
static const int buttonValues[NUMBER_OF_BUTTONS] = { 30, 150, 360, 535, 760 };
static Button currentButton = NONE;
static Button previousButton = NONE;
static Button pressedButton = NONE;

static boolean waiting = false;
static unsigned long nextTimeMillis = 0;
static const unsigned long DELAY_MILLIS = 50;

static RomeoButtonsHandler buttonsHandler;

void RomeoButtons::initialize(int pin)
{
  buttonsPin = pin;
  pinMode(buttonsPin, INPUT);
  buttonsHandler = NULL;
}

void RomeoButtons::setHandler(RomeoButtonsHandler handler)
{
  buttonsHandler = handler;
}

void RomeoButtons::refresh()
{
  if (!waiting)
  {
    currentButton = getButton(analogRead(buttonsPin));
  }
  if (currentButton != previousButton)
  {
    unsigned long currentTimeMillis = millis();
    if (!waiting)
    {
      waiting = true;
      nextTimeMillis = currentTimeMillis + DELAY_MILLIS;
    }    
    if (currentTimeMillis >= nextTimeMillis) waiting = false;
    else return;
      
    currentButton = getButton(analogRead(buttonsPin));
    if (currentButton != previousButton)
    {
      previousButton = currentButton;
      if (currentButton != pressedButton)
      {
        if (pressedButton != NONE)
        {
          if (buttonsHandler != NULL)
          {
            buttonsHandler(RELEASED, pressedButton);
          }
        }
      }
      pressedButton = currentButton;
      if (currentButton != NONE)
      {
        if (buttonsHandler != NULL)
        {
          buttonsHandler(PRESSED, currentButton);
        }
      }
    }
  }
}

Button RomeoButtons::getButton(int analogValue)
{
  for (int i = 0; i < NUMBER_OF_BUTTONS; i++)
  {
    if (analogValue < buttonValues[i])
    {
      return (Button)i;  
    }
  }
  return (Button)-1;
}

