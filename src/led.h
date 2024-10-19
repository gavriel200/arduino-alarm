#ifndef LED_H
#define LED_H

#include "onOffDevice.h"

class LED : public OnOffDevice
{
public:
    LED(uint8_t pin);
};

#endif
