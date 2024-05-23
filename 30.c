#include <nrfx.h>
#include <nrf5340_application.h>
#include <nrf.h>
#include <nrfx_config.h>
#include <nrfx_uarte.h>
#include <nrfx_systick.h>
#include <nrf_gpio.h>
#include <stdlib.h>
#include <math.h>
#include <nrfx_glue.h>
#include <time.h>
#include "30.h"
#include <stdio.h>

char clear_screen[] = "\033c";
volatile X = 0;
volatile Y = 0;

int random_x = 0;
int random_y = 0;
int bad_random_x = 0;
int bad_random_y = 0;
int points = 0;
int new_input = 0;
char rx_buffer;

static nrfx_uarte_t instance = NRFX_UARTE_INSTANCE(0);

//Triggas bara om rx triggas annars kommer programmet att loopa som vanligt.
void uarte_handler(nrfx_uarte_event_t const *p_event, void *p_context)
{
    nrfx_uarte_t * p_inst = p_context;
    if (p_event->type == NRFX_UARTE_EVT_RX_DONE)
    {
        //Input sätts till 1 och används i move user för att kontrolera att enbart ny data används.
       new_input = 1;
    }
}

void start_game_grupp_30(void)
{
    srand(time(NULL));
    const nrfx_uarte_config_t config = NRFX_UARTE_DEFAULT_CONFIG(PIN_TXD, PIN_RXD);
    
    NVIC_ClearPendingIRQ(NRFX_IRQ_NUMBER_GET(NRF_UARTE_INST_GET(0)));
    NVIC_EnableIRQ(NRFX_IRQ_NUMBER_GET(NRF_UARTE_INST_GET(0)));
   
    nrfx_uarte_init(&instance, &config, uarte_handler);
    nrfx_systick_init();

   
    
    uint8_t start[] = "\n\r Start \n\r";
    nrfx_uarte_tx(&instance, start, sizeof(start),0);
    while(nrfx_uarte_tx_in_progress(&instance)){}
    
    //skapa positioner för målet och motståndaren. 
    Uppdate_Random();

    //Loopa tills användaren vinner eller förlorar.
    while(points < 2 && points >= 0)
    {
        
        uint8_t msg[] = "\n\r Enter a value\n\r";
        nrfx_uarte_tx(&instance,msg, sizeof(msg),0);
        while(nrfx_uarte_tx_in_progress(&instance)){}
        
        nrfx_uarte_rx(&instance, &rx_buffer, sizeof(rx_buffer));

        Move_User();
        if(points >= 2)
        {
            uint8_t Win[] = "\n\r you won\n\r";
            nrfx_uarte_tx(&instance, Win, sizeof(Win),0);
            while(nrfx_uarte_tx_in_progress(&instance)){}
        }
        else if(points < 0)
        {
            uint8_t Lose[] = "\n\r you lost\n\r";
            nrfx_uarte_tx(&instance, Lose, sizeof(Lose),0);
            while(nrfx_uarte_tx_in_progress(&instance)){}
        }
        nrfx_systick_delay_ms(1000);
    }
}

void print(void)
{
    nrfx_uarte_tx(&instance, clear_screen, sizeof(clear_screen),0);
    while(nrfx_uarte_tx_in_progress(&instance)){}
            
    int i = 0;
    int j = 0;
    uint8_t board[] = "x";
    uint8_t new_line[]= "\n\r";
    
    //Hur många rader det ska vara.
    for(i = 0; i < HEIGHT; i++)
    {
        //hur många kolumner det ska vara.
        for(j = 0; j < WIDTH; j++)
        {
            //Om användarens kordinater är samma som kordinaterna i och j ska spelar ikonen skrivas ut.
            if(X == j && Y == i)
            {
                uint8_t user_sign[] = "O";
                nrfx_uarte_tx(&instance,user_sign, sizeof(user_sign),0);
                while(nrfx_uarte_tx_in_progress(&instance)){}
            }

            //Om målets kordinater är samma som kordinaterna i och j.
            else if(i == random_y && j == random_x)
            {
                uint8_t Goal[] = "*";
                nrfx_uarte_tx(&instance, Goal, sizeof(Goal),0);
                while(nrfx_uarte_tx_in_progress(&instance)){}
            }

            //Om det som ger minus poäng har samma kordinater som i och j. 
            else if(i == bad_random_y && j == bad_random_x)
            {
                uint8_t Enemy[] = "E";
                nrfx_uarte_tx(&instance, Enemy, sizeof(Enemy),0);
                while(nrfx_uarte_tx_in_progress(&instance)){}
            }

            else
            {
                nrfx_uarte_tx(&instance, board, sizeof(board),0);
                while(nrfx_uarte_tx_in_progress(&instance)){}
            }
        }

        nrfx_uarte_tx(&instance, new_line, sizeof(new_line),0);
    }
    nrfx_systick_delay_ms(100);
    char points_str[20];
    sprintf(points_str, "\n\rpoints %d\n\r", points);
    nrfx_uarte_tx(&instance, points_str, strlen(points_str),0);
    while(nrfx_uarte_tx_in_progress(&instance)){}
    return;
}

void Move_User(void)
{
    //Loopa en gång för varje ny input.
    while(new_input == 1)
    {
        //Ändra kordinaterna beroende på inputen.
        switch(rx_buffer)
        {
            case 'i':
            Y++;
            break;

            case 'k':
            Y--;
            break;

            case 'j':
            X--;
            break;

            case 'l':
            X++;
            break;

            case 'q':
            uint8_t msg_quit[] = "\n\r quting\n\r";
            nrfx_uarte_tx(&instance, msg_quit, sizeof(msg_quit),0);
            while(nrfx_uarte_tx_in_progress(&instance)){}
            exit(0);
            break;

            default:
            break;
        }
        new_input = 0;
    }

    //Se till att användar kordinaterna inte kan komma utanför spel planen.
    if(Y >= HEIGHT)
    {
        Y = HEIGHT - 1;
    }

    else if(Y <  0)
    {
        Y = 0;
    }

    if(X >= WIDTH)
    {
        X = WIDTH - 1;
    }

    else if(X < 0)
    {
        X = 0;
    }

    //När användaren kommer till någon av målet eller motståndaren så ska deras position uppdateras
    if(X == random_x && Y == random_y)
    {
        points++;
        Uppdate_Random();
    }
    if(X == bad_random_x && Y == bad_random_y)
    {
        points--;
        Uppdate_Random();
    }

    print();
}

int Make_random(void)
{
    return(rand() % WIDTH);
}

void Uppdate_Random(void)
{   
    random_x = Make_random();
    //se till att målet inte han komma utanför spel planen.
    if(random_x == 0)
    {
        random_x = Make_random();
    }

    random_y = Make_random();
    bad_random_x = Make_random();
    //Se till att motståndaren inte kommer utanför spel planen och att motståndaren och målet inte hamnar på samma kordinater.
    if(bad_random_x == 0 || bad_random_x == random_x)
    {
        bad_random_x = Make_random();
    }

    bad_random_y = Make_random();
}