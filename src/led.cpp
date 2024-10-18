#include "led.h"

Led::Led(uint8_t pin) : pin(pin)
{
    PinUtils::pinSet(pin, OUTPUT);
}

void Led::turnOn()
{
    PinUtils::pinWrite(pin, HIGH);
}

void Led::turnOff()
{
    PinUtils::pinWrite(pin, LOW);
}
