#include "RGBLed.h"

// Constructor to initialize the pins
RGBLed::RGBLed(uint8_t redPin, uint8_t greenPin, uint8_t bluePin)
    : redPin(redPin), greenPin(greenPin), bluePin(bluePin)
{
    // Set color pins as output
    PinUtils::pinSet(redPin, OUTPUT);
    PinUtils::pinSet(greenPin, OUTPUT);
    PinUtils::pinSet(bluePin, OUTPUT);
}

// Set the RGB LED to a specific color
void RGBLed::setColor(uint8_t red, uint8_t green, uint8_t blue)
{
    PinUtils::pinWrite(redPin, red ? HIGH : LOW);
    PinUtils::pinWrite(greenPin, green ? HIGH : LOW);
    PinUtils::pinWrite(bluePin, blue ? HIGH : LOW);
}

// Turn off the LED (all colors off)
void RGBLed::turnOff()
{
    setColor(0, 0, 0); // Set all colors to 0 (off)
}
