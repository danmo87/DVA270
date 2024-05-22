#ifndef GRUPP8spel
#define GRUPP8spel

#include <nrfx.h>
#include <nrf5340_application.h>
#include <nrfx_config.h>
#include <nrf.h>
#include <nrfx_uarte.h>
#include <nrf_gpio.h>
#include <nrfx_systick.h>
#include <nrfx_glue.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <soc/nrfx_irqs.h>

#define PIN_TXD 20
#define PIN_RXD 22

#define CLEAR_SCREEN "\033c"

extern nrfx_uarte_t instance;
extern uint8_t uarte_buffer;

static int newData = 0;

void start_game_grupp8();

void uarte_handler(nrfx_uarte_event_t const *p_event, void *p_context);

#endif