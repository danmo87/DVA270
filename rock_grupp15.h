#ifndef ROCK_GRUPP15_H
#define ROCK_GRUPP15_H

#include <nrfx.h>
#include <nrf5340_application.h>
#include <nrfx_config.h>
#include <nrf.h>
#include <nrfx_uarte.h>
#include <nrf_gpio.h>
#include <nrfx_systick.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nrfx_rtc.h>
#include <assert.h>
#include <nrfx_glue.h>
#include <time.h>

#define CLEAR_SCREEN"\033c"

#define PIN_TXD (20)
#define PIN_RXD (22)

void uarte_handler(nrfx_uarte_event_t const *p_event, void *p_context);

void start_game_grupp15(void);

#endif