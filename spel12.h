#include <nrfx.h>
#include <nrfx_uarte.h>
#include <nrfx_systick.h>
#include <stdio.h>
#include <string.h>
#include <nrfx_glue.h>
#include "time.h"

#ifndef GRID_H
#define GRID_H



void move_up(int Y);
void move_down(int Y);
int move_left(int X);
int move_right(int X);
void stop_running(void);

int is_running(void);
void print_grid(int Y, int X);

#define CLEAR_SCREEN "\033c"





void uarte_handler(nrfx_uarte_event_t const * p_event, void * p_context);
void send_message(const char * message);





#endif // GRID_H