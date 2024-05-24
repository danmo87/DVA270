#include "26.h"

#define BOARD_SIZE_X 3
#define BOARD_SIZE_Y 3

static char move = ' ';
static bool running = true;
static nrfx_uarte_t instance = NRFX_UARTE_INSTANCE(0);


void init_systick_and_uart() {
    nrfx_systick_init();
    const nrfx_uarte_config_t config = NRFX_UARTE_DEFAULT_CONFIG(PIN_TXD, PIN_RXD);
    nrfx_err_t err = nrfx_uarte_init(&instance, &config, NULL); 
    }

char read(void) {
    char input_string[1];
    read_string(input_string, sizeof(input_string));
    return input_string[0];
}

void uart_transmit_string(const char *string) {
    
    while (*string != '\0') {
        uart_transmit_char(*string);
        string++;
    }
}
void received_char(char c) {
    if (c == 'w' && move != 's') {
        move = 'w';
    } else if (c == 's' && move != 'w') {
        move = 's';
    } else if (c == 'd' && move != 'a') {
        move = 'd';
    } else if (c == 'a' && move != 'd') {
        move = 'a';
    } else if (c == 'q') {
        running = false;
    }
}

void uarte_handler(nrfx_uarte_event_t const * p_event, void * p_context) {
    if (p_event->type == NRFX_UARTE_EVT_RX_DONE) {
        char c = (char)p_event->data.rx.p_data[0];
        received_char(c);
        nrfx_uarte_rx(&instance, p_event->data.rx.p_data, 1);
    }
}


void print_board(int player_x, int player_y) {
    char board[BOARD_SIZE_Y][BOARD_SIZE_X * 2 + 1];

    for (int i = 0; i < BOARD_SIZE_Y; i++) {
        for (int j = 0; j < BOARD_SIZE_X * 2; j += 2) {
            board[i][j] = '.';
            board[i][j + 1] = ' ';
        }
        board[i][BOARD_SIZE_X * 2] = '\0';
    }

    board[player_y][player_x * 2] = 'P';

    for (int i = 0; i < BOARD_SIZE_Y; i++) {
        uart_transmit_string(board[i]);
        uart_transmit_string("\n\r");
    }
}

void start_game_grupp26() {
    int player_x = 0;
    int player_y = 0;

    uart_transmit_string("Välkommen till spelet!\n\r");
    uart_transmit_string("Ditt mål är att besöka en slumpmässig punkt på brädet.\n\r");

    // Slumpmässigt generera målpunkten
    srand(time(NULL));
    int goal_x = rand() % BOARD_SIZE_X;
    int goal_y = rand() % BOARD_SIZE_Y;

    while (running) {
        print_board(player_x, player_y);

        if (player_x == goal_x && player_y == goal_y) {
            uart_transmit_string("\ngrattis! du har nått målpunkten.\n\r");
            return;
        }

        uart_transmit_string("\nange din nästa drag (w = upp, s = ner, a = vänster, d = höger, q = avsluta): \n\r");

        move = read();
        

        switch (move) {
            case 'w':
                if (player_y > 0) {
                    player_y--;
                }
                break;
            case 's':
                if (player_y < BOARD_SIZE_Y - 1) {
                    player_y++;
                }
                break;
            case 'a':
                if (player_x > 0) {
                    player_x--;
                }
                break;
            case 'd':
                if (player_x < BOARD_SIZE_X - 1) {
                    player_x++;
                }
                break;
            case 'q':
                uart_transmit_string("Spelet avslutas.\n\r");
                running = false;
                break;
            default:
                uart_transmit_string("Ogiltigt drag. Försök igen.\n\r");
                break;
        }
    }
}

