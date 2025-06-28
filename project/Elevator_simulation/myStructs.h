#ifndef _STRUCTS_
#define _STRUCTS_

typedef enum Elevator_Status { STOPPED = 1, MOVING_UP = 2, MOVING_DOWN = 3 , OPENING = 4, CLOSING = 5, READY = 6 }Elevator_Status;

typedef struct sFloor {
	int building_level;
	int appartments;
	int waiting_for_elevator;
	int current_persons_in_floor;
	unsigned int visitors;
	struct Floor* nextFloor;
	struct Floor* prevFloor;
}Floor;

typedef struct sRequest {
	Floor* from_floor;
	Floor* to_floor;
	unsigned int num_of_people;
} Request;

typedef struct sNode {
	Request* request;
	struct Node* next;
}Node;


typedef struct sRequestList {
	Node* head;
	Node* newestRequest;
}RequestList;



typedef struct sElevator {
	int id;
	Elevator_Status status; //1: Stopped, 2: Moving_up, 3: Moving_down, 4: Opening, 5: Closing
	Floor* current_floor;
	double speed;
	double acceleration;
	int max_capacity;
	int current_capacity;
	int current_requests;
	RequestList* requests;
	int num_of_request;

}Elevator;

typedef struct sBuilding {
	int numFloors;
	Elevator* elevator;
	double distance_between_floors;
	Floor* startFloor;
	Floor* endFloor;
}Building;



#endif // !_STRUCTS_