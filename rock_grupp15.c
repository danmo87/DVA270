#include "rock_grupp15.h"

static nrfx_uarte_t instance = NRFX_UARTE_INSTANCE(0);
static uint8_t buffer[1];  // Buffer for receiving data
static volatile bool program_running = true;


void uarte_handler(nrfx_uarte_event_t const *p_event, void *p_context)
{
    uint8_t Clear_screen[] = CLEAR_SCREEN;      //Rensar skärmen
    char msg[1000];
    int random = rand() % 3 + 1;                //Gör random till en random värde
    nrfx_uarte_t * p_inst = p_context;
    random = rand() % 3 + 1;                    //Gör random värde igen bara för att vara extra säker att det randomnizar (är poänglös men något jag gillar att göra)
    if (p_event->type == NRFX_UARTE_EVT_RX_DONE)
    {
        memset(msg, 0, sizeof(msg));            //Rengör message och fyller allt med 0
        switch (buffer[0])
        {
            case 'q':                           //Stor eller litet Q avslutar programmet
                program_running = false;
                sprintf(msg, "\n\rProgram avslutas \n\r");
                break;
			case 'Q':
                program_running = false;
                sprintf(msg, "\n\rProgram avslutas \n\r");
                break;

            //Väljer en switch case där det är slumpmessad vad random väljer. 
            //Beroende på vad du väljer och vad random talet är så antingen vinner du, förlorar du eller blir det en draw 
            //T.ex. om du valde en tal 1 (som är sten) och random är 3 (som är sax) så vinner du.
            case '1':
                if(random == 3)
                {
                    uint8_t msg2[] = "\r\nYou opponent chose scissor...\r\nYou win!\r\n";
	                nrfx_uarte_tx(&instance, msg2, sizeof(msg2),0);
                    nrfx_systick_delay_ms(500);
                }
                else if(random == 2)
                {
                    uint8_t msg2[] = "\r\nYou opponent chose paper...\r\nYou lose!\r\n";
	                nrfx_uarte_tx(&instance, msg2, sizeof(msg2),0);
                    nrfx_systick_delay_ms(500);
                }
                else
                {
                    uint8_t msg2[] = "\r\nYou opponent chose rock...\r\nDraw!\r\n";
	                nrfx_uarte_tx(&instance, msg2, sizeof(msg2),0);
                    nrfx_systick_delay_ms(500);
                }
                break;
			case '2':
                if(random == 1)
                {
                    uint8_t msg2[] = "\r\nYou opponent chose rock...\r\nYou win!\r\n";
	                nrfx_uarte_tx(&instance, msg2, sizeof(msg2),0);
                    nrfx_systick_delay_ms(500);
                }
                else if(random == 3)
                {
                    uint8_t msg2[] = "\r\nYou opponent chose scissor...\r\nYou lose!\r\n";
	                nrfx_uarte_tx(&instance, msg2, sizeof(msg2),0);
                    nrfx_systick_delay_ms(500);
                }
                else
                {
                    uint8_t msg2[] = "\r\nYou opponent chose paper...\r\nDraw!\r\n";
	                nrfx_uarte_tx(&instance, msg2, sizeof(msg2),0);
                    nrfx_systick_delay_ms(500);
                }
                break;
            case '3':
                if(random == 2)
                {
                    uint8_t msg2[] = "\r\nYou opponent chose paper...\r\nYou win!\r\n";
	                nrfx_uarte_tx(&instance, msg2, sizeof(msg2),0);
                    nrfx_systick_delay_ms(500);
                }
                else if(random == 1)
                {
                    uint8_t msg2[] = "\r\nYou opponent chose rock...\r\nYou lose!\r\n";
	                nrfx_uarte_tx(&instance, msg2, sizeof(msg2),0);
                    nrfx_systick_delay_ms(500);
                }
                else
                {
                    uint8_t msg2[] = "\r\nYou opponent chose scissor...\r\nDraw!\r\n";
	                nrfx_uarte_tx(&instance, msg2, sizeof(msg2),0);
                    nrfx_systick_delay_ms(500);
                }
                break;
        }
        nrfx_systick_delay_ms(500);      //Skapar en delay så att användaren kan se resultatet
        

        // Om programmet är fortfarande i gång, startar intaget av nya tecken igen.
        if (program_running && buffer[0] != 'q' && buffer[0] != 'Q')
        {
            memset(buffer, 0, sizeof(buffer));
            nrfx_uarte_rx(&instance, buffer, sizeof(buffer));
        }
    }
}


void start_game_grupp15()
{
    srand(time(NULL));       //Ger random en seed
    nrfx_systick_init();     //Funktion för att kalla delay

     NVIC_ClearPendingIRQ(NRFX_IRQ_NUMBER_GET(NRF_UARTE_INST_GET(0)));
    NVIC_EnableIRQ(NRFX_IRQ_NUMBER_GET(NRF_UARTE_INST_GET(0)));


    const nrfx_uarte_config_t config = NRFX_UARTE_DEFAULT_CONFIG(PIN_TXD, PIN_RXD);
    nrfx_uarte_init(&instance, &config, uarte_handler);   //Kallar på uarte_handler


    uint8_t msg[1000];      //Skapar message
    uint16_t timer = 0;         //Skapar värde för timer
    uint16_t current_value;  //Gör så att man inte kan spamma knappar för bufferten
    uint8_t Clear_screen[] = CLEAR_SCREEN;    //Fixar så att man kan rensar skärmen

    while (program_running)
    {


        //Gör så att man inte kan spamma knappar för bufferten
        if(timer != current_value)
        {
            nrfx_uarte_rx(&instance, buffer, sizeof(buffer));// Startar ta emot data
            current_value = timer;
        }



        memset(msg, 0, sizeof(msg));           //Rengör message och fyller allt med 0

        //Printar timer och ökar värde på timer
        sprintf(msg, "\r\nTIMER: %d\r\n", timer);
        nrfx_uarte_tx(&instance, (uint8_t*)msg, sizeof(msg), 0);
        timer++;


        while (nrfx_uarte_tx_in_progress(&instance)) {};   //Gör så att interrupts kan hände närsomhelst så länge while loop är igång.
       

        uint8_t msg2[] = "\r\nChoose:\r\n\r\n1 = Rock\r\n\n2 = Paper\r\n\n3 = Scissor\r\n";    //Frågar spelarna om de vill välja sten, sax eller påse
	    nrfx_uarte_tx(&instance, msg2, sizeof(msg2),0);

        nrfx_systick_delay_ms(1000);                                                           //Delay 1 sekund så att spelarna kan se vad som händer innan screen_clear
        nrfx_uarte_tx(&instance, Clear_screen, sizeof(Clear_screen),0);
    }

}