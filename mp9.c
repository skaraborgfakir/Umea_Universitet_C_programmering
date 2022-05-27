/*
 * Programmering i C
 * Spring 22
 * Mastery test 9
 *
 * Date:         Time-stamp: <2022-05-22 13:36:04 stefan>
 * File:         mp9.c
 * Description:  A simple implementation of Conway's Game of Life.
 * Author:       Stefan Niskanen Skoglund
 * CS username:  ens21snd
 * Input:        Choice of initial configuration and then instruction
 *               to step or exit.
 * Output:       Prints the game field in each step.
 * Limitations:  No validation of input.
 */

#define __USE_XOPEN
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#include "game_of_life.h"
#include "game_of_life_file_handler.h"

/* Krav från uppgiftsektionen för Mästarprov 9:
 *   3 använd funktionen check_prog_params
 *   6 placera denna i mp9.c
 */
int check_prog_params(int argc, const char *argv[],
		      FILE **in_file_p, FILE **out_file_p);


/* Description: Start and run games, interact with the user.
 * Input:       About what initial structure and whether to step or exit.
 * Output:      Prints information to the user, and the game field in each step.
 *
 * Krav från uppgiftsektionen för Mästarprov 9:
 *   3 använd funktionen check_prog_params
 *   6 töm mp9.c på övriga funktioner förutom main och check_prog_params
 *
 * analys av mp9 i peppar:
 *   första skärmbilden man får är utan transformering, likaså är den dumpade
 *   resultatet samma sak, alltså ska filen läses in, läget visas och direkt kunna dumpas
 */
int main( int argc,
	  const char *argv[])
{
   field* current_field; /* världen */
   FILE *infil;         /* startläge */
   FILE *resultatsfil;  /* nuvarande läge sparas vid programavslut */

   /* används för läsning från terminal */
   const int bufferdim = 10;
   char buffer[bufferdim];

   /* se till att argumenten blir kontrollerade och att filerna blir öppnade
    */
   int arg_status = check_prog_params(argc, argv, &infil, &resultatsfil);

   if (!arg_status)
   {
      current_field = allocate_field();
      /* allt ok, avallokera sedan tidigare inladdad status, ladda nytt startläge och låt generationerna komma
       */
      if (!init_field( current_field, infil))
      {
	 int time_to_quit = 0;

	 while (!time_to_quit)
	 {
	    dump_field(current_field);

	    printf( "Select one of the following options:\n        (enter) Step\n        (any)   Save and exit\n");

	    char *input = fgets( buffer, bufferdim-1, stdin);
	    if (input==0)
	    {
	       time_to_quit = 1; /* 0 från fgets, inget att läsa - tryckte användaren på ^D ? */
	    }
	    else
	    {
	       if (strlen(buffer) == 1)
	       {
		  switch( buffer[0])
		  {
		     case '\n':
			time_to_quit = 0;
			break;
		     default:
			time_to_quit = 1;
		  }
	       }
	       else                       /* ett eller flera tecken, spelar ingen roll det är inte ett \n */
		  time_to_quit = 1;
	    }

	    if (!time_to_quit)
	       /* låt generationerna komma */
	       one_generation(current_field);
	 }

	 if (save_config_to_file(*current_field, resultatsfil))
	    exit(1);
      }
      else
	 exit(1);
   }
   else
      exit(1);

   exit(0);
}

/* Description: Checks the parameters to the program. Checks if the
 * call to the program has the right number of
 * parameters. Open the input and output files.
 * Input: The parameters to the program and two pointers to file
 *        pointers.
 * Output: Returns 0 when the files are correctly opened.
 *         Returns a non-zero value on failure.
 *
 * Krav från uppgiftsektionen för Mästarprov 9:
 *   3 använd funktionen check_prog_params
 *   6 placera denna i mp9.c
 *
 * Den här kontrollerar:
 *   att antalet argument är korrekt (det ska vara två st, två filnamn)
 *     annars: printf("Usage: %s <input configuration file> <output configuration file>\n",argv[0]);
 *   att första argumentet är en existerande fil
 *     annars: printf("Could not open the file: %s\n",argv[1]);
 *   att fil för utmatning är eller kan var skrivbar (den finns inte redan)
 *     annars: printf("Could not open the file: %s\n",argv[2]);
 */
int check_prog_params( int argc,
		       const char *argv[],
		       FILE **in_file_p,
		       FILE **out_file_p)
{
   if ( argc != 3 )
   {
      fprintf( stderr, "Usage: %s <input configuration file> <output configuration file>\n", argv[0]);
      return(1);
   }
   else
   {
      /* plocka fram attibuten för in_file (argv[2])
       *   existerar den ?
       *     raderar inte innehållet i en existerande
       */
      struct stat in_status;
      int stat_in_status = stat( argv[1], &in_status);
      if ( stat_in_status!=0 &&
	   errno == ENOENT)
      {
	 fprintf( stderr, "Could not open the file: %s\n", argv[1]);
	 return(1);
      }
      else
      {
	 /* Öppna inkommande för enbart läsning
	  */
	 *in_file_p = fopen( argv[1], "r");
	 if (*in_file_p==0)
	 {
	    fprintf( stderr, "Could not open the file: %s\n", argv[1]);
	    return(1);
	 }
      }

      /* plocka fram attibuten för out_file (argv[2])
       *   existerar den ?
       *     raderar inte innehållet i en existerande
       */
      struct stat ut_status;
      int stat_ut_status = stat( argv[2], &ut_status);
      if ( stat_ut_status!=0)
      {
	 /* misslyckad - varför ?
	  */
	 if (errno == ENOENT)
	 {
	    /* filen finns inte, alltså fritt fram att skapa en ny
	     * öppna filen som ett skrivbart FILE objekt, samtidigt skapas den (touch)
	     */
	    *out_file_p = fopen( argv[2], "w");
	    if(*out_file_p==0)
	    {
	       fprintf( stderr, "Could not open the file: %s\n", argv[2]);
	       return(1);
	    }
	    return(0);
	 }
	 else
	 {
	    fprintf( stderr, "Could not open the file: %s\n", argv[2]);
	    return(1);
	 }
      }
      else
      {
	 /* lyckades - den finns redan */
	 *out_file_p = fopen( argv[2], "w");
	 if(*out_file_p==0)
	 {
	    fprintf( stderr, "Could not open the file: %s\n", argv[2]);
	    return(1);
	 }
	 return(0);
      }
   }
}

/*
 * Local Variables:
 * c-file-style: "ellemtel"
 * compile-command: "make"
 * End:
 */
