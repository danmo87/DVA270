#include <nrfx.h>
#include <nrf5340_application.h>
#include <nrfx_config.h>
#include <nrf.h>
#include <nrfx_bsdriver.h>
#include <assert.h>
#include <stdio.h>
#include <Daniel_GPIO.h> //Ni behöver alltså inte inkludera er egna UARTE write, förutsätt att det finns en uarte_write, uarte_init osv osv
#include "grupp8spel.h"
#include "13.h" //inkludera er header fil.
#include "14.h"
#include "hungry_elephant_game14"
#include "grupp1.h" //inkludera er header fil.
#include "Grupp3.h"
#include "grupp_21.h"
#include "header17.h"//Grupp 17;Spelet går ut på att komma till rätt okänd koordinat mha. förflyttning med j,k,l,i
#include "grupp25_game.h"
#include "grupp6spel.h" // Grupp 6 sten sax påse
#include <group19.h>
#include <nonblocking.h>// Grupp 5 tjuv och polis spel wdas jlki för rörelse
#include <9.h>
#include <26.h>
#include <24.h> //grupp 24
#include <30.h>
#include "rock_grupp15.h"
#include "27.h"
#include "G16_games"
#include "spel.h"
#include "spel12.h"
#include "grupp10.h"

/* By Daniel Morberg*/



int main(void)
{

    int keypress = get_key();
    switch(keypress){

        case 1:
            //anrop till grupp 1's funktion
            break;
        case 10:
            start_game_grupp10();
        case 5:
            start_game_grupp5();
            break;
        case 3:
            start_game_grupp3();
            break;
        case 4:
            start_game_grupp4();
            break;
        case 7:
            start_game_grupp_7();
            break;
        case 21:
            start_game_grupp21();
            break;
        case 8:
            start_game_grupp8();
            break;
        case 12:
            start_game_grupp12()
        case 13:
            start_game_grupp13(); 
            break;
        case 14:
            start_game_grupp14(); 
            break;
        case 17:
            start_game_grupp17();
            break;
        case 6:
            start_game_grupp6();
            break;
        case 25:
            start_game_grupp25();
            break;
        case 17:
            start_game_grupp17();
            break;
        case 19:
            start_game_group19();
            break;
        case 9:
            init_start();
            break;
        case 24:
            start_game_group24();
            break;
         case 26:
            start_game_grupp26();
            break;
        case 30:
            start_game_grupp_30();
            break;
        case 15: 
            start_game_grupp15();
            break;
        case 23: 
            start_game_grupp23();
            break;
        case 20:
            start_game_grupp20();
            break;
        case 27:
            start_game_grupp27();
            break;
      default:
    }
}


