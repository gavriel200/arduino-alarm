#ifndef RGBLED_H
#define RGBLED_H

#include "pinUtils.h"

class RGBLed
{
private:
    uint8_t redPin, greenPin, bluePin;

public:
    RGBLed(uint8_t redPin, uint8_t greenPin, uint8_t bluePin);

    void setColor(uint8_t red, uint8_t green, uint8_t blue);
    void turnOff();
};

#endif
