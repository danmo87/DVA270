//grind.h
#ifndef matrix_game_7_H
#define matrix_game_7._H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "nrfx.h"
#include "nrf_gpio.h"
#include "nrfx_rtc.h"
#include "nrfx_uarte.h"
#include "nrfx_gpiote.h"
#include <nrfx_config.h>
#include <nrfx_systick.h>
#include <nrf5340_application.h>
#include <nrfx_glue.h>

// defines uarte usb pins
#define PIN_TXD 20
#define PIN_RXD 22

// defines ASCII colours
#define CLEAR_SCREEN "\033c"
#define RESET   "\033[0m"
#define BLACK   "\033[30m"      
#define RED     "\033[31m"      
#define GREEN   "\033[32m"      
#define YELLOW  "\033[33m"      
#define BLUE    "\033[34m"      
#define MAGENTA "\033[35m"      
#define CYAN    "\033[36m"     
#define WHITE   "\033[37m"    
#define BOLD    "\033[1m"
#define UNDERLINE "\033[4m"

// initates uarte and rtc
static nrfx_uarte_t instance = NRFX_UARTE_INSTANCE(0);
static nrfx_rtc_t rtc_instance = NRFX_RTC_INSTANCE(0);

// defines the size of the play area
#define MATRIX_X 10
#define MATRIX_Y 5

// defines the win point location in the play area
#define WIN_X 4
#define WIN_Y 3

/* a simple game where player character is represented as 8
and the win point is represented as a 2
precondition: none
postcondition: none
*/
void start_game_grupp_7();

/* print a sample menu for the game
precondition: none
postcondition: a printed menu
*/
void print_menu_game();



#endif 