#include "rm_btn.h"
#include "rm_cfg.h"

using namespace robot_mitya;

void buttonsHandler(robot_mitya::ButtonState buttonState, robot_mitya::Button button)
{
  String s = "";
  
  if (buttonState == PRESSED)
    s += "pressed ";
  else if (buttonState == RELEASED)
    s += "released ";
  else
    s += "bad state ";
    
  switch (button)
  {
    case S1: s += "S1"; break;
    case S2: s += "S2"; break;
    case S3: s += "S3"; break;
    case S4: s += "S4"; break;
    case S5: s += "S5"; break;
    default: s += "bad button";
  }
  
  Serial.println(s);
}

void setup()
{
  RomeoButtons::initialize(Cfg::BUTTONS_PIN);
  RomeoButtons::setHandler(buttonsHandler);
}

void loop()
{
  RomeoButtons::refresh(analogRead(Cfg::BUTTONS_PIN));
}
