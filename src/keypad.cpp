#include "Keypad.h"

Keypad::Keypad(uint8_t row1, uint8_t row2, uint8_t row3, uint8_t row4,
               uint8_t col1, uint8_t col2, uint8_t col3, uint8_t col4)
{
    // Initialize row and column pin arrays
    rowPins[0] = row1;
    rowPins[1] = row2;
    rowPins[2] = row3;
    rowPins[3] = row4;
    colPins[0] = col1;
    colPins[1] = col2;
    colPins[2] = col3;
    colPins[3] = col4;

    // Define the key layout (can be customized as per the actual keypad layout)
    keys[0][0] = '1';
    keys[0][1] = '2';
    keys[0][2] = '3';
    keys[0][3] = '4';
    keys[1][0] = '5';
    keys[1][1] = '6';
    keys[1][2] = '7';
    keys[1][3] = '8';
    keys[2][0] = '9';
    keys[2][1] = 'A';
    keys[2][2] = 'B';
    keys[2][3] = 'C';
    keys[3][0] = 'D';
    keys[3][1] = 'E';
    keys[3][2] = 'F';
    keys[3][3] = 'G';

    // Set row pins as outputs
    for (uint8_t i = 0; i < 4; i++)
    {
        PinUtils::pinSet(rowPins[i], OUTPUT);
    }

    // Set column pins as inputs with pull-up resistors
    for (uint8_t i = 0; i < 4; i++)
    {
        PinUtils::pinSet(colPins[i], INPUT);
        PinUtils::pinWrite(colPins[i], HIGH); // Enable pull-up
    }
}

void Keypad::setRowLow(uint8_t row)
{
    PinUtils::pinWrite(rowPins[row], LOW);
}

void Keypad::setRowHigh(uint8_t row)
{
    PinUtils::pinWrite(rowPins[row], HIGH);
}

bool Keypad::isColumnPressed(uint8_t col)
{
    return (PinUtils::pinRead(colPins[col]) == LOW); // Check if the column is pressed
}

char Keypad::getKey()
{
    for (uint8_t r = 0; r < 4; r++)
    {
        // Set the current row to low
        setRowLow(r);

        for (uint8_t c = 0; c < 4; c++)
        {
            if (isColumnPressed(c))
            {
                _delay_ms(300); // Debounce
                setRowHigh(r);  // Set the row back to high before returning

                return keys[r][c]; // Return the pressed key
            }
        }

        // Set the row back to high
        setRowHigh(r);
    }

    return '\0'; // Return null character if no key is pressed
}
