#ifndef DEFS_H
#define DEFS_H

#define NUM_FLOORS 7
#define NUM_ELEVATORS 2

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
