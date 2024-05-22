#include "13.h"


void uarte_write(uint8_t* data, uint8_t length){
    nrfx_uarte_tx(&instance, data, length, 0);
    while(nrfx_uarte_tx_in_progress(&instance)){
        
    }
}

void uarte_handler(nrfx_uarte_event_t const *p_event, void *p_context){
    nrfx_uarte_t * p_inst = p_context;
    if (p_event->type == NRFX_UARTE_EVT_RX_DONE)
    {
        switch(uarte_buffer){               //uses uarte inputs i,k,j,l too change coordinates of player in grid
            case 'i':
                if(y_player == 0){
                    y_player = 9;
                }
                else{
                y_player = y_player -1;
                }
                break;
            case 'k':
                if(y_player == 9){
                    y_player = 0;
                }
                else{
                y_player = y_player +1;
                }
                break;
            case 'j':
                if(x_player == 0){
                    x_player = 9;
                }
                else{
                x_player = x_player -1;
                }
                break;
            case 'l':
                if(x_player == 9){
                    x_player = 0;
                }
                else{
                x_player = x_player +1;
                }
                break;
        }
    }
}



void start_game_grupp13(){
    int x_food = rand() %10;
    int y_food = rand() %10;

    int y_chaser = 9;
    int x_chaser = 9;
    int chaser_direction;
    int chaser_counter = 0;

    int lost = 0;

    char grid[10][10];
    char new_line [] = "\r\n";
    uint8_t clear_screen [] = CLEAR_SCREEN;
    char win_screen[] = "YOU WIN";
    char losing_screen[] = "YOU LOSE";

    for(int i = 0; i < 10; i++){
        memset(grid[i], '_', sizeof(grid[i]));      //fills the grid with "_"
    }
    grid[y_food][x_food] = 'O';

    while(1){
        if(chaser_counter > 4){                     //chaser gets a new direction
            chaser_direction = rand() %4;
            chaser_counter = 0;
        }
        switch(chaser_direction){
            case 0:
            if(y_chaser < 9){
                y_chaser +=1;
            }
                break;
            case 1:
            if(y_chaser > 0){
                y_chaser -=1;
            }
                break;
            case 2:
            if(x_chaser < 9){
                x_chaser +=1;
            }
                break;
            case 3:
            if(x_chaser > 0){
                x_chaser -=1;
            }
                break;
        }
        if(y_food == y_player && x_food == x_player){          //if you eat food
            x_food = rand() %10;                                //randomizes new position of food
            y_food = rand() %10;
            grid[y_food][x_food] = 'O';
            counter +=1;                                    //score goes up
        }
        if(y_chaser == y_player && x_chaser == x_player){       //if chaser gets you
            lost = 1;
            break;
        }
        nrfx_uarte_rx(&instance, &uarte_buffer, sizeof(uarte_buffer));
        uarte_write(clear_screen, sizeof(clear_screen));

        
        for(int i = 0; i < 10; i++){                        //continouesly fills the grid with "_" while the game is playing
            memset(grid[i], '_', sizeof(grid[i]));
        }
        grid[y_player][x_player] = 'x';                 // places the player, food and chaser
        grid[y_food][x_food] = 'O';
        grid[y_chaser][x_chaser] = 'I';
        for(int z = 0; z < 10; z++){                    //prints the grid
            uarte_write(grid[z], sizeof(grid[z]));
            uarte_write(new_line, sizeof(new_line));
        }
        nrfx_systick_delay_ms(50);
        if(uarte_buffer == 'q'){                //quits the game with "q"
            break;
        }
        if(counter == 10){                  //wins the game at 10 score
            break;
        }
        chaser_counter +=1;
    }  
    if(counter == 10){
        uarte_write(win_screen, sizeof(win_screen));
    }
    else if(lost == 1){
        uarte_write(losing_screen, sizeof(losing_screen));
    }
}




