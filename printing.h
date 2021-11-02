#ifndef printing
#define printing

 #include "cell.h"

void print_column_numbers();
void print_field(struct cell (*playing_field)[], enum Boolean reveal_all, int placed_flags);


#endif