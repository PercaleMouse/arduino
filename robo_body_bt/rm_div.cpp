#include "rm_div.h"
#include "rm_equ.h"

#include <Arduino.h>

using namespace robot_mitya;

VoltageDivider::VoltageDivider(int dividerIndex, int dividerPin, float arefVoltage, float r1, float r2)
{
  this->dividerIndex = dividerIndex;
  
  this->dividerPin = dividerPin;
  
  // How to get voltPerUnit value:
  // Take voltmeter and meassure voltage between AREF and GND pins
  // on Arduino controller and divide it by 1024.
  float voltPerUnit = arefVoltage / 1024;
  
  float dividerRatio = (r1 + r2) / r2; // (R1+R2)/R2
  
  this->voltRatio = voltPerUnit * dividerRatio * 100;
}

unsigned int VoltageDivider::getVoltage()
{
  return voltRatio * Equipment::getVoltageDividerPinValue(this->dividerPin);
}

void VoltageDivider::setTimer(unsigned int timerDelay, void (*handler)(int, unsigned int))
{
  this->timerDelay = timerDelay;
  this->timerEnabled = timerDelay != 0;
  this->timerHandler = handler;
  this->timerLastTime = millis();
}

void VoltageDivider::refresh()
{
  if (!this->timerEnabled) return;
  
  unsigned int currentTime = millis();
  if (currentTime > (this->timerLastTime + this->timerDelay))
  {
    this->timerLastTime = currentTime;
    if (this->timerHandler != NULL)
    {
      this->timerHandler(this->dividerIndex, this->getVoltage());
    }
  }
}

