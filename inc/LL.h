/*part of header file LL.h*/
#ifndef LL_H
#define LL_H

struct Snode {
	int value;
	struct Snode* next;
};

typedef struct Snode node;

struct list {
	node* head;
};

typedef struct list LL;

//methods
void addFrontLL(LL* list, int number);

void constructLL(LL* list);

void printLL(LL* list);

void destructLL(LL* list);

int numbInLL(LL* list);

void addRearLL(LL* list, int number);

void copyLL(LL* emptyList, LL* copyList);

void deleteLL(LL* list, int number);

#endif
/*eof LL.h*/