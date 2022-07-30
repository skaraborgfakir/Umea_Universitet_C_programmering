/* -*- mode: c -*-
 *
 * Programmering i C
 * Spring 22
 * Mastery test 9
 *
 * Date:         Time-stamp: <2022-07-28 14:25:29 stefan>
 * File:         game_of_life_file_handler.c
 * Description:  A simple implementation of Conway's Game of Life.
 * Author:       Stefan Niskanen Skoglund
 * CS username:  ens21snd
 * Input:        Choice of initial configuration and then instruction
 *               to step or exit.
 * Output:       Prints the game field in each step.
 * Limitations:  No validation of input.
 */

/* #define __USE_XOPEN*/

#include <stdio.h>

#include "game_of_life.h"
#include "game_of_life_file_handler.h"

/* Description: Loads a configuration to the field structure from a
 *              file. It is the responsiblity of the caller to
 *              deallocate the dynamically allocated memory in the field
 *              structure through a call to the function destroy_field.
 *              The file pointed to by fp is closed.
 * Input: A pointer to where the created field structure should be
 *        assigned and a file pointer to the file with the initial
 *        configuration.
 * Output: Returns 0 on success, the field structure is created
 *         with the configuration from the file.
 *         Returns a non-zero value on failure.
 *
 * Krav från uppgiftsektionen för Mästarprov 9:
 *   3: implementera funktionen load_config_from_file
 *
 * Använder allokera_celler definierad i game_of_life.h
 */
int load_config_from_file(field *nuvarande_situation, FILE *fp)
{
   /* fscanf rekommenderas
    *
    * infilen ska redan vara öppnad
    */
   int resultat = 1;      /* som standard är förutsättning att ett filfel ska rapporterar, inte ok förrän det är visat att den är ok */
   int inmatningsfel = 0;

   if (fscanf( fp, "%u,%u\n", &(nuvarande_situation->rows), &(nuvarande_situation->cols)) == 2)
   {
      char buffer[nuvarande_situation->cols + 1];
      nuvarande_situation->cells = allokera_celler(nuvarande_situation->rows, nuvarande_situation->cols);

      int rad = 0;

      while (inmatningsfel == 0 &&          /* knäck körningen om ett fel är funnet */
	     rad < nuvarande_situation->rows)
      {
	 if (fscanf(fp, "%s", buffer) == 1)
	 {
	    int kolumn = 0;
	    while (inmatningsfel == 0 &&              /* knäck körningen om ett fel är funnet */
		   kolumn < nuvarande_situation->cols)
	    {
	       ((cell*)nuvarande_situation->cells[rad])[kolumn].current = DEAD;

	       switch (buffer[kolumn])
	       {
		  case '*':               /* bebodd cell */
		     ((cell*)nuvarande_situation->cells[rad])[kolumn].current = ALIVE;
		     break;
		  case '0':               /* obebodd cell */
		     ((cell*)nuvarande_situation->cells[rad])[kolumn].current = DEAD;
		     break;
		  default:                /* ett fel funnet (okänd bokstav), knäck körningen */
		     fprintf( stderr, "Incorrect configuration file format\n");
		     inmatningsfel = 1;
	       }
	       kolumn++;
	    }
	 }
	 else       /* ett fel funnet, knäck körningen */
	 {
	    fprintf( stderr, "Incorrect configuration file format\n");
	    inmatningsfel = 1;
	 }
	 rad++;
      }

      if (!inmatningsfel)  /* inget fel i körningen ?, om så signalera allt ok */
      {
	 resultat = 0;
      }
   }
   else /* felaktig input på första raden - fick vi två tal ? - NEJ */
   {
      fprintf( stderr, "Incorrect configuration file format\n");
   }

   fclose(fp);
   return resultat;
}

/* Description: Saves the current configuration of the field to a
 * specified file. The file pointed to by fp is closed.
 * Input: The field structure and a file pointer to the file
 *        where the final configuration should be saved.
 * Output: Returns 0 on success, the current configuration in the
 *         field is written to the file.
 *         Returns a non-zero value on failure.
 *
 * Krav från uppgiftsektionen för Mästarprov 9:
 *   3: implementera funktionen save_config_to_file
 *
 * Använder avallokera_ytan definierad i game_of_life.h ?
 */
int save_config_to_file( const field nuvarande_situation, FILE *fp)
{
   fprintf( fp, "%d,%d\n", nuvarande_situation.rows, nuvarande_situation.cols);
   for ( int y=0; y<nuvarande_situation.rows; y++)
   {
      for (int x=0; x<nuvarande_situation.cols; x++)
      {
	 switch( nuvarande_situation.cells[y][x].current)
	 {
	    case ALIVE:
	       fprintf(fp, "%s", "*");
	       break;
	    case DEAD:
	    default:
	       fprintf(fp, "%s", "0");
	 }
      }
      fprintf(fp, "\n");
   }

   fclose(fp);
   return 0;
}

/*
 * Local Variables:
 * c-file-style: "ellemtel"
 * compile-command: "make"
 * End:
 */
