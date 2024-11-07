#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <util/delay.h>

#include "pinUtils.h"

class Ultrasonic
{
private:
    uint8_t trigPin;
    uint8_t echoPin;
    uint8_t powerPin;

public:
    Ultrasonic(uint8_t trigPin, uint8_t echoPin, uint8_t powerPin);
    float measureDistance();
};

#endif
