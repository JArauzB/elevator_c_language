#include "myStructs.h"
#include "global_defines.h"
#include "ntk_5.4.h"

Elevator* elevator;

Elevator* constructElevator(Elevator* e) {
	RequestList* list = (RequestList*)malloc(sizeof(RequestList));
	list->head = (Node*)malloc(sizeof(Node));
	list->head->next = NULL;
	e = (Elevator*)calloc(1, sizeof(Elevator));
	e->id = 1;
	e->max_capacity = MAX_CAPACITY_ELEVATOR;
	e->speed = ELEVATOR_SPEED;
	e->acceleration = ELEVATOR_ACCELERATION;
	e->requests = list;
	e->status = READY;

	elevator = e;

	return e;
}

void OpenCloseDoors() {
	elevator->status = OPENING;
	//printf("	Opening doors...\n");
	Sleep(ELEVATOR_DOOR_SPEED);
	elevator->status = CLOSING;
	//printf("	Closing doors...\n");
	Sleep(ELEVATOR_DOOR_SPEED);
	elevator->status = READY;
}

void ElevatorMovingTime(unsigned int num_of_floors) {
	if (num_of_floors == 0) return;
	for (size_t i = 0; i < num_of_floors; i++)
	{
		double time = (((DISTANCE_BETWEEN_FLOORS) / ELEVATOR_SPEED) + SPEEDUP_SLOWDOWN_TIME);   //num_of_floors * DISTANCE_BETWEEN_FLOORS
		Sleep(time * 1000);
		if (elevator->status == MOVING_UP) {
			elevator->current_floor = elevator->current_floor->nextFloor;
		}
		else if (elevator->status == MOVING_DOWN) {
			elevator->current_floor = elevator->current_floor->prevFloor;
		}
	}
	
	//printf("Time: %.2fs\n", time);
	
}

void ElevatorMovingTo(Floor* toFloor) {
	int from = elevator->current_floor->building_level;
	int to = toFloor->building_level;

	if (from < to) {
		elevator->status = MOVING_UP;
		//printf("Moving up to Floor %d - ", to);
	}
	else {
		elevator->status = MOVING_DOWN;
		//printf("Moving down to Floor %d - ", to);
	}
	Sleep(100);
	//Wait for elevator to arrive at requesting floor
	ElevatorMovingTime(floorsBetween(from, to));
	elevator->status = STOPPED;
	//printf("Arrived at Floor %d\n", to);
	elevator->current_floor = toFloor;
	OpenCloseDoors();
}

//int checkAcceptedRequests(Elevator* elevatorInfo) {
//	//Check that elevator is empty
//	int closestFloor = NULL;
//	if (elevatorInfo->current_capacity == 0) {
//		//Taking persons that want to use the elevator
//		for (int i = 0; i < elevatorInfo->current_requests; i++) {
//			//Floor requests
//			Request currentRequest = elevatorInfo->requests[i];
//			if (elevatorInfo->current_floor <= currentRequest.from_floor) {
//				closestFloor = currentRequest.from_floor;
//				currentRequest.location = 1;
//			}
//		}
//		elevatorInfo->status = 2;
//		Sleep(5500); //Time calculation is : 4.5/3 = 1.5 seconds + 2 acceleration + 2 deceleration === 5.5
//		elevatorInfo->current_floor = elevatorInfo->current_floor++;
//		//take person
//		if (closestFloor != NULL && closestFloor == elevatorInfo->current_floor) {
//			elevatorInfo->status = OPENING;
//			Sleep(2000);
//			elevatorInfo->status = CLOSING;
//			Sleep(2000);
//			elevatorInfo->status = MOVING_DOWN;
//			elevatorInfo->current_capacity = elevatorInfo->current_capacity++;
//
//			return 1;
//		}
//
//		return 0;
//
//	}
//	else if (elevatorInfo->current_capacity < elevatorInfo->max_capacity) {
//		////Move elevator to the closest floor
//		//
//		////check if elevator is empty
//		////Loop all request to see which is the closest one
//		//int closestFloor = 0;
//		//for (int i = 0; i < elevatorInfo->current_requests; i++) {
//		//	//Floor requests
//		//	Request currentRequest = elevatorInfo->requests[i];
//		//	if()
//
//		//}
//	};
//}


const char* getElevatorStatus(Elevator_Status status) {
	const char* statusStr;
	switch (status)
	{
	case 1:
		statusStr = "STOPPED";
		break;
	case 2:
		statusStr = "MOVING UP";
		break;
	case 3:
		statusStr = "MOVING DOWN";
		break;
	case 4:
		statusStr = "OPENING";
		break;
	case 5:
		statusStr = "CLOSING";
		break;
	case 6:
		statusStr = "READY";
		break;
	default:
		statusStr = "??????";
		break;
	}
	return statusStr;
}