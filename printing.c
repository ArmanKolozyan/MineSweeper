#include "printing.h"
#include "macros.h"
#include <stdio.h>

void print_column_numbers() {
    printf("      ");
    for (int i = 0; i < columns; i++) {
        printf("%i    ", i);
    }
    printf("\n");
}

void print_field(struct cell playing_field[][columns], enum Boolean reveal_all, int placed_flags) {
    printf("Remaining flags: %i\n", total_bombs - placed_flags);
    print_column_numbers();
    for (int i = 0; i < rows; i++) {
        printf("   %i|", i);
        for (int j = 0; j < columns; j++) {
            struct cell *current_cell = &playing_field[i][j];
            if (current_cell->revealed || current_cell->flagged || reveal_all) {
                if (current_cell->flagged) { // sequence of conidition checks plays a crucial role, because otherwise a flagged cell would contain the neighbours_count
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