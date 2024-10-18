#include "Alarm.h"

Alarm::Alarm() : logger(9600), ultrasonic(3, 2, timer), keypad(8, 7, 6, 5, 9, 10, 11, 12),
                 blue1(A0), blue2(A1), blue3(A2), blue4(A3), red(A4), buzzer(13),
                 passwordIndex(0), mode(ON_START)
{
    for (int i = 0; i < 4; ++i)
    {
        this->password[i] = '\0';
        this->enteredPassword[i] = '\0';
    }
    password[4] = '\0';
    enteredPassword[4] = '\0';
}

void Alarm::setup()
{
    this->logger.log("Alarm setup\n");
    this->turnOffAllLEDs();
    this->buzzer.turnOff();
}

void Alarm::loop()
{
    switch (this->mode)
    {
    case ON_START:
        this->logger.log("Enter password to set\n");
        this->setPassword();
        this->mode = STANDBY;
        this->logger.log("Standby mode\n");
        break;

    case STANDBY:
        if (this->keypad.getKey())
        {
            this->turnOffAllLEDs();
            this->enterPassword();
            if (this->checkPassword())
            {
                this->logger.log("Password correct, entering watch mode in 60 seconds\n");
                this->timer.reset();
                this->mode = WATCH_COUNTDOWN;
            }
            else
            {
                this->logger.log("Incorrect password\n");
                this->blinkRedLED();
            }
        }
        break;

    case WATCH_COUNTDOWN:
        if (this->timer.elapsed() >= 10000)
        {
            this->mode = WATCH;
            this->logger.log("Watch mode activated\n");
        }
        break;

    case WATCH:
        uint16_t distance = this->ultrasonic.getDistance();
        this->logger.log("Distance: ");
        this->logger.log(distance);
        this->logger.log(" cm\n");
        if (distance < 100 && distance > 0)
        {
            this->logger.log("Motion detected, entering pre-alert mode\n");
            this->timer.reset();
            this->mode = PRE_ALERT;
            break;
        }
        break;

    case PRE_ALERT:
        this->logger.log("im here\n"); // Log to verify you're in this state
        if (this->timer.elapsed() < 10000)
        {
            this->logger.log("im 1\n"); // Log to verify you're in this state
            if (this->keypad.getKey())
            {
                this->turnOffAllLEDs();
                this->enterPassword();
                if (this->checkPassword())
                {
                    this->logger.log("Password correct, returning to standby\n");
                    this->mode = STANDBY;
                }
                else
                {
                    this->blinkRedLED();
                }
            }
        }
        else
        {
            this->logger.log("im 2\n"); // Log to verify you're in this state
            this->logger.log("Entering alert mode\n");
            this->mode = ALERT;  // Transition to ALERT mode after the timer has elapsed
            this->timer.reset(); // Reset the timer to start fresh for the ALERT state
        }
        this->logger.log("im 3\n"); // Log to verify you're in this state
        break;

    case ALERT:
        this->blinkRedLED();
        this->activateBuzzer();
        if (this->timer.elapsed() >= 200) // Check every 200ms
        {
            if (this->keypad.getKey())
            {
                this->turnOffAllLEDs();
                this->buzzer.turnOff();
                this->enterPassword();
                if (this->checkPassword())
                {
                    this->logger.log("Password correct, returning to standby\n");
                    this->mode = STANDBY;
                }
                else
                {
                    this->blinkRedLED();
                }
            }
            this->timer.reset();
        }
        break;
    }

    this->updateLEDs();
}
void Alarm::setPassword()
{
    passwordIndex = 0;
    while (passwordIndex < 4)
    {
        char key = this->keypad.getKey();
        if (key != '\0')
        {
            this->password[passwordIndex] = key;
            passwordIndex++;
            this->logger.log("Password digit set: ");
            this->logger.log(key);
            this->logger.log("\n");
            this->updateLEDs();
            this->timer.delay(200); // Delay to prevent multiple reads
        }
    }
    this->logger.log("Password set complete\n");
}

void Alarm::enterPassword()
{
    passwordIndex = 0;
    while (passwordIndex < 4)
    {
        char key = this->keypad.getKey();
        if (key != '\0')
        {
            this->enteredPassword[passwordIndex] = key;
            passwordIndex++;
            this->logger.log("Password digit entered: ");
            this->logger.log(key);
            this->logger.log("\n");
            this->updateLEDs();
            this->timer.delay(200); // Delay to prevent multiple reads
        }
    }
    this->logger.log("Password entry complete\n");
}

bool Alarm::checkPassword()
{
    for (int i = 0; i < 4; i++)
    {
        if (this->password[i] != this->enteredPassword[i])
        {
            return false;
        }
    }
    return true;
}

void Alarm::blinkRedLED()
{
    this->red.turnOn();
    this->timer.delay(200);
    this->red.turnOff();
    this->timer.delay(200);
    this->red.turnOn();
    this->timer.delay(200);
    this->red.turnOff();
}

void Alarm::turnOffAllLEDs()
{
    this->blue1.turnOff();
    this->blue2.turnOff();
    this->blue3.turnOff();
    this->blue4.turnOff();
    this->red.turnOff();
}

void Alarm::updateLEDs()
{
    this->turnOffAllLEDs();
    for (int i = 0; i < passwordIndex; i++)
    {
        switch (i)
        {
        case 0:
            this->blue1.turnOn();
            break;
        case 1:
            this->blue2.turnOn();
            break;
        case 2:
            this->blue3.turnOn();
            break;
        case 3:
            this->blue4.turnOn();
            break;
        }
    }
}

void Alarm::activateBuzzer()
{
    this->buzzer.turnOn();
}