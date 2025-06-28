#include <stdio.h>
#include <stdlib.h>
#include "ntk_5.4.c"
#include "..\inc\elevator.h"
#include "..\inc\building.h"

#define NUM_DINERS 5
#define EAT_TIMES 3

/* Our main creates a semaphore for every fork in an unlocked state. One philosopher
*  can immediately grab both forks and sets up the numEating semaphors to allow (N-1)
*  philsophers to try to grab their forks and start eating.
*/
semaphore fork[NUM_DINERS];
semaphore numEating;

unsigned __stdcall Philosopher(void* arg);
void thinking(int personID);
void eating(int PersonID);


int main(int argc, char** argv)
{
    printf("Main started. %d philosophers that eat %d times.\n", NUM_DINERS, EAT_TIMES);
    int i;
    srand(time(0));

    startNTK();


    //Create the forks
    for (int i = 0; i < NUM_DINERS; i++) {
        create_semaphore(&fork[i], 1, 1);  // 5, 5
    }

    //Create the philosophers and start them up
    for (int i = 0; i < NUM_DINERS; i++) {
        task* t = (task*)malloc(sizeof(task)); // dynamic task object
        create_task(t, Philosopher, &i, sizeof(int), 0);
    }

    //create numeating/ empty buffers where only 2 persons can eat
    create_semaphore(&numEating, 2, 2); // 0, 2

    //Wait for all threads to finish
    Sleep(25000);
    delete_semaphore(&numEating);
    for (i = 0; i < NUM_DINERS; i++) {
        delete_semaphore(&fork[i]);
    }
    printf("All done!\n");
}

void thinking(int  PersonID)
{
    printf("Person %d is thinking \n", PersonID);
    Sleep(rand() % 2000);
    // return 0;
}


void eating(int PersonID)
{
    printf("Person %d is Eating \n", PersonID);
    Sleep(rand() % 2000);
}

unsigned __stdcall Philosopher(void* arg)
{
    int* a = (int*)getArgument_task((task*)arg);
    for (int i = 0; i < EAT_TIMES; i++) {
        thinking(*a);
        wait_semaphore(&fork[*a]);
        wait_semaphore(&fork[(*a + 1) % NUM_DINERS]);
        wait_semaphore(&numEating);
        eating(*a);
        signal_semaphore(&fork[(*a + 1) % NUM_DINERS]);
        signal_semaphore(&fork[*a]);
        signal_semaphore(&numEating);
    }
    printf("Person %d is DONE eating \n", *a);
    //return 0;
}


//void create_building(building* new_building, int floors, int elevator_count) {
//    printf("Creating a building with %d floor(s) and %d elevator(s)\n", floors, elevator_count);
//    new_building->floors = floors;
//    new_building->elevator_count = elevator_count;
//    new_building->distance_between_floors = 4.5;
//}