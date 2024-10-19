#ifndef KEYPAD_H
#define KEYPAD_H

#include <util/delay.h>

#include "pinUtils.h"

class Keypad
{
private:
    uint8_t rowPins[4];
    uint8_t colPins[4];
    char keys[4][4] = {
        {'1', '2', '3', '4'},
        {'5', '6', '7', '8'},
        {'9', 'A', 'B', 'C'},
        {'D', 'E', 'F', 'G'}};

public:
    Keypad(uint8_t r1, uint8_t r2, uint8_t r3, uint8_t r4,
           uint8_t c1, uint8_t c2, uint8_t c3, uint8_t c4);
    char getKey();
};

#endif
