#include <24.h>

uint8_t gamearea[2][16] = {{"Card 1 2 3 4\n\r"},\
						   {"     * * * *\n\r"}};
						   
//* = unturned card
//card 1 = [1][5], card 2 = [1][7], card 3 = [1][9], card 4 = [1][11]

uint8_t game_area_cards[4] = {'7', '4', '5', '9'}; //cards, card 1 = 7

int playerIndex = 5, terminator = 1, timesRun = 0;


void uarte_write(uint8_t* data, uint8_t length){

    nrfx_uarte_tx(&instance, data, length, 0);
	while(nrfx_uarte_tx_in_progress(&instance)){};
}

//LAB 4

void uarte_handler(nrfx_uarte_event_t const *p_event, void *p_context)
{
	nrfx_uarte_t * p_inst = p_context;
    if (p_event->type == NRFX_UARTE_EVT_RX_DONE)
    {
		switch(uarte_buffer){
			case 'a':
				playerIndex = 1;
				gamearea[1][5] = game_area_cards[0];
				timesRun++;
				break;
			case 's':
				playerIndex = 2;
				gamearea[1][7] = game_area_cards[1];
				timesRun++;
				break;
			case 'd':
				playerIndex = 3;
				gamearea[1][9] = game_area_cards[2];
				timesRun++;
				break;
			case 'f':
				playerIndex = 4;
				gamearea[1][11] = game_area_cards[3];
				timesRun++;
				break;
			case 'q':
				terminator = 0;
				break;
		}
    }
}

void start_game_grupp24(void){
	int losing_index;
	nrfx_err_t errr = nrfx_uarte_init(&instance, &config, uarte_handler);
	NVIC_ClearPendingIRQ(NRFX_IRQ_NUMBER_GET(NRF_UARTE_INST_GET(0)));
	NVIC_EnableIRQ(NRFX_IRQ_NUMBER_GET(NRF_UARTE_INST_GET(0)));
	nrfx_systick_init();

	decide_losing_index(&losing_index);
	while(terminator){
		if(timesRun == 3){
			uint8_t win_msg[] = "\r\nYou WIN!\r\n";
			uarte_write(win_msg, sizeof(win_msg));
			terminator = 0;
		}
		else{	
			print_instructions();
			print_grid(gamearea);
			nrfx_uarte_rx(&instance, &uarte_buffer, sizeof(uarte_buffer));
			print_grid(gamearea);
			nrfx_systick_delay_ms(500);
			lose_check(losing_index);
		}
	}

}

void decide_losing_index(int* losing_index){
	srand(time(NULL));
	int index = rand()%5 +1; //gives 1 - 4
	*losing_index = index;
}

void print_instructions(void){
	uint8_t instructions[] = "a = card 1, s = card 2, d = card 3, f = card 4\r\n";
	uarte_write(instructions, sizeof(instructions));
}

void print_grid(uint8_t gamearea[2][16]){
	for(int row = 0; row < 2; row++){
		for(int lane = 0; lane < 15; lane++){
			uarte_write(&gamearea[row][lane], sizeof(gamearea[row][lane]));
		}
	}
}

void lose_check(int losing_index){
	uint8_t lose_msg[] = "\r\nYou lose\r\n";
	if(losing_index == playerIndex){
		uarte_write(lose_msg, sizeof(lose_msg));
		terminator = 0;
	}
}


