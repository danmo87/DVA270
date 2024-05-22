#include <nrfx.h>
#include <nrf5340_application.h>
#include <nrfx_config.h>
#include <nrf.h>
#include <nrf_gpio.h>
#include <nrfx_systick.h>
#include <nrfx_uarte.h>
#include <nrfx_rtc.h>
#include <nrfx_glue.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define CLEAR_SCREEN "\033c"

void start_game_grupp3 (void);

void send_int(int* test_int);

void uarte_handler(nrfx_uarte_event_t const *p_event, void *p_context);