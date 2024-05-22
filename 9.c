
#include <9.h>



static nrfx_uarte_t instance = NRFX_UARTE_INSTANCE(0);
static const nrfx_rtc_t rtc_instance = NRFX_RTC_INSTANCE(0);

uint8_t buffer;
int up = 0;
int side = 0;

uint32_t read_int(nrfx_uarte_t *instance, uint8_t num[]){

    int i = 0;

    do{
        nrfx_uarte_rx(instance, &buffer, sizeof(buffer));
        num[i] = buffer;
        i+=1;
    } while(num[i-1] != '\r');
    num[i-1] = '\0';


    uint32_t result = atoi(num);

    return result;

}

//initierar grejer samt kört start_game_grupp9()

void init_start() {


    NVIC_ClearPendingIRQ(NRFX_IRQ_NUMBER_GET(NRF_UARTE_INST_GET(0)));
    NVIC_EnableIRQ(NRFX_IRQ_NUMBER_GET(NRF_UARTE_INST_GET(0)));

    const nrfx_uarte_config_t config = NRFX_UARTE_DEFAULT_CONFIG(20,22);
    nrfx_systick_init();

    nrfx_err_t errr = nrfx_uarte_init(&instance, &config, uarte_handler);
    if (errr != 0){
    }


    
    start_game_grupp9();



}




void start_game_grupp9() {


    uint8_t clear_screen[] = CLEAR_SCREEN;

    char grid[10][10];


    char new_line[] = "\n\r";

    //äpplets kordinater
    uint8_t x = rand()%10;
    uint8_t y = rand()%10;
    uint8_t check = 1;


    while(check) {
        nrfx_uarte_rx(&instance, &buffer, sizeof(buffer));

        //ifall man vil quit
        if(buffer == 'q') {
            check = 0;
        }

        //clear screen
        nrfx_uarte_tx(&instance, clear_screen, sizeof(clear_screen), 0);
        while(nrfx_uarte_tx_in_progress(&instance));

        
        //gör om alla symboler i griden till *
        for(int i = 0; i<10; i++) {
            memset(grid[i], '*', sizeof(grid[i]));
        }

        //var vi är(x) och var äpplet är
        grid[up][side] = 'x';
        grid[y][x] = 'o';

        //printar griden
        for(int z = 0; z<10; z++) {
            nrfx_uarte_tx(&instance, grid[z], sizeof(grid[z]), 0);
            while(nrfx_uarte_tx_in_progress(&instance));
            nrfx_uarte_tx(&instance, new_line, sizeof(new_line), 0);
            while(nrfx_uarte_tx_in_progress(&instance));
        }

        //ifall man är på samma plats som äpplet så vinner man
        if(up == y && x == side) {
            uint8_t msg[] = "You Won";
            nrfx_uarte_tx(&instance, msg, sizeof(msg), 0);
            check = 0;
        }

        

        nrfx_systick_delay_ms(1000);


    }




}


void uarte_handler(nrfx_uarte_event_t const *p_event, void *p_context)
{
    nrfx_uarte_t * p_inst = p_context;
    if (p_event->type == NRFX_UARTE_EVT_RX_DONE)
    {
        //vi kollar på buffer och gör sen mattematiska beräkningar som ändrar kordinaterna för var vi är i griden
        switch(buffer) {
            case 'w':
                up = up-1;
                if(up == -1){
                    up = 9;
                }
                break;
            case 'a':
                side = side-1;
                if(side == -1) {
                    side = 9;
                }
                break;
            case 's':
                up = up+1;
                if(up == 10){
                    up = 0;
                }
                break;
            case 'd':
                side = side+1;
                if(side == 10) {
                    side = 0;
                }
                break;
        }
    }
}
