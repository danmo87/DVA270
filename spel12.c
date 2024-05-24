#include "spel.h"

#define GRID_height 1
#define GRID_width 5
 int running = 1;
int points = 0;

void move_up(int Y) {
    if (Y > 0) Y--;
}

void move_down(int Y) {
    if (Y < GRID_height - 1) Y++;
}

int move_left(int X) {
    if (X > 0) X--;
    return X;
}

int move_right(int X) {
    if (X < GRID_width - 1) X++;
    return X;
}

void stop_running(void) {
    running = 0;
}




int is_running(void) {
    return running;
}

void print_grid(int Y, int X) {
    char buffer[GRID_width * GRID_height * 3 + GRID_height + 1];  // 3 chars per cell, plus newline, plus null terminator
    char *ptr = buffer;

    for (int i = 0; i < GRID_height; i++)
    {
        for (int j = 0; j < GRID_width; j++)
        {
            if (i == Y && j == X)
            {
                ptr += sprintf(ptr, "X ");
            }
            else
            {
                ptr += sprintf(ptr, ". ");
            }
        }
        ptr += sprintf(ptr, "\n\r");
    }
    uarte_write(buffer,strlen(buffer));
    
}

int Game_Checker(int X, int random)
{
   if (X == random)
   {
      return 0;
   }
      
    else 
    return 1;
    

}

int Game(int x)
{
    srand(time(NULL));

    int random_number = rand()%5;



    return Game(x, random_number);
}

void start_game_grupp12(int choice)
{    
    
    if (Game(choice) == 0)
    {
        uint8_t msg[] = "YOU LOSE";
        uarte_write(msg, strlen(msg));
    }
    else if (points==3)
    {
        uint8_t msg1[] = "YOU WIN";
        uarte_write(msg1, strlen(msg1));
    }
    else 
    points++;

}