#include "cell.h"
#include "macros.h"
#include <stdio.h>
#include <unistd.h> //for the sleep-function


int remaining_nonbomb_cells = rows * columns - total_bombs;
extern enum Boolean game_won;
extern enum Boolean game_over;

void calculate_neighbours_bombs(struct cell playing_field[rows][columns]) {
    remaining_nonbomb_cells = rows * columns - total_bombs; // for a reset when the player wants to replay
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            struct cell *current_cell = &playing_field[i][j];
            if (current_cell->bomb) { // we don't have to count the neighbours of bombs
                continue;
            }
            {
                int neighbour_bombs = 0;
                for (int off_i = -1; off_i <= 1; off_i++) {
                    for (int off_j = -1; off_j <= 1; off_j++) { // checking all the neighbours of current_cell
                        int neighbour_i = i + off_i;
                        int neighbour_j = j + off_j;
                        if ((neighbour_i < 0) || (neighbour_i >= rows)) { // checking if we are not off the field
                            continue;
                        }
                        if ((neighbour_j < 0) || (neighbour_j >= columns)) { // checking if we are not off the field
                            continue;
                        }
                        {
                            struct cell *neighbour = &playing_field[neighbour_i][neighbour_j];
                            if (neighbour->bomb) {
                                neighbour_bombs++;
                            }
                        }
                    }
                }
                current_cell->neighbours_count = neighbour_bombs;
            }
        }
    }
}

void place_flag(struct cell *the_cell, int *placed_flags, int *correct_placed_flags) {
    if (!the_cell->flagged) {
        if (the_cell->revealed) {
            printf("Action cannot be done. Cell is revealed.\n");
            sleep(2);
        } else if (*placed_flags < total_bombs) {
            the_cell->flagged = TRUE;
            (*placed_flags)++; // brackets are necessary since "++"" has higher precedence than "*"
            if (the_cell->bomb) {
                (*correct_placed_flags)++;
                if (*correct_placed_flags == total_bombs) {
                    game_won = 1;
                }
            }
        } else {
            printf("No flags left!\n");
        }
    } else {
        remove_flag(the_cell, placed_flags, correct_placed_flags);
    }
}

void remove_flag(struct cell *the_cell, int *placed_flags, int *correct_placed_flags) { 
    the_cell->flagged = FALSE;
    (*placed_flags)--;
    if (the_cell->bomb) {
        (*correct_placed_flags)--;
    }
}

void reveal(struct cell playing_field[][columns], int row, int column, int *placed_flags, int *correct_placed_flags) {
    struct cell *the_cell = &playing_field[row][column];
    if (the_cell->flagged) {
        remove_flag(the_cell, placed_flags, correct_placed_flags); // personal choice: if the player wants to reveal a flagged cell, the flag is given back to the player before the reveal  
    }
    if (the_cell->bomb) {
        game_over = 1;
    } else if (the_cell->revealed) {
        printf("Cell is already revealed!\n");
        sleep(2);
    } else {
        the_cell->revealed = TRUE;
        remaining_nonbomb_cells--;
        if (remaining_nonbomb_cells == 0) {
            game_won = 1;
        }
        if (the_cell->neighbours_count == 0) {
            reveal_neighbours(playing_field, row, column, placed_flags, correct_placed_flags);
        }
    }
}

void reveal_neighbours(struct cell playing_field[][columns], int row, int column, int *placed_flags, int *correct_placed_flags) {
    for (int off_i = -1; off_i <= 1; off_i++) {
        for (int off_j = -1; off_j <= 1; off_j++) { // checking all the neighbours
            int neighbour_i = row + off_i;
            int neighbour_j = column + off_j;
            if ((neighbour_i < 0) || (neighbour_i >= rows)) { // checking if we are not off the field
                continue;
            }
            if ((neighbour_j < 0) || (neighbour_j >= columns)) { // checking if we are not off the field
                continue;
            }
            {
                struct cell *neighbour = &playing_field[neighbour_i][neighbour_j];
                if (!neighbour->revealed) // otherwise infinite loop (constantly asking each other to reveal themselves)
                {
                    reveal(playing_field, neighbour_i, neighbour_j, placed_flags, correct_placed_flags);
                }
            }
        }
    }
}