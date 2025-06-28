#include "ntk_5.4.h"
#include "myStructs.h"
#include "elevatorControls.h"
#include "global_defines.h"
#include <stdbool.h>

Building* constructBuilding();
void generateUI();
void body();
void loopFloors(Building* b);
void startProcess(Building* b);
void addNewRequest(Request* req);
int spawnRandomPersons(Floor* floor);
unsigned int floorsBetween(Floor* f1, Floor* f2);
void deleteRequest(Node* node);
Floor* getRandomFloor(int floor);
unsigned __stdcall checkFloors(void* arg);
unsigned __stdcall getRequests(void* arg);
unsigned __stdcall elevatorMovement(void* arg);
unsigned __stdcall updateUI(void* arg);
Building* building;
Floor* groundFloor;
mailBox* box;
semaphore elevatorCapacity;
semaphore* addRequest;
Elevator* elevator;


int main() {
	srand(time(0));

	startNTK();//START NTK and task
	box = (mailBox*)malloc(sizeof(mailBox)); //initiate mailbox
	create_mailBox(box, MAX_NUM_OF_REQUESTS, sizeof(Request)); //create mailbox

	addRequest = (semaphore*)malloc(sizeof(semaphore));
	create_semaphore(addRequest, 1, 1);//create semaphore for adding requests to elevator

	//Construct building and start the threads
	building = constructBuilding();
	startProcess(building);
	Sleep(APP_RUNTIME);
	//free(building);
	return 0;
}

Building* constructBuilding() {
	Building* b = (Building*)malloc(sizeof(Building));
	Floor* curFloor = (Floor*)calloc(1, sizeof(Floor)); // ground floor
	groundFloor = curFloor;

	//Create elevator instance
	elevator = constructElevator(elevator);

	create_semaphore(&elevatorCapacity, elevator->max_capacity, elevator->max_capacity);

	b->numFloors = NUM_OF_FLOORS;
	b->startFloor = curFloor;
	b->distance_between_floors = DISTANCE_BETWEEN_FLOORS;
	b->elevator = elevator;
	b->elevator->current_floor = b->startFloor;

	for (size_t i = 1; i < NUM_OF_FLOORS; i++)
	{
		Floor* newFloor = (Floor*)malloc(sizeof(Floor));
		newFloor->building_level = i;
		newFloor->appartments = NUM_APARTMENTS;
		newFloor->current_persons_in_floor = NUM_APARTMENTS;
		newFloor->waiting_for_elevator = 0;
		newFloor->visitors = 0;
		newFloor->prevFloor = curFloor;
		curFloor->nextFloor = newFloor;
		
		curFloor = newFloor;
	}
	curFloor->nextFloor = NULL;
	b->endFloor = curFloor;
	return b;
}

void startProcess(Building* b) {
	//floors
	Floor* curFloor = b->startFloor;
	for (size_t i = 0; i < NUM_OF_FLOORS; i++)
	{
		task* newTask = (task*)malloc(sizeof(task));
		create_task(newTask, checkFloors, curFloor, sizeof(Floor), 0);
		curFloor = curFloor->nextFloor;
	}
	//requests
	task* requestsTask = (task*)malloc(sizeof(Elevator));
	create_task(requestsTask, getRequests, b->elevator, sizeof(Elevator), 0);

	//elevator movement
	task * elevatorTask = (task*)malloc(sizeof(task));
	create_task(elevatorTask, elevatorMovement, b->elevator, sizeof(Elevator), 0);

	//UI updates
	task* uiTask = (task*)malloc(sizeof(task));
	create_task(uiTask, updateUI, uiTask, sizeof(task), 0);
}





//========= TASKS ==========

unsigned __stdcall updateUI(void* arg) {
	task* myTask = (task*)getArgument_task((task*)arg);
	while (1) {
		generateUI();
		Sleep(1000);
	}
}

unsigned __stdcall checkFloors(void* arg) {
	Floor* floor = (Floor*)getArgument_task((task*)arg);
	while (1)
	{
		//Here spawns the persons/request for the elevator per floor
		if (floor->building_level == (rand() % 4)) {
			//printf("Spawning person at building %d...\n", floor->building_level);
			spawnRandomPersons(floor);
			Request* newRequest = (Request*)malloc(sizeof(Request));
			newRequest->from_floor = floor;
			newRequest->to_floor = getRandomFloor((rand() % NUM_OF_FLOORS));
			
			//Check if request already exist, if yes, only increase the amount of people for that request.
			put_mailBox(box, newRequest);
		}
		Sleep(10000);
	}
}

unsigned __stdcall getRequests(void* arg) {
	//Elevator* elevator = (Elevator*)getArgument_task((task*)arg);
	while (1) {
		Sleep(1000);
		//header(); //Show all header information

		//Check if a request was made
		if (!isEmpty_mailBox(box)) {

			//add the request to the elevator request list. Semephore is being used in order to place the request safely in the list
			wait_semaphore(addRequest);
				//printf("Request found!\n");
				Request* req = (Request*)malloc(sizeof(Request));
				get_mailBox(box, req);
				addNewRequest(req);
			signal_semaphore(addRequest);

			
			//body(); //show updates
			////TODO: RUn the process for the elevator
			//elevatorInfo->requests[elevatorInfo->current_requests] = *req;
			//elevatorInfo->current_requests = elevatorInfo->current_requests++;
			//wait_semaphore(&elevatorCapacity); //wait until elevator has enough space
		}
		else {
			//printf("Mailbox empty\n");
			//body(); //show updates
		}
	}
}

unsigned __stdcall elevatorMovement(void* arg) {
	while (1) {
		if (elevator->num_of_request > 0) {
			Node* n = elevator->requests->head->next; //MAIN REQUEST
			Request* req = n->request;
			//printf("-REQUEST (%d -> %d) | Elevator on Floor %d-\n", req->from_floor->building_level, req->to_floor->building_level, elevator->current_floor->building_level);

			//Check if the request doesn't have the same from & to floor
			if (req->from_floor->building_level != req->to_floor->building_level) {
				//Check if elevator is ready to openen
				if (elevator->status == READY) {
					if (elevator->current_floor->building_level == req->from_floor->building_level) {
						//Already available at the floor where the request was made
						//printf("Going from Floor %d to Floor %d\n ", req->from_floor->building_level, req->to_floor->building_level);

						//Execute & delete request
						ElevatorMovingTo(req->to_floor);
						deleteRequest(n);
					}
					else {
						//Move to the floor where the request was made
						ElevatorMovingTo(req->from_floor);

						//Execute & delete request
						ElevatorMovingTo(req->to_floor);
						deleteRequest(n);

					}
					//printf("-REQUEST DONE--\n------------------------------------------------\n");
					//printf("Pending requests: %d\n\n", elevator->num_of_request);
				}
			}
			else {
				//Request starts & goes to the same floor, so just open/close the doors
				ElevatorMovingTo(req->to_floor);
				deleteRequest(n);
				//printf("-REQUEST DONE--\n------------------------------------------------\n");
			}
		}
		else {
			//printf("-No requests found-\n");
		}
		Sleep(1000);
		//int newStatus = checkAcceptedRequests(elevatorInfo);
		//if (newStatus == 1) signal_semaphore(&elevatorCapacity);
	}
}





//=========== FUNCTIONS ===============

void addNewRequest(Request* req) {
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->request = req;

	RequestList* list = elevator->requests;

	if (list->head->next == NULL) {
		list->head->next = newNode;
		newNode->next = NULL;
	}
	else {
		Node* lastNode = list->newestRequest;
		lastNode->next = newNode;
		newNode->next = NULL;
	}
	elevator->requests->newestRequest = newNode;
	elevator->num_of_request++;
}

void deleteRequest(Node* node) {
	if (node->next == NULL) {
		elevator->requests->head->next = NULL;
	}
	else {
		elevator->requests->head->next = node->next;
		node->next = NULL;
	}
	free(node->request);
	free(node);
	elevator->num_of_request--;
}

Floor* getRandomFloor(int floor) {
	Floor* curFloor = groundFloor;
	for (size_t i = 0; i < NUM_OF_FLOORS; i++)
	{
		if (curFloor->building_level == floor) {
			return curFloor;
		}
		curFloor = curFloor->nextFloor;
	}
}

unsigned int floorsBetween(int f1, int f2) {
	return (f1 > f2) ? (f1 - f2) : (f2 - f1);
}

int spawnRandomPersons(Floor* floor) {
	if (floor->current_persons_in_floor == 1) return 1;
	if (floor->current_persons_in_floor == 0) return 0;
	
	int n = (rand() % floor->current_persons_in_floor) + 1;
	floor->waiting_for_elevator += n;
	floor->current_persons_in_floor -= n;
	return n;
}


void generateUI() {
	system("cls");
	Floor* curFloor = building->endFloor;
	for (size_t i = 1; i <= NUM_OF_FLOORS; i++)
	{
		printf("------- FLOOR %d ---------------------------------------------------------\n", curFloor->building_level);

		if (elevator->current_floor->building_level != curFloor->building_level) {
			printf("Num of ppl:  %d    | \n", curFloor->current_persons_in_floor);
			printf("ppl waiting: %d    |\n", curFloor->waiting_for_elevator);
			printf("visitors: %d       |\n ", curFloor->visitors);

		}
		else {
			//Draw elevator
			printf("Num of ppl:  %d    | \t\t __________   ", curFloor->current_persons_in_floor);
			printf("%s\n", getElevatorStatus(elevator->status));
			if (elevator->requests->head->next != NULL) {
				Node* n = elevator->requests->head->next;
				Request* curRequest = n->request;
				printf("ppl waiting: %d    | \t\t |        |  ( %d -> %d )\n", curFloor->waiting_for_elevator, curRequest->from_floor->building_level, curRequest->to_floor->building_level);
			}
			else {
				printf("ppl waiting: %d    | \t\t |        | \n", curFloor->waiting_for_elevator);
			}
			printf("visitors: %d       | \t\t |________|\n ", curFloor->visitors);
		}
		
		printf("--------------------------------------------------------------------------\n");
		curFloor = curFloor->prevFloor;

		printf("\n\n");
	}
	printf("\n");
}

//void body() {
//	//Persons in floor
//	printf("Persons in floor:	|	");
//	Floor* curFloor = groundFloor;
//
//	for (size_t i = 0; i < NUM_OF_FLOORS; i++)
//	{
//		printf("%d	|	", curFloor->current_persons_in_floor);
//		curFloor = curFloor->nextFloor;
//	}
//	printf("\n");
//	//Requests 
//	for (int i = 0; i < elevator->current_requests; i++) {
//		printf("Elevator request %d	|	", i);
//
//		for (int j = 0; j < NUM_OF_FLOORS; j++)
//		{
//			Request currentRequest = elevator->requests[i];
//
//			if (currentRequest.from_floor->building_level == j && currentRequest. == 0) {
//				printf("to:%d	|	", currentRequest.to_floor);
//			}
//			else printf("%d	|	", 0);
//
//		}
//		printf("\n");
//	}
//	
//	//Elevator position
//	printf("Elevator position	|	");
//	curFloor = groundFloor;
//	for (size_t i = 0; i < NUM_OF_FLOORS; i++)
//	{
//		
//		if (elevatorInfo->current_floor == i) {
//			printf("1	|	");
//		}
//		else printf("0	|	");
//
//		curFloor = curFloor->nextFloor;
//	}
//	printf("\n");
//
//	//Elevator information
//	;
//	printf("Elev status: ");
//	getElevatorStatus(elevatorInfo->status);
//	printf("|	 \n");
//	printf("Elev. current cap.: %d	|\n", elevatorInfo->current_capacity);
//
//}