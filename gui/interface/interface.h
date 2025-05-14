//
// Created by Huwensong on 2025/5/14.
//

#ifndef SIMULATOR_INTERFACE_H
#define SIMULATOR_INTERFACE_H

#include <stdio.h>

int log_printf(const char *fmt, ...);

#define LOG_FORMAT(letter, format)

#define log_d(format, ...) log_printf(LOG_FORMAT(D, format), ##__VA_ARGS__)


void log_d(const char *format,...);
{
    printf(,)
}

#endif //SIMULATOR_INTERFACE_H
