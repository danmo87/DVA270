#include <nrfx.h>
#include <nrf5340_application.h>
#include <nrfx_config.h>
#include <nrf.h>
#include <nrfx_uarte.h>
#include <nrfx_systick.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define PIN_TXD 20
#define PIN_RXD 22

#define BUFFER_SIZE 40
static char recieve_buffer[BUFFER_SIZE];
static int buffer_pos = 0;
static int read_pos = 0;

static uint8_t rx_buffer; // char nrfx_uarte_rx writes to

static nrfx_uarte_t instance = NRFX_UARTE_INSTANCE(0); 

void uarte_handler(nrfx_uarte_event_t const *p_event, void *p_context)
{
    //nrfx_uarte_t* p_inst = p_context;
    
    if (p_event->type == NRFX_UARTE_EVT_RX_DONE)
    {
        
        nrfx_uarte_xfer_evt_t* data = &(p_event->data.rx);
        
        // Copy to buffer
        size_t size = data->bytes;
        for(int i = 0;i < size;i++){
            recieve_buffer[buffer_pos + i] = data->p_data[i]; // Data pointed at.
        }
        buffer_pos += size;
        
        nrfx_uarte_rx(&instance, &rx_buffer, sizeof(rx_buffer));
    }
}

// Game-state adjacent stuff
#define PLAYER_NONE 0
#define PLAYER_X 1
#define PLAYER_O 2

// NOTE: ENSURE THAT reset() RESETS THESE TO DEFAULT VALUES
static char player_name[] = {' ', 'X', 'O'};
static int board[3][3] = {0};
static int turn = PLAYER_X; // X goes first
static int turnNo = 0; // no of successful turns
static int winning_player = PLAYER_NONE; // Gets set if a player won.

void reset(){
    turn = PLAYER_X;
    turnNo = 0;
    winning_player = PLAYER_NONE;
    
    for(int y = 0;y < 3;y++){
        for(int x = 0;x < 3;x++){
            board[x][y] = 0;
        }
    }
}

 int move_is_winning_for(int x, int y, int player)
 {
     // This algorithm should also be able to work for
     // n pieces in a row, on a m*m board.
     // Time complexity is O(n)
     
     int count[3][3] = {0};

     // (-1 -1) (0, -1) (1  -1)
     // (-1  0) (0   0) (1   0)
     // (-1  1) (0   1) (1   1)

     // (x, y) -> (-x,  y)
     // (x, y) -> (x, -y)
     // composition: (x, y) -> (-x, -y)
     // ("other side" equation)

     for (int x_dir = -1; x_dir <= 1; x_dir++)
     {
         for (int y_dir = -1; y_dir <= 1; y_dir++)
         {
             if (x_dir == 0 && y_dir == 0) { continue; }
             count[x_dir + 1][y_dir + 1] += count[-x_dir + 1][-y_dir + 1]; // include counts from the other side
             for (int extent = 1; extent <= 3; extent++)
             {
                 int tested_x = x_dir * extent + x;
                 int tested_y = y_dir * extent + y;

                 if (tested_x < 0 || tested_x >= 3 ||
                    tested_y < 0 || tested_y >= 3)
                 {
                     break;
                 }

                 if (player != board[tested_x][tested_y])
                 {
                     break;
                 }

                 count[x_dir + 1][y_dir + 1]++;
                 if (count[x_dir + 1][y_dir + 1] >= 2) // The piece we placed is not counted
                 {
                     return 1;
                 }

             }
         }
     }
     return 0;
 }
 
 int player_won(){
    return winning_player;
}

int is_tied(){
    return turnNo >= 9;
}

// Update game state based on a single-char command.
void update(char input){
    // 1|2|3
    // 4|5|6
    // 7|8|9
    
    if(is_tied() || player_won() != PLAYER_NONE){
        return;
    }
    
    if(input < '1' || input > '9' ){
        return;
    }
    
    int moveNo = input - '1';
    int x = moveNo % 3;
    int y = moveNo / 3;
    
    if(board[x][y] != PLAYER_NONE){
        return;
    }
    
    board[x][y] = turn;
    if(move_is_winning_for(x, y, turn) != PLAYER_NONE){
        winning_player = turn;
    }
    turnNo++;
    turn = (turn == PLAYER_X) ? PLAYER_O : PLAYER_X;
    
}

// Prints the current game state
void print_state(){
    void apply_color(char **line_ptr, char *color){
        while(*color != '\0'){
            **line_ptr = *color;
            (*line_ptr)++;
            color++;
        }
    }
    
    char *colors[] = {"\033[0m", "\033[31m", "\033[32m"};
    
    // Stop any ongoing writes from ruining things
    while(nrfx_uarte_tx_in_progress(&instance)){}
    
    char clear[] = "\033c";
    nrfx_uarte_tx(&instance, clear, sizeof(clear), 0);
    while(nrfx_uarte_tx_in_progress(&instance)){}
    
    for(int y = 0;y < 3;y++){
        char line[30];
        char* line_ptr = line;
        
        for(int x = 0;x < 3;x++){
            if(board[x][y] == PLAYER_NONE){
                *line_ptr++ = '1' + (x + 3*y);
            } else {
                int playerNo = board[x][y];
                
                apply_color(&line_ptr, colors[playerNo]);
                *line_ptr++ = player_name[playerNo];
                apply_color(&line_ptr, colors[PLAYER_NONE]);
            }
            
            
            
            // if we aren't on the last line, add a vertical bar
            if(x <= 1){
                *line_ptr++ = '|';
            }
        }
        
        *line_ptr++ = '\r';
        *line_ptr++ = '\n';
        *line_ptr = '\0';
        
        nrfx_uarte_tx(&instance, line, strlen(line), 0);
        // we cannot modify contents of line while printing, so we must wait here.
        while(nrfx_uarte_tx_in_progress(&instance)){}
    }
    
    char buf[80];
    int len = 0;
    if(player_won() != PLAYER_NONE){
        len = sprintf(buf, "%c won!\r\n", player_name[winning_player]);
    } else if (is_tied()){
        len = sprintf(buf, "it's a tie!\r\n");
    } else {
        len = sprintf(buf, "it's %c's turn to play.\r\n", player_name[turn]);
    }
    
    char *line2 = buf + len;
    len += sprintf(line2, "r restart | q quit.\r\n");
    
    nrfx_uarte_tx(&instance, buf, len, 0);
    while(nrfx_uarte_tx_in_progress(&instance)){}
}

// mandatory name
void start_game_grupp27(void)
{   
    // Enable UARTE interrupt stuff
    NVIC_ClearPendingIRQ(NRFX_IRQ_NUMBER_GET(NRF_UARTE_INST_GET(0)));
    NVIC_EnableIRQ(NRFX_IRQ_NUMBER_GET(NRF_UARTE_INST_GET(0)));
    
    const nrfx_uarte_config_t config = NRFX_UARTE_DEFAULT_CONFIG(PIN_TXD, PIN_RXD);
    nrfx_err_t errr = nrfx_uarte_init(&instance, &config, uarte_handler);
    if(errr != NRFX_SUCCESS){
        assert(0);
    }
    
    nrfx_systick_init();
    
    // startup
    reset();
    print_state();
    
    nrfx_uarte_rx(&instance, &rx_buffer, sizeof(rx_buffer));
    
    uint32_t print_interval = 1000000*15;
    uint32_t print_time = nrf_systick_val_get();
    
    int main_loop = 1;
    while(main_loop){
        if(buffer_pos > 0){ // There is data in the buffer
            
            while(read_pos < buffer_pos){
                char input = recieve_buffer[read_pos];
                read_pos++;
                
                if(input == 'q'){
                    main_loop = 0;
                } else if (input == 'r') {
                    reset();
                    print_state();
                } else {
                    update(input);
                    print_state();
                }
            }            
            
            // Buffer empty!
            // Reset everything.
            read_pos = 0;   // do this first
            buffer_pos = 0;
        }
        
        if((print_time - nrf_systick_val_get()) > print_interval){
            nrf_systick_val_clear();
            print_time = nrf_systick_val_get();
            print_state();
        }
            
    }
    while(nrfx_uarte_tx_in_progress(&instance)){}
    char program_end[] = "Game ended...";
    nrfx_uarte_tx(&instance, program_end, sizeof(program_end), 0);
    
    
}