include "grupp_21.h"

uint8_t newRow[] = "\n\r";
int x = 0;
int loopean = 1;
int reaction_bar[SIZE];
int win = 0;


void start_game_grupp21(){

    uint8_t cls[] = CLEAR_SCREEN;
    uint8_t winscreen[] = "You've won!";
    
    // Initialize the line to 0

    for (int i = 0; i < SIZE; i++) {
    reaction_bar[i] = 0;
    }
    reaction_bar[0] = 1;
    reaction_bar[12] = 8; // The middle of the line becomes 8

    uarte_write(cls, sizeof(cls)); // Clearscreen
    print_array(SIZE, reaction_bar);
    
    while(loopean){
      
        
        nrfx_uarte_rx(&instance, &uarte_buffer, sizeof(uarte_buffer));
            
        uarte_write(cls, sizeof(cls));
        print_array(SIZE, reaction_bar);
        nrfx_systick_delay_ms(600); // Change this value for harder difficulty
        if(x < SIZE){
            if(x != 12){            // Keeps track of where on the line you are right now, does not write on the goal spot (8)
                reaction_bar[x] = 1;
            }
            if(x != 0 && x != 13){      // Does not remove the goal (8 in middle)
                reaction_bar[x-1] = 0;
            }
            x++;

        else{           // Loops back to the beginning if you reach the end without any input
        }
            reaction_bar[x-1] = 0;
            x = 0;
        }
        
    }
    // Win condition
    if(win){
        uarte_write(cls, sizeof(cls));
        uarte_write(winscreen, sizeof(winscreen));   
    }
}

void uarte_handler(nrfx_uarte_event_t const *p_event, void *p_context){
    nrfx_uarte_t * p_inst = p_context;
    if (p_event->type == NRFX_UARTE_EVT_RX_DONE){
      
        switch(uarte_buffer){
            case 'f':
            if(x == 13)     // If you successfully stop at number 8, the middle  
            {
                loopean = 0;
                win = 420;
            } else{
                x = 0;
                
            }
            break;

            case 'q': 
                loopean = 0; 
                break;
        }
    }
}

// Converts integers to a string of chars and prints it
void send_int(int i){ 
    char string[SIZE];
    sprintf(string, "%d ", i);
    nrfx_uarte_tx(&instance, string, strlen(string), 0);
    while(nrfx_uarte_tx_in_progress(&instance)){};
}

// Prints the whole line
void print_array(int size, int array[]) {
  
    uarte_write(newRow, sizeof(newRow)); // Adjusted positioning for the terminal

    uarte_write(newRow, sizeof(newRow));
    
    for(int i = 0; i<size; i++){
        send_int(array[i]);
    }

}