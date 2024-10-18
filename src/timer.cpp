#include "Timer.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

volatile uint32_t timer0_millis = 0;
volatile uint8_t timer0_fract = 0;

ISR(TIMER0_COMPA_vect)
{
    timer0_millis++;
    timer0_fract += 3; // 3/1024 = 0.00292969 (close to 3us)
    if (timer0_fract >= 125)
    {
        timer0_fract -= 125;
        timer0_millis++;
    }
}

Timer::Timer()
{
    // Initialize Timer0 for 1ms interrupt
    TCCR0A = (1 << WGM01);              // CTC mode
    TCCR0B = (1 << CS01) | (1 << CS00); // Prescaler 64
    OCR0A = 249;                        // 16MHz / (64 * (249 + 1)) = 1kHz
    TIMSK0 |= (1 << OCIE0A);            // Enable Timer0 Compare Match A interrupt
    sei();                              // Enable global interrupts

    reset();
}

void Timer::reset()
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        startTime = timer0_millis;
        startTimeFract = timer0_fract;
    }
}

uint32_t Timer::elapsed()
{
    return getMillis() - startTime;
}

uint32_t Timer::elapsedMicros()
{
    uint32_t m;
    uint8_t f;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        m = timer0_millis;
        f = timer0_fract;
    }
    return ((m - startTime) * 1000 + (f - startTimeFract) * 8);
}

void Timer::delay(uint32_t ms)
{
    uint32_t start = getMillis();
    while (getMillis() - start < ms)
        ;
}

void Timer::delayMicros(uint32_t us)
{
    uint32_t start = getMicros();
    while (getMicros() - start < us)
        ;
}

uint32_t Timer::getMillis()
{
    uint32_t m;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        m = timer0_millis;
    }
    return m;
}

uint32_t Timer::getMicros()
{
    uint32_t m;
    uint8_t f;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        m = timer0_millis;
        f = timer0_fract;
    }
    return (m * 1000 + f * 8);
}