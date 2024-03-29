//Defining constants
#ifndef DEFS_H
#define DEFS_H

#define NUM_FLOORS 7
#define NUM_ELEVATORS 1

#define ELEVATOR_WAIT_TIME 10000
#define TIME_BETWEEN_MOVEMENT 2000
#define TIME_TO_CALL_911 5000

#define DEFAULT_VALUE -1

enum class Direction {
    Up,
    Neutral,
    Down
};


#endif
