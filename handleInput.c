#include "handleInput.h"
#include "macros.h"
#include <stdio.h>
#include <unistd.h> //voor sleep

void clear_input() {
    char c;
    while ((c = getchar()) != '\n' && c != EOF) { //getchar() returns EOF in the event of a read error, so the loop should test for EOF also
        ;
    }
}

enum Boolean check_boundries(int user_row, int user_column) {
    return ((user_row >= 0) && (user_row < rows) && (user_column >= 0) && (user_column < columns));
}

enum Boolean get_arguments(int *user_row, int *user_column) {
    const int expecting_arguments = 2; // if the players provides more than 2 arguments, the first two are taken
    if (scanf("%i %i", user_row, user_column) != expecting_arguments) {
        printf("Please provide numbers as arguments.\n"); // because scanf() returns the number of fields that were successfully converted and assigned
        clear_input();
        sleep(2);
        return FALSE;
    };
    if (check_boundries(*user_row, *user_column)) {
        clear_input();
        return TRUE;
    } else {
        printf("Input is out of bounds! Try again.\n");
        clear_input();
        sleep(2);
        return FALSE;
    }
}

void get_input(struct cell playing_field[][columns], enum Command *command, int *user_row, int *user_column) {
    printf("Write your command: \n");
    char after_command;
    *command = getchar();
    after_command = getchar(); // needed to recognize erroneous input

    if (*command == PRINT && after_command == '\n') {
        ;
    } else if ((*command == REVEAL || *command == FLAG) && after_command == ' ') {
        if (!get_arguments(user_row, user_column)) { // because get_arguments return TRUE if everything went right, otherwise it returns FALSE
            //     print_field(playing_field, 0);
            get_input(playing_field, command, user_row, user_column);
        }
    } else if ((*command == REVEAL || *command == FLAG) && after_command == '\n') {
        printf("Please provide arguments after the command!\n");
        sleep(2);
        //  print_field(playing_field, 0);
        get_input(playing_field, command, user_row, user_column);
    } else {
        printf("Wrong command! Try again.\n");
        sleep(2);
        //  print_field(playing_field, 0);
        clear_input();
        get_input(playing_field, command, user_row, user_column);
    }
}

void process_input(struct cell playing_field[][columns], enum Command *command, int *user_row, int *user_column, int *placed_flags, int *correct_placed_flags) {
    if (*command == REVEAL) {
        (reveal(playing_field, *user_row, *user_column, placed_flags, correct_placed_flags));
    } else if (*command == FLAG) {
        place_flag(&playing_field[*user_row][*user_column], placed_flags, correct_placed_flags);
    } else if (*command == PRINT) {  // in the case of PRINT, we don't have to do anything here, but this piece is left here to emphasize that
    }
}

enum Boolean handle_replay() {
    printf("Press ENTER if you want to replay!\n");
    getchar() == '\n';
}