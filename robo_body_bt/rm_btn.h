// -------------------------------------------------------------------------------------
// file = "rm_btn.h" (Robot Mitya's Romeo buttons header file)
// company = "Robot Mitya community"
// Copyright © Dmitry Dzakhov 2013
//   This static class handle buttons on Robot Mitya's Romeo controller.
//   Tested on Romeo All-in-one Controller v1.1 and v2.
// -------------------------------------------------------------------------------------

#ifndef rm_btn_h
#define rm_btn_h

//#include <Arduino.h>

namespace robot_mitya
{
  class RomeoButtons
  {
    public:
      static void initialize();
      static void refresh();
    private:
      static int getButton(int analogValue);
  };
}

#endif
