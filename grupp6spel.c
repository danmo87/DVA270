#include <nrfx.h>
#include <nrf5340_application.h>
#include <nrfx_config.h>
#include <nrf.h>
#include <nrfx_uarte.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <nrfx_rtc.h>
#include <nrf_gpio.h>
#include <nrfx_systick.h>
#include "grupp6spel.h"
#define CLEAR_SCREEN "\033c"

static const nrfx_uarte_config_t config = NRFX_UARTE_DEFAULT_CONFIG(PIN_TXD, PIN_RXT);
static const nrfx_uarte_t instance = NRFX_UARTE_INSTANCE(0);

void uarte_handler(nrfx_uarte_event_t const *p_event, void *p_context)
{
    nrfx_uarte_t * p_inst = p_context;
    if (p_event->type == NRFX_UARTE_EVT_RX_DONE)
    {
       inputint = 1; 
    }
}

void start_game_grupp6(void){
    
    // initialiserar det som behövs
    nrfx_systick_init();
    NVIC_ClearPendingIRQ(NRFX_IRQ_NUMBER_GET(NRF_UARTE_INST_GET(0)));
    NVIC_EnableIRQ(NRFX_IRQ_NUMBER_GET(NRF_UARTE_INST_GET(0)));
    nrfx_err_t errr = nrfx_uarte_init(&instance, &config, uarte_handler);
    if (errr != 0){

    }
    srand(time(NULL));

    // Skapar viktiga variabler
    uint8_t inputstring = '0';
    int spelare;
    int dator;
    int spelarpoints = 0;
    int datorpoints = 0;
    int temp = 0;
    char instruktion[] = "\r\nTryck på G för ¤(sten) \r\nTryck på H för Y(sax) \r\nTryck på J för U(påse)\r\nTryck på Q för att avsluta spelet\r\n";
    nrfx_uarte_tx(&instance, &instruktion, strlen(instruktion),0);

    while(1){
        // hanterar inputs från uarte
        nrfx_uarte_rx(&instance, &inputstring, sizeof(inputstring));
        if(inputint == 1){
            
            inputint = 0;
           
            if(inputstring == 'q'){
                //skriver ut resultatet och avslutar matchen
                char slutarray[100];
                sprintf(slutarray, "\r\nMatchen avslutadees med att du fick %d poäng och datorn fick %d poäng\0", spelarpoints, datorpoints);
                nrfx_uarte_tx(&instance, &slutarray, strlen(slutarray),0);
                while(nrfx_uarte_tx_in_progress(&instance)){};return;
            }
            if(inputstring == 'g'){
                spelare = 1;
            }
            if(inputstring == 'h'){
                spelare = 2;
            }
            if(inputstring == 'j'){
                spelare = 3;
            }

            dator = (rand() % 3) + 1;
            temp = 1;
        }

        // avgör vem som vinner och printar ut resultatet från rundan
        if(temp == 0){
            //printar inget om ingen ny input
            char arr[] = "";
            nrfx_uarte_tx(&instance, &arr, sizeof(arr),0);
            while(nrfx_uarte_tx_in_progress(&instance)){};
        }
        else if(spelare == 1 && dator == 1 ){
            char likaspel1[] = "\r\nIngen vann för båda valde ¤";
            nrfx_uarte_tx(&instance, &likaspel1, sizeof(likaspel1),0);
            while(nrfx_uarte_tx_in_progress(&instance)){};
            temp = 0;
        }
        else if(spelare == 2 && dator == 2 ){
            char likaspel2[] = "\r\nIngen vann för båda valde Y";
            nrfx_uarte_tx(&instance, &likaspel2, sizeof(likaspel2),0);
            while(nrfx_uarte_tx_in_progress(&instance)){};
            temp = 0;
        }
        else if(spelare == 3 && dator == 3 ){
            char likaspel3[] = "\r\nIngen vann för båda valde U(";
            nrfx_uarte_tx(&instance, &likaspel3, sizeof(likaspel3),0);
            while(nrfx_uarte_tx_in_progress(&instance)){};
            temp = 0;
        }
        else if(spelare == 1 && dator == 2){
            char spelarevinner1[] = "\r\nDu vann för du valde ¤ och datorn valde Y";
            nrfx_uarte_tx(&instance, &spelarevinner1, sizeof(spelarevinner1),0);
            while(nrfx_uarte_tx_in_progress(&instance)){};
            spelarpoints++;
            temp = 0;
        }
        else if(spelare == 2 && dator == 3){
            char spelarevinner2[] = "\r\nDu vann för du valde Y och datorn valde U";
            nrfx_uarte_tx(&instance, &spelarevinner2, sizeof(spelarevinner2),0);
            while(nrfx_uarte_tx_in_progress(&instance)){};
            spelarpoints++;
            temp = 0;
        }
        else if(spelare == 3 && dator == 1 ){
            char spelarevinner3[] = "\r\nDu vann för du valde U och datorn valde ¤";
            nrfx_uarte_tx(&instance, &spelarevinner3, sizeof(spelarevinner3),0);
            while(nrfx_uarte_tx_in_progress(&instance)){};
            spelarpoints++;
            temp = 0;
        }
        else if(spelare == 1 && dator == 3){
            char datorvinner1[] = "\r\nDatorn vann för du valde ¤ och datorn valde U";
            nrfx_uarte_tx(&instance, &datorvinner1, sizeof(datorvinner1),0);
            while(nrfx_uarte_tx_in_progress(&instance)){};
            datorpoints++;
            temp = 0;
        }
        else if(spelare == 2 && dator == 1){
            char datorvinner2[] = "\r\nDatorn vann för du valde Y och datorn valde ¤";
            nrfx_uarte_tx(&instance, &datorvinner2, sizeof(datorvinner2),0);
            while(nrfx_uarte_tx_in_progress(&instance)){};
            datorpoints++;
            temp = 0;
        }
        else if(spelare == 3 && dator == 2){
            char datorvinner3[] = "\r\nDatorn vann för du valde U och datorn valde Y";
            nrfx_uarte_tx(&instance, &datorvinner3, sizeof(datorvinner3),0);
            while(nrfx_uarte_tx_in_progress(&instance)){};
            datorpoints++;
            temp = 0;
        }
    }
}









