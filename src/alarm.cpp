#include "alarm.h"

Alarm::Alarm(uint8_t blueLed1, uint8_t blueLed2, uint8_t blueLed3, uint8_t blueLed4,
             uint8_t redLed, uint8_t buzzerPin, uint8_t trigPin, uint8_t echoPin,
             uint8_t powerPin, uint8_t r1, uint8_t r2, uint8_t r3, uint8_t r4,
             uint8_t c1, uint8_t c2, uint8_t c3, uint8_t c4)
    : blueLeds{LED(blueLed1), LED(blueLed2), LED(blueLed3), LED(blueLed4)},
      redLed(redLed),
      buzzer(buzzerPin),
      ultrasonicSensor(trigPin, echoPin, powerPin),
      keypad(r1, r2, r3, r4, c1, c2, c3, c4),
      currentState(STARTUP),
      password{}, // Initialize password array to zeros
      averageDistance(0.0f)
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
    turnOnBlueLeds();
    Logger::log("Enter 4-digit password:\n");

    for (uint8_t i = 0; i < PASSWORD_LENGTH; i++)
    {
        char key;
        while (!(key = keypad.getKey()))
        {
            // Wait for valid key press
        }

        if (i == 0)
        {
            turnOffBlueLeds();
        }

        Logger::log(key);
        Logger::log(", ");
        password[i] = key;
        displayPasswordFeedback(i);
    }

    password[PASSWORD_LENGTH] = '\0';
    _delay_ms(LED_DISPLAY_DELAY_MS);
    turnOffBlueLeds();

    Logger::log("\nPassword set. Entering STANDBY mode.\n");
    Logger::log("STANDBY: Enter password to arm the system.\n");
    currentState = STANDBY;
}

void Alarm::handleStandby()
{
    if (checkPassword())
    {
        Logger::log("\nPassword correct. Arming in 10 seconds...\n");
        for (int i = 10; i > 0; i--)
        {
            Logger::log(i);
            Logger::log(i != 1 ? ", " : "");
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

    static bool forward = true;
    static uint8_t currentLed = 0;

    while (currentState == WATCHING)
    {
        turnOffAllLights();
        blueLeds[currentLed].on();

        // Update LED pattern
        if (forward)
        {
            if (currentLed >= 3)
            {
                forward = false;
                currentLed = 2;
            }
            else
            {
                currentLed++;
            }
        }
        else
        {
            if (currentLed <= 0)
            {
                forward = true;
                currentLed = 1;
            }
            else
            {
                currentLed--;
            }
        }

        float currentDistance = ultrasonicSensor.measureDistance();
        if (currentDistance < averageDistance * MOTION_THRESHOLD)
        {
            Logger::log("Motion detected! Entering PRE_ALERT mode.\nCurrent distance: ");
            Logger::log(currentDistance);
            Logger::log(" cm\n");
            currentState = PRE_ALERT;
            break;
        }
        _delay_ms(WATCHING_DELAY_MS);
    }
}

void Alarm::handlePreAlert()
{
    Logger::log("PRE_ALERT: Red LED on.\n");
    redLed.on();

    Logger::log("PRE_ALERT: Enter password within 10 seconds.\n");
    uint32_t startTime = Timer::getMillis();

    while (Timer::getMillis() - startTime < PASSWORD_TIMEOUT_MS)
    {
        if (checkPassword())
        {
            redLed.off();
            Logger::log("\nPassword correct. Returning to STANDBY mode.\n");
            Logger::log("STANDBY: Enter password to arm the system.\n");
            currentState = STANDBY;
            return;
        }
    }

    redLed.off();
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
            Logger::log("STANDBY: Enter password to arm the system.\n");
            currentState = STANDBY;
            break;
        }
    }
}

bool Alarm::checkPassword()
{
    char enteredPassword[PASSWORD_LENGTH + 1] = {0};
    uint8_t passwordIndex = 0;
    uint32_t startTime = Timer::getMillis();

    turnOffBlueLeds();

    while (passwordIndex < PASSWORD_LENGTH &&
           Timer::getMillis() - startTime < PASSWORD_TIMEOUT_MS)
    {
        char key = keypad.getKey();
        if (key != 0)
        {
            Logger::log(key);
            Logger::log(", ");
            enteredPassword[passwordIndex] = key;
            displayPasswordFeedback(passwordIndex);
            passwordIndex++;
        }
    }

    _delay_ms(LED_DISPLAY_DELAY_MS);

    if (passwordIndex < PASSWORD_LENGTH)
    {
        turnOffBlueLeds();
        return false;
    }

    bool isCorrect = stringCompare(enteredPassword, password);
    turnOffBlueLeds();

    if (!isCorrect)
    {
        Logger::log("\nIncorrect password.\n");
        alarmBuzz(4);
    }

    return isCorrect;
}

void Alarm::displayPasswordFeedback(uint8_t digitIndex)
{
    if (digitIndex < 4)
    {
        blueLeds[digitIndex].on();
    }
}

void Alarm::turnOnBlueLeds()
{
    for (auto &led : blueLeds)
    {
        led.on();
    }
}

void Alarm::turnOffBlueLeds()
{
    for (auto &led : blueLeds)
    {
        led.off();
    }
}

void Alarm::turnOffAllLights()
{
    turnOffBlueLeds();
    redLed.off();
}

void Alarm::measureAverageDistance()
{
    float totalDistance = 0;
    for (uint8_t i = 0; i < DISTANCE_SAMPLES; i++)
    {
        totalDistance += ultrasonicSensor.measureDistance();
        _delay_ms(WATCHING_DELAY_MS);
    }
    averageDistance = totalDistance / DISTANCE_SAMPLES;
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

bool Alarm::stringCompare(const char *str1, const char *str2)
{
    if (!str1 || !str2)
        return false;

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