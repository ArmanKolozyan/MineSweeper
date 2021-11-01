#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

const int total_bombs = 10;
const int rows = 10;
const int columns = 10;
int placed_flags = 0;
int correct_placed_flags = 0;
int game_over = 0;
int game_won = 0;
int remaining_nonbomb_cells = rows * columns - total_bombs;

enum Boolean {
    FALSE = 0,
    TRUE
};

enum Command {
    REVEAL = 'r',
    PRINT = 'p',
    FLAG = 'f'
}; // hoofdletters ook checken?

struct cell {
    enum Boolean bomb;
    enum Boolean revealed;
    enum Boolean flagged;
    int neighbours_count;
};

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

void print_column_numbers() {
    printf("      ");
    for (int i = 0; i < columns; i++) {
        printf("%i    ", i);
    }
    printf("\n");
}

void print_field(struct cell (*playing_field)[columns], int reveal_all) {
    if (!reveal_all) {
        printf("Remaining flags: %i\n", total_bombs - placed_flags);
    }
    print_column_numbers();
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
            sleep(2);
        } else if (placed_flags < total_bombs) {
            the_cell->flagged = TRUE;
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
    the_cell->flagged = FALSE;
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
        sleep(2);
    } else {
        the_cell->revealed = TRUE;
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

int get_arguments(struct cell playing_field[][columns], int *user_row, int *user_column) {
    if (scanf("%i %i", user_row, user_column) != 2) {
        printf("Please provide numbers as arguments.\n");
        clear_input();
        sleep(2);
        return 0;
    }; // if 3 args, I take 2
    if (check_boundries(*user_row, *user_column)) {
        clear_input();
        return 1;
    } else {
        printf("Input is out of bounds! Try again.\n");
        clear_input(); // check of '\n' in array zit, zo ja => doe niks anders clear_input
        sleep(2);
        print_field(playing_field, 0);
        return 0;
    }
}

void get_input(struct cell playing_field[][columns], enum Command *command, int *user_row, int *user_column) {
    char after_command;
    printf("Write your command: \n");
    *command = getchar();
    after_command = getchar();
    if (*command == PRINT && after_command == '\n') {
        ;
    } else if ((*command == REVEAL || *command == FLAG) && after_command == ' ') {
        if (!get_arguments(playing_field, user_row, user_column)) {
            print_field(playing_field, 0);
            get_input(playing_field, command, user_row, user_column);
        }
    } else if ((*command == REVEAL || *command == FLAG) && after_command == '\n') {
        printf("Please provide arguments after the command!\n");
        sleep(2);
        print_field(playing_field, 0);
        get_input(playing_field, command, user_row, user_column);
    } else {
        printf("Wrong command! Try again.\n");
        sleep(2);
        print_field(playing_field, 0);
        clear_input();
        get_input(playing_field, command, user_row, user_column);
    }
}

void handle_input(struct cell playing_field[][columns], enum Command *command, int *user_row, int *user_column) {
    if (*command == REVEAL) // 'r' enz. globale variabelen maken als bv. REVEAL_KEY
    {
        reveal(playing_field, *user_row, *user_column);
        print_field(playing_field, 0);
    } else if (*command == FLAG) {
        place_flag(&playing_field[*user_row][*user_column]);
        print_field(playing_field, 0);
    } else if (*command == PRINT) {
        print_field(playing_field, 1);
    }
}

enum Boolean handle_replay() {
    printf("Press ENTER if you want to replay!\n");
    getchar() == '\n';
}

// input

void main() {
    struct cell playing_field[rows][columns]; // globaal maken
    enum Command command;
    int user_row = rand() % rows; // will be overwritten if command is 'r' or 'f'
    int user_column = rand() % columns;
    initialize_field(playing_field);
    print_field(playing_field, 0);
    get_input(playing_field, &command, &user_row, &user_column);
    install_bombs(playing_field, user_row, user_column);
    calculate_neighbours_bombs(playing_field);
    handle_input(playing_field, &command, &user_row, &user_column);
    while (!game_over && !game_won) {
        get_input(playing_field, &command, &user_row, &user_column);
        handle_input(playing_field, &command, &user_row, &user_column);
    }
    if (game_won) {
        printf("\nYOU WON! GOOD JOB!\n"); // mogelijkheid om opnieuw te spelen!
        print_field(playing_field, 1);
    } else {
        print_field(playing_field, 1);
        printf("\nGAME OVER!\n");
    }
    if (handle_replay()) {
        main();
    }
}