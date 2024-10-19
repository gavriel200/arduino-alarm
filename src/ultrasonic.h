#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <util/delay.h>

#include "pinUtils.h"

class Ultrasonic
{
private:
    uint8_t trigPin;
    uint8_t echoPin;

public:
    Ultrasonic(uint8_t trigPin, uint8_t echoPin);
    float measureDistance();
};

#endif
