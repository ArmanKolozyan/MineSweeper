#include "cell.h"
#include "macros.h"
#include <stdio.h>
#include <unistd.h> //voor sleep
int remaining_nonbomb_cells = rows * columns - total_bombs;
extern int game_won;
extern int game_over;

void calculate_neighbours_bombs(struct cell playing_field[rows][columns]) {
    remaining_nonbomb_cells = 0; // why, explain
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            struct cell *current_cell = &playing_field[i][j];
            if (current_cell->bomb) {
                continue;
            }
            {
                int neighbour_bombs = 0;
                for (int off_i = -1; off_i <= 1; off_i++) {
                    for (int off_j = -1; off_j <= 1; off_j++) { // checking all the neighbours
                        int neighbour_i = i + off_i;
                        int neighbour_j = j + off_j;
                        if ((neighbour_i < 0) || (neighbour_i >= rows)) {
                            continue;
                        }
                        if ((neighbour_j < 0) || (neighbour_j >= columns)) {
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
            (*placed_flags)++;
            if (the_cell->bomb) {
                (*correct_placed_flags)++;
                if (*correct_placed_flags == total_bombs) {
                    printf("case 1");
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

void remove_flag(struct cell *the_cell, int *placed_flags, int *correct_placed_flags) { // altijd zo'n argument meegeven!!
    the_cell->flagged = FALSE;
    (*placed_flags)--;
    if (the_cell->bomb) {
        (*correct_placed_flags)--;
    }
}

int reveal(struct cell playing_field[][columns], int row, int column, int *placed_flags, int *correct_placed_flags) {
    struct cell *the_cell = &playing_field[row][column];
    if (the_cell->flagged) {
        remove_flag(the_cell, placed_flags, correct_placed_flags);
    }
    if (the_cell->bomb) {
        game_over = 1;
        return 0;
    } else if (the_cell->revealed) {
        printf("Cell is already revealed!\n");
        sleep(2);
        return 1;
    } else {
        the_cell->revealed = TRUE;
        remaining_nonbomb_cells--;
        if (remaining_nonbomb_cells == 0) {
            game_won = 1;
            return 1;
        }
        if (the_cell->neighbours_count == 0) {
            reveal_neighbours(playing_field, row, column, placed_flags, correct_placed_flags);
            return 1;
        }
    }
}

void reveal_neighbours(struct cell playing_field[][columns], int row, int column, int *placed_flags, int *correct_placed_flags) {
    for (int off_i = -1; off_i <= 1; off_i++) {
        for (int off_j = -1; off_j <= 1; off_j++) {
            int neighbour_i = row + off_i;
            int neighbour_j = column + off_j;
            if ((neighbour_i < 0) || (neighbour_i >= rows)) {
                continue;
            }
            if ((neighbour_j < 0) || (neighbour_j >= columns)) {
                continue;
            }
            {
                struct cell *neighbour = &playing_field[neighbour_i][neighbour_j];
                if (!neighbour->revealed) // otherwise infinite loop (constantly asking each other to reveal themselves
                {
                    reveal(playing_field, neighbour_i, neighbour_j, placed_flags, correct_placed_flags);
                }
            }
        }
    }
}