#include <nrfx.h>
#include <nrf5340_application.h>
#include <nrfx_config.h>
#include <nrf.h>
#include <nrfx_uarte.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "G16_term.h"

static nrfx_uarte_t instance = NRFX_UARTE_INSTANCE(0);
static uint8_t isInit = false;

void init_term()
{
    if(isInit) return;
    isInit = true;

    const nrfx_uarte_config_t config = NRFX_UARTE_DEFAULT_CONFIG(PIN_TXD, PIN_RXD);

    nrfx_err_t errr = nrfx_uarte_init(&instance, &config, NULL);
}

void init_term_non_blocking(void(*eventHandler)(nrfx_uarte_event_t const *, void *))
{
    if(isInit) return;
    isInit = true;

    NVIC_ClearPendingIRQ(NRFX_IRQ_NUMBER_GET(NRF_UARTE_INST_GET(0)));
    NVIC_EnableIRQ(NRFX_IRQ_NUMBER_GET(NRF_UARTE_INST_GET(0)));

    const nrfx_uarte_config_t config = NRFX_UARTE_DEFAULT_CONFIG(PIN_TXD, PIN_RXD);

    nrfx_err_t errr = nrfx_uarte_init(&instance, &config, eventHandler);
}

void uart_write(uint8_t * data, uint8_t length)
{
    nrfx_uarte_tx(&instance, data, length, 0);

    while(nrfx_uarte_tx_in_progress(&instance)){};
}

void term_print_raw(uint8_t * data, uint8_t length)
{
    uart_write(data, length);
}

void term_print(char* str)
{
    uint8_t len = strlen(str);
    
    uint8_t temp[len];

    strcpy(temp, str);

    uart_write(temp, sizeof(temp));
}

void term_print_int(int num)
{
    char arr[20];

    itoa(num, arr, 10);

    term_print(arr);
}

void term_input_raw(char *ptr, int size)
{
    nrfx_uarte_rx(&instance, ptr, size);
}

void term_input_string(uint8_t * arr, uint8_t maxIndex)
{
    uint8_t integer;
    uint8_t i = 0;

    while(integer != '\r' && i < maxIndex-1)
    {
        nrfx_uarte_rx(&instance, &integer, sizeof(integer));
        uart_write(&integer, sizeof(integer));
        arr[i] = integer;
        i++;
    }
    
    arr[i] = '\0';
}

void term_input_char_arr(uint8_t * arr, uint8_t maxIndex)
{
    uint8_t integer;
    uint8_t i = 0;

    while(integer < maxIndex)
    {
        nrfx_uarte_rx(&instance, &integer, sizeof(integer));
        arr[i] = integer;
        i++;
    }
}

void term_color(uint8_t r, uint8_t g, uint8_t b)
{
    char str[30];

    sprintf(str, FG_COLOR, r, g, b);

    term_print(str);
}

void term_bg_color(uint8_t r, uint8_t g, uint8_t b)
{
    char str[30];

    sprintf(str, BG_COLOR, r, g, b);

    term_print(str);
}

void term_pos(int x, int y)
{
    char str[30];

    sprintf(str, SET_POS, y, x);

    term_print(str);
}

void term_draw_at(int x, int y, char* chars)
{
    term_pos(x, y);
    term_print(chars);
}

void term_clear()
{
    term_print(CLEAR_SCREEN);
}

void term_reset()
{
    term_print(RESET_GFX);
}