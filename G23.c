#include <stdio.h>
#include <assert.h>
#include <nrfx.h>
#include <nrf5340_application.h>
#include <nrfx_config.h>
#include <nrf.h>
#include <nrfx_uarte.h>
#include <stdlib.h>
#include <string.h>
#include <nrfx_systick.h>
#include <time.h>
#include <G23.h>

#define UART_INSTANCE_ID 0 
#define UART_TX_PIN 20
#define UART_RX_PIN 22 
#define GRID_SIZE 20

typedef struct {
    int x, y;
} Position;

static nrfx_uarte_t uarte_instance = NRFX_UARTE_INSTANCE(UART_INSTANCE_ID);
static Position Player[1];
static int Player_length = 1;
static char dir;
static bool running = true;
static int points = 0;

void uart_send(const char *message) { 
    nrfx_uarte_tx(&uarte_instance, (uint8_t *)message, strlen(message), 0);
    while (nrfx_uarte_tx_in_progress(&uarte_instance));
}

void clear_screen() {
    char clear_command[8] = "\033[2J\033[H";
    uart_send(clear_command);
}


void draw_grid() {
    char buffer[(GRID_SIZE * 3 + 2) * GRID_SIZE + 50];
    int index = 0;

    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) { 
            bool is_Player = false; 


            for (int k = 0; k < 1; k++) {
                if (Player[k].x == j && Player[k].y == i) { 
                    is_Player = true;
                    break;
                }
            }


            buffer[index++] = is_Player ? 'P' : '.';
            buffer[index++] = ' ';
        }

        buffer[index++] = '\r';
        buffer[index++] = '\n';
    }

    buffer[index] = '\0';
    uart_send(buffer);
}




void handle_received_char(char c) {
    if (c == 'w' && dir != 's') {
        dir = 'w';
    } else if (c == 's' && dir != 'w') {
        dir = 's';
    } else if (c == 'a' && dir != 'd') {
        dir = 'a';
    } else if (c == 'd' && dir != 'a') {
        dir = 'd';
    } else if (c == 'q') {
        running = false;
    }
}

void uarte_event_handler(nrfx_uarte_event_t const * p_event, void * p_context) {
    if (p_event->type == NRFX_UARTE_EVT_RX_DONE) {
        char c = (char)p_event->data.rx.p_data[0];

        handle_received_char(c);


        nrfx_uarte_rx(&uarte_instance, p_event->data.rx.p_data, 1);
    }
}

void init_Spelare() {
    Player[0].x = GRID_SIZE / 3;
    Player[0].y = GRID_SIZE / 3;
}



void move_Player() {
    Position new_head = Player[0];

    switch (dir) {
        case 'w': new_head.y--; break;
        case 's': new_head.y++; break;
        case 'a': new_head.x--; break;
        case 'd': new_head.x++; break;
    }

    if (check_wall_collision(new_head) == 1) {
        running = false;
        return;
    }

    if (check_win_condition(new_head) == 1) {
        running = false;
        return;
    }


    update_player_position(new_head);
}

int check_wall_collision(Position head) {
    if (head.x < 0 || head.x >= GRID_SIZE || head.y < 0 || head.y >= GRID_SIZE) {
        return 1;
    }
    return 0;
}

int check_win_condition(Position head) {
    if (head.x == GRID_SIZE / 2 && head.y == GRID_SIZE / 2) {
        char win[20] = "Congrats you Won!";
        uart_send(win);
        return 1;
    }
    return 0;
}

void update_player_position(Position new_head) {

    for (int i = 1; i > 0; i--) {
        Player[i] = Player[i - 1];
    }

    Player[0] = new_head;
}





int start_game_grupp20(void) {
    nrfx_err_t err;
    nrfx_uarte_config_t uarte_config = NRFX_UARTE_DEFAULT_CONFIG(UART_TX_PIN, UART_RX_PIN);
    err = nrfx_uarte_init(&uarte_instance, &uarte_config, uarte_event_handler);
    assert(err == NRFX_SUCCESS);


    nrfx_systick_init();

    NVIC_ClearPendingIRQ(NRFX_IRQ_NUMBER_GET(NRF_UARTE_INST_GET(UART_INSTANCE_ID)));
    NVIC_EnableIRQ(NRFX_IRQ_NUMBER_GET(NRF_UARTE_INST_GET(UART_INSTANCE_ID)));

    static char rx_buffer;
    err = nrfx_uarte_rx(&uarte_instance, &rx_buffer, 1);
    assert(err == NRFX_SUCCESS);

    init_Spelare();

    while (running) {
        clear_screen();
        draw_grid();
        move_Player();
        nrfx_systick_delay_ms(100);
        
    }

    nrfx_uarte_uninit(&uarte_instance);
    return 0;
}
