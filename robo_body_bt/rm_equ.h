// -------------------------------------------------------------------------------------
// file = "rm_equ.h" (Robot Mitya equipment header file)
// company = "Robot Mitya community"
// Copyright © Dmitry Dzakhov 2013
//   This static class initialize and control Robot Mitya's equipment:
//   DC motors, head and tail servos, headlights etc.
// -------------------------------------------------------------------------------------

#ifndef rm_equ_h
#define rm_equ_h

// For Leonardo (Romeo V2) board we use SoftwareServo library, because of Timers lack.
#ifdef USBCON
  #include <SoftwareServo.h>
#else
  #include <Servo.h>
#endif
#include <Arduino.h>

namespace robot_mitya
{
  class Equipment
  {
    public:
      static void initialize();
      static void setHeadlightState(int value);
      static void moveHead(String plane, int degree);
      static void moveTail(int degree);
      static void moveMotor(String side, int speed);
  };
}

#endif
