#include "pinUtils.h"

void PinUtils::setPinMode(uint8_t pin, uint8_t mode)
{
    if (pin < 8)
    {
        if (mode == INPUT)
        {
            DDRD &= ~(1 << pin);
        }
        else
        {
            DDRD |= (1 << pin);
        }
    }
    else if (pin < 14)
    {
        pin -= 8;
        if (mode == INPUT)
        {
            DDRB &= ~(1 << pin);
        }
        else
        {
            DDRB |= (1 << pin);
        }
    }
    else if (pin < 20)
    {
        pin -= 14;
        if (mode == INPUT)
        {
            DDRC &= ~(1 << pin);
        }
        else
        {
            DDRC |= (1 << pin);
        }
    }
}

void PinUtils::digitalWritePin(uint8_t pin, uint8_t value)
{
    if (pin < 8)
    {
        if (value == LOW)
        {
            PORTD &= ~(1 << pin);
        }
        else
        {
            PORTD |= (1 << pin);
        }
    }
    else if (pin < 14)
    {
        pin -= 8;
        if (value == LOW)
        {
            PORTB &= ~(1 << pin);
        }
        else
        {
            PORTB |= (1 << pin);
        }
    }
    else if (pin < 20)
    {
        pin -= 14;
        if (value == LOW)
        {
            PORTC &= ~(1 << pin);
        }
        else
        {
            PORTC |= (1 << pin);
        }
    }
}

int PinUtils::digitalReadPin(uint8_t pin)
{
    if (pin < 8)
    {
        return (PIND & (1 << pin)) != 0;
    }
    else if (pin < 14)
    {
        pin -= 8;
        return (PINB & (1 << pin)) != 0;
    }
    else if (pin < 20)
    {
        pin -= 14;
        return (PINC & (1 << pin)) != 0;
    }
    return 0;
}