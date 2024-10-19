#include "onOffDevice.h"

OnOffDevice::OnOffDevice(uint8_t pin) : pin(pin)
{
    PinUtils::setPinMode(pin, OUTPUT);
}

void OnOffDevice::on()
{
    PinUtils::digitalWritePin(pin, HIGH);
}

void OnOffDevice::off()
{
    PinUtils::digitalWritePin(pin, LOW);
}

void OnOffDevice::toggle()
{
    int currentState = PinUtils::digitalReadPin(pin);
    PinUtils::digitalWritePin(pin, !currentState);
}