
#include "header17.h"
#define CLEAR_SCREEN "\033c"

static const nrfx_uarte_config_t config = NRFX_UARTE_DEFAULT_CONFIG(PIN_TXD, PIN_RXD);

static nrfx_uarte_t uarte_instance = NRFX_UARTE_INSTANCE(0);
int running =1; 
static int X, Y = 0;
static uint8_t uarte_buffer; 


void start_game_grupp17(void)
{

    while (running) //Så länge man inte vinner eller trycker 'q' så fortsätter spelet
    {
        nrfx_uarte_rx(&uarte_instance, &uarte_buffer, sizeof(uarte_buffer));
        show_and_update();
        nrfx_systick_delay_ms(1000);	//Vänta lite innan den kör om whilen

    }

}

void uarte_write( uint8_t* data, uint8_t length )
{
    nrfx_uarte_tx(&uarte_instance, data, length, 0);
    while(nrfx_uarte_tx_in_progress(&uarte_instance)){};
}


void uarte_handler(nrfx_uarte_event_t const *p_event, void *p_context)
 {

    
    if (p_event->type == NRFX_UARTE_EVT_RX_DONE) 
    {
        uint8_t input = uarte_buffer;
        switch(input) 
        {
            case 105: //i 
                if (Y > 0) Y--;
                break;
            case 107: //k 
                if (Y < 9) Y++;
                break;
            case 106: // j
                if (X > 0) X--;
                break;
            case 108: // l
                if (X < 9) X++;
                break;
            case 113:  //q 
                running = 0;
                break;
            default:
                break;
        }   // förflyttning av koordinater mha. tangenterna som omvandlats till ascii bokstäver.

    }
}




void update_matrix(void) 
{
    int matrix[10][10];
    uint8_t msg4[] = "\n\r";
    uarte_write(msg4, sizeof(msg4));
    for (int i = 0; i < 10; i++) 
    {
        for (int j = 0; j < 10; j++)
        {
            matrix[i][j] = '-';
        }
    }
    matrix[Y][X] = 'S';

    int randarrx[3]={3,2};
    int randarry[3]={1,4};
    if(Y==1 && X==3|| Y==4 && X==2) //Kommer användaren till rätt koordinat dvs. den okända koordinaten så Vinner man.
    {
        uint8_t msg3[] = "Du vann!!";   
        uarte_write(msg3, sizeof(msg3));
        running=0;
    }
}



void show_matrix(void) 
{
    char buffer[20]; //Buffer för att konvertera int till sträng
    int matrix[10][10];

    for (int i = 0; i < 10; i++) 
    {
        for (int j = 0; j < 10; j++) 
        {
            sprintf(buffer, "%c ", matrix[i][j]); 
            uarte_write((uint8_t*)buffer, strlen(buffer)); //skriver ut matrisen
        }
        
        uint8_t msg1[] = "\n\r";  //radbrytning
        uint8_t msg2[] = "\n\r";  

        
        uarte_write(msg1, sizeof(msg1));
        uarte_write(msg2, sizeof(msg2));

    }
    sprintf(buffer, "Current position: X=%d, Y=%d\n\r\r", X, Y); //Vart man gått mha. tangenter
    uarte_write((uint8_t*)buffer, strlen(buffer));

}
void show_and_update(void) 
{
    update_matrix();
    show_matrix();
}




