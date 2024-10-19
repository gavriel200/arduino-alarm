#ifndef PIN_UTILS_H
#define PIN_UTILS_H

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
public:
    static void setPinMode(uint8_t pin, uint8_t mode);
    static void digitalWritePin(uint8_t pin, uint8_t value);
    static int digitalReadPin(uint8_t pin);
};

#endif