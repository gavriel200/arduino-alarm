#ifndef ALARM_H
#define ALARM_H

#include "Logger.h"
#include "Ultrasonic.h"
#include "Keypad.h"
#include "led.h"
#include "Timer.h"

enum AlarmMode
{
    ON_START,
    STANDBY,
    WATCH_COUNTDOWN,
    WATCH,
    PRE_ALERT,
    ALERT
};

class Alarm
{
public:
    Alarm();
    void setup();
    void loop();

private:
    Logger logger;
    Timer timer;
    Ultrasonic ultrasonic;
    Keypad keypad;
    Led blue1, blue2, blue3, blue4, red, buzzer;

    char password[5];
    char enteredPassword[5];
    int passwordIndex;
    AlarmMode mode;

    void setPassword();
    void enterPassword();
    bool checkPassword();
    void blinkRedLED();
    void turnOffAllLEDs();
    void updateLEDs();
    void activateBuzzer();
};

#endif // ALARM_H