/* -*- mode: c -*-
 *
 * Programmering i C
 * Spring 22
 * Mastery test 9
 *
 * Date:         Time-stamp: <2022-07-13 17:09:57 stefan>
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
typedef struct
 {
   char current;
   char next;
} cell;

/* Krav från uppgiftsektionen för Mästarprov 9:
 *   2: skapa en struktur som beskriver världen som organismerna ska vara i
 */
typedef struct
{
   int rows;
   int cols;

   cell **cells;  /* pekare-till-pekare (eller pekare till vektor av pekare till cell-vektorer) */
} field;

/* Krav från uppgiftsektionen för Mästarprov 9:
 *   funktioner för manuell minnesallokering:
 *     4: implementera funktioner för dynamisk minneshantering
 *     6: placera de här i game_of_life.c
 */
field* allokera_spelytan();
void   avallokera_ytan( field** current_field);
cell** allokera_celler( int rows, int cols);
void   avallokera_celler( field *current_field);

/* uppdaterad version av init_field
 * använder load_config_from_file från game_of_life_file_handler
 */
int init_field( field *current_field, FILE *infil);

/* dump av skärmbild
 */
void utskrift( field *current_field);

/* vad ska hända ?,
 *   vilken fortsätter finnas eller inte ?
 *   vilka nya kommer ?
 */
int kalkylera_generationsskifte( field *nuvarande_situation);

/* låt en ny ny generation komma
 */
void generationsskifte( field* nuvarande_situation);

#endif

/*
 * Local Variables:
 * c-file-style: "ellemtel"
 * compile-command: "make"
 * End:
 */
