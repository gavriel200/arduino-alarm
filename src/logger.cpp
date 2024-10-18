#include "Logger.h"

Logger::Logger(uint16_t baud_rate)
{
    setupBaudRate(baud_rate);
}

Logger::Logger()
{
    setupBaudRate(9600);
}

void Logger::setupBaudRate(uint16_t baud_rate)
{
    uint16_t ubrr_value = (F_CPU / 16 / baud_rate) - 1;

    // Set high and low bytes of UBRR
    UBRR0H = (ubrr_value >> 8);
    UBRR0L = ubrr_value;

    UCSR0B = (1 << TXEN0); // Enable transmitter

    // Set frame format: 8 data bits, 1 stop bit, no parity
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void Logger::log(const char *str)
{
    while (*str)
    {
        this->uartTransmit(*str);
        str++;
    }
}

void Logger::log(char str)
{
    this->uartTransmit(str);
}

void Logger::log(uint16_t data)
{
    char buffer[6];
    itoa(data, buffer, 10);
    this->log(buffer);
}

void Logger::uartTransmit(char data)
{
    // Wait for transmit buffer to be empty
    while (!(UCSR0A & (1 << UDRE0)))
    {
    }

    // Put data into the buffer, this sends the data
    UDR0 = data;
}
