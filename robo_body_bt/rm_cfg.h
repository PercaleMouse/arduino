#ifndef rm_cfg_h
#define rm_cfg_h

namespace robot_mitya
{
  class Cfg
  {
    public:
      // Пины контроллера для управления двигателями робота (цифровые выходы).
      static const int MOTOR_LEFT_SPEED_PIN = 5;
      static const int MOTOR_LEFT_DIRECTION_PIN = 4;
      static const int MOTOR_RIGHT_SPEED_PIN = 6;
      static const int MOTOR_RIGHT_DIRECTION_PIN = 7;
      
      // Пины контроллера для управления сервоприводами робота (цифровые выходы).
      static const int SERVO_HEAD_HORIZONTAL_PIN = 9;
      static const int SERVO_HEAD_VERTICAL_PIN = 10;
      static const int SERVO_TAIL_PIN = 11;
      
      // Пин управления фарами.
      static const int LIGHT_PIN = 13;
      
      
      static const int SERVO_HEAD_HORIZONTAL_MIN_DEGREE = 0;
      static const int SERVO_HEAD_HORIZONTAL_MAX_DEGREE = 180;
      static const int SERVO_HEAD_VERTICAL_MIN_DEGREE = 0;
      static const int SERVO_HEAD_VERTICAL_MAX_DEGREE = 90;
      static const int SERVO_TAIL_MIN_DEGREE = 10;   // Not ZERO, because in the boundary position servo is vibrating a lot
      static const int SERVO_TAIL_MAX_DEGREE = 170;   // Not 180, because in the boundary position servo is vibrating a lot
      
      static const int SERVO_HEAD_HORIZONTAL_DEFAULT_STATE = 90;
      static const int SERVO_HEAD_VERTICAL_DEFAULT_STATE = 90;
      static const int SERVO_TAIL_DEFAULT_STATE = 90;
  };
}

#endif
