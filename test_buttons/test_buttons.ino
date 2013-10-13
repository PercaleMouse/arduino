#include "rm_btn.h"

using namespace robot_mitya;

void setup()
{
  RomeoButtons::initialize();
}

void loop()
{
  RomeoButtons::refresh();
}

