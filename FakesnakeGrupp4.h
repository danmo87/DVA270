#include <nrfx.h>
#include <nrf5340_application.h>
#include <nrfx_config.h>
#include <nrf.h>
#include <nrf_gpio.h>
#include <nrfx_uarte.h>
#include <nrfx_systick.h>
#include <stdlib.h> 
#include <stdio.h> 
#include<string.h>
#include <time.h>
#include <nrfx_rtc.h>
#include <nrfx_glue.h>

int read_number (char buffer[]);
void read_string  (char string[]);
void send_int (int data);
 void uarte_handler(nrfx_uarte_event_t const *p_event, void *p_context);