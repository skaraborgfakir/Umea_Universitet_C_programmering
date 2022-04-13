/*
 * Programmering i C
 * Spring 22
 * Mastery test 8
 *
 * Date:         Time-stamp: <2022-04-13 18:22:37 stefan>
 * File:         mp8.c
 * Description:  A simple implementation of Conway's Game of Life.
 * Author:       Stefan Niskanen Skoglund
 * CS username:  ens21snd
 * Input:        Choice of initial configuration and then instruction
 *               to step or exit.
 * Output:       Prints the game field in each step.
 * Limitations:  No validation of input.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Constants, representation of states */
#define ALIVE 'X'
#define DEAD '.'

/* Declaration of data structure */
typedef struct {
  char current;
  char next;
} cell;

/* Declaration of functions */
void init_field    ( const int rows, const int cols, cell field[rows][cols]);
void load_glider   ( const int rows, const int cols, cell field[rows][cols]);
void load_semaphore( const int rows, const int cols, cell field[rows][cols]);
void load_random   ( const int rows, const int cols, cell field[rows][cols]);
void load_custom   ( const int rows, const int cols, cell field[rows][cols]);


/* Description: Start and run games, interact with the user.
 * Input:       About what initial structure and whether to step or
 *              exit.
 * Output:      Prints information to the user, and the game field in
 *              each step.
 */

int main( void)
{
  return 0;
}


/* Description: Initialize all the cells to dead, then asks the user
 *              about which structure to load, and finally load the
 *              structure.
 * Input:       The field array and its size.
 * Output:      The field array is updated.
 */

void init_field( const int rows, const int cols,
		 cell field[rows][cols])
{
  for (int r = 0 ; r < rows ; r++) {
    for (int c = 0 ; c < cols ; c++) {
      field[r][c].current = DEAD;
    }
  }

  printf("Select field spec to load ");
  printf("([G]lider, [S]emaphore, ");
  printf("[R]andom or [C]ustom): ");

  int ch = getchar();

  /* Ignore following newline */
  if (ch != '\n') {
    getchar();
  }

  switch (ch) {
  case 'g':
  case 'G':
    load_glider(rows, cols, field);
    break;
  case 's':
  case 'S':
    load_semaphore(rows, cols, field);
    break;
  case 'r':
  case 'R':
    load_random(rows, cols, field);
    break;
  case 'c':
  case 'C':
  default:
    load_custom(rows, cols, field);
  }
}


/* Description: Inserts a glider into the field.
 * Input:       The field array and its size.
 * Output:      The field array is updated.
 */

void load_glider(const int rows, const int cols,
		 cell field[rows][cols])
{
	field[0][1].current = ALIVE;
	field[1][2].current = ALIVE;
	field[2][0].current = ALIVE;
	field[2][1].current = ALIVE;
	field[2][2].current = ALIVE;
}


/* Description: Inserts a semaphore into the field.
 * Input:       The field array and its size.
 * Output:      The field array is updated.
 */

void load_semaphore(const int rows, const int cols,
		    cell field[rows][cols])
{
	field[8][1].current = ALIVE;
	field[8][2].current = ALIVE;
	field[8][3].current = ALIVE;
}


/* Description: Inserts a random structure into the field.
 * Input:       The field array and its size.
 * Output:      The field array is updated. There is a 50 % chance
 *              that a cell is alive.
 */

void load_random(const int rows, const int cols,
		 cell field[rows][cols])
{

}


/* Description: Lets the user specify a structure that then
 *              is inserted into the field.
 * Input:       The field array and its size.
 * Output:      The field array is updated.
 */

void load_custom(const int rows, const int cols,
		 cell field[rows][cols])
{
	printf("Give custom format string: ");
	do {
		int r, c;
		scanf("%d,%d", &r, &c);
		field[r][c].current = ALIVE;
	} while (getchar() != '\n');
}
