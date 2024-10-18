#ifndef KEYPAD_H
#define KEYPAD_H

#include "pinUtils.h"

#include <avr/io.h>
#include <util/delay.h>

class Keypad
{
private:
    uint8_t rowPins[4]; // Row pins for the keypad
    uint8_t colPins[4]; // Column pins for the keypad
    char keys[4][4];    // 4x4 matrix for the keys

    // Helper methods to set rows/columns
    void setRowLow(uint8_t row);
    void setRowHigh(uint8_t row);
    bool isColumnPressed(uint8_t col);

public:
    // Constructor
    Keypad(uint8_t row1, uint8_t row2, uint8_t row3, uint8_t row4,
           uint8_t col1, uint8_t col2, uint8_t col3, uint8_t col4);

    // Initialize keypad
    void begin();

    // Get the currently pressed key
    char getKey();
};

#endif // KEYPAD_H
