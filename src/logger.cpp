#include "logger.h"

#define BAUD 9600
#define UBRR_VALUE ((F_CPU / 16 / BAUD) - 1)

void Logger::init()
{
    UBRR0H = (uint8_t)(UBRR_VALUE >> 8);
    UBRR0L = (uint8_t)UBRR_VALUE;
    UCSR0B = (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void Logger::log(const char *message)
{
    while (*message)
    {
        while (!(UCSR0A & (1 << UDRE0)))
            ;
        UDR0 = *message++;
    }
}

void Logger::log(int value)
{
    char buffer[20];
    snprintf(buffer, sizeof(buffer), "%d", value);
    log(buffer);
}

void Logger::log(unsigned int value)
{
    char buffer[20];
    snprintf(buffer, sizeof(buffer), "%u", value);
    log(buffer);
}

void Logger::log(long value)
{
    char buffer[20];
    snprintf(buffer, sizeof(buffer), "%ld", value);
    log(buffer);
}

void Logger::log(unsigned long value)
{
    char buffer[20];
    snprintf(buffer, sizeof(buffer), "%lu", value);
    log(buffer);
}

void Logger::log(float value, uint8_t precision)
{
    char buffer[20];
    dtostrf(value, 0, precision, buffer);
    log(buffer);
}
