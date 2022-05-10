/*
 * Programmering i C
 * Spring 22
 * Mastery test 9
 *
 * Date:         Time-stamp: <2022-05-08 19:15:36 stefan>
 * File:         game_of_life.c
 * Description:  A simple implementation of Conway's Game of Life.
 * Author:       Stefan Niskanen Skoglund
 * CS username:  ens21snd
 *
 * ska innehålla själva maskinimplementationen med regler och policy från mp8.c
 * exv reglerna för hur en generation hanteras och hur nästa generation ska se ut
 */

#define __USE_XOPEN
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "game_of_life.h"

/* Hjälpfunktioner för att definiera vissa operationer
 */
void dump_field( field current_field);
int neighbours( field *current_field, const int row, const int col);

/* Krav från uppgiftsektionen för Mästarprov 9:
 *   6: placera de här i game_of_life.c
 */
cell **allocate_cells ( const int nrows,
			const int ncols)
{
   cell **cells_ptr = malloc( sizeof(cell *) * nrows);
   for (int i = 0; i < nrows ; i++)
   {
      *(cells_ptr + i) = malloc( sizeof(cell) * ncols);
   }

   return( cells_ptr);
}

/* Krav från uppgiftsektionen för Mästarprov 9:
 *   6: placera de här i game_of_life.c
 */
void dispose_field( cell **current_cells)
{
   /*  free( current_field.cells;*/
}

/* Krav från uppgiftsektionen för Mästarprov 9:
 *   6: flytta flertalet funktioner från mp8.c till game_of_life.c
 */
int neighbours( field *current_field,
		const int row,
		const int col)
{
   int n_neighbours = 0;

   for ( int y = row - 1; y<=row + 1; y++)
      for ( int x = col-1; x <=col+1 ; x++)
	 if ( y >= 0 && x >= 0 &&                                   /* kontroll att sökningen är i cellerna */
	      y < current_field->rows && x < current_field->cols && /* inte nedanför eller för långt till höger */
	      !( y == row && x == col) &&                           /* se till att inte få med cellen självt !!! */
	      ((cell *) current_field->cells[y])[x].current == ALIVE)
	    n_neighbours++;

   return( n_neighbours);
}

/* Krav från uppgiftsektionen för Mästarprov 9:
 *   6: flytta flertalet funktioner från mp8.c till game_of_life.c
 */
void dump_field( field current_field)
{
   for (int y = 0; y < current_field.rows ; y++)
   {
      for (int x = 0; x < current_field.cols ; x++)
      {
	 if ( ((cell *) current_field.cells[y])[x].current == DEAD)
	    putchar('.');
	 else
	    putchar('X');
	 putchar(' ');
      }
      putchar('\n');
   }
}

/* låt en generation kvarstå, dö ut eller födas
 */
void one_generation( field current_field)
{
   for (int y = 0; y < current_field.rows ; y++)
   {
      for (int x = 0; x < current_field.cols ; x++)
      {
	 int n_neighbours = neighbours( &current_field, y, x);
	 switch( (( cell *) current_field.cells[y])[x].current)
	 {
	    case ALIVE:
	       if ( n_neighbours < 2)       /* inte tillräckligt mycket stöd från grannar för att överleva */
		  (( cell *) current_field.cells[y])[x].next = DEAD;
	       else if ( n_neighbours > 3 ) /* överbefolkning */
		  (( cell *) current_field.cells[y])[x].next = DEAD;
	       else
		  (( cell *) current_field.cells[y])[x].next = ALIVE;
	       break;
	    case DEAD:
	    default:
	       if ( n_neighbours == 3) /* tre levande grannar så en klon migrerar till cellen */
		  (( cell *) current_field.cells[y])[x].next = ALIVE;
	       else
		  (( cell *) current_field.cells[y])[x].next = DEAD;
	       break;
	 }
      }
   }
   /* låt en generation gå
    */
   for (int y = 0; y < current_field.rows ; y++)
   {
      for (int x = 0; x < current_field.cols ; x++)
      {
	 switch( (( cell *) current_field.cells[y])[x].current)
	 {
	    case ALIVE:
	       if ( (( cell *) current_field.cells[y])[x].next == DEAD)
		  (( cell *) current_field.cells[y])[x].current = DEAD;
	       break;
	    case DEAD:
	    default:
	       if ( (( cell *) current_field.cells[y])[x].next == ALIVE)
		  (( cell *) current_field.cells[y])[x].current = ALIVE;
	 }
      }
   }
}

/* Description: Initialize all the cells to dead, then asks the user
 *              about which structure to load, and finally load the
 *              structure.
 * Input:       The field array and its size.
 * Output:      The field array is updated.
 *
 * modifierad för att använda sig av field-strukturen
 *
 * Krav från uppgiftsektionen för Mästarprov 9:
 *   6: flytta flertalet funktioner från mp9.c till game_of_life.c
 */
void init_field( const int rows,
		 const int cols,
		 cell field[rows][cols])
{

}

/*
 * Local Variables:
 * c-file-style: "ellemtel"
 * compile-command: "make"
 * End:
 */
