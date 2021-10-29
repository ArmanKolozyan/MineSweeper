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
int game_over = 0;
struct cell
{
    int bomb;
    int revealed;
    int neighbours_bombs_count;
};

void initialize_field(struct cell playing_field[][columns])
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

void calculate_neighbours_bombs(struct cell playing_field[][columns])
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            struct cell *current_cell = &playing_field[i][j];
            if (current_cell->bomb == 1)
            {
                continue;
            }
            {
                int total_bombs = 0;
                for (int off_i = -1; off_i <= 1; off_i++)
                {
                    for (int off_j = -1; off_j <= 1; off_j++)
                    {
                        int neighbour_i = i + off_i;
                        int neighbour_j = j + off_j;
                        if ((neighbour_i < 0) || (neighbour_i >= rows))
                        {
                            continue;
                        }
                        if ((neighbour_j < 0) || (neighbour_j >= columns))
                        {
                            continue;
                        }
                        {
                            struct cell *neighbour = &playing_field[neighbour_i][neighbour_j];
                            if (neighbour->bomb)
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

void print_board(struct cell (*playing_field)[columns])
{
    printf("\n\n");
    printf ("     0 1 2 3\n");
    for (int i = 0; i < rows; i++)
    {
        printf("   %i", i); 
        for (int j = 0; j < columns; j++)
        {
            struct cell *current_cell = &playing_field[i][j];
            if (current_cell->revealed)
            {
                printf(" %i", current_cell->neighbours_bombs_count);
            }
            else
            {
                printf(" !");
            };
        }
        printf("\n");
    }
}

void reveal(struct cell playing_field[][columns], int r, int c)
{
    struct cell *the_cell = &playing_field[r][c];
    if (the_cell->bomb)
    {
        game_over = 1;
    }
    else
    {
        the_cell->revealed = 1;
    }
}

void main(void)
{
    struct cell playing_field[4][4];
    initialize_field(playing_field);
    calculate_neighbours_bombs(playing_field);
    char command;
    int user_row;
    int user_column;
    while (!game_over)
    {
        printf("Write your command: \n");
        while (scanf("%c %i %i", &command, &user_row, &user_column) != 3)
        {
            printf("Wrong command! Try again:\n");
        }
        if (command == 'r')
        {
            reveal(playing_field, user_row, user_column);
            print_board(playing_field);
        }
        else
        {
            printf("Wrong command! Try again:\n");
        }
    }
    printf("Game over!");
}
