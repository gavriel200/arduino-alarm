#ifndef LOGGER_H
#define LOGGER_H

#include <stdint.h>
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>

class Logger
{
public:
    static void init();
    static void log(const char *message);
    static void log(char value);
    static void log(int value);
    static void log(unsigned int value);
    static void log(long value);
    static void log(unsigned long value);
    static void log(float value, uint8_t precision = 2);
};

#endif
