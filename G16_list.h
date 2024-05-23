#ifndef LIST_HG16
#define LIST_HG16

#include <nrfx.h>

#define PIN_TXD 20
#define PIN_RXD 22
#define CLEAR_SCREEN "\033c"

/*******************************************************/
/* DVA270 - Interface för dubbellänkad lista           */
/* Anpassad efter interface från DVA244                */
/* av Stefan Bygde och Caroline Uppsäll                */
/* Vid fel eller frågor, kontakta jonas.larsson@mdu.se */
/*******************************************************/


#define LIST_LEN 1000

typedef int Data;

//struktdefinition för vår dubbellänkade lista
typedef struct node
{
	Data data;
	struct node* next;
	struct node* previous;
	uint8_t used; //indikerar om noden används. När vi tar bort en nod från listan så måste vi sätta used till 0
}Node;

typedef Node *List; //Listan representeras av en nodpekare. 

/* Funktionsdeklarationer */

//Returnera en tom lista
List create_empty_list(void);

//Är listan tom?
//Returnerar 1 om listan är tom, annars 0
int is_empty(const List list);

//Lägg till en nod först i listan
void add_first(List *list, const Data data);

//Lägg till nod sist i listan
void add_last(List *list, const Data data);

//Ta bort första noden i listan
void remove_first(List *list);

//Ta bort sista noden i listan
void remove_last(List *list);

//Töm listan (ta bort alla noder ur listan)
void clear_list(List *list);

//Skriv ut listan genom UART
void print_list(const List list);

//Returnera första datat i listan
Data get_first_element(const List list);

//Returnera sista datat i listan
Data get_last_element(const List list);

/* Vill ni ha mer att göra? Implementera även följande funktioner.
De kan komma att användas i framtiden, men är inget krav för laboration 2*/

//ta bort data ur listan (första förekomsten), returnera 0 om datat inte finns, annars 1
//int remove_element(List *list, const Data data);

//Sök efter data i listan, returnera 1 om datat finns, annars 0.
//int search(const List list, const Data data);
//Version av ovanstående:
Node* search(const List list, uint8_t data);

void swap_next(List* node);

int list_len(List list);

//returnera hur många noder som finns i listan
//int number_of_nodes(const List list);

#endif