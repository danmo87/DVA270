#ifndef TERM
#define TERM

#define PIN_TXD 20
#define PIN_RXD 22

#define CLEAR_SCREEN "\033c"
#define FG_COLOR "\033[38;2;%d;%d;%dm"
#define BG_COLOR "\033[48;2;%d;%d;%dm"
#define RESET_GFX "\033[0m"
#define SET_POS "\033[%d;%dH"
#define CURSOR_HIDE "\033[?25l"
#define CURSOR_SHOW "\033[?25h"

#include <nrfx_uarte.h>

// Group 16s Terminal library

//initialize the terminal and uart, in blocking mode
void init_term();

//initialize the terminal and uart, in non blocking mode
void init_term_non_blocking(void(*eventHandler)(nrfx_uarte_event_t const *, void *));

//print a null-terminated string to the terminal
void term_print(char* str);

//input a certain amont of bits
void term_input_raw(char *ptr, int size);

//print an int to the terminal
void term_print_int(int num);

// input a string, either until CRLF or maxIndex reached
void term_input_string(uint8_t * arr, uint8_t maxIndex);

// input maxIndex number of bits
void term_input_char_arr(uint8_t * arr, uint8_t maxIndex);

//set the color of the terminal text
void term_color(uint8_t r, uint8_t g, uint8_t b);

//set the color of the text background
void term_bg_color(uint8_t r, uint8_t g, uint8_t b);

//set the position of the cursor
void term_pos(int x, int y);

//print string at position
void term_draw_at(int x, int y, char* chars);

//reset the terminal, resets color and other graphics
void term_reset();

//clear the terminal, blank screen
void term_clear();



#endif