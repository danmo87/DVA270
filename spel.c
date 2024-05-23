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
#include <spel.h>

#define UART_INSTANCE_ID 0 // instance 0 istället för att skriva 0 alltid
#define UART_TX_PIN 20  // TX pin
#define UART_RX_PIN 22  // RX pin
#define GRID_SIZE 20 // grid size locked to 20
#define MAX_SNAKE_LENGTH 100 // max orm längd 

typedef struct {
    int x, y;
} Position;

static nrfx_uarte_t uarte_instance = NRFX_UARTE_INSTANCE(UART_INSTANCE_ID);
static Position snake[MAX_SNAKE_LENGTH];
static int snake_length = 1; // ssätter första längden på ormen till 1 för ormen kan inte vara 0
static Position food; // positionen på maten
static char vag; //direction på ormen
static bool running = true; //vår kondition to infinite-ish loop
static int points = 0; // var för att lagra poäng för att skriva ut

// send string function omskriven från förra labbar
void uart_send(const char *message) { 
    nrfx_uarte_tx(&uarte_instance, (uint8_t *)message, strlen(message), 0);
    while (nrfx_uarte_tx_in_progress(&uarte_instance));
}

/* skickar ansci exit code för att kleara screenen /033 2J är för att cleara terminal 
och 033 H för att reseta cursor */
void clear_screen() {
    // size 8 för att deras size är 8
    char clear_command[8] = "\033[2J\033[H";
    // skickar kommandon till skärmen
    uart_send(clear_command);
}

// draw grid funktionen för att skriva ut 2d arrayen
void draw_grid() {
    char buffer[(GRID_SIZE * 3 + 2) * GRID_SIZE + 50]; // Buffret storlek är så så att den ska hålla alla karaktärer och newline och carriage retur
    int index = 0;

    for (int i = 0; i < GRID_SIZE; i++) { // kör för första deminsionen
        for (int j = 0; j < GRID_SIZE; j++) { // kör för andra deminsionen
            bool is_snake = false; // check om det snake bool var
            for (int k = 0; k < snake_length; k++) {
                if (snake[k].x == j && snake[k].y == i) { // om det är snake ändra bool varren till true
                    is_snake = true;
                    break;
                }
            }
            if (is_snake) { // om snake är i den pos printa S
                buffer[index++] = 'S';
            } else if (food.x == j && food.y == i) { // om food är i den positionen printa F
                buffer[index++] = 'F';
            } else { // annars .
                buffer[index++] = '.';
            }
            buffer[index++] = ' '; // mellanrum imellan
        }
        // newline and start of line
        buffer[index++] = '\r';
        buffer[index++] = '\n';
    }
    // lägger till pointsen i slutet av buffret
    index += sprintf(&buffer[index], "Points: %d\r\n", points);
    buffer[index] = '\0'; // avslutaar
    uart_send(buffer); // skriver ut med funk send
}

// lägger mat
void place_food() {
    srand(time(NULL));
    food.x = rand() % GRID_SIZE;
    food.y = rand() % GRID_SIZE;
}

//(från canvas)
void uarte_event_handler(nrfx_uarte_event_t const * p_event, void * p_context) {
    // Om händelsen är av typen "mottagning klar"
    if (p_event->type == NRFX_UARTE_EVT_RX_DONE) {
        // Konvertera det första mottagna byte till en char och lagra i variabeln c
        char c = (char)p_event->data.rx.p_data[0];
        switch (c) {
            case 'w': if (vag != 's') vag = 'w'; break;// kollar så att vi inte går i två motsatta hål samtidigt och vår dir var ändras till hållet
            case 's': if (vag != 'w') vag = 's'; break;// samma
            case 'a': if (vag != 'd') vag = 'a'; break;// samma 
            case 'd': if (vag != 'a') vag = 'd'; break;// samma
            case 'q': running = false; break;
            default: break;
        }
        nrfx_uarte_rx(&uarte_instance, p_event->data.rx.p_data, 1); // sätt på rx igen
    }
}

// spawnar ormen i mitten av gridden
void init_snake() {
    snake[0].x = GRID_SIZE / 2;
    snake[0].y = GRID_SIZE / 2;
    place_food(); // callar mat
}

// rörlse
void move_snake() { // samma som förb uppgift
    Position new_head = snake[0];
    switch (vag) { // beroende på dir varren så rör ormen sig och fortsätter röra sig i den dir
        case 'w': new_head.y--; break; 
        case 's': new_head.y++; break;
        case 'a': new_head.x--; break;
        case 'd': new_head.x++; break;
    }

    // krock med väggar
    if (new_head.x < 0 || new_head.x >= GRID_SIZE || new_head.y < 0 || new_head.y >= GRID_SIZE) { // om huvudet på x,y är mindre eller större än grid storlek running = false
        running = false;
        return;
    }

    /*for (int i = 0; i < snake_length; i++) {
        if (new_head.x == snake[i].x && new_head.y == snake[i].y) {
            running = false;
            return;
        }
    }*/

    // röra ormen omvänt i arrayen
    for (int i = snake_length; i > 0; i--) {
        // rör elementerna till höger med en pos
        snake[i] = snake[i - 1];
    }
    // assignar nya huvudet
    snake[0] = new_head;

    // om orm påträffar mat öka längd och lägg mat igen
    if (new_head.x == food.x && new_head.y == food.y) {
        points++;
        snake_length++;
        place_food();
    }
}

int start_game_grupp20(void) {
    //init uart
    nrfx_err_t err;
    nrfx_uarte_config_t uarte_config = NRFX_UARTE_DEFAULT_CONFIG(UART_TX_PIN, UART_RX_PIN);
    err = nrfx_uarte_init(&uarte_instance, &uarte_config, uarte_event_handler);
    assert(err == NRFX_SUCCESS);

    // init systick
    nrfx_systick_init();
    // (från canvas) tömma pågåande inturrupts och sätta på inturrupts
    NVIC_ClearPendingIRQ(NRFX_IRQ_NUMBER_GET(NRF_UARTE_INST_GET(UART_INSTANCE_ID)));
    NVIC_EnableIRQ(NRFX_IRQ_NUMBER_GET(NRF_UARTE_INST_GET(UART_INSTANCE_ID)));

    static char rx_buffer; // Deklarera en statisk buffert för mottagna data
    err = nrfx_uarte_rx(&uarte_instance, &rx_buffer, 1); // Init en mottagning
    assert(err == NRFX_SUCCESS);

    // initialize ormen
    init_snake();

    while (running) { // vår infinite ish loop
        clear_screen(); // tömm skärmen med ansi escape codes
        draw_grid(); // ritar gridden och snaken och pointsen 
        move_snake(); // kollar rörlse på ormen
        nrfx_systick_delay_ms(100); // delay så att det inte sker för nsabbt
        if (points == 10) // win state som labbet kräver
        {
            char win[20] = "Congrats you Won!";
            uart_send(win);
            running = false;
        }
        
    }

    // un init uarte instance
    nrfx_uarte_uninit(&uarte_instance);
    return 0;
}
