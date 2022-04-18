/*
 * Programmering i C
 * Spring 22
 * Mastery test 9
 *
 * Date:         Time-stamp: <2022-04-18 14:19:40 stefan>
 * File:         mp9.c
 * Description:  A simple implementation of Conway's Game of Life.
 * Author:       Stefan Niskanen Skoglund
 * CS username:  ens21snd
 * Input:        Choice of initial configuration and then instruction
 *               to step or exit.
 * Output:       Prints the game field in each step.
 * Limitations:  No validation of input.
 */

#include <stdio.h>
#define __USE_XOPEN
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
void init_field    ( const int rows, const int cols, cell field[rows][cols]);
void atlasutskrift ( const int nrows, const int ncols, cell field[nrows][ncols]);
int antalgrannar   ( const int nrows, const int ncols, cell atlas[nrows][ncols], const int row, const int col);

/* Description: Start and run games, interact with the user.
 * Input:       About what initial structure and whether to step or
 *              exit.
 * Output:      Prints information to the user, and the game field in
 *              each step.
 */

int main( void)
{
   /*
    * krav : fixa en värld (matris av celler) med en visst antal positioner
    * exemplet i peppar använder
    *
    * exemplet använder 20x20 så låt oss göra så
    */

   cell atlas[nrows][ncols];
   init_field( nrows, ncols, atlas);

   int avsluta = 0;
   const int bufferdim = 10;
   char buffer[bufferdim];

   while(!avsluta)
   {
      atlasutskrift( nrows, ncols, atlas);

      printf( "Select one of the following options:\n(enter) Step\n(any)   Exit\n");

      char *input = fgets( buffer, bufferdim-1, stdin);

      if ( input == 0)
	 avsluta = 1; /* 0 från fgets, inget att läsa - tryckte användaren på ^D ? */
      else
      {
	 if ( strlen( buffer) == 1 && !strcmp( "\n", buffer)) /* tom rad */
	    avsluta = 0;
	 else
	    avsluta = 1;
      }
   }

   return 0;
}

int antalgrannar( const int nrows, const int ncols, cell atlas[nrows][ncols],
		  const int row, const int col)
{

   return 0;
}


/*
 */
void atlasutskrift( const int nrows, const int ncols, cell atlas[nrows][ncols] )
{
   for (int r = 0 ; r < nrows ; r++) {
      for (int c = 0 ; c < ncols ; c++) {
	 putc( atlas[r][c].current, stdout);
      }
      putc( '\n', stdout);
   }
}

/* Description: Initialize all the cells to dead, then asks the user
 *              about which structure to load, and finally load the
 *              structure.
 * Input:       The field array and its size.
 * Output:      The field array is updated.
 *
 * given av uppgiften - ändra inte
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

/*
 * Local Variables:
 * c-file-style: "k&r"
 * compile-command: "make"
 * End:
 */
