#include "floor.h"

#ifndef BUILDING_H
#define BUILDING_H

typedef struct Sbuilding{
	int floors;
	int elevator_count; //This field is used to count how many elevators in this building are
	double distance_between_floors;
}building;


void create_building(building* new_building, int floors, int elevator_count);

#endif