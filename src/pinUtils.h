#ifndef PINUTILS_H
#define PINUTILS_H

#include <avr/io.h>

#define A0 14
#define A1 15
#define A2 16
#define A3 17
#define A4 18
#define A5 19

#define INPUT 0
#define OUTPUT 1

#define LOW 0
#define HIGH 1

class PinUtils
{
private:
    static volatile uint8_t *getPortRegister(uint8_t pin)
    {
        if (pin < 8) // Pins 0-7: Port D
            return &PORTD;
        if (pin < 14) // Pins 8-13: Port B
            return &PORTB;
        // Analog Pins A0-A5: Port C (Pins 14-19)
        return &PORTC;
    }

    static volatile uint8_t *getDDRRegister(uint8_t pin)
    {
        if (pin < 8)
            return &DDRD;
        if (pin < 14)
            return &DDRB;
        return &DDRC;
    }

    static volatile uint8_t *getPINRegister(uint8_t pin)
    {
        if (pin < 8)
            return &PIND;
        if (pin < 14)
            return &PINB;
        return &PINC;
    }

    static volatile uint8_t getBitMask(uint8_t pin)
    {
        if (pin < 8)
            return (1 << pin);
        if (pin < 14)
            return (1 << (pin - 8));
        return (1 << (pin - 14));
    }

public:
    static void pinSet(uint8_t pin, uint8_t mode)
    {
        volatile uint8_t *ddr = getDDRRegister(pin);
        uint8_t mask = getBitMask(pin);
        if (mode == OUTPUT)
            *ddr |= mask; // Set as output
        else
            *ddr &= ~mask; // Set as input
    }

    static void pinWrite(uint8_t pin, uint8_t state)
    {
        volatile uint8_t *port = getPortRegister(pin);
        uint8_t mask = getBitMask(pin);
        if (state == HIGH)
            *port |= mask; // Set pin high
        else
            *port &= ~mask; // Set pin low
    }

    static uint8_t pinRead(uint8_t pin)
    {
        volatile uint8_t *pinReg = getPINRegister(pin);
        uint8_t mask = getBitMask(pin);
        return (*pinReg & mask) ? HIGH : LOW; // Read pin state
    }
};

#endif