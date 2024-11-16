#ifndef ALARM_H
#define ALARM_H

#include "led.h"
#include "buzzer.h"
#include "ultrasonic.h"
#include "keypad.h"
#include "logger.h"
#include "timer.h"

enum AlarmState
{
    STARTUP,
    STANDBY,
    WATCHING,
    PRE_ALERT,
    ALERT
};

class Alarm
{
private:
    LED blueLeds[4];
    LED redLed;
    Buzzer buzzer;
    Ultrasonic ultrasonicSensor;
    Keypad keypad;
    AlarmState currentState;
    char password[5];
    float averageDistance;

    void handleStartup();
    void handleStandby();
    void handleWatching();
    void handlePreAlert();
    void handleAlert();
    bool checkPassword();
    void measureAverageDistance();
    void turnOnBlueLeds();
    void turnOffBlueLeds();
    void turnOffAllLights();
    void blinkAllBlueLeds();
    void moveBlueLedsSideToSide();
    void alarmBuzz(int times);
    bool stringCompare(const char *str1, const char *str2);

public:
    Alarm(uint8_t blueLed1, uint8_t blueLed2, uint8_t blueLed3, uint8_t blueLed4,
          uint8_t redLed, uint8_t buzzer, uint8_t trigPin, uint8_t echoPin, uint8_t powerPin,
          uint8_t r1, uint8_t r2, uint8_t r3, uint8_t r4,
          uint8_t c1, uint8_t c2, uint8_t c3, uint8_t c4);
    void run();
};

#endif
