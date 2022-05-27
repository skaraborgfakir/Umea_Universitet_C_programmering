/*
 * Programmering i C
 * Spring 22
 * Mastery test 9
 *
 * Date:         Time-stamp: <2022-05-22 13:40:04 stefan>
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
#include "game_of_life_file_handler.h"

/* Hjälpfunktioner för att definiera vissa operationer
 */
void dump_field( field *current_field);
int neighbours( field *current_field, const int row, const int col);

/* Krav från uppgiftsektionen för Mästarprov 9:
 *   funktioner för manuell minnesallokering
 *     4: implementera funktioner för dynamisk minneshantering
 *     6: placera de här i game_of_life.c
 *
 * allokering och avallokering av uppgifter om själva ytan (dimensioner)
 */
field* allocate_field()
{
   return malloc(sizeof(field));
}
void dispose_field(field** current_field)
{
   if ( current_field != 0 &&
	*current_field != 0)
   {
      free(*current_field);
      *current_field=0;
   }
}

/* Krav från uppgiftsektionen för Mästarprov 9:
 *   allokering och avallokering av uppgifter om själva ytan (dimensioner)
 *
 *   använder uppgifter från current_field för att kontrollera vektorns storlek
 */
cell** allocate_cells(int rows, int cols)
{
   cell** cells_ptr = malloc( sizeof(cell *) * rows);

   for (int i = 0; i < rows ; i++)
   {
      cells_ptr[i] = malloc( sizeof(cell) * cols);
   }
   return cells_ptr;
}
void dispose_cells(field* current_field)
{
   if (current_field->cells != 0)
      free(current_field->cells);

   current_field->cells=0;
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
void dump_field( field* current_field)
{
   for (int y = 0; y < current_field->rows ; y++)
   {
      for (int x = 0; x < current_field->cols ; x++)
      {
	 if ( ((cell *) current_field->cells[y])[x].current == DEAD)
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
void one_generation( field* current_field)
{
   for (int y = 0; y < current_field->rows ; y++)
   {
      for (int x = 0; x < current_field->cols ; x++)
      {
	 int n_neighbours = neighbours( current_field, y, x);
	 switch( (( cell *) current_field->cells[y])[x].current)
	 {
	    case ALIVE:
	       if ( n_neighbours < 2)       /* inte tillräckligt mycket stöd från grannar för att överleva */
		  (( cell *) current_field->cells[y])[x].next = DEAD;
	       else if ( n_neighbours > 3 ) /* överbefolkning */
		  (( cell *) current_field->cells[y])[x].next = DEAD;
	       else
		  (( cell *) current_field->cells[y])[x].next = ALIVE;
	       break;
	    case DEAD:
	    default:
	       if ( n_neighbours == 3) /* tre levande grannar så en klon migrerar till cellen */
		  (( cell *) current_field->cells[y])[x].next = ALIVE;
	       else
		  (( cell *) current_field->cells[y])[x].next = DEAD;
	       break;
	 }
      }
   }
   /* låt en generation gå
    */
   for (int y = 0; y < current_field->rows ; y++)
   {
      for (int x = 0; x < current_field->cols ; x++)
      {
	 if ( current_field->cells[y][x].next == ALIVE)
	    current_field->cells[y][x].current = ALIVE;
	 else
	    current_field->cells[y][x].current = DEAD;

	 /* switch( (( cell *) current_field.cells[y])[x].current) */
	 /* { */
	 /*    case ALIVE: */
	 /*       if ( (( cell *) current_field.cells[y])[x].next == DEAD) */
	 /*	  (( cell *) current_field.cells[y])[x].current = DEAD; */
	 /*       break; */
	 /*    case DEAD: */
	 /*    default: */
	 /*       if ( (( cell *) current_field.cells[y])[x].next == ALIVE) */
	 /*	  (( cell *) current_field.cells[y])[x].current = ALIVE; */
	 /* } */
      }
   }
}

/* Description: Initialisera alla celler till dead/tom, läs in från
 *              infil vilka som ska vara upptagna/levande
 * Input:       pekare till en yta
 *              pekare till FILE-struktur för infil
 * Output:      int som markör för om inläsningen lyckades
 *
 * modifierad för att använda sig av field-strukturen
 *
 * Krav från uppgiftsektionen för Mästarprov 9:
 *   6: flytta flertalet funktioner från mp9.c till game_of_life.c
 */
int init_field(field *current_field, FILE *infil)
{
   if (current_field != 0 &&
       current_field->cells != 0)
      dispose_cells(current_field);

   /* fprintf( stderr, "int init_field(field *current_field, FILE *infil)\n");*/
   if (!load_config_from_file(current_field, infil))
      return 0;
   else
      return 1;
}

/*
 * Local Variables:
 * c-file-style: "ellemtel"
 * compile-command: "make"
 * End:
 */
