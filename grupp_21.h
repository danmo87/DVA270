#ifndef GRUPP_21_H
#define GRUPP_21_H
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

#define SIZE 25
#define CLEAR_SCREEN "\033c"
#define PIN_TXD 20
#define PIN_RXD 22

void uarte_handler(nrfx_uarte_event_t const *p_event, void *p_context);

void print_array(int size, int array[]);

void send_int(int i);

#endif