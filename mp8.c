/* -*- mode: c -*-
 *
 * Programmering i C
 * Time-stamp: <2022-07-05 11:25:05 stefan>
 * Spring 22
 * Mastery test 8
 *
 * Date:         Time-stamp: <2022-04-17 22:18:57 stefan>
 * File:         mp8.c
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

#include <stdio.h>
#define __USE_XOPEN
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

/* Constants, representation of states */
#define ALIVE 'X'
#define DEAD '.'

/* Declaration of data structure */
typedef struct {
   char current;
   char next;
} cell;

/* exemplet i peppar använder 20x20 så låt oss göra så
 */
const int antal_kolumner = 20;
const int antal_rader = 20;

/* Declaration of functions
 * paramtrarnas namn i en deklaration i C, måste inte vara exakt lika hur själva definitionen ser ut
  */
void init_field                 ( const int rows, const int cols, cell field[rows][cols]);
void load_glider                ( const int rows, const int cols, cell field[rows][cols]);
void load_semaphore             ( const int rows, const int cols, cell field[rows][cols]);
void load_random                ( const int rows, const int cols, cell field[rows][cols]);
void load_custom                ( const int rows, const int cols, cell field[rows][cols]);
void cellutskrift               ( const int antal_rader, const int antal_kolumner, cell cellerna[antal_rader][antal_kolumner]);
int  antalgrannar               ( const int antal_rader, const int antal_kolumner, cell cellerna[antal_rader][antal_kolumner], const int rad, const int kolumn);
void kalkylera_generationsskifte( const int antal_rader, const int antal_kolumner, cell cellerna[antal_rader][antal_kolumner]);
void generationsskifte          ( const int antal_rader, const int antal_kolumner, cell cellerna[antal_rader][antal_kolumner]);
/*
 * påskägg
 */
void load_exempel_sidan_1( const int antal_rader, const int antal_kolumner, cell cellerna[antal_rader][antal_kolumner]);

/* Description: Start and run games, interact with the user.
 * Input:       About what initial structure and whether to step or
 *              exit.
 * Output:      Prints information to the user, and the game field in
 *              each step.
 */
int main( void)
{
   /*
    * krav : fixa en värld (matris av celler) med en visst antal positioner
    * exemplet i peppar använder
    *
    * exemplet använder 20x20 så låt oss göra så
    */
   int avsluta = 0;
   cell cellerna[antal_rader][antal_kolumner];
   const int bufferdim = 10;
   char buffer[bufferdim];

   init_field( antal_rader, antal_kolumner, cellerna);

   while(!avsluta)
   {
      cellutskrift( antal_rader, antal_kolumner, cellerna);                 /* utskrift av världen med cellernas invånare */
      kalkylera_generationsskifte( antal_rader, antal_kolumner, cellerna);  /* bestämm vilka celler som i nästa generation fortfarande ska leva, dö eller uppstå */
      generationsskifte( antal_rader, antal_kolumner, cellerna);            /* låt den nya generationen komma */

      printf( "Select one of the following options:\n(enter) Step\n(any)   Exit\n");

      char *input = fgets( buffer, bufferdim-1, stdin);

      /* den här gillar jag inte riktigt, det är för mycket som blir implicit */
      /* om input fortfarande är lika med buffer så kan längden vara 0 ? */
      if ( input == 0 ||            /* 0 från fgets, inget att läsa - tryckte användaren på ^D ? */
				    /* det här skiljer sig från provprogrammet i peppar, pröva ^D där ! */
	   strlen( buffer ) == 0 || /* helt tom rad ? om raden är tom men input är skilt från 0, hur blir det här sant ?*/
	   strcmp( "\n", buffer)    /* det finns en rad, innehåller den enbart ett ensamt radbrytningstecken eller inte ? */
	 )
	 avsluta = 1;
   }

   return 0;
}

/* description: skriv ut världens invånare och de celler som för stunden är tomma
 *
 * parametrar: antal_rader och antal_kolumner: dimensioner för världen
 *             cellerna : världen själv, en matris med två olika uppgifter för varje cell
 *
 * returnerar inte något, allt hanteras mha sidoeffekter via pekaren in i ytan (cell)
 */
void cellutskrift( const int antal_rader, const int antal_kolumner, cell cellerna[antal_rader][antal_kolumner] )
{
   for ( int rad = 0 ; rad < antal_rader ; rad++) {
      for ( int kolumn = 0 ; kolumn < antal_kolumner ; kolumn++) {
	 putc( cellerna[rad][kolumn].current, stdout);
	 putc( ' ', stdout);
      }
      putc( '\n', stdout);
   }
}

/* Description: Initialize all the cells to dead, then asks the user
 *              about which structure to load, and finally load the
 *              structure.
 * Input:       The field array and its size.
 * Output:      The field array is updated.
 *
 * returnerar inte något, allt hanteras mha sidoeffekter via pekaren in i ytan (cell)
 *
 * given av uppgiften - ändra inte
 */
void init_field( const int rows, const int cols,
		 cell field[rows][cols])
{
   /* sätt alla celler som tomma
    */
   for (int r = 0 ; r < rows ; r++) {
      for (int c = 0 ; c < cols ; c++) {
	 field[r][c].current = DEAD;
      }
   }

   printf("Select field spec to load ");
   printf("([G]lider, [S]emaphore, ");
   printf("[R]andom or [C]ustom): ");

   int ch = getchar();

   /* Ignore following newline */
   if (ch != '\n') {
      getchar();
   }

   switch (ch) {
      case '1':
	 load_exempel_sidan_1(rows, cols, field);
	 break;
      case 'g':
      case 'G':
	 load_glider(rows, cols, field);
	 break;
      case 's':
      case 'S':
	 load_semaphore(rows, cols, field);
	 break;
      case 'r':
      case 'R':
	 load_random(rows, cols, field);
	 break;
      case 'c':
      case 'C':
      default:
	 load_custom(rows, cols,  field);
   }
}


/* Description: Inserts a glider into the field.
 * Input:       The field array and its size.
 * Output:      The field array is updated.
 *
 *              Koden använder sidoeffekter !
 *
 * given av uppgiften - ändra inte
 * glider diagonalt från höger mot motstående hörn
 */
void load_glider( const int rows, const int cols,
		  cell field[rows][cols])
{
   field[0][1].current = ALIVE;
   field[1][2].current = ALIVE;
   field[2][0].current = ALIVE;
   field[2][1].current = ALIVE;
   field[2][2].current = ALIVE;
}


/* Description: Inserts a semaphore into the field.
 * Input:       The field array and its size.
 * Output:      The field array is updated.
 *
 * given av uppgiften - ändra inte
 *
 * tre horisontellt placerade lampor lyser vs
 * att den mittre och en rakt ovanför och en rakt nedanför lyser
 *
 * en evighetsautomata - växlar kontinuerligt mellan två konfigurationer
 */
void load_semaphore( const int rows, const int cols,
		     cell field[rows][cols])
{
   field[8][1].current = ALIVE;
   field[8][2].current = ALIVE;
   field[8][3].current = ALIVE;
}


/* Description: exemplet från specifikationens sida 1
 * Input:       cellerna med uppgifter om dess geometri (bredd och höjd)
 * Output:      vilka celler som är bebodda
 *
 *              Koden använder sidoeffekter !
 *
 * sex st celler i ett kryssmönster - ingen kvar efter den 15:e generationen
 *
 */

void load_exempel_sidan_1( const int antal_rader,
			   const int antal_kolumner,
			   cell      cellerna[antal_rader][antal_kolumner])
{
   cellerna[5][4].current = ALIVE;
   cellerna[5][6].current = ALIVE;
   cellerna[6][5].current = ALIVE;
   cellerna[7][5].current = ALIVE;
   cellerna[8][4].current = ALIVE;
   cellerna[8][6].current = ALIVE;
}


/* Description: Inserts a random structure into the field.
 *              För varje cell i ytan, beräkna ett slumptal och
 *              om slumptalet är störren än hälften av maxtalet för slumpserien, låt cellen vara bebodd.
 * Input:       The field array and its size.
 * Output:      The field array is updated. There is a 50 % chance
 *              that a cell is alive.
 *
 *              Koden använder sidoeffekter !
 */
void load_random( const int rows, const int cols,
		  cell field[rows][cols])
{
   /*
    * slumptalet behöver ha en seed - klassisk, använd klockan
    *
    */
   srand(time(0));

   for (int x = 0; x < cols; x++)
   {
      for (int y = 0; y < rows; y++)
      {
	 if ( rand() > RAND_MAX/2 )       /* hälften av slumptalen ska ge en bebodd cell (lika stor chans att slumptalet är högt eller lågt) */
	    field[y][x].current = ALIVE;  /* skulle ha kunnat kolla om slumptalet är jämnt, samma fördelning (förhoppningsvis.) */
      }
   }
}

/* Description: Lets the user specify a structure that then
 *              is inserted into the field.
 * Input:       The field array and its size.
 * Output:      The field array is updated.
 *
 * given av uppgiften - ändra inte
 */
void load_custom( const int rows, const int cols,
		  cell field[rows][cols])
{
   printf( "Give custom format string: ");
   do
   {
      int r, c;
      scanf( "%d,%d", &r, &c);
      field[r][c].current = ALIVE;
   }
   while ( getchar() != '\n');
}

/* Description: sök igenom cellerna i ytan och tag beslut på vilka celler som
 *              ska fortsätta vara bebodd, bli tom (död) eller fortsätta antingen som tom
 *              eller ska få en ny inneboende
 * Input:       antal_rader och antal_kolumner: världens storlek
 *              cellerna: själva världen med de olika cellerna
 * Output:      ingen, förändringar görs med sidoeffekter, för varje cell finns det ett fält där framtida status lagras
 *              framtida status används i generationsskifte()
 */
void kalkylera_generationsskifte( const int antal_rader,
				  const int antal_kolumner,
				  cell cellerna[antal_rader][antal_kolumner])
{
   /* iterering igenom cellerna för att hitta nödvändiga förändringar
    *
    * se: https://conwaylife.com/wiki/Conway%27s_Game_of_Life
    */
   for ( int rad = 0 ; rad < antal_rader ; rad++)
   {
      for ( int kolumn = 0 ; kolumn < antal_kolumner ; kolumn++)
      {
	 int grannar = antalgrannar( antal_rader, antal_kolumner, cellerna, rad, kolumn);  /* antal levande/bebodda celler runt en en viss cell */

	 switch ( cellerna[rad][kolumn].current )
	 {
	    case ALIVE:
	       switch (grannar)
	       {
		  case 0:                                /* inte tillräckligt mycket stöd från grannar för att överleva */
		  case 1:
		     cellerna[rad][kolumn].next = DEAD;
		     break;
		  case 2:                                /* implicit två eller tre levande grannar - överlever */
		  case 3:
		     cellerna[rad][kolumn].next = ALIVE;
		     break;
		  default:                               /* mer än 3 grannar - svält och överbefolkning ! */
		     cellerna[rad][kolumn].next = DEAD;
	       }
	       break;

	    case DEAD:
	    default:
	       if ( grannar == 3) /* tre levande grannar så en klon migrerar till cellen */
		  cellerna[rad][kolumn].next = ALIVE;
	       else
		  cellerna[rad][kolumn].next = DEAD;
	 }
      }
   }
}

/* beskrivning: antalgrannar: beräknar antal levande celler runt en specifik cell
 *
 * parametrar: antal_rader och antal_kolumner, dimensioner för världen
 *             cellerna världen själv, en matris med två olika uppgifter för varje cell
 *             cellens_rad och cellens_kolumn: för vilken cell ska antal levande grannar beräknas
 * returnerar: antal levande grannar/bebodda celler runt en specifik ( intervall 0..8 )
 */
int antalgrannar( const int antal_rader, const int antal_kolumner, cell cellerna[antal_rader][antal_kolumner],
		  const int cellens_rad, const int cellens_kolumn)
{
   /* algoritm:
    *   påbörja sökning i cell snett uppåt vänster från aktuell cell
    *   därefter sök av radvis, börja i vänster
    *   rad två, den mittersta är aktuell cell
    */
   int antalgrannar = 0;

   for ( int rad = cellens_rad-1 ; rad <= cellens_rad + 1 ; rad++) /* sök igenom 3 rader */
   {
      for ( int kolumn = cellens_kolumn-1 ; kolumn <= cellens_kolumn + 1 ; kolumn++) /* sök igenom 3 kolumner */
	 if ( rad >= 0 && kolumn >= 0                            &&  /* kontroll att sökningen är i cellerna */
	      rad < antal_rader && kolumn < antal_kolumner       &&  /* inte nedanför eller för långt till höger */
	      !( rad == cellens_rad && kolumn == cellens_kolumn) &&  /* se till att inte få med cellen självt !!! */
	      cellerna[rad][kolumn].current == ALIVE)                /* lever grannen ? */
	    antalgrannar ++;
   }

   assert( antalgrannar >= 0 && antalgrannar < 9);

   return antalgrannar;  /* intervallet 0..8 */
}

/* beskrivning: låt några försvinna och låt en ny generation komma
 * parametrar: antal_rader och antal_kolumner, dimensioner för världen
 *             cellerna världen själv, en matris med två olika uppgifter för varje cell
 * returnerar inget - resultatet hanteras mha sidoeffekter
 *            fält i matrisen cellerna i main() förändras via en pekare
 */
void generationsskifte( const int antal_rader,
			const int antal_kolumner,
			cell cellerna[antal_rader][antal_kolumner])
{
   /* låt en generation gå/leva-och-dö
    */
   for ( int rad = 0 ; rad < antal_rader ; rad++)
   {
      for ( int kolumn = 0 ; kolumn < antal_kolumner ; kolumn++)
      {
	 switch ( cellerna[rad][kolumn].current )          /* utgå från nuvarande status */
	 {
	    case ALIVE:                                    /* levande cell */
	       if ( cellerna[rad][kolumn].next == DEAD )   /* dör, pga överbefolkning eller otillräckligt stöd från intilliggande ? */
		  cellerna[rad][kolumn].current = DEAD;
	       break;

	    case DEAD:                                     /* tom/död cell */
	       if ( cellerna[rad][kolumn].next == ALIVE )  /* cellen får en inneboende/klon från intilliggande */
		  cellerna[rad][kolumn].current = ALIVE;
	 }
      }
   }
}

/*
 * Local Variables:
 * c-file-style: "ellemtel"
 * compile-command: "make"
 * End:
 */
