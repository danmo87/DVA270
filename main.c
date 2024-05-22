#include <nrfx.h>
#include <nrf5340_application.h>
#include <nrfx_config.h>
#include <nrf.h>
#include <nrfx_bsdriver.h>
#include <assert.h>
#include <stdio.h>
#include <Daniel_GPIO.h> //Ni behöver alltså inte inkludera er egna UARTE write, förutsätt att det finns en uarte_write, uarte_init osv osv
#include "grupp8spel.h"

/* By Daniel Morberg*/



int main(void)
{
int keypress = get_key();
  switch(keypress){
    case 8:
    start_game_grupp8();
    break;
  }
}
