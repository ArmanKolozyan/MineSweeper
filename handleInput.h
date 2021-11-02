#ifndef handleInput
#define handleInput


#include "cell.h"

void clear_input();
int check_boundries(int user_row, int user_column);
int get_arguments(struct cell (*playing_field)[], int *user_row, int *user_column);
void get_input(struct cell (*playing_field)[], enum Command *command, int *user_row, int *user_column);
void handle_input(struct cell (*playing_field)[], enum Command *command, int *user_row, int *user_column, int *placed_flags, int *correct_placed_flags);
enum Boolean handle_replay();


#endif