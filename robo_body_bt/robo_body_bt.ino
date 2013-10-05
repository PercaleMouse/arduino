// -------------------------------------------------------------------------------------
// file = "robot_body_bt.ino"
// company = "Robot Mitya community"
// Copyright © Dmitry Dzakhov 2013
//   Simple sketch that demonstrates how to control
//   Robot through serial connection. It could be
//   USB wire or Bluetooth connection.
// -------------------------------------------------------------------------------------

// For Leonardo (Romeo V2) board we use SoftwareServo library, because of Timers lack.
#ifdef USBCON
  #include <SoftwareServo.h>
#else
  #include <Servo.h>
#endif

#include "rm_equ.h"
#include "rm_cfg.h"


using namespace robot_mitya;

Equipment *equ = new Equipment();

// Эхо-режим. Возврат всех полученных сообщений.
const boolean ECHO_MODE = false;

// Длина сообщения:
const int MESSAGELENGTH = 5;
// Текущий необработанный текст из буфера, прочитанного на предыдущей итерации чтения:
String MessageBuffer = "";

// Sketch initialization
void setup()
{
  // Set speed for serial port on pins 0,1 :
  #ifdef USBCON   // For Leonardo (Romeo V2) board support
      Serial1.begin(9600);
  #else
      Serial.begin(9600);
  #endif

  // Инициализация двигателей (установка нулевой скорости):
  moveMotor("G", 0);

  Equipment::initialize();
}

// Функция главного цикла:
void loop()
{
  processMessageBuffer(); // Receive all messages and process them

  // For Leonardo (Romeo V2) board we use SoftwareServo library, because of Timers lack.
  #ifdef USBCON
    SoftwareServo::refresh();
  #endif
}

// Извлечение и обработка команд из входного буфера:
void processMessageBuffer()
{
    
// Если что пришло из буфера, добавляем.
  #ifdef USBCON   // For Leonardo (Romeo V2) board support
    while (Serial1.available() > 0)
    {
      MessageBuffer += (char)Serial1.read();
    }
  #else
    while (Serial.available() > 0)
    {
      MessageBuffer += (char)Serial.read();
    }
  #endif
// Размер буфера
  int bufferLength = MessageBuffer.length();

  if (bufferLength < MESSAGELENGTH)
  {
    return;
  }
//  Serial1.println("MessageBuffer: " + MessageBuffer);

  // Последовательно извлекаю из полученной строки команды длиной MESSAGELENGTH символов.
  // Что не уместилось пойдёт снова в MessageBuffer.
  int i = 0;
  String message;

  while (i < bufferLength-MESSAGELENGTH+1)
  { //Ищем первый символ не-цифру в шестнадцатиричом исчислении. Это будет комманда значит.
    if(((MessageBuffer[i]>='0')&&(MessageBuffer[i]<='9'))||((MessageBuffer[i]>='A')&&(MessageBuffer[i]<='F'))||((MessageBuffer[i]>='a')&&(MessageBuffer[i]<='f')))
    {  //Оказалась цифра
          #ifdef USBCON   // For Leonardo (Romeo V2) board support
             Serial1.print("#0008"); // цифра в шестнадцатеричном исчислении, вместо команды. Если несколько цифр подряд, то придет несколько сообщений.    
          #else
             Serial.print("#0008"); // цифра в шестнадцатеричном исчислении, вместо команды. Если несколько цифр подряд, то придет несколько сообщений.    
          #endif

          i++;
    } else
    {  //Попалась не цифра
        message = MessageBuffer.substring(i, i+MESSAGELENGTH);
      
        processMessage( message );
        i += MESSAGELENGTH;        
    }
  }
  MessageBuffer = MessageBuffer.substring(i, bufferLength); 
}

boolean parseMessage(String message, String &command, int &value)
{
  if (message.length() != 5)
  {
    command = "";
    value = 0;
    return false;
  }
    
  command = (String)message[0];
  int digit1, digit2, digit3, digit4;
  if (hexCharToInt(message[1], digit1) && hexCharToInt(message[2], digit2) &&
    hexCharToInt(message[3], digit3) && hexCharToInt(message[4], digit4))
  {
    value = 4096 * digit1 + 256 * digit2 + 16 * digit3 + digit4;
    return true;
  }
  else
  {
    command = "";
    value = 0;
    return false;
  }
}

boolean hexCharToInt(char ch, int &value)
{
  if ((ch >= '0') && (ch <= '9'))
    value = ch - '0';
  else if ((ch >= 'A') && (ch <= 'F'))
    value = 10 + ch - 'A';
  else if ((ch >= 'a') && (ch <= 'f'))
    value = 10 + ch - 'a';
  else
  {
    value = 0;
    return false;
  }
  
  return true;
}

// Процедура обработки сообщения:
void processMessage(String message)
{
  if (ECHO_MODE)
  {
    #ifdef USBCON   // For Leonardo (Romeo V2) board support
      Serial1.print(message);
    #else
      Serial.print(message);
    #endif

  }  
  
  // Парсер команды:
  String command;
  int value;
  if (! parseMessage(message, command, value))
  {
    #ifdef USBCON   // For Leonardo (Romeo V2) board support
      Serial1.flush();
      Serial1.print("#0001"); // неверное сообщение – возникает, если сообщение не удалось разобрать на команды/событие и значение
    #else
      Serial.flush();
      Serial.print("#0001"); // неверное сообщение – возникает, если сообщение не удалось разобрать на команды/событие и значение
    #endif
    return;
  }
  
  executeAction(command, value, false);
}

void executeAction(String command, unsigned int value, boolean inPlaybackMode)
{
  switch(command[0]) {  // Сейчас у нас односимвольные команды, но на случай развития команда определена как String
    case 'W':
    case 'Z':
    {
      #ifdef USBCON   // For Leonardo (Romeo V2) board support
        Serial1.print("#0007"); // недопустимая команда вне РобоСкрипт
      #else
        Serial.print("#0007"); // недопустимая команда вне РобоСкрипт
      #endif

      return;
    }
    case 'L':
    case 'R':
    case 'G':  
    {
      // Команда двигателям:
      moveMotor(command, value);
      break;
    }
    case 'H':  
    case 'V':  
    {
      // Команда голове:
      Equipment::moveHead(command, value);
      break;
    }
//    case 'h':
//    {
//      // startTurn method requires a signed millisecond parameter.
//      // And the command 'h' has value stored in centiseconds (1/100
//      // of a second). That's why we need to multiply by 10.
//      signed long sentiSeconds = (signed int)value;
//      servoHeadHorizontal.startTurn(sentiSeconds * 10, true);
//      break;
//    }
//    case 'v':
//    {
//      // startTurn method requires a signed millisecond parameter.
//      // And the command 'v' has value stored in centiseconds (1/100
//      // of a second). That's why we need to multiply by 10.
//      signed long sentiSeconds = (signed int)value;
//      servoHeadVertical.startTurn(sentiSeconds * 10, true);
//      break;
//    }
    case 'T':  
    {
      Equipment::moveTail(value);
      break;
    }
//    case 't':      
//    {
//      if (value != 0)
//      {
//        servoTail.startSwing(value, 250, 6, 70, 0.9, true);
//      }
//      break;
//    }
//    case 'n':     
//    {
//      if (value != 0)
//      {
//        servoHeadHorizontal.startSwing(value, 400, 2.5, 60, 0.75, true);
//      }
//      break;
//    }
//    case 'y':
//    {
//      if (value != 0)
//      {
//        servoHeadVertical.startSwing(value, 400, 2.5, 30, 0.8, true);
//      }
//      break;
//    }
//    case 'M':
//    {
//      switch(value)
//      {
//        case 0x0101:
//        {
//          happyReflexStart();
//          if (inPlaybackMode)
//          {
//            sendMessageToRobot(command, value);
//          }
//          break; 
//        }
//        case 0x0102:
//        {
//          const int ReadyToPlayVerticalDegree = 70;
//          int verticalAmplitude = abs((ReadyToPlayVerticalDegree - servoHeadVertical.read()) * 2);
//          boolean swingDirection = ReadyToPlayVerticalDegree > servoHeadVertical.read();
//          servoHeadVertical.startSwing(2, 400, 0.25, verticalAmplitude, 1, swingDirection);
//          servoHeadHorizontal.startSwing(2, 250, 3.5, 40, 0.8, true);
//          servoTail.startSwing(value, 250, 6, 70, 0.9, true);
//          readyToPlayReflexStart();
//          if (inPlaybackMode)
//          {
//            sendMessageToRobot(command, value);
//          }
//          break; 
//        }
//        case 0x0103:
//        {
//          int verticalAmplitude = (servoHeadVertical.read() - Configuration::SERVO_HEAD_VERTICAL_MIN_DEGREE) * 2;
//          servoHeadVertical.startSwing(2, 6000, 0.25, verticalAmplitude, 1, false);
//          servoHeadHorizontal.startSwing(2, 750, 2, 60, 0.6, true);
//          sadReflexStart();
//          if (inPlaybackMode)
//          {
//            sendMessageToRobot(command, value);
//          }
//          break;
//        }
//        case 0x0104:
//        {
//          angryReflexStart();
//          if (inPlaybackMode)
//          {
//            sendMessageToRobot(command, value);
//          }
//          break;
//        }
//        case 0x0105:  
//        {
//          int musicTacts = 12;
//          servoHeadVertical.startSwing(1, musicPeriod, musicTacts, 30, 1, true);
//          servoHeadHorizontal.startSwing(2, musicPeriod * musicTacts / 1.5, 1.5, 50, 1, true);
//          servoTail.startSwing(value, musicPeriod, musicTacts, 70, 1, true);
//          musicReflexStart();
//          break;
//        }
//        default:
//        {
//          sendMessageToRobot(command, value);
//        }
//      } // (M-command's switch)
//      break;
//    }
    case 'I':    
    {
      Equipment::setHeadlightState(value);
      break;
    }
//    case 's':    
//    {
//      // Команда выстрела пушке:
//      irsend.sendSony(0xABC0, 16);
//      irrecv.enableIRIn(); // (надо для повторной инициализации ИК-приёмника)
//      break;
//    }
    case '=':  // "=NDDD" checks the battery status. N=Number of Voltage Devider. DDD=Interval (data will be sent every DDD*10 . voltage will be send back as "~" command + Voltage Devider Number + voltage*100. i.e. for 5.02V for Voltage Devider number 0 the command will be "~01F6", for Devider Number number 1 "~11F6"
    {          // You can use value to set timer to receive battery status. i.e. "=0100" (timer 0) or "=1100" (timer 1) will send battery status every 4096 millisecons for timer number 0 or 1. "=0000" will send battery status only once and will switch off the timer is it was set before.
//      sendMessageToRobot("~",(value & 0xF000) | VCCRead(value>>12));
//      SetVCCTimer(value>>12, (value & 0x0FFF)*10);
      break;
    }
    case '~': // just ignore this command
    {
      break;
    }
    default:
    {
      ////Serial1.flush();
      #ifdef USBCON   // For Leonardo (Romeo V2) board support
        Serial1.print("#0002"); // неизвестная команда
      #else
        Serial.print("#0002"); // неизвестная команда
      #endif        
      return;
    }
  }//main switch
  #ifdef USBCON   // For Leonardo (Romeo V2) board support
    Serial1.print("#0000"); // Успешное выполнение команды, (потом можно удалить.)
  #else
    Serial.print("#0000"); // Успешное выполнение команды, (потом можно удалить.)
  #endif
}

// Управление двигателями.
void moveMotor(String side, int speed)
{
  bool directionPinValue = speed > 0 ? LOW : HIGH;
  
  if (speed < 0) {
    speed = - speed;
  }
 
  if (speed > 255) {
    speed = 255;
  }
  
  if (speed == 0) {
    directionPinValue = LOW; // это для режима отключения мотора
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

void sendMessageToRobot(String command, unsigned int value)
{
  String hexValue = String(value, HEX);
  hexValue.toUpperCase();
  while (hexValue.length() < 4) hexValue = "0" + hexValue;
  #ifdef USBCON   // For Leonardo (Romeo V2) board support
    Serial1.print(command + hexValue);
  #else
    Serial.print(command + hexValue);
  #endif
}

