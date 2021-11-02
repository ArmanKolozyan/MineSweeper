#ifndef cello
#define cello

#include "datatypes.h"


struct cell {
    enum Boolean bomb;
    enum Boolean revealed;
    enum Boolean flagged;
    int neighbours_count;
};



void calculate_neighbours_bombs(struct cell (*playing_field)[]);
void place_flag(struct cell *the_cell, int *placed_flags, int *correct_placed_flags);
void remove_flag(struct cell *the_cell, int *placed_flags, int *correct_placed_flags);
int reveal(struct cell (*playing_field)[], int row, int column, int *placed_flags, int *correct_placed_flags);
void reveal_neighbours(struct cell (*playing_field)[], int row, int column, int *placed_flags, int *correct_placed_flags);


#endif