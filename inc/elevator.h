#ifndef ELEVETOR_H
#define ELEVETOR_H

//typedef struct Selevator elevator;

typedef struct Selevator {
	int id;
	short status;
	int current_floor;
	int position_on_floor;// This is the position of the elevator where person is waiting 
	double speed;
	double acceleration;
	int max_capacity;
	int current_capacity;
}elevator;



//struct list {
//	node* head;
//};
//
//typedef struct list LL;

//methods
//void addFrontLL(LL* list, int number);



#endif