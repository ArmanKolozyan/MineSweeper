/* 
In plaats van alle mijnen meteen te plaatsen, moet je hiermee wachten
tot de speler gevraagd heeft om het eerste vakje te onthullen. Op die
manier vermijd je dat de speler meteen, in de eerste beurt, op een
mijn stapt en het spel verliest.
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int amount_of_bombs = 3;
int rows = 4;
int columns = 4;
struct cell {int bomb; 
             int revealed; 
             int neighbors_count;
             };

void fill_the_field(struct cell (*playing_field)[columns]) {
    srand(time(0));
    int placed_bombs = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            struct cell *current_cell = &playing_field[i][j];
            current_cell->revealed = 0;
            current_cell->neighbors_count = 0; /* default values geven? */
            if ((placed_bombs < amount_of_bombs) && ((rand() % 10) < 3)) {
                current_cell->bomb = 1;
                placed_bombs++;
            } else {
                current_cell->bomb = 0;
            }

        }
    }


}

void main(void) {
    struct cell playing_field[4][4];
    fill_the_field(playing_field);
}