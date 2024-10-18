#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

class Timer
{
public:
    Timer();
    void reset();
    uint32_t elapsed();
    uint32_t elapsedMicros();
    void delay(uint32_t ms);
    void delayMicros(uint32_t us);
    uint32_t getMillis();
    uint32_t getMicros();

private:
    uint32_t startTime;
    uint8_t startTimeFract;
};

#endif // TIMER_H