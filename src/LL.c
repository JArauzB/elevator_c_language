/*part of source file LL.c*/
#include "..\inc\LL.h"
#include <stdio.h>
#include <stdlib.h>

void constructLL(LL* list)
{
	node* newNode;
	newNode = malloc(sizeof(node));
	newNode->value = NULL;
	newNode->next = NULL;
	list->head = newNode;
	printf("Creating head of the linked list\n");


}

void addFrontLL(LL* list, int number) {
	printf("Adding element %d to the FIRST position.\n", number);
	node* newNode;
	newNode = malloc(sizeof(node));

	newNode->value = number;
	newNode->next = list->head;
	list->head = newNode;
}

void printLL(LL* list)
{
	printf("Printing linked list values\n values: ");
	node * p;
	p = list->head;
	if(p == list->head) printf("Head -> ");
	while (p->next != NULL) {
		printf("%d ", p->value);
		p = p->next;
	}
	printf("\n");
}

void destructLL(LL* list)
{
	printf("Deleting all values of the linked list\n");
	node* p;
	p = list->head;
	while (p->next != NULL) {
		free(p);
		p = p->next;
	}
}

int numbInLL(LL* list)
{
	int count = 0;
	node* p;
	p = list->head;
	
	while (p->next != NULL) {
		count = count + 1;
		p = p->next;
	}
	return count;
}

void addRearLL(LL* list, int number)
{
	printf("Adding element %d to the LAST position.\n", number);
	node* newNode;
	newNode = malloc(sizeof(node));
	newNode->value = number;
	node* p;
	p = list->head;
	int lenght = numbInLL(list) - 1;
	int count = 0;
	//if linked list is empty
	if (lenght < 0) {
		addFrontLL(list, number);
		return;
	}
	while (count < lenght) {
		p = p->next;
		count++;
		
	}
	node* tmpHead = p->next;
	p->next = newNode;
	newNode->next = tmpHead;
}

void copyLL(LL* emptyList, LL* copyList)
{
	printf("Copying linked list.\n");
	node* p;
	p = copyList->head;

	while (p->next != NULL) {
		addRearLL(emptyList, p->value);
		p = p->next;
	}
	printf("First linked list.\n");
	printLL(copyList);
	printf("Second linked list.\n");
	printLL(emptyList);

}

void deleteLL(LL* list, int number)
{
	node* p;
	LL * newListWithoutElement;
	constructLL(&newListWithoutElement);
	p = list->head;
	while (p->next != NULL) {
		if (p->value != number) {
			addRearLL(&newListWithoutElement, p->value);
		}
		p = p->next;
	}
	list->head = &newListWithoutElement->head;
}


/*eof LL.c*/
