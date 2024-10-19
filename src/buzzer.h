#ifndef BUZZER_H
#define BUZZER_H

#include "onOffDevice.h"

class Buzzer : public OnOffDevice
{
public:
    Buzzer(uint8_t pin);
};

#endif
