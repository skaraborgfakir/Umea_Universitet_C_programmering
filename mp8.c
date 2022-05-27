/* -*-c-*-
 *
 * Programmering i C
 * Time-stamp: <2022-05-27 17:04:06 stefan>
 * Spring 22
 * Mastery test 8
 *
 * Date:         Time-stamp: <2022-04-17 22:18:57 stefan>
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
/* exemplet i peppar använder
 *
 * exemplet använder 20x20 så låt oss göra så
 */
const int ncols = 20;
const int nrows = 20;

/* Declaration of functions */
void init_field    ( const int rows, const int cols, cell field[rows][cols]);
void load_glider   ( const int rows, const int cols, cell field[rows][cols]);
void load_semaphore( const int rows, const int cols, cell field[rows][cols]);
void load_random   ( const int rows, const int cols, cell field[rows][cols]);
void load_custom   ( const int rows, const int cols, cell field[rows][cols]);
void cellutskrift  ( const int nrows, const int ncols, cell field[nrows][ncols]);
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
     int avsluta = 0;
     cell cellerna[nrows][ncols];
     const int bufferdim = 10;
     char buffer[bufferdim];

     init_field( nrows, ncols, cellerna);

     while(!avsluta)
     {
	  cellutskrift( nrows, ncols, cellerna);

	  /* iterering igenom cellerna för att hitta nödvändiga förändringar
	   *
	   * se: https://conwaylife.com/wiki/Conway%27s_Game_of_Life
	   */
	  for ( int rad = 0 ; rad < nrows ; rad++)
	  {
	       for ( int kolumn = 0 ; kolumn < ncols ; kolumn++)
	       {
		    int grannar = antalgrannar( nrows, ncols, cellerna, rad, kolumn);

		    switch ( cellerna[rad][kolumn].current )
		    {
		    case ALIVE:
			 if ( grannar < 2 )      /* inte tillräckligt mycket stöd från grannar för att överleva */
			      cellerna[rad][kolumn].next = DEAD;
			 else if ( grannar > 3 ) /* överbefolkning */
			      cellerna[rad][kolumn].next = DEAD;
			 else                    /* implicit två eller tre levande grannar - överlever */
			      cellerna[rad][kolumn].next = ALIVE;
			 break;

		    case DEAD:
		    default:
			 if ( grannar == 3) /* tre levande grannar så en klon migrerar till cellen */
			      cellerna[rad][kolumn].next = ALIVE;
			 else
			      cellerna[rad][kolumn].next = DEAD;
		    }
	       }
	  }

	  /*
	   * låt en generation gå
	   */
	  for ( int rad = 0 ; rad < nrows ; rad++)
	  {
	       for ( int kolumn = 0 ; kolumn < ncols ; kolumn++)
	       {
		    switch ( cellerna[rad][kolumn].current )
		    {
		    case ALIVE:
			 if ( cellerna[rad][kolumn].next == ALIVE )
			      cellerna[rad][kolumn].current = ALIVE;
			 else
			      cellerna[rad][kolumn].current = DEAD;
			 break;

		    case DEAD:
			 if ( cellerna[rad][kolumn].next == ALIVE )
			      cellerna[rad][kolumn].current = ALIVE;
		    }
	       }
	  }

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

int antalgrannar( const int antal_rader, const int antal_kolumner, cell cellerna[antal_rader][antal_kolumner],
		  const int cellens_rad, const int cellens_kolumn)
{
     /*
      * algoritm:
      *   påbörja sökning i cell snett uppåt vänster från aktuell cell
      *   därefter sök av radvis, börja i vänster
      *   rad två, den mittersta är aktuell cell
      */
     int antalgrannar = 0;

     for ( int rad = cellens_rad-1 ; rad <= cellens_rad + 1 ; rad++) /* sök igenom 3 rader */
     {
	  for ( int kolumn = cellens_kolumn-1 ; kolumn <= cellens_kolumn + 1 ; kolumn++) /* sök igenom 3 kolumner */
	       if ( rad >= 0 && kolumn >= 0                            &&  /* kontroll att sökningen är i cellerna */
		    rad < antal_rader && kolumn < antal_kolumner       &&  /* inte nedanför eller för långt till höger */
		    !( rad == cellens_rad && kolumn == cellens_kolumn) &&  /* se till att inte få med cellen självt !!! */
		    cellerna[rad][kolumn].current == ALIVE)                /* lever grannen ? */
		    antalgrannar ++;
     }

     return antalgrannar;
}


/*
 */
void cellutskrift( const int antal_rader, const int antal_kolumner, cell cellerna[antal_rader][antal_kolumner] )
{
     for (int rad = 0 ; rad < antal_rader ; rad++) {
	  for (int kolumn = 0 ; kolumn < antal_kolumner ; kolumn++) {
	       putc( cellerna[rad][kolumn].current, stdout);
	       printf( " ");
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


/* Description: Inserts a glider into the field.
 * Input:       The field array and its size.
 * Output:      The field array is updated.
 *
 * given av uppgiften - ändra inte
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
 *
 * given av uppgiften - ändra inte
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
 *
 * ej klar - skrives efter egen skön
 */

void load_random(const int rows, const int cols,
		 cell field[rows][cols])
{
     /*
      * slumptalet behöver ha en seed - klassisk, använd klockan
      */

     for (int x = 0; x < cols; x++)
     {
	  for (int y = 0; y < rows; y++)
	  {
	       double slump = drand48();
	       /*
		* högre än 0,5 lev !
		*/
	       if ( slump > 0.5 )
		    field[y][x].current = ALIVE;
	       else
		    field[y][x].current = DEAD;
	  }
     }
}


/* Description: Lets the user specify a structure that then
 *              is inserted into the field.
 * Input:       The field array and its size.
 * Output:      The field array is updated.
 *
 * given av uppgiften - ändra inte
 */

void load_custom(const int rows, const int cols,
		 cell field[rows][cols])
{
     printf( "Give custom format string: ");
     do
     {
	  int r, c;
	  scanf( "%d,%d", &r, &c);
	  field[r][c].current = ALIVE;
     }
     while ( getchar() != '\n');
}

/*
 * Local Variables:
 * c-file-style: "k&r"
 * compile-command: "make"
 * End:
 */
