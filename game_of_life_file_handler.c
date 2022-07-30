/* -*- mode: c -*-
 *
 * Programmering i C
 * Spring 22
 * Mastery test 9
 *
 * Date:         Time-stamp: <2022-07-20 16:37:15 stefan>
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
   int resultat = 0;

   if ( fscanf( fp, "%u,%u\n", &(nuvarande_situation->rows), &(nuvarande_situation->cols)) == 2)
   {
      char buffer[nuvarande_situation->cols + 1];
      nuvarande_situation->cells = allokera_celler(nuvarande_situation->rows, nuvarande_situation->cols);

      int y = 0;

      while ( resultat == 0 && y < nuvarande_situation->rows)
      {
	 if ( fscanf(fp, "%s", buffer) == 1)
	 {
	    int x = 0;
	    while ( resultat == 0 && x < nuvarande_situation->cols )
	    {
	       ((cell*)nuvarande_situation->cells[y])[x].current = DEAD;

	       switch( buffer[x] )
	       {
		  case '*':
		     ((cell*)nuvarande_situation->cells[y])[x].current = ALIVE;
		     break;
		  case '0':
		     ((cell*)nuvarande_situation->cells[y])[x].current = DEAD;
		     break;
		  default:
		     /* fputs( "Incorrect configuration file format\n", stderr);*/
		     fprintf( stderr, "Incorrect configuration file %s\n", "format");
		     resultat = 1;
	       }
	       x++;
	    }
	 }
	 else
	 {
	    /* fputs( "Incorrect configuration file format\n", stderr);*/
	    fprintf( stderr, "Incorrect configuration file format\n");
	    resultat = 1;
	 }
	 y++;
      }
   }
   else
   {
      /* fputs( "Incorrect configuration file format\n", stderr);
       */
      /* felaktig input på första raden - fick vi två tal ? - NEJ
	 fprintf( stderr, "Incorrect configuration file format\n");
	 fflush( stderr);
      */
      fprintf( stderr, "Incorrect configuration file %s\n", "format");

      resultat = 1;
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
