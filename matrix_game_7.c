//grind.c
#include <matrix_game_7.h>
int x = 0;
int y = 0;
int mc = 8;
int game_end = 0;
int win_area = 2;
uint8_t uarte_buffer;

// game field: MATRIX_Y and MATRIX X is found at grind.h
int matrix[MATRIX_Y][MATRIX_X];

void start_game_grupp_7(){

    // fills the game field with 0s
    for(int i = 0; i < MATRIX_Y;i++){
        for(int j = 0; j < MATRIX_X;j++){
            matrix[i][j] = 0;
        }
    }

    matrix[x][y] = mc; // player character
    matrix[WIN_X][WIN_Y] = win_area; // win point
    print_menu_game();
    wait_seconds(3);

    // game loop start
    do{
        clear_screen();
        matrix[WIN_X][WIN_Y] = win_area; // win point
        nrfx_uarte_rx(&instance, &uarte_buffer, sizeof(uarte_buffer));
        // prints the game field
        for(int i = 0; i < MATRIX_Y;i++){
            for(int j = 0; j < MATRIX_X;j++){
                int number = matrix[i][j];
                send_int(number);
            }
            uart_transmit_string("\n\r");
        }
        wait_seconds(1);

        // checks the win scenario and exits accourdingly
        if (matrix[WIN_X][WIN_Y] == 8){
            uart_transmit_string(BLUE BOLD);
            uart_transmit_string("╔══════════════════════════════════════════════════╗\n\r");
            uart_transmit_string("║                    " YELLOW BOLD "YOU WON" BLUE BOLD "                       ║\n\r");
            uart_transmit_string("╚══════════════════════════════════════════════════╝\n\n\r");
            uart_transmit_string(RESET);
            game_end = 1;
        }
        
    }while (game_end != 1); // game loop end

    //notifies the end of program
    uart_transmit_string(RED BOLD);
    uart_transmit_string("╔══════════════════════════════════════════════════╗\n\r");
    uart_transmit_string("║                " YELLOW BOLD "GAME HAS ENDED" RED BOLD "                    ║\n\r");
    uart_transmit_string("╚══════════════════════════════════════════════════╝\n\n\r");
    uart_transmit_string(RESET);
    //end
}


// prints a premade menu interface for the game
void print_menu_game() {
    uart_transmit_string(MAGENTA BOLD);
    uart_transmit_string("╔══════════════════════════════════════════════════╗\n\r");
    uart_transmit_string("║                   " YELLOW BOLD "Matrix GAME" MAGENTA BOLD "                    ║\n\r");
    uart_transmit_string("╚══════════════════════════════════════════════════╝\n\n\r");

    uart_transmit_string(GREEN);
    uart_transmit_string("Use the following keys to controll the game\n\n\r");

    uart_transmit_string(CYAN BOLD);
    uart_transmit_string("I: ");
    uart_transmit_string(RESET);
    uart_transmit_string("Move Up\n\r");

    uart_transmit_string(CYAN BOLD);
    uart_transmit_string("K: ");
    uart_transmit_string(RESET);
    uart_transmit_string("Move Down\n\r");

    uart_transmit_string(CYAN BOLD);
    uart_transmit_string("J: ");
    uart_transmit_string(RESET);
    uart_transmit_string("Move Left\n\r");

    uart_transmit_string(CYAN BOLD);
    uart_transmit_string("L: ");
    uart_transmit_string(RESET);
    uart_transmit_string("Move Right\n\n\r");

    uart_transmit_string(RED BOLD);
    uart_transmit_string("Q: ");
    uart_transmit_string(RESET);
    uart_transmit_string("END PROGRAM\n\n\r");

    uart_transmit_string(CYAN BOLD);
    uart_transmit_string("Wait 3 seconds to start\n\n\r");
    uart_transmit_string(RESET);

    uart_transmit_string(RED BOLD);
    uart_transmit_string("╔══════════════════════════════════════════════════╗\n\r");
    uart_transmit_string("║                     " YELLOW BOLD "START" RED BOLD "                        ║\n\r");
    uart_transmit_string("╚══════════════════════════════════════════════════╝\n\n\r");
    uart_transmit_string(RESET);
}

// Funktion för att läsa in en sträng från UART
void read_string(char *str, size_t max_len) {
    int i;
    for (i = 0; i < max_len + 1; i++) {
        nrfx_uarte_rx(&instance, &uarte_buffer, sizeof(uarte_buffer));
        if (uarte_buffer == '\r')
            break;
        str[i] = uarte_buffer;
    }
    str[i] = '\0'; // Add null terminator
}

// Funktion för att läsa in ett heltal från UART
int read_input(void) {
    char input_str[5];
    read_string(input_str, sizeof(input_str));
    return atoi(input_str);
}

char read_(void) {
    char input_str[1];
    read_string(input_str, sizeof(input_str));
    return input_str[0];
}

// Funktion för att skicka ett heltal via UART
void send_int(int num) {
    char int_str[50];
    memset( int_str, '\0', sizeof(int_str));
    sprintf(int_str, "%d", num);

    int qwe = strlen(int_str);
    nrfx_uarte_tx(&instance, int_str, sizeof(int_str),0);
    while(nrfx_uarte_tx_in_progress(&instance)){};
}
	
// Funktion för att skicka en enskild tecken via UART
void uart_transmit_char(char character) {
    nrfx_uarte_tx(&instance, (uint8_t*)&character, 1, 0);
    while(nrfx_uarte_tx_in_progress(&instance)){};
}

// Funktion för att skicka en sträng via UART
void uart_transmit_string(const char *string) {
    // Loopa igenom strängen tills null-tecknet nås.
    while (*string != '\0') {
        // Skicka varje karaktär i strängen via UARTE.
        uart_transmit_char(*string);
        // Gå till nästa karaktär i strängen.
        string++;
    }
}

// Funktion för att rensa skärmen
void clear_screen() {
    uart_transmit_string(CLEAR_SCREEN);

}

// Funktion för att vänta ett visst antal sekunder
void wait_seconds(int seconds) {
    int counter = 0;
    do {
        counter = nrfx_rtc_counter_get(&rtc_instance);
    } while (counter <= (32768 * seconds));
    nrfx_rtc_counter_clear(&rtc_instance);
}
void uarte_handler(nrfx_uarte_event_t const *p_event, void *p_context){
    nrfx_uarte_t * p_inst = p_context;
    if (p_event->type == NRFX_UARTE_EVT_RX_DONE) {
        
        switch(uarte_buffer) {
            case 'i': //upp
                //clear_screen();
                if(y > 0){
                    matrix[y][x] = 0;
                    y--;
                }
                matrix[y][x] = mc;
                break;
            case 'k': //down
                //clear_screen();
                if(y < MATRIX_Y-1){
                    matrix[y][x] = 0;
                    y++;
                }
                matrix[y][x] = mc;
                break;
            case 'j'://left
                //clear_screen();
                if(x > 0){
                    matrix[y][x] = 0;
                    x--;
                }
                matrix[y][x] = mc;
                break;
            case 'l'://right
                //clear_screen();
                if(x < MATRIX_X-1){
                    matrix[y][x] = 0;
                    x++;
                }
                matrix[y][x] = mc;
                break;
            case 'q': // quit game
                game_end = 1;
                break;
            default: // no movement
                //clear_screen();
                matrix[y][x] = mc;
                break;
        }
        uarte_buffer = NULL;
    }
}

// Funktion för att initiera systick och UART
void init_systick_and_uart() {
    nrfx_systick_init();
    nrfx_rtc_enable(&rtc_instance);
    const nrfx_uarte_config_t config = NRFX_UARTE_DEFAULT_CONFIG(PIN_TXD, PIN_RXD);
    //nrfx_err_t err = nrfx_uarte_init(&instance, &config, NULL); //old way
    nrfx_err_t err = nrfx_uarte_init(&instance, &config, uarte_handler); //with interupts
    if (err != NRFX_SUCCESS) {
    }
}