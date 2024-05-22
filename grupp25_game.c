#include "grupp25_game.h"

uint8_t tab[] = "  ", newRow[] = "\n\r";
int x = 0, y = 0, prevY = 0, prevX = 0, winY = 6, winX = 8, flag = 0, loopean = 1;
int matrix[SIZE][SIZE];
uint8_t cls[] = CLEAR_SCREEN;
uint8_t deathMsg[] = "The matrix has caught up with you", winMsg[] = "You have winning";
       
void uarte_write(uint8_t* data, uint8_t length){
    nrfx_uarte_tx(&instance, data, length, 0);
    while(nrfx_uarte_tx_in_progress(&instance)){};
}

void init_all(){
// Initialize the matrix to 0
    for (int j = 0; j < SIZE; j++) {
        for (int i = 0; i < SIZE; i++) {
            matrix[i][j] = 0;
        }
    }

    // Initiate the first position and the winning position
    matrix[y][x] = 1;
    matrix[winY][winX] = 7;
    
    // Prints everything once on startup
    uarte_write(cls, sizeof(cls));
    print_array(SIZE, matrix);
}


void update_game(){
    // This is called in a loop to update the game whenever you get an input
    nrfx_uarte_rx(&instance, &uarte_buffer, sizeof(&uarte_buffer));  
        if(flag){
            flag = 0;
            uarte_write(cls, sizeof(cls));
            print_array(SIZE, matrix);
            
        }

        // Removing the previous position
        if(prevX != x){
            matrix[y][prevX] = 0;
            prevX = x;
        }
        if(prevY != y){
            matrix[prevY][x] = 0;
            prevY = y;
        }

        // Win condition
        if(y == winY && x == winX){
            loopean = 0;
            uarte_write(cls, sizeof(cls));
            uarte_write(winMsg, sizeof(winMsg));
        }

        // No invalid values
        if(y < SIZE && x < SIZE && y >= 0 && x >= 0){
            matrix[y][x] = 1;
        }
        else{
            loopean = 0;
            uarte_write(cls, sizeof(cls));
            uarte_write(deathMsg, sizeof(deathMsg));
        }
}

void uarte_handler(nrfx_uarte_event_t const *p_event, void *p_context){
    nrfx_uarte_t * p_inst = p_context;
    if (p_event->type == NRFX_UARTE_EVT_RX_DONE){
        flag = 1;
        switch(uarte_buffer){
            case 'w': 
                y++;
                break;

            case 's': 
                y--;
                break;

            case 'a':   
                x--;
                break;

            case 'd': 
                x++;
                break;

            case 'q': 
                loopean = 0; 
                break;
        }
    }
    
}

void send_int(int i){
    char string[SIZE];
    sprintf(string, "%d ", i);
    nrfx_uarte_tx(&instance, string, strlen(string), 0);
    while(nrfx_uarte_tx_in_progress(&instance)){};
}

void print_array(int size, int matrix[SIZE][SIZE]) {
    uarte_write(tab, sizeof(tab));

    for(int j = 0; j < size; j++) {
        // Print column numbers
        send_int(j);
    }

    uarte_write(newRow, sizeof(newRow));
    
    for(int i = size - 1; i >= 0; i--) { //Print from bottom to top
        //Print row number
        send_int(i);
        for (int j = 0; j < size; j++) {
            send_int(matrix[i][j]);
        }
        uarte_write(newRow, sizeof(newRow));
    }
}

void start_game_grupp25(){
    init_all();
    while(loopean){  
        update_game();
    }
}
