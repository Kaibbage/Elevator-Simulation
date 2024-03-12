#ifndef DEFS_H
#define DEFS_H

#define NUM_FLOORS 7
#define NUM_ELEVATORS 3

#define ELEVATOR_WAIT_TIME 4000
#define TIME_BETWEEN_MOVEMENT 1000

enum class Direction {
    Up,
    Neutral,
    Down
};

enum class ButtonType {
    Up,
    Down,
    Destination,
    Open,
    Close,
    Help
};

#endif
