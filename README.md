# FV-UNPL

## Project Description

A company UpAndDown (UAD) delivers elevators with embedded systems.
In a new building, it is unknown yet how many elevators need to placed, therefore there is a simulation needed to determine whether 1, 2 or 3 elevators should be used.
The building has n floors (1..n) and on each floor there are m apartments (1 person per apartment).

### Requirements:
The simulation is an embedded solution, should be written in C and using the technologies learned in UNPL (sockets, semaphores, threads, mutexes, mailboxes).

### Functionality:
Each floor (2..n) is implemented as a separate process, running on some computer in the network. Each floor spawns (simulation arriving) people, going to some other floor (mostly to the ground floor being floor nr. 1).
Each floor also keeps track of the nr of people in the apartments, so never more than m persons can leave a floor at the same time.
Floor 1 is implemented as a separate process, spawning people, going to another floor (2..n).
Distance between floors is 4.5m.
Elevator (as a separate thread) moves between floors with a speed of 3 m/s, starting (acceleration) takes 2 seconds, and stopping (deceleration) takes also 2 seconds.
Elevator can contain max. 5 people.
People entering and leaving the elevator takes 3 seconds per person.
Most people leave and arrive at the building during rush hour (6am-9am, 11am-1pm and 4pm-7pm)

### Control:

Control, implemented as a process, with multiple threads, receives requests (people waiting at floor i, moving to floor j) and communicates to elevator to move from floor i to j. If, in the meantime, an other request is received which is in between the current movement, theelevator stops and picks up the people.
Important aspect for the control-unit is the logging of important data, like:
• Average waiting time for people
• Average travel time for people
• Average idle time for elevator
• …
This is needed to determine whether 1, 2 or 3 elevators need to be placed in the building. More functionality to be discussed with the coaches.

## Requirements to be implemented

we need to make use of 
threads:
semaphores:
-waits 
mailboxes:
-sends and communicates to the floors in order to check for new requests
-communication between elevator and floor
mutexes
-


The developers of this project are:

-Jorge Arauz

-Nicky Dokter
