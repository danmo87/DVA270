#include "nonblocking.h"

int arr[10][10];
int switcher;
nrfx_uarte_t instance = NRFX_UARTE_INSTANCE(0);
static uint8_t uarte_buffer;

void start_game_grupp5(){


     switcher = 0;

    nrfx_systick_init();
    NVIC_ClearPendingIRQ(NRFX_IRQ_NUMBER_GET(NRF_UARTE_INST_GET(0)));
    NVIC_EnableIRQ(NRFX_IRQ_NUMBER_GET(NRF_UARTE_INST_GET(0)));

    const nrfx_uarte_config_t config = NRFX_UARTE_DEFAULT_CONFIG(20,22);
    nrfx_err_t errr = nrfx_uarte_init(&instance, &config, uarte_handler);
    if(errr != 0)
    {

    }

    int y = 0, x = 0, f = 9, p = 9;
    char arrClear[] = CLEAR_SCREEN;
    char change = 'o';
    int round = 0;

    do{
        uarte_write(arrClear, sizeof(arrClear));
        uint8_t msg[] = " \n Hello do you want to increase/decrease y width 1 i/k\n\r";
        uint8_t msg2[] = " \n Do you want to increase/decrease x width 1 l/j\n\r";
        uint8_t msg3[] = " \n Do you want to exit q\n\r";
        uint8_t msg5[] = " \n Round \r";
        
        //uarte_write(msg5, sizeof(msg5));
        //uarte_write(msg, sizeof(msg));
        //uarte_write(msg2, sizeof(msg2));
        //uarte_write(msg3, sizeof(msg3));

        uarte_write(msg5, sizeof(msg5));
        send_int(round);
        print_newLine();

        errr = nrfx_uarte_rx (&instance, &uarte_buffer, sizeof(uarte_buffer));

        change = uarte_buffer;

        if(switcher){
            switch(change){// Tangenter för att förflytta sig 

                case 's': y = y + 1; 
                if(y > 9){
                    y = 0;
                }
                break;

                case 'w': y = y - 1;
                if(y < 0){
                    y = 9;
                }
                break;

                case 'd': x = x + 1;
                if(x > 9){
                    x = 0;
                }
                break;

                case 'a': x = x - 1;
                if(x < 0){
                    x = 9;
                }
                break;

                case 'k': f = f + 1;
                 if(f > 9){
                    f = 0;
                }
                break;

                case 'i': f = f - 1;
                if(f < 0){
                    f = 9;
                }
                break;

                case 'l': p = p + 1;
                 if(p > 9){
                    p = 0;
                }
                break;

                case 'j': p = p - 1;
                if(p < 0){
                    p = 9;
                }
                break;
        
                case 'q': return;
                break; 
            }
            switcher = 0;
        }

        if(x == p && y == f){
            uarte_write(arrClear, sizeof(arrClear));
            uint8_t msg4[] = " \n The CHASER won!\n\r";
            uarte_write(msg4, sizeof(msg4));

            y = 0; 
            x = 0; 
            f = 9; 
            p = 9;
            round++;


        nrfx_systick_delay_ms(4000);

        }

        if(change != 'q'){
        the_Mighty_Assembly_Of_Arrays(x, y, p, f);
        the_Mighty_Assembly_Of_Arrays_Printer();
        }

        nrfx_systick_delay_ms(200);
    }while(change != 'q');

}

void uarte_handler(nrfx_uarte_event_t const *p_event, void *p_context)
{
    nrfx_uarte_t * p_inst = p_context;
    if (p_event->type == NRFX_UARTE_EVT_RX_DONE)
    {
    //gör något
    switcher = 1;
    }
}

void the_Mighty_Assembly_Of_Arrays(int x, int y, int p, int f){ // X = horizontel, Y = vertikal. position för spelarna

    if (x < 0 || y < 0 || x > 10 || y > 10){
            
        return;
    }


    for(int i = 0; i < 10; i++){

        for(int n = 0; n < 10; n++){

            arr[i][n] = 0;
        }

    }

    arr[y][x] = 1;
    arr[f][p] = 2;
       
}

void the_Mighty_Assembly_Of_Arrays_Printer(void){//printar ut spelfältet

    for(int i = 0; i < 10; i++){

        for(int n = 0; n < 10; n++){

           send_int(arr[i][n]);
        }
        print_newLine();
    }
    
}

void print_newLine(void){
	
	uint8_t a[] = "\n\r";
	uarte_write(a, sizeof(a));

}

void send_int(int int1){ 
    
    int length;
    uint8_t str4[20];
   
    sprintf(str4, "%d", int1);

    length = strlen(str4);

    uarte_write(str4, length);
} 

void uarte_write(uint8_t* data, uint8_t length){

    nrfx_uarte_tx(&instance, data, length,0);
    while(nrfx_uarte_tx_in_progress(&instance)){};
}
