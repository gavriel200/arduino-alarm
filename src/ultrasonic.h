#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <stdint.h>
#include "Timer.h"

class Ultrasonic
{
public:
    Ultrasonic(uint8_t triggerPin, uint8_t echoPin, Timer &timer);
    uint16_t getDistance();

private:
    uint8_t triggerPin;
    uint8_t echoPin;
    Timer &timer;
};

#endif // ULTRASONIC_H
