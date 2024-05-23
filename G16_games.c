#include <nrfx.h>
#include <nrf5340_application.h>
#include <nrfx_config.h>
#include <nrf.h>
#include <nrfx_uarte.h>
#include <stdlib.h>
#include <nrfx_systick.h>
#include "G16_term.h"
#include "G16_list.h"

#define GAMEAREA_HEIGHT 6
#define GAMEAREA_WIDTH 20

//Game area width and height (for snake)
#define WIDTH 20
#define HEIGHT 20

//Boolean flags for input
static bool inputFlag = false;
static bool rxWait = false;

//Callback function for uart interrupts
void rxHandler(nrfx_uarte_event_t const *p_event, void *p_context)
{
    nrfx_uarte_t * p_inst = p_context;

    if (p_event->type == NRFX_UARTE_EVT_RX_DONE)
    {
        inputFlag = true;
    }
}

int encode_segment_pos_data(int x,int y)
{
    return (y*WIDTH) + x;
}
//decode "position data" to x and y
void decode_segment_pos_data(int* x,int* y, int data)
{
    *x = data % WIDTH;
    *y = data / WIDTH;
}

//implementation of "real" modulo, which doesnt return negative numbers
int mod(int a, int b)
{
   if(b < 0) //you can check for b == 0 separately and do what you want
     return -mod(-a, -b);
   int ret = a % b;
   if(ret < 0)
     ret+=b;
   return ret;
}

//check if the given x and y is colliding(shaing space) with the snake
int check_snake_collision(List snake, int x, int y)
{
    List iter = snake;

    do
    {
        int pos, xS, yS;
        pos = iter->data;
        decode_segment_pos_data(&xS, &yS, pos);

        if( x == xS && y == yS) return true;
    }
    while(iter = iter->next);

    return false;
}

//function for updating the snake
int move_snake(List* snake, int* dir, int* apple)
{
    int x, y, pos;

    //move the snakes head forward
    pos = get_first_element(*snake);
    decode_segment_pos_data(&x,&y,pos);
    x = mod(x + dir[0], WIDTH);
    y = mod(y + dir[1], HEIGHT);


    //if the snakes head is colliding with the snake, return -1 (lose)
    if(check_snake_collision(*snake, x, y)) return -1;
    
    //draw the snake head
    term_pos(2+x*2,2+y);
    term_bg_color(0,255,0);
    term_print("  ");
    add_first(snake,encode_segment_pos_data(x,y));

    //if the sakes head is in the same place as the apple
    if(apple[0] == x && apple[1] == y)
    {
        do{
        apple[0] = rand() % WIDTH;
        apple[1] = rand() % HEIGHT;

        }
        while(check_snake_collision(*snake, apple[0], apple[1]));

        term_pos(2+apple[0]*2,2+apple[1]);

        term_bg_color(255,0,0); 

        term_print("  ");

        term_reset();

        //return 1 (apple eaten score+)
        return 1;
    }

    //if apple was not eaten, remove the last segment of the snake

    pos = get_last_element(*snake);
    remove_last(snake);

    decode_segment_pos_data(&x,&y,pos);
    term_pos(2+x*2,2+y);
    term_bg_color(0,0,0);
    term_print("  ");
    term_reset();
    return 0;
}

//function for drawing rectangles in the console
void draw_rect(int xPos, int yPos, int rectWIDTH, int rectHEIGHT)
{
    for(int i = 0; i < rectWIDTH; i++)
    {
        term_pos(xPos+i, yPos);

        term_print(" ");

        term_pos(xPos+i, yPos+rectHEIGHT-1);

        term_print(" ");

    }

    for(int i = 0; i < rectHEIGHT; i++)
    {
        term_pos(xPos, yPos+i);

        term_print(" ");

        term_pos(xPos+rectWIDTH-1, yPos+i);

        term_print(" ");
    }
}

//start the snake game
void start_snake() 
{
    term_clear();

    term_bg_color(100,100,100);
    draw_rect(1,1,WIDTH*2+2,HEIGHT+2);
    term_reset();
    term_pos(0, HEIGHT+3);
    term_print("W: Move Up   S: Move Down   D: Move Right   A: Move Left\r\nQ: Quit");

    term_print(CURSOR_HIDE);

    int dir[2] = {1,0};
    int apple[2] ={0,0};
    int score = 0;
    int status = true;

    apple[0] = rand() % WIDTH;
    apple[1] = rand() % HEIGHT;

    term_pos(2+apple[0]*2,2+apple[1]);
    term_bg_color(255,0,0);
    term_print("  ");
    List snake; 
    term_reset();
    
    //create the snake
    snake = create_empty_list();
    add_last(&snake, encode_segment_pos_data(0,6));
    add_first(&snake, encode_segment_pos_data(0,6));
    add_first(&snake, encode_segment_pos_data(0,6));

    char input = '0';

    //main game loop
    while(true)
    {
        //if the program is not waiting for input
        if(!rxWait)
        {
            //get new input
            term_input_raw(&input, sizeof(input));
            rxWait = true;
        }

        //if input has arrived
        if(inputFlag)
        {
            //switch case for controlling the snake and quitting
            switch(input)
            {
                case('w'):
                    if(dir[1] == 1) break;
                    dir[0] = 0;
                    dir[1] = -1;
                    break;
                case('s'):
                    if(dir[1] == -1) break;
                    dir[0] = 0;
                    dir[1] = 1;
                    break;
                case('d'):
                    if(dir[0] == -1) break;
                    dir[0] = 1;
                    dir[1] = 0;

                    break;
                case('a'):
                    if(dir[0] == 1) break;
                    dir[0] = -1;
                    dir[1] = 0;
                    break;
                case('q'):
                    return;
                    break;
            }

            inputFlag = false;
            rxWait = false;
        }

        //wait for a while between updates, wait gets shorter as the score is increased
        nrfx_systick_delay_ms(150-(score/4));

        term_pos((WIDTH)-4,0);

        term_print("Score:");

        //print score
        term_print_int(score);

        //move the snake and act according to the return value
        switch(move_snake(&snake, dir, apple))
        {
            //game over
            case(-1):
                term_pos((WIDTH)-8, HEIGHT/2);
        
                term_print("Game Over, Score:");
        
                term_print_int(score);
                //wait for a while so the user has time to see their score
                nrfx_systick_delay_ms(4000);
                return;
                break;
            //apple eaten, increase score
            case (1):
                score += 10;
                break;
        }
    }
}

void setXYI(int* gameareaX, int* gameareaY, int* index, int setX, int setY, int setI)
{
    *gameareaX = setX;
    *gameareaY = setY;
    *index = setI;
}

void start_memory()
{
    term_clear();
    term_reset();

    char input = '0';

    int x;
    int y;
    int i;
    int gameareaX1;
    int gameareaY1;
    int index1;
    int gameareaX2;
    int gameareaY2;
    int index2;
    int turned = 0;
    int points = 0;
    int maxPoints = 10;
    char num1 = '+';
    char num2 = '-';
    char intArr[5 * 4] = { '1', '6', '9', '2', '8', '4', '0', '3', '6', '9', '4', '2', '7', '1', '5', '3', '8', '5', '7', '0' };
    char charArr[5 * 4] = { 'w', 'e', 'r', 't', 'y','u', 'i', 'o', 'p', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'z', 'x' };
    uint8_t gamearea[GAMEAREA_HEIGHT][GAMEAREA_WIDTH] = {{"-------------\n\r"}, \
                                                         {"| w e r t y |\n\r"}, \
                                                         {"| u i o p a |\n\r"}, \
                                                         {"| s d f g h |\n\r"}, \
                                                         {"| j k l z x |\n\r"}, \
                                                         {"-------------\n\r"}};

    while(true)
    {
        if(!rxWait)
        {
            term_input_raw(&input, sizeof(input));
            rxWait = true;
        }

        if(inputFlag)
        {
            // A switch-case for every letter in the gamearea.
            switch(input)
            {
                case('w'):
                    x = 1;      // Which array in gamearea the letter is in.
                    y = 2;      // Which place in the array the letter has.
                    i = 0;      // Which number in intArr the letter should be "replaced" by.
                    gamearea[x][y] = intArr[i];     // Replaces the letter in gamearea by the number in intArr ("turns the letter").
                    turned++;       // Adds one to turned to tell that one letter has been turned.
                    break;
                case('e'):
                    x = 1;
                    y = 4;
                    i = 1;
                    gamearea[x][y] = intArr[i];
                    turned++;
                    break;
                case('r'):
                    x = 1;
                    y = 6;
                    i = 2;
                    gamearea[x][y] = intArr[i];
                    turned++;
                    break;
                case('t'):
                    x = 1;
                    y = 8;
                    i = 3;
                    gamearea[x][y] = intArr[i];
                    turned++;
                    break;
                case('y'):
                    x = 1;
                    y = 10;
                    i = 4;
                    gamearea[x][y] = intArr[i];
                    turned++;
                    break;
                case('u'):
                    x = 2;
                    y = 2;
                    i = 5;
                    gamearea[x][y] = intArr[i];
                    turned++;
                    break;
                case('i'):
                    x = 2;
                    y = 4;
                    i = 6;
                    gamearea[x][y] = intArr[i];
                    turned++;
                    break;
                case('o'):
                    x = 2;
                    y = 6;
                    i = 7;
                    gamearea[x][y] = intArr[i];
                    turned++;
                    break;
                case('p'):
                    x = 2;
                    y = 8;
                    i = 8;
                    gamearea[x][y] = intArr[i];
                    turned++;
                    break;
                case('a'):
                    x = 2;
                    y = 10;
                    i = 9;
                    gamearea[x][y] = intArr[i];
                    turned++;
                    break;
                case('s'):
                    x = 3;
                    y = 2;
                    i = 10;
                    gamearea[x][y] = intArr[i];
                    turned++;
                    break;
                case('d'):
                    x = 3;
                    y = 4;
                    i = 11;
                    gamearea[x][y] = intArr[i];
                    turned++;
                    break;
                case('f'):
                    x = 3;
                    y = 6;
                    i = 12;
                    gamearea[x][y] = intArr[i];
                    turned++;
                    break;
                case('g'):
                    x = 3;
                    y = 8;
                    i = 13;
                    gamearea[x][y] = intArr[i];
                    turned++;
                    break;
                case('h'):
                    x = 3;
                    y = 10;
                    i = 14;
                    gamearea[x][y] = intArr[i];
                    turned++;
                    break;
                case('j'):
                    x = 4;
                    y = 2;
                    i = 15;
                    gamearea[x][y] = intArr[i];
                    turned++;
                    break;
                case('k'):
                    x = 4;
                    y = 4;
                    i = 16;
                    gamearea[x][y] = intArr[i];
                    turned++;
                    break;
                case('l'):
                    x = 4;
                    y = 6;
                    i = 17;
                    gamearea[x][y] = intArr[i];
                    turned++;
                    break;
                case('z'):
                    x = 4;
                    y = 8;
                    i = 18;
                    gamearea[x][y] = intArr[i];
                    turned++;
                    break;
                case('x'):
                    x = 4; 
                    y = 10;
                    i = 19;
                    gamearea[x][y] = intArr[i];
                    turned++;
                    break;
                case('q'):
                    term_print("Thank you for playing!");
                    return;
                    break;
            }
        }
        
        // If the letter you've turned is the first letter x, y and i should be saved to the first "card".
        if(turned == 1)
        {
            setXYI(&gameareaX1, &gameareaY1, &index1, x, y, i);
        }

        // If the letter you've turned is the second letter x, y and i should be saved to the second "card".
        if(turned == 2)
        {
            setXYI(&gameareaX2, &gameareaY2, &index2, x, y, i);
        }

        // If any letter has been turned:
        if(turned > 0)
        {
            // If no card has been turned before the number should be saved into num1.
            if(num1 == '+')
            {
                num1 = intArr[i];
            }
            // If num1 contains a number, the number should be saved into num2.
            else
            {
                num2 = intArr[i];
            }
        }
        
        term_print("\033cPoints: ");
        term_print_int(points);
        term_print("\n\r");

        // Prints the gamearea:
        for(int i = 0; i < GAMEAREA_HEIGHT; i++)
        {
            term_print(gamearea[i]);
        }

        // Win condition:
        if(points == maxPoints)
        {
            term_print("You won!");
            break;
        }

        // If you have turned two letters the numbers undernethe should be compared:
        if(turned == 2)
        {
            // If the numbers are the same:
            if(num1 == num2)
            {
                points++;       // you get a point.
            }
            // If they're not the same:
            else
            {
                gamearea[gameareaX1][gameareaY1] = charArr[index1];     // the first number resets to the letter (turns back).
                gamearea[gameareaX2][gameareaY2] = charArr[index2];     // the second number resets to the letter (turns back).
            }

            num1 = '+';     // num1 resets
            num2 = '-';     // num2 resets
            turned = 0;     // turned resets
        }

        inputFlag = false;
        rxWait = false;

        nrfx_systick_delay_ms(500);
    }
}

void start_game_grupp16()
{
    init_term_non_blocking(rxHandler);
    nrfx_systick_init();

    term_print("Group 16 has made two games, now Snake will be played.");
    nrfx_systick_delay_ms(3000);

    start_snake();

    term_print("Group 16s second game is Memory which will be played now.");
    nrfx_systick_delay_ms(3000);

    start_memory();
}