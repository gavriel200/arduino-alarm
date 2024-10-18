#ifndef LOGGER_H
#define LOGGER_H

#include <avr/io.h>
#include <stdlib.h>

class Logger
{
public:
    Logger(uint16_t baud_rate);
    Logger(); //  default baud rate (9600)

    void log(const char *str);
    void log(char str);
    void log(uint16_t str);

private:
    void uartTransmit(char data);
    void setupBaudRate(uint16_t baud_rate);
};

#endif
