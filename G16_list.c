#include "G16_list.h"
#include "G16_term.h"
#include <assert.h>
#include <nrfx_uarte.h>
#include <string.h>



//Denna funktion används istället för free för att frigöra en nod
static void free_node(List to_remove)
{
	to_remove->previous = NULL;
	to_remove->next = NULL;
	to_remove->data = 0;
	to_remove->used = 0;
}

/***************************************************************************************************************/
/* Denna funktion returnerar en pekare till en ledig nod. Om alla LIST_LEN noder är i bruk redan så returnerar */
/* funktionen NULL. Ni behöver alltså kontrollera returvärdet. För att "frigöra" en nod används free_node.     */
/* Denna funktion (get_node) ska bara kallas från create_list_node.                                            */
/***************************************************************************************************************/
static Node* get_node()
{
	static uint8_t init = 0; 		//eftersom init är statisk så kommer den bara få startvärdet 0 vid första anropet av get_node, detta upprepas inte vid senare anrop
	static Node list[LIST_LEN]; 

	if (init == 0)					//körs första gången get_node anropas för att "nollställa" samtliga noder
	{
		for(int i = 0; i < LIST_LEN; i++)
		{
			free_node(&list[i]);
		}
		init = 1;
	}

	for(int i = 0; i < LIST_LEN; i++)
	{
		if(list[i].used == 0)
		{
			list[i].used = 1;
			return &list[i];
		}
	}
	
	return NULL;
}



//Returnera en tom lista - funktionen är färdig
List create_empty_list(void)
{
    return NULL;
}

//Här skapas nya noder. Denna funktion är den enda som ska kalla på get_node.
//Denna funktion ska i sin tur bara kallas av add_first och add_last.
static Node * create_list_node(const Data data)
{
	//glöm inte att kolla så att get_node inte returnerade NULL
	Node* node = get_node();
	assert(node != NULL);
	node->data = data;

	return node;
}

//Är listan tom?
//Returnerar 1 om listan är tom, annars 0
int is_empty(const List list)
{
	return list == NULL;
}

//Lägg till en nod först i listan
void add_first(List *list, const Data data)
{
	Node* node = create_list_node(data);

	if (*list != NULL)
	{
		node->next = *list;
		(*list)->previous = node;
	}

	*list = node;
}

//lägg till nod sist i listan
void add_last(List *list, const Data data)
{
	Node* node = create_list_node(data);

	if (*list != NULL)
	{
		Node* currentNode = *list;

		while (currentNode->next != NULL)
		{
			currentNode = currentNode->next;
		}

		currentNode->next = node;
		node->previous = currentNode;
	}
	else
	{
		*list = node;
	}
}

//Ta bort första noden i listan
//precondition: listan är inte tom (testa med assert)
void remove_first(List *list)
{
	assert(!is_empty(*list));

	if ((*list)->next == NULL)
	{
		free_node(*list);
		*list = NULL;
		return 0;
	}

	Node* node2 = (*list)->next;
	node2->previous = NULL;
	free_node(*list);
	*list = NULL;
	*list = node2;
}

//ta bort sista noden i listan
//precondition: listan är inte tom (testa med assert)
void remove_last(List *list)
{
	assert(!is_empty(*list));

	if ((*list)->next == NULL)
	{
		free_node(*list);
		*list = NULL;
		return 0;
	}

	Node* lastNode = *list;

	while (lastNode->next != NULL)
	{
		lastNode = lastNode->next;
	}
	
	Node* secondLast = lastNode->previous;
	secondLast->next = NULL;
	free_node(lastNode);
}

//töm listan (ta bort alla noder ur listan)
//postcondition: Listan är tom, *list är NULL
void clear_list(List *list)
{
	//alla noder ska frigöras
	if ((*list)->previous == NULL && (*list)->next == NULL)
	{
		free_node(*list);
		*list = NULL;
		return 0;
	}

	*list = (*list)->next;
	free_node((*list)->previous);
	(*list)->previous = NULL;
	clear_list(list);
}

//Skriv ut listan genom UART
void print_list(const List list)
{
	static int something = 1;

	if (list->previous == NULL)
	{
		something = 1;
	}
	
	int d = list->data;
	uint8_t strArr[4];

	itoa(something, &strArr, 10);
	term_print(strArr);
	term_print(": ");

	itoa(d, &strArr, 10);
	term_print(strArr);
	term_print("\n\r");

	if (list->next == NULL)
	{
		return 0;
	}
	
	something++;
	print_list((list->next));
}

//returnera första datat i listan
//precondition: listan är inte tom (testa med assert)
Data get_first_element(const List list)
{
	assert(!is_empty(list));

	return list->data;
}

//returnera sista datat i listan. 
//precondition: listan är inte tom (testa med assert)
Data get_last_element(const List list)
{
	assert(!is_empty(list));

	Node* currentNode = list;

	while (currentNode->next != NULL)
	{
		currentNode = currentNode->next;
	}

	return currentNode->data;
}

Node* search(const List list, uint8_t data)
{
	assert(!is_empty(list));

	if (list->data == data)
	{
		return list;
	}

	if (list->next == NULL)
	{
		return NULL;
	}

	search(list->next, data);
}

void swap_next(List* node)
{
	Node *head, *a, *b, *tail;

	

	assert((*node)->next != NULL);

	a = *node;
	b = a->next;
	head = a->previous;
	tail = b->next;

	if(head != NULL)
	{
		head->next = b;
	}
	
	b->previous = head;
	b->next = a;
	a->previous = b;
	a->next = tail;

	if(tail != NULL)
	{
		tail->previous = a;
	}

	*node = b;
}

int list_len(List list)
{
	Node* currentNode = list;
	int len = 0;

	if(list == NULL)
	{
		return 0;
	}

	while (currentNode->next != NULL)
	{
		currentNode = currentNode->next;
		len++;
	}

	return len+1;
}