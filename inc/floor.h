#include "elevator.h"

#ifndef FLOOR_H
#define FLOOR_H


typedef struct Sfloor() {
	int building_level;
	elevator elevators[];
	int count_elevators; // how many elevetors there are 
	int appartments;
	int current_persons_in_floor;
} floor;

#endif