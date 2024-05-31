#ifndef GRUPP11_H
#define GRUPP11_H

#include <nrfx.h>
#include <nrf5340_application.h>
#include <nrfx_config.h>
#include <nrf.h>
#include <nrfx_uarte.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define PIN_TXD 20
#define PIN_RXD 22

#define CLEAR_SCREEN "\033c"

static nrfx_uarte_t instance = NRFX_UARTE_INSTANCE(0);
static nrfx_uarte_t rtc_instance = NRFX_UARTE_INSTANCE(0);


void uarte_write(uint8_t* data, uint8_t length);
void print_game_status(int user_score, int computer_score);
char uarte_read(void);
void start_game_grupp11(char user_choice, int *user_score, int *computer_score);
void init_systick_and_uart(void);

extern nrfx_uarte_t instance; // extern deklaration av variabeln

#endif // GRUPP11_H
