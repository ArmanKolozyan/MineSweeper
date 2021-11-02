#ifndef handleInput
#define handleInput


#include "cell.h"
#include "datatypes.h"

void clear_input();
void get_input(struct cell (*playing_field)[], enum Command *command, int *user_row, int *user_column);
void process_input(struct cell (*playing_field)[], enum Command *command, int *user_row, int *user_column, int *placed_flags, int *correct_placed_flags);
enum Boolean handle_replay();


#endif