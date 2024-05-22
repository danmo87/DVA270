#ifndef header17.h
#define header17.h
#include <nrfx.h>
#include <nrf5340_application.h>
#include <nrfx_config.h>
#include <nrf.h>
#include <nrfx_uarte.h>
#include <stdlib.h>
#include <nrf_gpio.h>
#include <nrfx_systick.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#define PIN_TXD 20
#define PIN_RXD 22



void start_game_grupp17(void);
void uarte_write( uint8_t* data, uint8_t length );

void uarte_handler(nrfx_uarte_event_t const *p_event, void *p_context);

void update_matrix(void);


void show_matrix(void);

void show_and_update(void);


#endif
