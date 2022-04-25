/*
 * Programmering i C
 * Spring 22
 * Mastery test 9
 *
 * Date:         Time-stamp: <2022-04-18 17:50:21 stefan>
 * File:         game_of_life_file_handler.c
 * Description:  A simple implementation of Conway's Game of Life.
 * Author:       Stefan Niskanen Skoglund
 * CS username:  ens21snd
 * Input:        Choice of initial configuration and then instruction
 *               to step or exit.
 * Output:       Prints the game field in each step.
 * Limitations:  No validation of input.
 */

#ifndef GAME_OF_LIFE_H
#define GAME_OF_LIFE_H

/* Constants, representation of states */
#define ALIVE 'X'
#define DEAD '.'

/* Declaration of data structure */
typedef struct {
  char current;
  char next;
} cell;

typedef struct {
  int rows;
  int cols;

  cell **cells;
} field;

/* Declaration of functions */
/* livstidshantering av field struktur - använder malloc !
 */
cell **allocate_field ( field *field_ptr);
void dispose_field( field *current_field);

void dump_field( field current_field);
int neighbours ( field current_field,
		 const int row,
		 const int col);

#endif
/*
 * Local Variables:
 * c-file-style: "k&r"
 * compile-command: "make"
 * End:
 */
