#ifndef GRUPP_30_H
#define GRUPP_30_H

#include <nrfx.h>
#include <nrf5340_application.h>
#include <nrf.h>
#include <nrfx_config.h>
#include <nrfx_uarte.h>
#include <nrfx_systick.h>
#include <nrf_gpio.h>
#include <stdlib.h>
#include <math.h>
#include <nrfx_glue.h>
#include <time.h>
#include <stdio.h>

#define WIDTH 10
#define HEIGHT 10
#define PIN_TXD 20
#define PIN_RXD 22

extern volatile X;
extern volatile Y;

extern char rx_buffer;
extern int random_x;
extern int random_y;
extern int bad_random_x;
extern int bad_random_y;
extern int points;
extern int new_input;
extern char clear_screen[];


void Move_User(void);
void print(void);
void uarte_handler(nrfx_uarte_event_t const *p_event, void *p_context);
int Make_Random(void);
void Uppdate_Random(void);
void start_game_grupp_30(void);
#endif