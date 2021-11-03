// -------------------------------------------------------------------
// game:     MineSweeper (Command Line Version)
// author:   Arman Kolozyan
// year:     2021-2022
// -------------------------------------------------------------------

#include "cell.h"
#include "datatypes.h"
#include "handleInput.h"
#include "macros.h"
#include "printing.h"
#include <stdio.h>
#include <stdlib.h> // for the rand- & srand-functions
#include <time.h>   // for the time-function

/*
2 global variables that are modified by external functions (see cell.c) when the player has won/lost.
*/
int game_over;
int game_won;

/* 
Fills the 2D array (defined in the main function) with structs and gives default values to the struct members.
*/
void initialize_field(struct cell playing_field[rows][columns]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            struct cell *current_cell = &playing_field[i][j];
            current_cell->bomb = FALSE;
            current_cell->revealed = FALSE;
            current_cell->flagged = FALSE;
            current_cell->neighbours_count = 0;
        }
    }
}

/* 
Places the number of bombs specified in "macros.h".
To prevent the player from immediately, in the first turn, stepping on a mine and losing the game, this function receives the 
row and column of the first call of reveal/flag. If the first call is a print, then it receives a random row and column.
*/
void install_bombs(struct cell playing_field[rows][columns], int first_chosen_row, int first_chosen_column) {
    srand(time(0)); //to seed rand with a starting value
    int placed_bombs = 0;
    while (placed_bombs != total_bombs) {
        int bomb_row = rand() % rows;
        int bomb_column = rand() % columns;
        if ((bomb_row != first_chosen_row) || (bomb_column != first_chosen_column)) {
            struct cell *bomb_cell = &playing_field[bomb_row][bomb_column];
            if (!bomb_cell->bomb) { // to ensure that all bombs are placed in different cells
                bomb_cell->bomb = TRUE;
                placed_bombs++;
            }
        }
    }
}

/* 
Calls the "print_field" function to print the whole field. The second paramater indicates whether the 
field has to be revealed. 
*/
void call_the_printer(struct cell playing_field[rows][columns], enum Command command, int placed_flags) {
    if (command == PRINT) {
        print_field(playing_field, TRUE, placed_flags);
    } else {
        print_field(playing_field, FALSE, placed_flags);
    }
}

/*
To prevent the player from immediately, in the first turn, stepping on a mine and losing the game, 
the first command is handled separately to ensure that the first given row and column of the player does not contain a mine.
*/
void initialize(struct cell playing_field[rows][columns], int *placed_flags, int *correct_placed_flags, enum Command *command, int *user_row, int *user_column) {
    initialize_field(playing_field);
    printf("Welcome to MineSweeper!\n\nPlease type the first letter of one of the commands below along with the necessary arguments in digits.\n");
    printf("p(rint)\nr(eveal) [row] [column]\nf(lag) [row] [column]\n\n");
    printf("Have fun playing!\n\n\n");
    print_field(playing_field, FALSE, *placed_flags);
    get_input(playing_field, command, user_row, user_column);
    install_bombs(playing_field, *user_row, *user_column);
    calculate_neighbours_bombs(playing_field);
    process_input(playing_field, command, user_row, user_column, placed_flags, correct_placed_flags);
    call_the_printer(playing_field, *command, *placed_flags);
}

void main() {
    game_won = FALSE;
    game_over = FALSE;
    struct cell playing_field[rows][columns];
    int placed_flags = 0;
    int correct_placed_flags = 0; // if correct_placed_flags == total_bombs => the player has won
    enum Command command;
    int user_row = rand() % rows;       // will be overwritten if command is 'r' or 'f'
    int user_column = rand() % columns; // will be overwritten if command is 'r' or 'f'

    initialize(playing_field, &placed_flags, &correct_placed_flags, &command, &user_row, &user_column);

    while (!game_over && !game_won) {
        get_input(playing_field, &command, &user_row, &user_column);
        process_input(playing_field, &command, &user_row, &user_column, &placed_flags, &correct_placed_flags);
        if (!game_over && !game_won) { // to ensure that in case of a win or loss (determined by the above "process_input"), the field is not printed twice
            call_the_printer(playing_field, command, placed_flags);
        }
    }

    print_field(playing_field, TRUE, placed_flags); // after winning/losing the game, the whole revealed field is printed
    if (game_won) {
        printf("\nYOU WON! GOOD JOB!\n");
    } else {
        printf("\nGAME OVER!\n");
    }
    if (handle_replay()) {
        main();
    }
}