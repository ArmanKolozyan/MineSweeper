#include "handleInput.h"
#include "macros.h"
#include "printing.h"
#include <stdio.h>
#include <unistd.h> //voor sleep

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

void handle_input(struct cell playing_field[][columns], enum Command *command, int *user_row, int *user_column, int *placed_flags, int *correct_placed_flags) {
    if (*command == REVEAL) // 'r' enz. globale variabelen maken als bv. REVEAL_KEY
    {
        if (reveal(playing_field, *user_row, *user_column, placed_flags, correct_placed_flags)) {
        }
    } else if (*command == FLAG) {
        place_flag(&playing_field[*user_row][*user_column], placed_flags, correct_placed_flags);
    } else if (*command == PRINT) {
    }
}

enum Boolean handle_replay() {
    printf("Press ENTER if you want to replay!\n");
    getchar() == '\n';
}