#include "game_grupp3.h"

int new_data = 0;

void start_game_grupp3 (void)
{
    int X = 0;
    int Y = 0;
    int A = 3;
    int B = 7;
    int C = 5;
    int AB = 1;
    int BA = 1;
    int AC = 1;
    int BC = 1;
    int points = 0;
    int run = 1;
    uint8_t string_X[] = "X ";
    uint8_t string_O[] = "O ";
    uint8_t string_Z[] = "Z ";
    uint8_t newline[] = "\r\n";
    uint8_t clearscreen[] = CLEAR_SCREEN;
    uint8_t win_msg[] = "Congratulations! You won!";
    uint8_t end_msg[] = "You got ";
    uint8_t point_msg[] = "points";



    

    init_gpio();

    while(run)
    {
        uarte_write(clearscreen);
        
        nrfx_uarte_rx(&instance, &uarte_buffer, sizeof(uarte_buffer));
        if (new_data)
        {
            switch(uarte_buffer) //Position i X & Y led vid stegning
            {
                case 'i':
                
                    if (Y > 0)
                    {
                        Y--;
                    }
                    break;
                
                case 'j':
                
                    if (X > 0)
                    {
                        X--;
                    }
                    break;
                
                case 'k':
                
                    if (Y < 9)
                    {
                        Y++;
                    }
                    break;
                
                case 'l':
                
                   
                    if (X < 9)
                    {
                        X++;
                    }
                    break;
                
                case 'q':
                
                    run = 0;
                    break;
                
            }
        }
        for (int i = 0; i < 10; i++)
        {
            for (int j = 0; j < 10; j++) //Platserna för poängen samt att dem ändras till O när man passerat positionen
            {
                if (i == Y && j == X)
                {
                    uarte_write(string_X);
                }
                else if (i == A && j == B && AB == 1)
                {
                    uarte_write(string_Z);
                }
                else if (i == B && j == A && BA == 1)
                {
                    uarte_write(string_Z);
                }
                else if (i == A && j == C && AC == 1)
                {
                    uarte_write(string_Z);
                }
                else if (i == B && j == C && BC == 1)
                {
                    uarte_write(string_Z);
                }
                else
                {
                    uarte_write(string_O);
                }
            }
            uarte_write(newline);
        }

        //Antal poäng, max 4p
        if (Y == A && X == B)
        {
            if (AB)
            {
                points++;
                AB = 0;
            }
        }
        
        if (Y == B && X == A)
        {
            if (BA)
            {
                points++;
                BA = 0;
            }
        }
        if (Y == A && X == C)
        {
            if (AC)
            {
                points++;
                AC = 0;
            }
        }
        if (Y == B && X == C)
        {
            if (BC)
            {
                points++;
                BC = 0;
            }
        }

        //Skrive ut meddelande samt poäng
        if (X == 9 && Y == 9)
        {
            if (points == 4)
            {
                uarte_write(win_msg);
                uarte_write(newline);
                uarte_write(end_msg);
                send_int(points);
                uarte_write(point_msg);
                run = 0;
            }
            else
            {
                uarte_write(end_msg);
                send_int(points);
                uarte_write(point_msg);
                run = 0;
            }
            
        }
        new_data = 0;
        nrfx_systick_delay_ms(500);
    }

    
}

void send_int(int* test_int)
{
    uint8_t ny_string[20];
    memset(ny_string, '\0', sizeof(ny_string));
    sprintf(ny_string, "%d ", test_int);
    uarte_write(ny_string);
}

void uarte_handler(nrfx_uarte_event_t const *p_event, void *p_context)
{
    nrfx_uarte_t * p_inst = p_context;
    if (p_event->type == NRFX_UARTE_EVT_RX_DONE)
    {
        new_data = 1;
    }
}