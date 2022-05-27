/*
 * Programmering i C
 * Spring 22
 * Mastery test 9
 *
 * Date:         Time-stamp: <2022-05-22 12:55:37 stefan>
 * File:         game_of_life_file_handler.c
 * Description:  A simple implementation of Conway's Game of Life.
 * Author:       Stefan Niskanen Skoglund
 * CS username:  ens21snd
 * Input:        Choice of initial configuration and then instruction
 *               to step or exit.
 * Output:       Prints the game field in each step.
 * Limitations:  No validation of input.
 */

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
 * Använder allocate_cells definierad i game_of_life.h
 */
int load_config_from_file(field *the_field, FILE *fp)
{
   /* fscanf rekommenderas
    *
    * infilen ska redan vara öppnad
    */

   int res = fscanf(fp, "%d,%d\n", &(the_field->rows), &(the_field->cols));
   if (res != 2)
   {
      /* felaktig input på första raden - fick vi två tal ? - NEJ
       */
      fprintf(stderr, "Incorrect configuration file format\n");
      fclose(fp);
      return(1);
   }
   else
   {
      the_field->cells = allocate_cells(the_field->rows, the_field->cols);

      char buffer[the_field->cols + 1];

      for (int y = 0; y < the_field->rows; y++)
      {
	 int res = fscanf(fp, "%s", buffer);

	 if ( res != 1 )
	 {
	    fprintf(stderr, "Incorrect configuration file format\n");
	    fclose(fp);
	    return(1);
	 }

	 for (int x = 0; x < the_field -> cols; x++)
	 {
	    ((cell*)the_field->cells[y])[x].current = DEAD;

	    switch( buffer[x] )
	    {
	       case '*':
		  ((cell*)the_field->cells[y])[x].current = ALIVE;
		  break;
	       case '0':
		  ((cell*)the_field->cells[y])[x].current = DEAD;
		  break;
	       default:
		  fprintf(stderr, "Incorrect configuration file format\n");
		  fclose(fp);
		  return(1);
	    }
	 }
      }
   }

   fclose( fp);
   return(0);
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
 * Använder dispose_field definierad i game_of_life.h ?
 */
int save_config_to_file(const field the_field, FILE *fp)
{
   fprintf(fp, "%d,%d\n", the_field.rows, the_field.cols);
   for (int y=0; y<the_field.rows; y++)
   {
      for (int x=0; x<the_field.cols; x++)
      {
	 switch(the_field.cells[y][x].current)
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
   return(0);
}

/*
 * Local Variables:
 * c-file-style: "ellemtel"
 * compile-command: "make"
 * End:
 */
