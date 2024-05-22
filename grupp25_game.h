#ifndef GRUPP25_GAME_H
#define GRUPP25_GAME_H
#include <nrfx.h>
#include <nrf5340_application.h>
#include <nrfx_config.h>
#include <nrf.h>
#include <nrfx_uarte.h>
#include <nrf_gpio.h>
#include <nrfx_systick.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include <math.h>
#include <nrfx_glue.h>

#define SIZE 10
#define CLEAR_SCREEN "\033c"
#define PIN_TXD 20
#define PIN_RXD 22

void uarte_write(uint8_t* data, uint8_t length);

void uarte_handler(nrfx_uarte_event_t const *p_event, void *p_context);

void print_array(int size, int matrix[][size]);

void send_int(int i);

// Initiate all things to run the code
void init_all();

// Update the game
void update_game();

// Runs the game
void start_game_grupp25();

#endif