#pragma once
#include "myStructs.h"

int checkAcceptedRequests(Elevator* elevatorInfo);

const char* getElevatorStatus(Elevator_Status status);

Elevator* constructElevator(Elevator* e);

void ElevatorMovingTo(Floor* to);

void ElevatorMovingTime(unsigned int num_of_floors);

void OpenCloseDoors();