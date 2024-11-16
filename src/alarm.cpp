#include "alarm.h"

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
        switch (this->currentState)
        {
        case STARTUP:
            this->handleStartup();
            break;
        case STANDBY:
            this->handleStandby();
            break;
        case WATCHING:
            this->handleWatching();
            break;
        case PRE_ALERT:
            this->handlePreAlert();
            break;
        case ALERT:
            this->handleAlert();
            break;
        }
    }
}

void Alarm::handleStartup()
{
    this->turnOnBlueLeds();
    Logger::log("Enter 4-digit password:\n");
    int passwordIndex = 0;
    while (passwordIndex < 4)
    {
        char key = this->keypad.getKey();
        if (key != 0)
        {
            if (passwordIndex == 0)
            {
                this->turnOffBlueLeds();
            }
            Logger::log(key);
            Logger::log(", ");
            this->password[passwordIndex] = key;
            this->blueLeds[passwordIndex].on(); // Turn on LED and keep it on
            passwordIndex++;
        }
    }
    this->password[4] = '\0';
    _delay_ms(1000); // Keep LEDs on for a second after password is set
    this->turnOffBlueLeds();
    Logger::log("\nPassword set. Entering STANDBY mode.\n");
    Logger::log("STANDBY: Enter password to arm the system.\n");
    this->currentState = STANDBY;
}

void Alarm::handleStandby()
{
    if (this->checkPassword())
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
        this->currentState = WATCHING;
        Logger::log("\nSystem armed. Entering WATCHING mode.\n");
    }
}

void Alarm::handleWatching()
{
    Logger::log("WATCHING: Measuring average distance...\n");
    this->measureAverageDistance();
    Logger::log("Average distance: ");
    Logger::log(this->averageDistance);
    Logger::log(" cm\n");
    Logger::log("WATCHING!\n");

    static bool forward = true;
    static int currentLed = 0;

    while (this->currentState == WATCHING)
    {
        turnOffAllLights();
        blueLeds[currentLed].on();

        if (forward)
        {
            currentLed++;
            if (currentLed == 3)
                forward = false;
        }
        else
        {
            currentLed--;
            if (currentLed == 0)
                forward = true;
        }

        float currentDistance = this->ultrasonicSensor.measureDistance();
        if (currentDistance < this->averageDistance * 0.5)
        {
            Logger::log("Motion detected! Entering PRE_ALERT mode.\nCurrent distance: ");
            Logger::log(currentDistance);
            Logger::log(" cm\n");
            this->currentState = PRE_ALERT;
        }
        _delay_ms(100);
    }
}

void Alarm::handlePreAlert()
{
    Logger::log("PRE_ALERT: Red LED on.\n");
    this->redLed.on();

    Logger::log("PRE_ALERT: Enter password within 10 seconds.\n");
    uint32_t startTime = Timer::getMillis();
    while (Timer::getMillis() - startTime < 10000)
    {
        if (this->checkPassword())
        {
            this->redLed.off();
            Logger::log("\nPassword correct. Returning to STANDBY mode.\n");
            Logger::log("STANDBY: Enter password to arm the system.\n");
            this->currentState = STANDBY;
            return;
        }
    }
    this->redLed.off();
    Logger::log("Time's up! Entering ALERT mode.\n");
    this->currentState = ALERT;
}

void Alarm::handleAlert()
{
    Logger::log("ALERT: Alarm triggered!\n");
    while (this->currentState == ALERT)
    {
        this->alarmBuzz(10);

        if (this->checkPassword())
        {
            Logger::log("\nPassword correct. Returning to STANDBY mode.\n");
            Logger::log("STANDBY: Enter password to arm the system.\n");
            this->currentState = STANDBY;
        }
    }
}

bool Alarm::checkPassword()
{
    char enteredPassword[5] = {0};
    int passwordIndex = 0;
    uint32_t startTime = Timer::getMillis();

    this->turnOffBlueLeds(); // Ensure all LEDs are off at the start

    while (passwordIndex < 4 && Timer::getMillis() - startTime < 10000)
    {
        char key = this->keypad.getKey();
        if (key != 0)
        {
            Logger::log(key);
            Logger::log(", ");
            enteredPassword[passwordIndex] = key;
            this->blueLeds[passwordIndex].on();
            passwordIndex++;
        }
    }

    _delay_ms(1000); // Keep LEDs on for a second after password is entered

    if (passwordIndex < 4)
    {
        this->turnOffBlueLeds();
        return false;
    }

    if (this->stringCompare(enteredPassword, this->password))
    {
        this->turnOffBlueLeds();
        return true;
    }
    else
    {
        Logger::log("\nIncorrect password.\n");
        this->turnOffBlueLeds();
        this->alarmBuzz(4);
        return false;
    }
}

void Alarm::turnOnBlueLeds()
{
    for (int i = 0; i < 4; i++)
    {
        this->blueLeds[i].on();
    }
}

void Alarm::turnOffBlueLeds()
{
    for (int i = 0; i < 4; i++)
    {
        this->blueLeds[i].off();
    }
}

void Alarm::turnOffAllLights()
{
    for (int i = 0; i < 4; i++)
    {
        this->blueLeds[i].off();
    }
    this->redLed.off();
}

void Alarm::measureAverageDistance()
{
    float totalDistance = 0;
    for (int i = 0; i < 50; i++)
    {
        totalDistance += this->ultrasonicSensor.measureDistance();
        _delay_ms(100);
    }
    this->averageDistance = totalDistance / 50;
}

void Alarm::alarmBuzz(int times)
{
    for (int i = 0; i < times; i++)
    {
        this->redLed.on();
        this->buzzer.on();
        _delay_ms(100);
        this->redLed.off();
        this->buzzer.off();
        _delay_ms(100);
    }
}

bool Alarm::stringCompare(const char *str1, const char *str2)
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
