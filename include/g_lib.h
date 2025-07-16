#pragma once

#include "utils.h"
#include <cstdio>
#include <cstdlib>
#include <stdio.h>
#include <sys/stat.h>

#ifdef _WIN32
#define DEBUG_BREAK() __debugbreak()

#elif __linux__
#define DEBUG_BREAK() __builtin_debugtrap()

#elif __APPLE__
#define DEBUG_BREAK() __builtin_trap()

#endif

#define BS_TRACE(msg, ...) _log("TRACE: ", msg, COLOR_GREEN, ##__VA_ARGS__);
#define BS_WARNING(msg, ...) \
    _log("WARNING: ", msg, COLOR_YELLOW, ##__VA_ARGS__);
#define BS_ERROR(msg, ...) _log("ERROR: ", msg, COLOR_RED, ##__VA_ARGS__);

#define BS_ASSERT(x, msg, ...)            \
    {                                     \
        if(!x) {                          \
            BS_ERROR(msg, ##__VA_ARGS__); \
            DEBUG_BREAK();                \
        }                                 \
    }

enum TextColor {
    COLOR_BLACK = 0,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_YELLOW,
    COLOR_BLUE,
    COLOR_MAGENTA,
    COLOR_CYAN,
    COLOR_WHITE,
    COlOR_COUNT
};

template <typename... Args>
void _log(char* prefix, char* message, TextColor textColor, Args... args)
{
    static const char* TextColorTable[COlOR_COUNT] = {
        "\x1b[30m", // COLOR_BLACK
        "\x1b[31m", // COLOR RED
        "\x1b[32m", // COLOR GREEN
        "\x1b[33m", // COLOR YELLOW
        "\x1b[34m", // COLOR BLUE
        "\x1b[35m", // COLOR MAGENTA
        "\x1b[36m", // COLOR CYAN
        "\x1b[37m"  // COLOR WHITE
    };

    char formatBuffer[BASIC_BUFFER_SIZE] = {};
    // TODO: Look into snprintf, lsp is complaining that sprintf is deprecated
    sprintf(formatBuffer, "%s %s %s \033[0m", TextColorTable[textColor], prefix, message);

    char textBuffer[BASIC_BUFFER_SIZE] = {};
    sprintf(textBuffer, formatBuffer, args...);

    puts(textBuffer);
}
