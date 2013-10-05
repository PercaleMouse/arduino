#ifndef rm_act_h
#define rm_act_h

#include <Arduino.h>

namespace robot_mitya {
  class Action {
    public:
      static void excute(String command, int value);
  };
}

#endif
