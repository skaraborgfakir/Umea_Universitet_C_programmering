/*
 * Programmering i C
 * Spring 22
 * Mastery test 9
 *
 * Date:         Time-stamp: <2022-05-07 23:12:37 stefan>
 * File:         game_of_life_file_handler.c
 * Description:  A simple implementation of Conway's Game of Life.
 * Author:       Stefan Niskanen Skoglund
 * CS username:  ens21snd
 */

#ifndef GAME_OF_LIFE_H
#define GAME_OF_LIFE_H

/* Constants, representation of states
 */
#define ALIVE 'X'
#define DEAD '.'

/* Declaration of data structure
 */
typedef struct {
   char current;
   char next;
} cell;

/* Krav från uppgiftsektionen för Mästarprov 9:
 *   2: skapa en struktur som beskriver världen som organismerna ska vara i
 */
typedef struct {
   int rows;
   int cols;

   cell **cells;  /* pekare-till-pekare (eller pekare till vektor av pekare till cell-vektorer) */
} field;

/* Krav från uppgiftsektionen för Mästarprov 9:
 *   4: implementera funktioner för dynamisk minneshantering
 *   6: placera de här i game_of_life.c
 */
cell **allocate_cells ( const int nrows, const int ncols);
/* Krav från uppgiftsektionen för Mästarprov 9:
 *   4: implementera funktioner för dynamisk minneshantering
 *   6: placera de här i game_of_life.c
 */
void dispose_field( cell **current_cells);

/* dump av skärmbild
 */
void dump_field( field current_field);

/* en generation
 */
void one_generation( field current_field);

#endif

/*
 * Local Variables:
 * c-file-style: "ellemtel"
 * compile-command: "make"
 * End:
 */
