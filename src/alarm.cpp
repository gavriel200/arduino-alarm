#include "alarm.h"

bool stringCompare(const char *str1, const char *str2)
{
    while (*str1 && *str2)
    {
        if (*str1 != *str2)
        {
            return false;
        }
        str1++;
        str2++;
    }
    return (*str1 == '\0' && *str2 == '\0');
}

Alarm::Alarm(uint8_t blueLed1, uint8_t blueLed2, uint8_t blueLed3, uint8_t blueLed4,
             uint8_t redLed, uint8_t buzzerPin, uint8_t trigPin, uint8_t echoPin, uint8_t powerPin, uint8_t r1, uint8_t r2, uint8_t r3, uint8_t r4,
             uint8_t c1, uint8_t c2, uint8_t c3, uint8_t c4)
    : blueLeds{LED(blueLed1), LED(blueLed2), LED(blueLed3), LED(blueLed4)},
      redLed(redLed), buzzer(buzzerPin), ultrasonicSensor(trigPin, echoPin, powerPin),
      keypad(r1, r2, r3, r4, c1, c2, c3, c4), currentState(STARTUP)
{
    Logger::init();
    Timer::init();
}

void Alarm::run()
{
    Logger::log("Alarm system starting...\n");
    while (true)
    {
        switch (currentState)
        {
        case STARTUP:
            handleStartup();
            break;
        case STANDBY:
            handleStandby();
            break;
        case WATCHING:
            handleWatching();
            break;
        case PRE_ALERT:
            handlePreAlert();
            break;
        case ALERT:
            handleAlert();
            break;
        }
    }
}

void Alarm::handleStartup()
{
    Logger::log("Enter 4-digit password:\n");
    int passwordIndex = 0;
    while (passwordIndex < 4)
    {
        char key = keypad.getKey();
        if (key != 0)
        {
            Logger::log(key);
            Logger::log(", ");
            password[passwordIndex] = key;
            blueLeds[passwordIndex].on(); // Turn on LED and keep it on
            passwordIndex++;
        }
    }
    password[4] = '\0';
    _delay_ms(1000); // Keep LEDs on for a second after password is set
    turnOffBlueLeds();
    Logger::log("\nPassword set. Entering STANDBY mode.\n");
    currentState = STANDBY;
}

void Alarm::handleStandby()
{
    Logger::log("STANDBY: Enter password to arm the system.\n");
    if (checkPassword())
    {
        Logger::log("\nPassword correct. Arming in 10 seconds...\n");
        for (int i = 10; i > 0; i--)
        {
            Logger::log(i);
            if (i != 1)
            {
                Logger::log(", ");
            }
            _delay_ms(1000);
        }
        currentState = WATCHING;
        Logger::log("\nSystem armed. Entering WATCHING mode.\n");
    }
}

void Alarm::handleWatching()
{
    Logger::log("WATCHING: Measuring average distance...\n");
    measureAverageDistance();
    Logger::log("Average distance: ");
    Logger::log(averageDistance);
    Logger::log(" cm\n");
    Logger::log("WATCHING!\n");

    while (currentState == WATCHING)
    {
        float currentDistance = ultrasonicSensor.measureDistance();
        if (currentDistance < averageDistance * 0.5)
        {
            Logger::log("Motion detected! Entering PRE_ALERT mode.\n");
            currentState = PRE_ALERT;
        }
        _delay_ms(100);
    }
}

void Alarm::handlePreAlert()
{
    Logger::log("PRE_ALERT: Enter password within 10 seconds.\n");
    uint32_t startTime = Timer::getMillis();
    while (Timer::getMillis() - startTime < 10000)
    {
        if (checkPassword())
        {
            Logger::log("\nPassword correct. Returning to STANDBY mode.\n");
            currentState = STANDBY;
            return;
        }
    }
    Logger::log("Time's up! Entering ALERT mode.\n");
    currentState = ALERT;
}

void Alarm::handleAlert()
{
    Logger::log("ALERT: Alarm triggered!\n");
    while (currentState == ALERT)
    {
        alarmBuzz(10);

        if (checkPassword())
        {
            Logger::log("\nPassword correct. Returning to STANDBY mode.\n");
            currentState = STANDBY;
        }
    }
}

bool Alarm::checkPassword()
{
    char enteredPassword[5] = {0};
    int passwordIndex = 0;
    uint32_t startTime = Timer::getMillis();

    turnOffBlueLeds(); // Ensure all LEDs are off at the start

    while (passwordIndex < 4 && Timer::getMillis() - startTime < 10000)
    {
        char key = keypad.getKey();
        if (key != 0)
        {
            Logger::log(key);
            Logger::log(", ");
            enteredPassword[passwordIndex] = key;
            blueLeds[passwordIndex].on();
            passwordIndex++;
        }
    }

    _delay_ms(1000); // Keep LEDs on for a second after password is entered

    if (passwordIndex < 4)
    {
        turnOffBlueLeds();
        return false;
    }

    if (stringCompare(enteredPassword, password))
    {
        turnOffBlueLeds();
        return true;
    }
    else
    {
        Logger::log("\nIncorrect password.\n");
        turnOffBlueLeds();
        alarmBuzz(4);
        return false;
    }
}

void Alarm::turnOffBlueLeds()
{
    for (int i = 0; i < 4; i++)
    {
        blueLeds[i].off();
    }
}

void Alarm::measureAverageDistance()
{
    float totalDistance = 0;
    for (int i = 0; i < 50; i++)
    {
        totalDistance += ultrasonicSensor.measureDistance();
        _delay_ms(100);
    }
    averageDistance = totalDistance / 50;
}

void Alarm::alarmBuzz(int times)
{
    for (int i = 0; i < times; i++)
    {
        redLed.on();
        buzzer.on();
        _delay_ms(100);
        redLed.off();
        buzzer.off();
        _delay_ms(100);
    }
}
