#ifndef ON_OFF_DEVICE_H
#define ON_OFF_DEVICE_H

#include "pinUtils.h"

class OnOffDevice
{
protected:
    uint8_t pin;

public:
    OnOffDevice(uint8_t pin);
    virtual void on();
    virtual void off();
    virtual void toggle();
};

#endif