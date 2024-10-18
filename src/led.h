#ifndef LED_H
#define LED_H

#include "pinUtils.h"

class Led
{
private:
    uint8_t pin;

public:
    Led(uint8_t pin);
    void turnOn();
    void turnOff();
};

#endif