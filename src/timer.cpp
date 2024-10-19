#include "timer.h"

volatile uint32_t Timer::milliseconds = 0;

ISR(TIMER1_COMPA_vect)
{
    Timer::incrementMillis();
}

void Timer::init()
{
    // Set up Timer1 to interrupt every 1 ms
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
    OCR1A = 15999; // for 16MHz AVR
    TCCR1B |= (1 << WGM12);
    TCCR1B |= (1 << CS10);
    TIMSK1 |= (1 << OCIE1A);
    sei();
}

uint32_t Timer::getMillis()
{
    uint32_t ms;
    cli();
    ms = milliseconds;
    sei();
    return ms;
}

void Timer::incrementMillis()
{
    milliseconds++;
}