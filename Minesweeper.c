int game_over;
int game_won;
#include "cell.h"
#include "datatypes.h"
#include "handleInput.h"
#include "macros.h"
#include "printing.h"
#include <stdio.h>
#include <stdlib.h> // voor rand
#include <time.h>   //voor time
#include <unistd.h> //voor sleep

void initialize_field(struct cell playing_field[][columns]) {
    srand(time(0));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            struct cell *current_cell = &playing_field[i][j];
            current_cell->bomb = FALSE;
            current_cell->revealed = FALSE;
            current_cell->flagged = FALSE;
            current_cell->neighbours_count = 0; /* default values geven? */
        }
    }
}

void install_bombs(struct cell playing_field[][columns], int first_chosen_row, int first_chosen_column) {
    int placed_bombs = 0;
    while (1) {
        int bomb_row = rand() % rows;
        int bomb_column = rand() % columns;
        if ((bomb_row != first_chosen_row) || (bomb_column != first_chosen_column)) {
            struct cell *bomb_cell = &playing_field[bomb_row][bomb_column];
            if (!bomb_cell->bomb) {
                bomb_cell->bomb = TRUE;
                placed_bombs++;
                if (placed_bombs == total_bombs) {
                    break;
                }
            }
        }
    }
}

void main() {
    game_won = 0;
    game_over = 0;
    int placed_flags = 0;
    int correct_placed_flags = 0;
    struct cell playing_field[rows][columns]; // globaal maken
    enum Command command;
    int user_row = rand() % rows; // will be overwritten if command is 'r' or 'f'
    int user_column = rand() % columns;
    initialize_field(playing_field);
    print_field(playing_field, 0, placed_flags);
    get_input(playing_field, &command, &user_row, &user_column);
    install_bombs(playing_field, user_row, user_column);
    calculate_neighbours_bombs(playing_field);
    handle_input(playing_field, &command, &user_row, &user_column, &placed_flags, &correct_placed_flags);
    print_field(playing_field, 0, placed_flags);
    while (!game_over && !game_won) {
        get_input(playing_field, &command, &user_row, &user_column);
        handle_input(playing_field, &command, &user_row, &user_column, &placed_flags, &correct_placed_flags);
        if (command == PRINT) {
            print_field(playing_field, 1, placed_flags);
        } else {
            print_field(playing_field, 0, placed_flags);
        }
    }
    if (game_won) {
        print_field(playing_field, 1, placed_flags);
        printf("\nYOU WON! GOOD JOB!\n"); // mogelijkheid om opnieuw te spelen!
    } else {
        print_field(playing_field, 1, placed_flags);
        printf("\nGAME OVER!\n");
    }
    if (handle_replay()) {
        printf("OK1\n");
        main();
    }
}