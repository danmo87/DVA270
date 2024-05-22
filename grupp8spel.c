#include "grupp8spel.h"

extern int newData; // håller koll på om det finns nån ny data att läsa eller ej

void start_game_grupp8()
{
    NVIC_ClearPendingIRQ(NRFX_IRQ_NUMBER_GET(NRF_UARTE_INST_GET(0)));
	NVIC_EnableIRQ(NRFX_IRQ_NUMBER_GET(NRF_UARTE_INST_GET(0)));
	
	const nrfx_uarte_config_t config = NRFX_UARTE_DEFAULT_CONFIG(PIN_TXD, PIN_RXD);

	nrfx_err_t errr = nrfx_uarte_init(&instance, &config, uarte_handler);

	nrfx_systick_init();
    
    uint8_t clearscreen[] = CLEAR_SCREEN;

    //^initierar det som behövs
    
    //textmeddelanden vi kan vilja ge användaren
    char winMsg[] = {"You won!"};
	char text1[] = {"\r\n"};
    
	char grid[10][10]; //vår 10x10 spelplan
    
	
    //våra kordinater
	int X = 0;
	int Y = 0;

	srand(8);

	uarte_write(clearscreen, sizeof(clearscreen));

	int numbOfFood = 0; // håller koll på om vi "ätit" eller ej

    //sätter "maten" på slumpmässiga kordinater
	int randX = rand()%9;
	int randY = rand()%9;

	while(1){

		nrfx_uarte_rx(&instance, &uarte_buffer, sizeof(uarte_buffer)); //letar hela tiden efter input
		
		uarte_write(clearscreen, sizeof(clearscreen));

		if(newData == 1) //switchen körs bara om det finns data att läsa från putty
		{
            
            //Vi styr vår gubbe i(upp), j(vänster), k(nedåt), l(höger). 'q' avslutar spelet. 
			switch(uarte_buffer)
			{
				case 'i': 
					if(Y > 0)
					{
						Y--; 
					}
				
				break;
				case 'k': 
					if(Y < 9)
					{
						Y++; 
					}
				
				break;
				case 'j': 
					if(X > 0)
					{
						X--; 
					}
				
				break;
				case 'l': 
					if(X < 9)
					{
						X++; 
					}
				
				break;
				case 'q': return;
			}
			newData = 0; //Återställer newData så att den ska vänta på input igen innan switchen aktiveras
		}
		

		for(int i = 0; i < 10; i++) // fyller alla arrays med '.'
		{
			memset(grid[i], '.', sizeof(grid[i]));
		}

		grid[randY][randX] = 'O'; //Sätter ut 'maten' på de slumpmässiga koordinaterna

		if(Y == randY && X == randX)
		{
			numbOfFood++; //Ifall spelaren befinner sig på 'matens' koordinater, räknas den 'maten' tagen.
		}

		grid[Y][X] = 'x'; //Spelarens nuvarande koordinater visar sig som 'X'


		for(int j = 0; j < 10; j++) //Printar ut grid
		{
			uarte_write(grid[j], sizeof(grid[j]));
			uarte_write(text1, sizeof(text1));
		}

		if(numbOfFood >= 1) //Kollar ifall spelaren har tagit 'maten', isånnafall har den vunnit.
		{
			uarte_write(text1, sizeof(text1));
			uarte_write(winMsg, sizeof(winMsg));

			return;
		}

		nrfx_systick_delay_ms(500); //Väntar en halvsekund innan programmet fortsätter.

	};
}


void uarte_handler(nrfx_uarte_event_t const *p_event, void *p_context)
{
    nrfx_uarte_t * p_inst = p_context;
    if (p_event->type == NRFX_UARTE_EVT_RX_DONE)
    {
    	newData = 1; //Ifall newData är 1 innebär det att det finns data att läsa från putty.
    }
}