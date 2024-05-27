#include "grupp10.h"
nrfx_uarte_t instance = NRFX_UARTE_INSTANCE(0);





void stop_running(void)
{
    running = 0;
}

int is_running(void)
{
    return running;
}

void uarte_handler(nrfx_uarte_event_t const * p_event, void * p_context)
{
    if (p_event->type == NRFX_UARTE_EVT_RX_DONE)
    {
        nrfx_uarte_rx(&instance, &uarte_buffer, 1);
        switch (uarte_buffer)
        {
            case 's':  // sten
            case 'x':  // sax
            case 'p':  // påse
                play_game(uarte_buffer);
                break;
            case 'q':
                stop_running();
                break;
        }
    }
}

void send_message(const char * message)
{
    nrfx_uarte_tx(&instance, (uint8_t *)message, strlen(message), 0);
    while(nrfx_uarte_tx_in_progress(&instance)){};
}



void play_game(char user_choice)
{
    char choices[] = {'s', 'x', 'p'};
    char computer_choice = choices[rand() % 3];  // Randomly select 's', 'x', or 'p'
    char print [100];
    uint8_t print_lenght;

    // Skriv ut både användarens och datorns val samtidigt
    print_lenght = sprintf(print, "Du valde: %c, Datorn valde: %c\n\r", user_choice, computer_choice);
    nrfx_uarte_tx(&instance, print, print_lenght, 0);

    if (user_choice == computer_choice) {
        print_lenght = sprintf(print, "Det är oavgjort!\n\r");
        nrfx_uarte_tx(&instance, print, print_lenght, 0);
    } else if ((user_choice == 's' && computer_choice == 'x') ||
               (user_choice == 'x' && computer_choice == 'p') ||
               (user_choice == 'p' && computer_choice == 's')) {
        print_lenght = sprintf(print, "du vinner!\n\r");
        nrfx_uarte_tx(&instance, print, print_lenght, 0);
        user_score++;
    } else {
        print_lenght = sprintf(print, "dator vinner!\n\r");
        nrfx_uarte_tx(&instance, print, print_lenght, 0);
        computer_score++;
    }
}

void start_game_grupp10(void)
{
nrfx_uarte_rx(&instance, &uarte_buffer, 1);
    
    while (is_running() && (user_score < 5) && (computer_score < 5)) // Kontrollera även poäng
    {
        //uint8_t clear_screen[] = CLEAR_SCREEN;
       // nrfx_uarte_tx(&instance, clear_screen, sizeof(clear_screen), 0);
        nrfx_systick_delay_ms(1000);  // Delay to wait for user input

        char score_message[50];
        sprintf(score_message, "Poäng: Du - %d, Dator - %d\n\r", user_score, computer_score);
        send_message(score_message);
    }

    if (user_score >= 5) {
        
        char Result_message[] = "Grattis! Du har vunnit spelet!\n\r";
        send_message(Result_message);
    } else if (computer_score >= 5) {
        char Result_message[] = "tyvär! Dator har vunnit spelet!\n\r";
        send_message(Result_message);
    }

    
    char End_message[] = "Programmet avslutas!\n\r";
    send_message(End_message);
    return 0;
}