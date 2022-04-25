/*
 * Programmering i C
 * Spring 22
 * Mastery test 9
 *
 * Date:         Time-stamp: <2022-04-18 17:51:40 stefan>
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
 * file. It is the responsiblity of the caller to
 * deallocate the dynamically allocated memory in the field
 * structure through a call to the function destroy_field.
 * The file pointed to by fp is closed.
 * Input: A pointer to where the created field structure should be
 *        assigned and a file pointer to the file with the initial
 *        configuration.
 * Output: Returns 0 on success, the field structure is created
 *         with the configuration from the file.
 *         Returns a non-zero value on failure.
 */
int load_config_from_file(field *the_field, FILE *fp)
{
     /* fscanf rekommenderas
      *
      * infilen ska redan vara öppnad
      */
     int nrows = 0;
     int ncols = 0;
     field the_field;

     int res = fscanf( fp, "%d,%d\n", &field.rows, &field.cols);
     if ( res != 2)
     {
	  /* felaktig input på första raden
	   */
	  return 1;
     }
     else
     {
	  cell ** cells = allocate_field( &the_field);

	  if ( the_field.cells == NULL )
	  {
	       /* misslyckad allokering
		*/
	       return 1;
	  }
     }
}

/* Description: Saves the current configuration of the field to a
 * specified file. The file pointed to by fp is closed.
 * Input: The field structure and a file pointer to the file
 *        where the final configuration should be saved.
 * Output: Returns 0 on success, the current configuration in the
 *         field is written to the file.
 *         Returns a non-zero value on failure.
 */
int save_config_to_file(const field the_field, FILE *fp)
{
     /* fprintf rekommenderas
      */
}

/*
 * Local Variables:
 * c-file-style: "k&r"
 * compile-command: "make"
 * End:
 */
