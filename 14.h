// Grupp 14: en angenäm h-fil ;)

//libraries:
#include <nrfx.h>
#include <nrf5340_application.h>
#include <nrfx_config.h>
#include <nrf.h>
#include <nrfx_uarte.h> //nrfx_uarte_event_t m.fl.
#include <nrf_gpio.h> //I/O pins
#include <nrfx_systick.h> //delays
#include <stdio.h>
#include <stdlib.h> //rand() & srand()
#include <time.h> //time()
#include <string.h> //string functions
#include <nrfx_rtc.h> //Real-Time-Clock
#include <assert.h>

//definitioner
#define CLEAR_SCREEN "\033c" //'\033'(ny termonal), 'c'(rensa terminalen)

//nRF5340 pins:
#define PIN_TXD 20 //transmit
#define PIN_RXD 22 //recive
#define LED1 28
#define LED2 29
#define LED3 30
#define LED4 31
#define BUTTON1 23
#define BUTTON2 24
#define BUTTON3 8
#define BUTTON4 9


//startup funktioner
void nRF5340setup();
void nRF5340LED();
void nRF5340Buttons();
void nRF5340RTC();

//skriv/läs funktioner
void uarte_write(uint8_t* data, uint8_t length);
void uarte_write_clear();
void uarte_write_new_line();
int read_int(uint8_t* numberArr);
void make_string(uint8_t* charArray, int tal);

//skriv-funktioner
int scan_visible_int(int size);
int scan_int(int size);
void scan_visible_string(uint8_t* str, uint8_t length);
void scan_string(uint8_t* str, uint8_t length);

/*
Övriga tillägg: 
Aktuell baud == 115200
*/