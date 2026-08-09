#pragma once


typedef enum {
    ASCII_NULL        = 0x00,  /* Null terminator (end of the string) */
    ASCII_NEWLINE     = 0x0A,  /* Line Feed (LF) (\n) */
    ASCII_CARRIAGE    = 0x0D,  /* Carriage Return (CR) (\r) */
    ASCII_TAB         = 0x09,  /* Horizontal Tab (\t) */
    ASCII_BACKSPACE   = 0x08,  /* Backspace */
    ASCII_BELL        = 0x07,  /* Bell (sound signal) */
    ASCII_ESCAPE      = 0x1B,  /* Escape (ESC) */

    ASCII_SPACE       = 0x20,  /* ' ' First printable Symbol */
    ASCII_0           = 0x30,  /* '0' Symbol */
    ASCII_9           = 0x39,  /* '9' Symbol */

    ASCII_A           = 0x41,  /* 'A' Symbol */
    ASCII_Z           = 0x5A,  /* 'Z' Symbol */

    ASCII_a           = 0x61,  /* 'a' Symbol */
    ASCII_z           = 0x7A,  /* 'a' Symbol */
    ASCII_TILDA       = 0x7E,  /* '~' Last printable Symbol */
    ASCII_DEL         = 0x7F   /* Delete */
} ASCII_t; // TODO: should be uint8_t

static inline bool isPrintable(char chr) {
    return (
        (chr >= ASCII_SPACE) &&
        (chr <= ASCII_TILDA)
    );
}
static inline bool isLineEnd(char chr) {
        return (
        (chr == ASCII_NEWLINE) ||
        (chr == ASCII_CARRIAGE)
    );
}

#define STAB    "  "

#define ESC     "\x1B""["

#define CLEAR_SCREEN        ESC "2J"
#define CLEAR_LINE          ESC "2K"

#define CURSOR_HOME         ESC "H"
#define CURSOR_UP(n)        ESC #n "A"
#define CURSOR_DOWN(n)      ESC #n "B"
#define CURSOR_FORWARD(n)   ESC #n "C"
#define CURSOR_BACK(n)      ESC #n "D"
#define CURSOR_SET(x,y)     ESC #x ";" #y "H"
#define CURSOR_SHOW         ESC "?25h"
#define CURSOR_HIDE         ESC "?25l"

#define MOUSE_ON            ESC "?1003h"
#define MOUSE_OFF           ESC "?1003l"

#define SOUND_BEEP          "\a"

// text features
#define _TA(n)          ESC #n "m"
#define TEXT_RESET      _TA(0)
#define TEXT_BOLD       _TA(1)
#define TEXT_UNDERLINE  _TA(4)
#define TEXT_BLINK      _TA(5)
#define TEXT_REVERSE    _TA(7)

#define TEXT_BLACK   _TA(30)
#define TEXT_RED     _TA(31)
#define TEXT_GREEN   _TA(32)
#define TEXT_YELLOW  _TA(33)
#define TEXT_BLUE    _TA(34)
#define TEXT_MAGENTA _TA(35)
#define TEXT_CYAN    _TA(36)
#define TEXT_WHITE   _TA(37)

#define BG_BLACK   _TA(40)
#define BG_RED     _TA(41)
#define BG_GREEN   _TA(42)
#define BG_YELLOW  _TA(43)
#define BG_BLUE    _TA(44)
#define BG_MAGENTA _TA(45)
#define BG_CYAN    _TA(46)
#define BG_WHITE   _TA(47)

#define TEXT_BRIGHT_BLACK   _TA(90)
#define TEXT_BRIGHT_RED     _TA(91)
#define TEXT_BRIGHT_GREEN   _TA(92)
#define TEXT_BRIGHT_YELLOW  _TA(93)
#define TEXT_BRIGHT_BLUE    _TA(94)
#define TEXT_BRIGHT_MAGENTA _TA(95)
#define TEXT_BRIGHT_CYAN    _TA(96)
#define TEXT_BRIGHT_WHITE   _TA(97)

#define BG_BRIGHT_BLACK   _TA(100)
#define BG_BRIGHT_RED     _TA(101)
#define BG_BRIGHT_GREEN   _TA(102)
#define BG_BRIGHT_YELLOW  _TA(103)
#define BG_BRIGHT_BLUE    _TA(104)
#define BG_BRIGHT_MAGENTA _TA(105)
#define BG_BRIGHT_CYAN    _TA(106)
#define BG_BRIGHT_WHITE   _TA(107)


#ifndef DEBUG
#   warning DEBUG not defined!!!
#   define DEBUG 2
#else
    // #warning DEBUG  defined V
#endif


#if DEBUG > 0
#   include <stdio.h>
#   define LLOG(...)    printf(__VA_ARGS__);                                /* Lazy log */
#   define LOG(...)     do {LLOG(__VA_ARGS__); fflush(stdout);} while (0)   /* Strong log */
#else
#   warning DEBUG not defined!!!
#   define LLOG(...)
#   define LOG(...)
#endif
#   define INFO(...)       LLOG(TEXT_GREEN __VA_ARGS__); LOG(TEXT_RESET "\n")
#   define WARNING(...)    LLOG(TEXT_YELLOW __VA_ARGS__); LOG(TEXT_RESET "\n")
#   define ERROR(...)      LLOG(TEXT_RED __VA_ARGS__);    LOG(TEXT_RESET "\n")
#   define HALT(...)       ERROR(__VA_ARGS__);  while (1) {}
