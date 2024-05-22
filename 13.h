#ifndef LIST_H
#define LIST_H

#include <nrfx.h>
#include <nrfx_glue.h>
#include <nrfx_uarte.h>
#include <nrf_gpio.h>
#include <stdlib.h>
#include <stdio.h>
#include <nrfx_rtc.h>

/*******************************************************/
/* DVA270 - Interface för dubbellänkad lista           */
/* Anpassad efter interface från DVA244                */
/* av Stefan Bygde och Caroline Uppsäll                */
/* Vid fel eller frågor, kontakta jonas.larsson@mdu.se */
/*******************************************************/
static int x_player = 0;
static int y_player = 0;
static int counter = 0;

static nrfx_uarte_t instance = NRFX_UARTE_INSTANCE(0);
static uint8_t uarte_buffer;
static const nrfx_rtc_t rtc_instance = NRFX_RTC_INSTANCE(0);

#define ARR_LEN 20
#define SEC (32768)
#define PIN_TXD (20)
#define PIN_RXD (22)
#define LED1 28
#define LED2 29
#define LED3 30
#define LED4 31

#define CLEAR_SCREEN "\033c"
void read_string(char arr[]);

int read_int(char arr[]);

void uarte_write(uint8_t* data, uint8_t length);

void send_int(int x, char arr[]);

void uarte_handler(nrfx_uarte_event_t const *p_event, void *p_context);

void start_game_grupp13();



#endif