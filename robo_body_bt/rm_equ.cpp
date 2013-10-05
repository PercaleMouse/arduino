#include "rm_equ.h"
#include "rm_cfg.h"

using namespace robot_mitya;

Servo* servoHeadHorizontal = new Servo();
Servo* servoHeadVertical = new Servo();
Servo* servoTail = new Servo();

void Equipment::initialize()
{
  // Initializing headlights:
  pinMode(Cfg::LIGHT_PIN, OUTPUT);
  digitalWrite(Cfg::LIGHT_PIN, LOW);

  // Initializng and setting horizontal servo into install-phone position:
  pinMode(Cfg::SERVO_HEAD_HORIZONTAL_PIN, OUTPUT);
  servoHeadHorizontal->attach(
    Cfg::SERVO_HEAD_HORIZONTAL_PIN,
    Cfg::SERVO_HEAD_HORIZONTAL_MIN_DEGREE,
    Cfg::SERVO_HEAD_HORIZONTAL_MAX_DEGREE);
  moveHead("H", Cfg::SERVO_HEAD_HORIZONTAL_DEFAULT_STATE);

  // Initializng and setting vertical servo into install-phone position:
  pinMode(Cfg::SERVO_HEAD_VERTICAL_PIN, OUTPUT);
  servoHeadVertical->attach(
    Cfg::SERVO_HEAD_VERTICAL_PIN,
    Cfg::SERVO_HEAD_VERTICAL_MIN_DEGREE,
    Cfg::SERVO_HEAD_VERTICAL_MAX_DEGREE);
  moveHead("V", Cfg::SERVO_HEAD_VERTICAL_DEFAULT_STATE);

  // Setting up the tail:
  pinMode(Cfg::SERVO_TAIL_PIN, OUTPUT);
  servoTail->attach(
    Cfg::SERVO_TAIL_PIN,
    Cfg::SERVO_TAIL_MIN_DEGREE,
    Cfg::SERVO_TAIL_MAX_DEGREE);
  moveTail(Cfg::SERVO_TAIL_DEFAULT_STATE);
}

void Equipment::setHeadlightState(int value)
{
  if (value == 0)
  {
    digitalWrite(Cfg::LIGHT_PIN, LOW);
  }
  else if (value == 1)
  {
    digitalWrite(Cfg::LIGHT_PIN, HIGH);
  }
}

void Equipment::moveHead(String plane, int degree)
{
  if (plane == "H") // (horizontal plane)
  {
    servoHeadHorizontal->write(degree);
  }
  else if (plane == "V") // (vertical plane)
  {
    servoHeadVertical->write(degree);
  }
}

void Equipment::moveTail(int degree)
{
  servoTail->write(degree);
}

void Equipment::moveMotor(String side, int speed)
{
  bool directionPinValue = speed > 0 ? LOW : HIGH;
  
  if (speed < 0) {
    speed = - speed;
  }
 
  if (speed > 255) {
    speed = 255;
  }
  
  if (speed == 0) {
    directionPinValue = LOW; // turns motor off
  }
  
  if ((side == "L") || (side == "G")) {
    digitalWrite(Cfg::MOTOR_LEFT_DIRECTION_PIN, directionPinValue);
    analogWrite(Cfg::MOTOR_LEFT_SPEED_PIN, speed);
  }
  
  if ((side == "R") || (side == "G")) {
    digitalWrite(Cfg::MOTOR_RIGHT_DIRECTION_PIN, directionPinValue);
    analogWrite(Cfg::MOTOR_RIGHT_SPEED_PIN, speed);
  }
}

