/*
 * Programmering i C
 * Spring 22
 * Mastery test 9
 *
 * Date:         Time-stamp: <2022-04-18 17:39:12 stefan>
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

#include "game_of_life.h"

int check_prog_params(int argc, const char *argv[],
		      FILE **in_file_p, FILE **out_file_p);

/* Description: Start and run games, interact with the user.
 * Input:       About what initial structure and whether to step or exit.
 * Output:      Prints information to the user, and the game field in each step.
 */
int main( int argc,
	  char *argv[])
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

/* Description: Checks the parameters to the program. Checks if the
 * call to the program has the right number of
 * parameters. Open the input and output files.
 * Input: The parameters to the program and two pointers to file
 *        pointers.
 * Output: Returns 0 when the files are correctly opened.
 *         Returns a non-zero value on failure.
 */
int check_prog_params(int argc,
		      const char *argv[],
		      FILE **in_file_p,
		      FILE **out_file_p)
{
   if ( argc == 3 )
   {
      /*
       * verifiera att argv[1] är en existerande fil
       */
      struct stat in_status;
      if ( stat( argv[1], &in_status) != 0)
      {
	 /* stat misslyckades
	  */
	 return ( 1);
      }
      else
      {
	 /* stat lyckades
	  */
      }
   }
   else
      return (1);
}

/*
 * Local Variables:
 * c-file-style: "k&r"
 * compile-command: "make"
 * End:
 */
