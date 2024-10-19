#include "keypad.h"

Keypad::Keypad(uint8_t r1, uint8_t r2, uint8_t r3, uint8_t r4,
               uint8_t c1, uint8_t c2, uint8_t c3, uint8_t c4)
{
    rowPins[0] = r1;
    rowPins[1] = r2;
    rowPins[2] = r3;
    rowPins[3] = r4;
    colPins[0] = c1;
    colPins[1] = c2;
    colPins[2] = c3;
    colPins[3] = c4;

    for (int i = 0; i < 4; i++)
    {
        PinUtils::setPinMode(rowPins[i], INPUT);
        PinUtils::setPinMode(colPins[i], OUTPUT);
        PinUtils::digitalWritePin(colPins[i], HIGH);
    }
}

char Keypad::getKey()
{
    for (int c = 0; c < 4; c++)
    {
        PinUtils::digitalWritePin(colPins[c], LOW);
        for (int r = 0; r < 4; r++)
        {
            if (PinUtils::digitalReadPin(rowPins[r]) == LOW)
            {
                _delay_ms(300); // Debounce
                if (PinUtils::digitalReadPin(rowPins[r]) == LOW)
                {
                    PinUtils::digitalWritePin(colPins[c], HIGH);
                    return keys[r][c];
                }
            }
        }
        PinUtils::digitalWritePin(colPins[c], HIGH);
    }
    return 0;
}
