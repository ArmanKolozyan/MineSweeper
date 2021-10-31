#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

const int total_bombs = 4;
const int rows = 4;
const int columns = 4;
int placed_flags = 0;
int correct_placed_flags = 0;
int game_over = 0;
int game_won = 0;
int remaining_nonbomb_cells = rows * columns - total_bombs;
struct cell {
    int bomb;
    int revealed;
    int flagged;
    int neighbours_count;
};
struct cell playing_field[4][4]; // globaal maken

void initialize_field(struct cell playing_field[][columns]) {
    srand(time(0));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            struct cell *current_cell = &playing_field[i][j];
            current_cell->bomb = 0;
            current_cell->revealed = 0;
            current_cell->flagged = 0;
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
                bomb_cell->bomb = 1;
                placed_bombs++;
                if (placed_bombs == total_bombs) {
                    break;
                }
            }
        }
    }
}

void print_field(struct cell (*playing_field)[columns], int reveal_all) {
    printf("Remaining flags: %i\n", total_bombs - placed_flags);
    printf("      0    1    2    3\n");
    for (int i = 0; i < rows; i++) {
        printf("   %i|", i);
        for (int j = 0; j < columns; j++) {
            struct cell *current_cell = &playing_field[i][j];
            if (current_cell->revealed || current_cell->flagged || reveal_all) {
                if (current_cell->flagged) { // sequence of conidition checks plays a crucial role
                    printf(" F ||");
                } else if (!current_cell->bomb) {
                    printf(" %i ||", current_cell->neighbours_count);
                } else {
                    printf(" B ||");
                }
            } else {
                printf("   ||");
            };
        }
        printf("\n");
    }
}

// cell operations

void calculate_neighbours_bombs(struct cell playing_field[][columns]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            struct cell *current_cell = &playing_field[i][j];
            if (current_cell->bomb) {
                continue;
            }
            {
                int total_bombs = 0;
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
                                total_bombs++;
                            }
                        }
                    }
                }
                current_cell->neighbours_count = total_bombs;
            }
        }
    }
}

void remove_flag(struct cell *the_cell);

void place_flag(struct cell *the_cell) {
    if (!the_cell->flagged) {
        if (the_cell->revealed) {
            printf("Action cannot be done. Cell is revealed.\n");
        } else if (placed_flags < total_bombs) {
            the_cell->flagged = 1;
            placed_flags++;
            if (the_cell->bomb) {
                correct_placed_flags++;
                if (correct_placed_flags == total_bombs) {
                    game_won = 1;
                }
            }
        } else {
            printf("No flags left!");
        }
    } else {
        remove_flag(the_cell);
    }
}

void remove_flag(struct cell *the_cell) { // altijd zo'n argument meegeven!!
    the_cell->flagged = 0;
    placed_flags--;
    if (the_cell->bomb) {
        correct_placed_flags--;
    }
}

void reveal_neighbours(struct cell playing_field[][columns], int row, int column);

void reveal(struct cell playing_field[][columns], int row, int column) {
    struct cell *the_cell = &playing_field[row][column];
    if (the_cell->bomb) {
        game_over = 1;
    } else if (the_cell->revealed) {
        printf("Cell is already revealed!\n");
    } else {
        the_cell->revealed = 1;
        remaining_nonbomb_cells--;
        if (the_cell->flagged) {
            remove_flag(the_cell);
        }
        if (remaining_nonbomb_cells == 0) {
            game_won = 1;
        }
        if (the_cell->neighbours_count == 0) {
            reveal_neighbours(playing_field, row, column);
        }
    }
}

void reveal_neighbours(struct cell playing_field[][columns], int row, int column) {
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
                    reveal(playing_field, neighbour_i, neighbour_j);
                }
            }
        }
    }
}

// cell operations

// input
void clear_input() {
    char c;
    while ((c = getchar()) != '\n' && c != EOF) { //getchar() returns EOF in the event of a read error, so the loop should test for EOF also
        ;
    }
}

int check_boundries(int user_row, int user_column) {
    return ((user_row >= 0) && (user_row < rows) && (user_column >= 0) && (user_column < columns));
}

int get_arguments(int *user_row, int *user_column) {
    char userInput[10];
    fgets(userInput, 9, stdin);
    if (sscanf(userInput, "%i %i", user_row, user_column) < 2) // als er 3 args zijn neem ik 2, no prob?
    {
        printf("Too few arguments! Try again.\n");
        sleep(2);
        print_field(playing_field, 0);
        return 0;
    } else if (check_boundries(*user_row, *user_column)) {
        return 1;
    } else {
        printf("Input is out of bounds! Try again.\n");
        sleep(2);
        print_field(playing_field, 0);
        return 0;
    }
}

void get_input(char *command, int *user_row, int *user_column) {
    char after_command;
    printf("Write your command: \n");
    *command = getchar();
    after_command = getchar();
    if (*command == 'p' && after_command == '\n') {
        ;
    } else if ((*command == 'r' || *command == 'f') && after_command == ' ') {
        if (!get_arguments(user_row, user_column)) {
            get_input(command, user_row, user_column);
        }
    } else if ((*command == 'r' || *command == 'f') && after_command == '\n') {
        printf("Please provide arguments after the command!\n");
        sleep(2);
        print_field(playing_field, 0);
        get_input(command, user_row, user_column);
    } else {
        printf("Wrong command! Try again.\n");
        sleep(2);
        print_field(playing_field, 0);
        clear_input();
        get_input(command, user_row, user_column);
    }
}

void handle_input(struct cell playing_field[][columns], char *command, int *user_row, int *user_column) {
    if (*command == 'r') // 'r' enz. globale variabelen maken als bv. REVEAL_KEY
    {
        reveal(playing_field, *user_row, *user_column);
        print_field(playing_field, 0);
    } else if (*command == 'f') {
        place_flag(&playing_field[*user_row][*user_column]);
        print_field(playing_field, 0);
    } else if (*command == 'p') {
        game_over = 1;
    }
}

// input

void main(void) {
    char command;
    int user_row = rand() % rows; // will be overwritten if command is 'r' or 'f'
    int user_column = rand() % columns;
    initialize_field(playing_field);
    print_field(playing_field, 0);
    get_input(&command, &user_row, &user_column);
    install_bombs(playing_field, user_row, user_column);
    calculate_neighbours_bombs(playing_field);
    handle_input(playing_field, &command, &user_row, &user_column);
    while (!game_over && !game_won) {
        get_input(&command, &user_row, &user_column);
        handle_input(playing_field, &command, &user_row, &user_column);
    }
    if (game_won) {
        printf("You won! Good job!");
        print_field(playing_field, 1);
    } else {
        print_field(playing_field, 1);
        printf("\nGame over!");
    }
}