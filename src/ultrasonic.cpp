#include "ultrasonic.h"

Ultrasonic::Ultrasonic(uint8_t trigPin, uint8_t echoPin, uint8_t powerPin) : trigPin(trigPin), echoPin(echoPin), powerPin(powerPin)
{
    PinUtils::setPinMode(trigPin, OUTPUT);
    PinUtils::setPinMode(echoPin, INPUT);
    PinUtils::setPinMode(powerPin, OUTPUT);

    PinUtils::digitalWritePin(powerPin, HIGH);
}

float Ultrasonic::measureDistance()
{
    PinUtils::digitalWritePin(trigPin, LOW);
    _delay_us(2);
    PinUtils::digitalWritePin(trigPin, HIGH);
    _delay_us(10);
    PinUtils::digitalWritePin(trigPin, LOW);

    unsigned long duration = 0;
    while (PinUtils::digitalReadPin(echoPin) == LOW)
    {
    }
    while (PinUtils::digitalReadPin(echoPin) == HIGH)
    {
        duration++;
        _delay_us(1);
    }

    return duration * 0.034 / 2;
}