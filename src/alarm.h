#ifndef ALARM_H
#define ALARM_H

#include <stdint.h>
#include <util/delay.h>

#include "led.h"
#include "buzzer.h"
#include "ultrasonic.h"
#include "keypad.h"
#include "logger.h"
#include "timer.h"

enum State
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
    static constexpr uint8_t PASSWORD_LENGTH = 4;
    static constexpr uint32_t PASSWORD_TIMEOUT_MS = 10000; // 10 seconds
    static constexpr uint32_t DISTANCE_SAMPLES = 50;
    static constexpr float MOTION_THRESHOLD = 0.5;
    static constexpr uint32_t LED_DISPLAY_DELAY_MS = 1000;
    static constexpr uint32_t WATCHING_DELAY_MS = 100;

    LED blueLeds[4];
    LED redLed;
    Buzzer buzzer;
    Ultrasonic ultrasonicSensor;
    Keypad keypad;
    State currentState;
    char password[PASSWORD_LENGTH + 1]; // +1 for null terminator
    float averageDistance;

    // State handlers
    void handleStartup();
    void handleStandby();
    void handleWatching();
    void handlePreAlert();
    void handleAlert();

    // Utility functions
    bool checkPassword();
    void turnOnBlueLeds();
    void turnOffBlueLeds();
    void turnOffAllLights();
    void measureAverageDistance();
    void alarmBuzz(int times);
    bool stringCompare(const char *str1, const char *str2);
    void displayPasswordFeedback(uint8_t digitIndex);

public:
    /**
     * @brief Constructs a new Alarm object
     *
     * @param blueLed1-4 Pin numbers for the four blue LEDs
     * @param redLed Pin number for the red LED
     * @param buzzerPin Pin number for the buzzer
     * @param trigPin Ultrasonic sensor trigger pin
     * @param echoPin Ultrasonic sensor echo pin
     * @param powerPin Ultrasonic sensor power pin
     * @param r1-4 Keypad row pins
     * @param c1-4 Keypad column pins
     */
    Alarm(uint8_t blueLed1, uint8_t blueLed2, uint8_t blueLed3, uint8_t blueLed4,
          uint8_t redLed, uint8_t buzzerPin, uint8_t trigPin, uint8_t echoPin,
          uint8_t powerPin, uint8_t r1, uint8_t r2, uint8_t r3, uint8_t r4,
          uint8_t c1, uint8_t c2, uint8_t c3, uint8_t c4);

    /**
     * @brief Starts the alarm system main loop
     * This function runs indefinitely, handling different states of the alarm system
     */
    void run();
};

#endif // ALARM_H