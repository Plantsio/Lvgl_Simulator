//
// Created by Huwensong on 2025/5/14.
//

#ifndef SIMULATOR_LOG_H
#define SIMULATOR_LOG_H

#define LOG_LEVEL_ERROR       0
#define LOG_LEVEL_WARNING     1
#define LOG_LEVEL_INFO        2
#define LOG_LEVEL_DEBUG       3
#define LOG_LEVEL_VERBOSE     4

#define LOG_LEVEL             LOG_LEVEL_DEBUG

#define LOG_COLOR_BLACK   "30"
#define LOG_COLOR_RED     "31" //ERROR
#define LOG_COLOR_GREEN   "32" //INFO
#define LOG_COLOR_YELLOW  "33" //WARNING
#define LOG_COLOR_BLUE    "34"
#define LOG_COLOR_MAGENTA "35"
#define LOG_COLOR_CYAN    "36" //DEBUG
#define LOG_COLOR_GRAY    "37" //VERBOSE
#define LOG_COLOR_WHITE   "38"

#define LOG_COLOR(COLOR)  "\033[0;" COLOR "m"
#define LOG_BOLD(COLOR)   "\033[1;" COLOR "m"
#define LOG_RESET_COLOR   "\033[0m"

#define LOG_COLOR_E       LOG_COLOR(LOG_COLOR_RED)
#define LOG_COLOR_W       LOG_COLOR(LOG_COLOR_YELLOW)
#define LOG_COLOR_I       LOG_COLOR(LOG_COLOR_GREEN)
#define LOG_COLOR_D       LOG_COLOR(LOG_COLOR_CYAN)
#define LOG_COLOR_V       LOG_COLOR(LOG_COLOR_GRAY)

#include <cstdio>

#define LOG_FORMAT(letter, format)  LOG_COLOR_ ## letter "[" #letter "][%s:%u] %s(): " format LOG_RESET_COLOR "\n",__FILE__, __LINE__, __FUNCTION__

#if LOG_LEVEL >= LOG_LEVEL_ERROR
#define log_e(format, ...) printf(LOG_FORMAT(E, format), ##__VA_ARGS__)
#else
#define log_e(format, ...) do {} while(0)
#endif

#if LOG_LEVEL >= LOG_LEVEL_WARNING
#define log_w(format, ...) printf(LOG_FORMAT(W, format), ##__VA_ARGS__)
#else
#define log_w(format, ...) do {} while(0)
#endif

#if LOG_LEVEL >= LOG_LEVEL_INFO
#define log_i(format, ...) printf(LOG_FORMAT(I, format), ##__VA_ARGS__)
#else
#define log_i(format, ...) do {} while(0)
#endif

#if LOG_LEVEL >= LOG_LEVEL_DEBUG
#define log_d(format, ...) printf(LOG_FORMAT(D, format), ##__VA_ARGS__)
#else
#define log_d(format, ...) do {} while(0)
#endif

#if LOG_LEVEL >= LOG_LEVEL_VERBOSE
#define log_v(format, ...) printf(LOG_FORMAT(V, format), ##__VA_ARGS__)
#else
#define log_v(format, ...) do {} while(0)
#endif

#endif //SIMULATOR_LOG_H
