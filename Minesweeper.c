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
struct cell
{
    int bomb;
    int revealed;
    int neighbours_bombs_count;
};

void fill_the_field(struct cell (*playing_field)[columns])
{
    srand(time(0));
    int placed_bombs = 0;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            struct cell *current_cell = &playing_field[i][j];
            current_cell->revealed = 0;
            current_cell->neighbours_bombs_count = 0; /* default values geven? */
            if ((placed_bombs < amount_of_bombs) && ((rand() % 10) < 3))
            {
                current_cell->bomb = 1;
                placed_bombs++;
            }
            else
            {
                current_cell->bomb = 0;
            }
        }
    }
}

void calculate_neighbours_bombs(struct cell (*playing_field)[columns])
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            struct cell *current_cell = &playing_field[i][j];
            if (current_cell->bomb == 1) {
                break;
            }
            {
                int total_bombs = 0;
                for (int off_i = -1; off_i <= 1; off_i++)
                {
                    for (int off_j = -1; off_j <= 1; off_j++)
                    {
                        int current_neighbour_i = i + off_i;
                        int current_neighbour_j = j + off_j;
                        if ((current_neighbour_i < 0) || (current_neighbour_i >= rows)) {
                            continue;
                        }
                        if ((current_neighbour_j < 0) || (current_neighbour_j >= columns)) {
                            continue;
                        }
                        {
                            struct cell *current_neighbour = &playing_field[current_neighbour_i][current_neighbour_j];
                            if (current_neighbour->bomb)
                            {
                                total_bombs++;
                            }
                        }
                    }
                }
                current_cell->neighbours_bombs_count = total_bombs;
            }
        }
    }
}

void test_it(struct cell (*playing_field)[columns])
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            struct cell *current_cell = &playing_field[i][j];
            if (current_cell->bomb)
            {
                printf("B ");
            }
            else
            {
                printf("%i ", current_cell->neighbours_bombs_count);
            };
        }
        printf("\n");
    }
}

void main(void)
{
    struct cell playing_field[4][4];
    fill_the_field(playing_field);
    calculate_neighbours_bombs(playing_field);
    test_it(playing_field);
}
