#include "Ultrasonic.h"
#include "PinUtils.h"
#include <util/delay.h>

Ultrasonic::Ultrasonic(uint8_t triggerPin, uint8_t echoPin, Timer &timer)
    : triggerPin(triggerPin), echoPin(echoPin), timer(timer)
{
    PinUtils::pinSet(triggerPin, OUTPUT);
    PinUtils::pinSet(echoPin, INPUT);
}

uint16_t Ultrasonic::getDistance()
{
    // Trigger pulse
    PinUtils::pinWrite(triggerPin, LOW);
    _delay_us(2); // Wait for 2 microseconds
    PinUtils::pinWrite(triggerPin, HIGH);
    _delay_us(10); // Send 10 microsecond pulse
    PinUtils::pinWrite(triggerPin, LOW);

    // Wait for echo to start (with timeout)
    uint32_t startTime = timer.getMicros();
    while (PinUtils::pinRead(echoPin) == LOW)
    {
        if (timer.getMicros() - startTime > 30000) // 30ms timeout
        {
            return 0; // Timeout occurred, return 0 or error value
        }
    }

    // Echo started, record the time
    uint32_t echoStartTime = timer.getMicros();

    // Wait for echo to end (with timeout)
    startTime = timer.getMicros();
    while (PinUtils::pinRead(echoPin) == HIGH)
    {
        if (timer.getMicros() - startTime > 30000) // 30ms timeout
        {
            return 0; // Timeout occurred, return 0 or error value
        }
    }

    // Echo ended, calculate duration
    uint32_t echoDuration = timer.getMicros() - echoStartTime;

    // Convert echo duration to distance (in cm)
    // Sound travels at about 343 meters per second, or 29 microseconds per centimeter
    // We divide by 2 because the sound travels to the object and back
    return echoDuration / 29 / 2; // Distance in cm
}
