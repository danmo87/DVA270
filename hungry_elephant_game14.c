//c-fil

#include "14.h"
#include "hungry_elephant_game14.h"

#define toWin 10

static nrfx_uarte_t instance = NRFX_UARTE_INSTANCE(0);
static uint8_t uarte_buffer; //lagrar inputs

void uarte_handler(nrfx_uarte_event_t const *p_event, void *p_context)
{
	nrfx_uarte_t * p_inst = p_context;
	if (p_event->type == NRFX_UARTE_EVT_RX_DONE)
	{
		//gör inget
	}
}

void start_game_grupp14() {
	
	uint8_t grid[12][12]; 
	uint8_t str[200] = "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\r\n\n\rYou ar an hungry elephant on a potatoe-field!\n\rYou need to eat potatoes to geet full and not become hungry forever! :(\n\rGood Luck!\n\rPress 's' to start game!\n\r";
	uint8_t clearscreen[] = CLEAR_SCREEN;
	uint8_t x = 1;
	uint8_t y = 1;
	uint8_t _x = 5;
	uint8_t _y = 5;
	int running = 0;
	int score = 0;

	memset(grid, '\0', sizeof(grid)); 

	//uarte_write_clear();
	uarte_write(str, sizeof(str));
	while (running == 0) {
		nrfx_uarte_rx(&instance, &uarte_buffer, sizeof(uarte_buffer));
		if (uarte_buffer == 's') {
			running = 1;
		}
	}
	
	
	while (running) {
		//bygg karta
		sprintf(grid[0], "Potatoes!!\n\r", score);
		sprintf(grid[11], "\n\rScore: %d\n\r", score);
		for (int i = 1; i < 11; i++) {
			for (int n = 0; n < 10; n++) {
				grid[i][n] = '_';
			}
			grid[i][10] = '\n';
			grid[i][11] = '\r';
		}

		//ange kordinater:
		if (x == _x && y == _y) {
            _x = (rand() % 10);
            _y = (rand() % 9 + 1);
			score++;
			if (score >= toWin) {
				running = 0;
				sprintf(str, "You Win, Score: %d", score);
			}
        }
		grid[y][x] = '@';
		grid[_y][_x] = 'o'; //apple

		//nrfx_uarte_tx(&instance, clearscreen, sizeof(clearscreen), 0);
		nrfx_uarte_tx(&instance, grid, sizeof(grid), 0);
		
		nrfx_systick_delay_ms(100);

		//uarte_buffer = 0;
		nrfx_uarte_rx(&instance, &uarte_buffer, sizeof(uarte_buffer));
		//ändra kordinater
		switch (uarte_buffer) {
			case 'l': //höger
			memset(str, '\0', sizeof(str));
			sprintf(str, "höger\n\r");
			if (x < 9) {
				x++;
			}
			break;
			case 'j': //vänster
			memset(str, '\0', sizeof(str));
			sprintf(str, "vänster\n\r");
			if (x > 0) {
				x--;
			}
			break;
			case 'i': //upp
			memset(str, '\0', sizeof(str));
			sprintf(str, "upp\n\r");
			if (y > 1) {
				y--;
			}
			break;
			case 'k': //ner
			memset(str, '\0', sizeof(str));
			sprintf(str, "ner\n\r");
			if (y < 10) {
				y++;
			}
			break;
			case 'q': //avsluta
			memset(str, '\0', sizeof(str));
			sprintf(str, "Färdigspelat\n\r");
			running = 0;
			break;
			case 's': //avsluta
			memset(str, '\0', sizeof(str));
			sprintf(str, "Spela\n\r");
			running = 1;
			//score = 0;
			break;
		}
	}
	if (score >= toWin) {
		sprintf(str, "You Win! with a fabolous score of %d\n\rThis was a game of a elephant eating potatoes", score);
		nrfx_uarte_tx(&instance, str, sizeof(str), 0);
	}
	//uarte_write_clear();
	nrfx_uarte_tx(&instance, str, sizeof(str), 0);
}