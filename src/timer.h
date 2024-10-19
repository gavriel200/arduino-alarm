#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

class Timer
{
private:
    static volatile uint32_t milliseconds;

public:
    static void init();
    static uint32_t getMillis();
    static void incrementMillis();
};

#endif
