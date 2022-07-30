/* -*- mode: c -*-
 *
 * Programmering i C
 * Spring 22
 * Mastery test 9
 *
 * Date:         Time-stamp: <2022-07-28 14:01:26 stefan>
 * File:         mp9.c
 * Description:  A simple implementation of Conway's Game of Life.
 * Author:       Stefan Niskanen Skoglund
 * CS username:  ens21snd
 * Input:        Choice of initial configuration and then instruction
 *               to step or exit.
 * Output:       Prints the game field in each step.
 * Limitations:  No validation of input.
 *
 * svensk beskrivning:
 *               Conways livsspel är ett klassiskt exempel på cellautomata
 *               som kan visualisera termerna stöd/support och överbefolkning
 *
 * I koden och kommentarer använder jag svenska därför att det är mitt starkaste spräk
 * för förståelse och reflektion över något.
 *
 * Jag refererar till de olika skall i specifikationen för att enklare kontrollera att de möts
 */

/* #define __USE_XOPEN */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>

#include "game_of_life.h"
#include "game_of_life_file_handler.h"

/* Krav från uppgiftsektionen för Mästarprov 9:
 *   3 använd funktionen check_prog_params
 *   6 placera denna i mp9.c
 */
int check_prog_params( int argc, const char *argv[],
		       FILE **in_file_p, FILE **out_file_p);
int verifiera_infil(const char *filnamn, FILE **in_file_p);
int verifiera_utfil(const char *filnamn, FILE **out_file_p);

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
   field* nuvarande_situation=0; /* världen */
   FILE*  infil=0;               /* ursprungsläge */
   FILE*  resultatsfil=0;        /* nuvarande situation, sparas vid programavslut */
   int    programstatus = 1;     /* används i exit - som standard ses programmets körning alltid
				  * innan det är känt hur resultat blev som felaktig
				  */

   /* används för läsning från terminal */
   const int bufferdim = 10;
   char      buffer[bufferdim];

   /* normal prompt i programmet
    */
   char* prompttext = "Select one of the following options:\n        (enter) Step\n        (any)   Save and exit\n";

   /* se till att argumenten blir kontrollerade och att filerna blir öppnade
    */
   int arg_status = check_prog_params(argc, argv, &infil, &resultatsfil);

   if (!arg_status)
   {
      /* allt ok, avallokera sedan tidigare inladdad status, ladda nytt startläge och låt generationerna komma
       */
      if (nuvarande_situation != NULL)
      {
	 avallokera_ytan( &nuvarande_situation);
      }
      nuvarande_situation = allokera_spelytan();

      if (!init_field( nuvarande_situation, infil))
      {
	 int tvinga_programslut = 0;     /* sätts till 1 när programmet ska avslutas */
	 int kartfel = 0;                /* kartfel är resultat från kalkylera_generationsskifte - något är fel i kartan */

	 while (!tvinga_programslut)
	 {
	    utskrift( nuvarande_situation);
	    printf( prompttext);
	    char *input = fgets( buffer, bufferdim-1, stdin);   /* kan inte använda getline iom att buffer är en lokal variabel */
	    if (input==0)                                       /* ej allokerad med malloc */
	    {
	       tvinga_programslut = 1; /* 0 från fgets, inget att läsa - tryckte användaren på ^D ? */
	    }
	    else
	    {
	       if (strlen(buffer) == 1 &&    /* lazy eval - en ny körning om buffer innehåller enbart ett ensamt radbryt/return */
		   buffer[0]=='\n')          /* finns det några andra tecken innan radbrytet så avslutas programmet */
	       {
		  kartfel = kalkylera_generationsskifte( nuvarande_situation);  /* låt en ny generation komma */
		  if (!kartfel)
		  {
		     generationsskifte( nuvarande_situation);
		  }
		  else
		  {
		     tvinga_programslut = 1;
		  }
	       }
	       else /* ett eller flera tecken, spelar ingen roll det är inte ett \n */
	       {
		  tvinga_programslut = 1;
	       }
	    }
	 }

	 if (!kartfel &&                                                 /* om kartan inte är felaktigt */
	     !save_config_to_file( *nuvarande_situation, resultatsfil))  /* spara nuvarande situation */
	 {
	    programstatus = 0;                                            /* lyckas skrivning så kan körningen ses som OK  */
	 }
      }
   }

   return programstatus;
}

/* Description: Checks the parameters to the program. Checks if the
 * call to the program has the right number of
 * parameters. Open the input and output files.
 * Input: The parameters to the program and two pointers to file
 *        pointers.
 * Output: Returns 0 when the files are correctly opened.
 *         Returns a non-zero value on failure.
 *     olika resultat:
 *                 1 - fel antal argument till programmet (argc == 3 !)
 *                 2 - första argumentet är inte en existerande läsbar fil
 *                 4 - andra argumentet anger inte ett filnamn till en fil som är skrivbar (den
 *                     måste inte existera men det ska vara möjligt att skapa den.)
 *             felen är definierade för att de ev. ska kunna ackumulera (därför inte 3 i serien, nästa skulle kunna vara 8)
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
   int fel_i_parametrar = 1;  /* resultat av testerna - alltid innan det är sagt så fel */

   if (argc == 3) /* kontrollera att argv innehåller tre strängar - programmets namn och */
   {              /* namnet på två filer - den sista av dem måste inte finnas - den måste dock kunna vara skrivbar ! */
      FILE* infil_tmp_p;
      FILE* utfil_tmp_p;

      switch (verifiera_infil( argv[1], &infil_tmp_p))
      {
	 case 0:   /* allt ok med infilen
		    * fortsätt med destinationsfilen
		    */
	    if (!verifiera_utfil( argv[2], &utfil_tmp_p)) /* allt ok med utfilen, lämna vidare filerna till anropande funktion (main) */
	    {
	       *in_file_p = infil_tmp_p;
	       *out_file_p = utfil_tmp_p;
	       fel_i_parametrar = 0;
	    }
	    else    /* något fel med utfilen */
	    {
		  fprintf( stderr, "Could not open the file: %s\n", argv[2]);
		  fclose( infil_tmp_p);
		  fel_i_parametrar = 4;
	    }
	    break;

	 case 1:        /* infilen existerar inte */
	 case 2:        /* misslyckad öppning av den */
	    fprintf( stderr, "Could not open the file: %s\n", argv[1]);
	    fel_i_parametrar = 2;
	    break;

	 default:        /* det är något problem med infilen */
	    fprintf( stderr, "Usage: %s <input configuration file> <output configuration file>\n", argv[0]);
	    fel_i_parametrar = 2;
      }
   }
   else  /* fel antal parametrar till programmet */
   {
      fprintf( stderr, "Usage: %s <input configuration file> <output configuration file>\n", argv[0]);
   }

   return fel_i_parametrar;
}

/* Beskrivning: kontrollera att stigen (path) och namnet för infil
 * är till en existerande läsbar fil
 * nominerar filnamn en existerande fil så försöker den öppna filen för läsning
 *
 * parametrarna:
 *    filnamn : namn inklusive stig (path) för en fil med en existerande värld av cellautomata
 *    fil_p   : pekare-till-pekare till en FILE struktur
 *
 * returnerar: icke-noll (positivt heltal) om det är något knas
 *          1  : filen existerar inte
 *          2  : misslyckad öppning för läsning - oläsbar pga rättigheter ?
 *          99 : ett annat men obestämt fel
 *
 * sidoeffekter används
 * rutinen skriver inte ut några felmeddelanden - detta är upp till anropande rutin !
 */
int verifiera_infil( const char *filnamn, FILE **fil_p)
{
   int resultat = 1;  /* resultat av testerna - alltid innan det är sagt så fel */
   FILE* tmp_fil_p;

/* kontrollera mha access att filen är läsbar dock är access ett säkerhetshål - om
 * programmet körs under sudo kan access se bortom den temporära root-behörigheten
 * men den efterföljande open kommer att köras som effektivt root, vilket ger en
 * attackmöjlighet om det är möjligt att utnyttja fördröjningen.
 *
 * en temporär användning av open() under användarens
 * normala id är skyddat mot manipulation
 */
   if (access( filnamn, R_OK) == 0)
   {
      /* Öppna inkommande för enbart läsning
       */
      tmp_fil_p = fopen( filnamn, "r");
      if (tmp_fil_p != 0)
      {
	 /* vänta med att köra över innehållet i fil_p tills att det är kontrollerat att
	  * filen är öppningsbar
	  */
	 *fil_p = tmp_fil_p;
	 resultat = 0;        /* markera att allt är OK */
      }
      else
      {
	 resultat = 99; /* knas-fel borde enligt access() vara läsbar, men filens status kan ändra sig ! */
      }
   }
   else
   {
      switch (errno)
      {
	 case ENOENT:       /* no-entry : filen existerar inte */
	    resultat = 1;
	    break;
	 case EACCES:      /* oläsbar fil pga att rättigheterna inte räcker till, hanterar exv ACL */
	    resultat = 2;
	    break;
	    /* de följande felen går relativt lätt att provocera fram */
	 case ENAMETOOLONG: /* någon del i filnamnet (en path/stig) är för lång
			     * tänk en CIFS delning från linux mot en äldre windows
			     */
	 case ENOTDIR:      /* en del i filnamnet (en path/stig) är inte någon mapp
			     */
	 case ELOOP:        /* någonstans i filnamnet finns det en cirkel - symbolisk länk som
			     * pekar till sig själv
			     */
	 default:
	    resultat = 99;
      }
   }

   return resultat;
}

/* Beskrivning: kontrollera att stigen (path) och namnet för utfilen
 * är till en inte nödvändigtvis existerande men skrivbar fil
 *
 * parametrarna:
 *    filnamn: namn (path) till filen
 *    fil_p  : pekare-till-pekare till en FILE struktur, resultat från
 *             en lyckad fopen hamnar här
 *
 * returnerar: icke-noll (positivt heltal) om det är något knas
 *    4  : misslyckad öppning för skrivning - omöjligt pga rättigheter ?
 *         filsystem som inte är skrivbart ?
 *    8  : icke-existerande del av filnamn (path)
 *    99 : ett annat men obestämt fel
 *
 * sidoeffekter används
 * rutinen skriver inte ut några felmeddelanden - detta är upp till anropande rutin !
 */
int verifiera_utfil( const char *filnamn, FILE **fil_p)
{
   int resultat = 1;  /* resultat av testerna - alltid innan det är sagt så fel */
   FILE* tmp_fil_p;
   struct stat ut_status;

   /* plocka fram attibuten för out_file (filnamn)
    *   existerar den ?
    *     raderar inte innehållet i en existerande
    */
   if (stat( filnamn, &ut_status) == 0)
   {
      /* lyckades - den finns redan ?
       */
      tmp_fil_p = fopen( filnamn, "w");
      if (tmp_fil_p == 0 )
      {
	 resultat = 4;
      }
      else
      {
	 *fil_p = tmp_fil_p;
	 resultat = 0;      /* markera att filen är ok */
      }
   }
   else
   {
      /* misslyckad - varför ?
       */
      switch( errno)
      {
	 case ENOENT:
	    /* filen finns inte, alltså fritt fram att skapa en ny
	     * öppna filen som ett skrivbart FILE objekt, samtidigt skapas
	     * den (touch)
	     */
	    tmp_fil_p = fopen( filnamn, "w");
	    if( tmp_fil_p!=0)
	    {
	       *fil_p = tmp_fil_p;
	       resultat = 0;      /* markera att filen är ok */
	    }
	    else
	    {
	       resultat = 4;
	    }
	    break;
	 case EACCES:
	 case ENAMETOOLONG:
	 case ELOOP:
	    resultat = 4;
	    break;
	 case ENOTDIR:
	    resultat = 8;
	    break;
	 default:
	    /* stat fungerade inte, ingen som är läsbar ?
	     */
	    resultat = 99;
	    break;
      }
   }

   return resultat;
}

/*
 * Local Variables:
 * c-file-style: "ellemtel"
 * compile-command: "make"
 * End:
 */
