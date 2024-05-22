
#include <FakesnakeGrupp4.h>
nrfx_uarte_t instance = NRFX_UARTE_INSTANCE(0);
static uint8_t uarte_buffer;
static uint8_t string[];
static int changer = 0;
//NRFX_RTC_INSTANCE(0)

int read_number (char buffer[])
{
   int i;
  
   i = atoi (buffer);

  return i;
 }

 void read_string  (char string[])
 {

 for(uint8_t i=0; i<20; i++){
 nrfx_uarte_rx(&instance, &uarte_buffer, sizeof(uarte_buffer));
 if (uarte_buffer== 13) { string[i]='\0';
 break;}
 else {string[i]= uarte_buffer;}

 }

}

void send_int (int data)
{
 uint8_t Leopolf[20];


 sprintf(Leopolf,"%d\n\r", data);
 uarte_write(Leopolf, strlen(Leopolf));

 }

void uarte_write(uint8_t Leopolf[], int length)
 {
 nrfx_uarte_tx(&instance, Leopolf, length, 0);
 while(nrfx_uarte_tx_in_progress(&instance)){};
 }
 
 void uarte_handler(nrfx_uarte_event_t const *p_event, void *p_context)
{
    nrfx_uarte_t * p_inst = p_context;
    if (p_event->type == NRFX_UARTE_EVT_RX_DONE)
    {
       changer = 1;
    }
 
}



void start_game_grupp4 (void)
{
   NVIC_ClearPendingIRQ(NRFX_IRQ_NUMBER_GET(NRF_UARTE_INST_GET(0)));
NVIC_EnableIRQ(NRFX_IRQ_NUMBER_GET(NRF_UARTE_INST_GET(0)));
   const nrfx_uarte_config_t config = NRFX_UARTE_DEFAULT_CONFIG(20, 22);

nrfx_err_t err = nrfx_uarte_init(&instance, &config, uarte_handler);
//nrfx_err_t err = nrfx_uarte_init(&instance, &config, NULL);
nrfx_systick_init();
if (err != 0) {}
int exit = 0;
   int Y = 0, X = 0;
char arrays[10][10];
srand(time(NULL));
     //Initialize the array with the character '0'
    for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
        arrays[i][j] = '0';
    }
}
int A,B,C,D;
A = rand()%10;
B = rand()%10;
C = rand()%10;
D = rand()%10;
arrays[A][B] = '#';
arrays[C][D] = '#';
do 
{
int count = 0;


//Print the 10x10 array using UART
for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      
   nrfx_uarte_tx(&instance, &arrays[i][j], sizeof(char), 0); while(nrfx_uarte_tx_in_progress(&instance)){}; //Send each character individually
    }
    uint8_t vall[] = "\n\r";
 nrfx_uarte_tx(&instance, vall, sizeof(vall), 0); while(nrfx_uarte_tx_in_progress(&instance)){};//Send newline character after each row
  
}
    


    uint8_t val[] = "\n\rS = Down\n\rW = Up\n\rD = Right\n\rA = Left\n\rq = exit\n\r";
 nrfx_uarte_tx(&instance, val, sizeof(val), 0); while(nrfx_uarte_tx_in_progress(&instance)){};
  //nrfx_systick_delay_ms(200);
   nrfx_uarte_rx(&instance, &uarte_buffer, sizeof(uarte_buffer)); //while(nrfx_uarte_tx_in_progress(&instance)){};
   nrfx_systick_delay_ms(200);
   //print options
   //read input
   int count2 = 0;
   if(changer ==1)
   {
   switch(uarte_buffer){

   
   case 's':
      Y++;

      if (arrays[Y][X] == 'X' || Y > 9)
      {
           uint8_t torsk[] = "\n\rYou lost\n\r";
 nrfx_uarte_tx(&instance, torsk, sizeof(torsk), 0); while(nrfx_uarte_tx_in_progress(&instance)){};
         return 0;
      }
for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      if (arrays[i][j] == '0')
      {count2++;}
   
    }
}


      if(arrays[Y][X] == '#' && count2 > 0)
      {
         int Not=0;
         while (Not == 0)
         {
        int L, K;
       L =  rand()%10;
       K =  rand()%10;




       if (arrays[L][K] == '0')
       {arrays[L][K] = '#'; Not = 1;}
         }
      }
      arrays[Y][X] = 'X';
      //arrays[Y-1][X] = '0';
    
   break;
   
   case 'w':
      Y--; 
      
      if (arrays[Y][X] == 'X' || Y < 0)
      {
           uint8_t torsk[] = "\n\rYou lost\n\r";
 nrfx_uarte_tx(&instance, torsk, sizeof(torsk), 0); while(nrfx_uarte_tx_in_progress(&instance)){};
         return 0;
      }
       for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      if (arrays[i][j] == '0')
      {count2++;}
   
    }
       }

      if(arrays[Y][X] == '#' && count2 > 0)    
      {
         int Not=0;
         while (Not == 0)
         {
        int L, K;
       L =  rand()%10;
       K =  rand()%10;
       if (arrays[L][K] == '0')
       {arrays[L][K] = '#'; Not = 1;}
         }
      }
      arrays[Y][X] = 'X';
      //arrays[Y+1][X] = '0';
      
      break;
   
   case 'd':
      X++;
      
      if (arrays[Y][X] == 'X' || X > 9)
      {
           uint8_t torsk[] = "\n\rYou lost\n\r";
 nrfx_uarte_tx(&instance, torsk, sizeof(torsk), 0); while(nrfx_uarte_tx_in_progress(&instance)){};
         return 0;
      }
       for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      if (arrays[i][j] == '0')
      {count2++;}
   
    }
       }


      if(arrays[Y][X] == '#' && count2 > 0)
      {
         int Not=0;
         while (Not == 0)
         {
        int L, K;
       L =  rand()%10;
       K =  rand()%10;
       if (arrays[L][K] == '0')
       {arrays[L][K] = '#'; Not = 1;}
         }
      }
      arrays[Y][X] = 'X';
      //arrays[Y][X-1] = '0';
    
       break;
   
   case 'a':
      X--; 
      
      if (arrays[Y][X] == 'X' || X < 0)
      {
           uint8_t torsk[] = "\n\rYou lost\n\r";
 nrfx_uarte_tx(&instance, torsk, sizeof(torsk), 0); while(nrfx_uarte_tx_in_progress(&instance)){};
         return 0;
      }
       for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      if (arrays[i][j] == '0')
      {count2++;}
   
    }
       }


      if(arrays[Y][X] == '#' && count2 > 0)
      {
         int Not=0;
         while (Not == 0)
         {
        int L, K;
       L =  rand()%10;
       K =  rand()%10;
       if (arrays[L][K] == '0')
       {arrays[L][K] = '#'; Not = 1;}
         }
      }
      arrays[Y][X] = 'X';
      //arrays[Y][X+1] = '0';
      
      break;

   case 'q':

       exit = 1;
       break;
  default: break;//error
}
for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      if (arrays[i][j] == 'X')
      {count++;}
   
    }
    
  
}
if (count == 98) {uint8_t win[] = "\n\rYou won\n\r";
 nrfx_uarte_tx(&instance, win, sizeof(win), 0); while(nrfx_uarte_tx_in_progress(&instance)){}; return 0;}
changer = 0;

}

         }while (exit != 1);
        
}



