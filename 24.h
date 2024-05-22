#ifndef HFILEN_H
#define HFILEN_H

#include <nrfx.h>
#include <nrf5340_application.h>
#include <nrfx_config.h>
#include <nrf.h>
#include <nrfx_uarte.h>
#include <nrf_gpio.h>
#include <nrfx_rtc.h>
#include <nrfx_systick.h>
#include <nrfx_glue.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define PIN_TXD (20)
#define PIN_RXD (22)

#define CLEAR_SCREEN "\033c"


//UARTE
static const nrfx_uarte_t instance = NRFX_UARTE_INSTANCE(0);
static const nrfx_uarte_config_t config = NRFX_UARTE_DEFAULT_CONFIG(PIN_TXD, PIN_RXD);
static uint8_t uarte_buffer;

void uarte_write(uint8_t* data, uint8_t length);

void uarte_handler(nrfx_uarte_event_t const *p_event, void *p_context);

void start_game_grupp24(void);

void decide_losing_index(int* losing_index);

void print_instructions(void);

void print_grid(uint8_t gamearea[2][16]);

void lose_check(int losing_index);
#endif