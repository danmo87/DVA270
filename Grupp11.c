#include "Grupp11.h"


void uarte_write(uint8_t* data, uint8_t length)
{
    nrfx_uarte_tx(&instance, data, length, 0);
}



void print_game_status(int user_score, int computer_score)
{
    char status_msg[100];
    uint8_t status_length = sprintf(status_msg, "Current scores: ME %d, Computer %d\r\n", user_score, computer_score);
    uarte_write((uint8_t*)status_msg, status_length);
}


char uarte_read()
{
    if (nrfx_uarte_rx_ready(&instance, 0))
    {
        char received_char;
        nrfx_uarte_rx(&instance, (uint8_t*)&received_char, sizeof(received_char));
        return received_char;
    }
    return '\0';
}


void start_game_grupp11(char user_choice, int *user_score, int *computer_score)
{
    char choices[] = {'s', 'x', 'p'};
    char computer_choice = choices[rand() % 3];
    char print[100];
    uint8_t print_length;

    if (user_choice == computer_choice)
    {
        print_length = sprintf(print, "its a tie! Someone has to win!\r\n");
    }
    else if ((user_choice == 's' && computer_choice == 'x') ||
             (user_choice == 'x' && computer_choice == 'p') ||
             (user_choice == 'p' && computer_choice == 's'))
    {
        print_length = sprintf(print, "You win! You are the master!\r\n");
        (*user_score)++;
    }
    else
    {
        print_length = sprintf(print, "Computer wins! you noob\r\n");
        (*computer_score)++;
    }

    uarte_write((uint8_t*)print, print_length);
    
}

void init_systick_and_uart() {
    
    nrfx_systick_init();
    nrfx_rtc_enable(&rtc_instance);
    const nrfx_uarte_config_t config = NRFX_UARTE_DEFAULT_CONFIG(PIN_TXD, PIN_RXD);
    //nrfx_err_t err = nrfx_uarte_init(&instance, &config, NULL); //old way
    nrfx_err_t err = nrfx_uarte_init(&instance, &config, start_game_grupp11); //with interupts
    if (err != NRFX_SUCCESS) {
    }
}