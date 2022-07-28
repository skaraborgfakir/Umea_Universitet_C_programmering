/* -*- mode: c -*-
 *
 * Programmering i C
 * Spring 22
 * Mastery test 9
 *
 * Date:         Time-stamp: <2022-07-28 12:23:55 stefan>
 * File:         game_of_life.c
 * Description:  A simple implementation of Conway's Game of Life.
 * Author:       Stefan Niskanen Skoglund
 * CS username:  ens21snd
 *
 * ska innehålla själva maskinimplementationen med regler och policy från mp8.c
 * exv reglerna för hur en generation hanteras och hur nästa generation ska se ut
 */

/* #define __USE_XOPEN*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "game_of_life.h"
#include "game_of_life_file_handler.h"

/* Hjälpfunktioner för att definiera vissa operationer
 */
int antalgrannar( field *nuvarande_situation, const int rad, const int kolumn);

/* Beskrivning: allokering av struktur för uppgifter om själva ytan (dimensioner)
 * returnerar:  en pekare till en minnesarea som kan rymma en 'field' struct
 *
 * använder sidoeffekter (malloc-anropet)
 *
 * Krav från uppgiftsektionen för Mästarprov 9:
 *   funktioner för manuell minnesallokering
 *     4: implementera funktioner för dynamisk minneshantering
 *     6: placera de här i game_of_life.c
 */
field* allokera_spelytan()
{
   return malloc( sizeof( field));
}

/* Beskrivning: avallokering av strukturen som har uppgifter om själva ytan (dimensioner)
 *
 * använder sidoeffekter
 */
void avallokera_ytan(field** nuvarande_situation)
{
   if ( nuvarande_situation != 0 &&
	*nuvarande_situation != 0)
   {
      free(*nuvarande_situation);
      *nuvarande_situation=0;
   }
}

/* Beskrivning: allokera lagring för spelytan
 *              allokera en minnesarea som kan rymma spelytans bredd antal av 'struct cell', vilket är en pekare
 *              allokera en minnesarea som kan innehålla ytans höjd antal av dessa pekare
 *
 * Krav från uppgiftsektionen för Mästarprov 9:
 *   allokering och avallokering av uppgifter om själva ytan (dimensioner)
 *   använder uppgifter från current_field för att kontrollera vektorns storlek
 */
cell** allokera_celler(int antal_rader, int antal_kolumner)
{
   cell** cells_p = malloc( sizeof(cell *) * antal_rader);

   int i = 0;
   while( i < antal_rader)
   {
      cells_p[i] = malloc( sizeof(cell) * antal_kolumner); /* allokera antal_kolumner antal av struct cell
							    * och
							    */
      i++;
   }
   return cells_p;
}
void avallokera_celler(field* nuvarande_situation)
{
   if (nuvarande_situation->cells != 0)
   {
      free(nuvarande_situation->cells);
   }

   nuvarande_situation->cells=0;
}

/* Beskrivning: beräkna antal levande grannar till en cell
 * parametrar:  nuvarande_situation: världen som den ser ut just nut
 *              rad, kolumn:         för vilken cell i världen som antal grannar ska beräknas
 * returnerar:  antal levande grannar/antal bebodda celler (0-8)
 *
 * Använder inte sidoeffekter
 */
int antalgrannar( field *nuvarande_situation,
		  const int cellens_rad,
		  const int cellens_kolumn)
{
   int resultat = 0;

   /* for ( int y = cellens_rad - 1; y<=cellens_rad + 1; y++) */
   /* { */
   /*    for ( int x = cellens_kolumn-1; x <=cellens_kolumn+1 ; x++) */
   /*    { */
   /*	 if ( y >= 0 && x >= 0 &&                                               /\* kontroll att sökningen är i cellerna *\/ */
   /*	      y < nuvarande_situation->rows && x < nuvarande_situation->cols && /\* inte nedanför eller för långt till höger *\/ */
   /*	      !( y == cellens_rad && x == cellens_kolumn) &&                    /\* se till att inte räkna med cellen självt !!! *\/ */
   /*	      ((cell *) nuvarande_situation->cells[y])[x].current == ALIVE)     /\* lever grannen ? *\/ */
   /*	 { */
   /*	    resultat++; */
   /*	 } */
   /*    } */
   /* } */

   int rad =    cellens_rad - 1;        /* kan skrivas om så att rad aldrig är < 0 */
   int kolumn = cellens_kolumn - 1;     /* dito */
   while (rad <= (cellens_rad+1))
   {
      while (kolumn <= (cellens_kolumn + 1))
      {
	 if ( rad >= 0 && kolumn >= 0 &&                                               /* kontroll att sökningen är i cellerna, kan skrivas om så att rad/kolumn aldrig är utanför */
	      rad < nuvarande_situation->rows && kolumn < nuvarande_situation->cols && /* inte nedanför eller för långt till höger */
	      !( rad == cellens_rad && kolumn == cellens_kolumn) &&                    /* se till att inte räkna med cellen självt !!! */
	      ((cell *) nuvarande_situation->cells[rad])[kolumn].current == ALIVE)     /* lever grannen ? */
	 {
	    resultat++;
	 }
	 kolumn++;
      }

      /* se till att starta om sökningen i vänster position */
      kolumn = cellens_kolumn - 1; /* kan skrivas om så att kolumn aldrig är < 0 */
      rad++;
   }

   assert( resultat >= 0 && resultat < 9);   /* omöjlighet, en cell kan inte har mer än 8 grannar */

   return( resultat);
}

/* Krav från uppgiftsektionen för Mästarprov 9:
 *   6: flytta flertalet funktioner från mp8.c till game_of_life.c
 *
 * skulle kunna skrivas om till att vara rekursiv
 */
void utskrift( field* nuvarande_situation)
{
   int rad = 0;
   int kolumn = 0;

   while ( rad < nuvarande_situation->rows)
   {
      while ( kolumn < nuvarande_situation->cols)
      {
	 switch ( ((cell *)nuvarande_situation->cells[rad])[kolumn].current)
	 {
	    case ALIVE:
	       putc( 'X', stdout);
	       break;

	    case DEAD:
	    default:
	       putc( '.', stdout);
	       break;
	 }
	 putc( ' ', stdout);
	 kolumn++;
      }
      putc( '\n', stdout);
      kolumn = 0;  /* ny rad, skriv ut från vänster kant */
      rad++;
   }
}

/* Beskrivning: låt en generation kvarstå, dö ut eller födas
 * parametrar:
 * returnerar:
 *
 * använder sidoeffekter
 */
void generationsskifte( field* nuvarande_situation)
{
   /* låt en generation gå
    */
   int rad = 0;
   int kolumn = 0; /* start i vänster kolumn */

   while ( rad < nuvarande_situation->rows)
   {
      while ( kolumn < nuvarande_situation->cols)
      {
	 if ( nuvarande_situation->cells[rad][kolumn].next == ALIVE)
	 {
	    nuvarande_situation->cells[rad][kolumn].current = ALIVE;
	 }
	 else
	 {
	    nuvarande_situation->cells[rad][kolumn].current = DEAD;
	 }

	 kolumn++;
      }

      kolumn = 0; /* ny rad, börja i vänstra kolumnen */
      rad++;
   }
}

/* Beskrivning: sök igenom cellerna i ytan och tag beslut på vilka celler som
 *              ska fortsätta vara bebodd, bli tom (död) eller fortsätta antingen som tom
 *              eller ska få en ny inneboende
 * Input:       nuvarande_situation - hur världen ser ut just nu, vilka celler som är tomma eller bebodda
 * Output:      om current har ett odefinierat värde (ej ALIVE/DEAD) returneras 1
 *              förändringar görs i övrigt med sidoeffekter, för varje cell finns det ett fält där framtida
 *              status lagras
 *
 * framtida status används i generationsskifte()
 *
 * använder sidoeffekter
 */
int kalkylera_generationsskifte( field* nuvarande_situation)
{
   int resultat = 0; /* sätts till 1 om något blev helt fel
		      */
   /* iterering igenom cellerna för att hitta nödvändiga förändringar
    *
    * se: https://conwaylife.com/wiki/Conway%27s_Game_of_Life
    */
   int rad = 0;
   int kolumn = 0;
   int grannar;

   while ( rad < nuvarande_situation->rows &&
	   !resultat)
   {
      while ( kolumn < nuvarande_situation->cols &&
	      !resultat)
      {
	 grannar = antalgrannar( nuvarande_situation, rad, kolumn);

	 /* test av felhantering i funktionen - provocerar fram fel
	  *
	  * if ( rad == 2 && kolumn == 1)
	  * (( cell *) nuvarande_situation->cells[rad])[kolumn].current = 'a';
	  */
	 switch( (( cell *) nuvarande_situation->cells[rad])[kolumn].current)
	 {
	    case ALIVE:
	       switch( grannar)
	       {
		  case 0:     /* inte tillräckligt mycket stöd från grannar för att överleva */
		  case 1:
		     (( cell *) nuvarande_situation->cells[rad])[kolumn].next = DEAD;
		     break;
		  case 2:     /* två eller tre levande grannar - överlever */
		  case 3:
		     (( cell *) nuvarande_situation->cells[rad])[kolumn].next = ALIVE;
		     break;
		  case 4:     /* överbefolkning */
		  default:
		     (( cell *) nuvarande_situation->cells[rad])[kolumn].next = DEAD;
		     break;
	       }
	       break;
	    case DEAD:       /* tom cell, kan den få någon inneboende i nästa generation ? */
	       if ( grannar == 3) /* tre levande grannar så en klon migrerar till cellen */
	       {
		  (( cell *) nuvarande_situation->cells[rad])[kolumn].next = ALIVE;
	       }
	       break;
	    default:    /* omöjligt, hit ska vi inte kunna komma eftersom init_field ska sätta de till DEAD !*/
	       resultat = 1;
	 }
	 kolumn++;
      }

      kolumn = 0; /* ny rad, börja i vänstra kolumnen */
      rad++;
   }

   return resultat;
}

/* Description: Initialisera alla celler till dead/tom, läs in från
 *              infil vilka som ska vara upptagna/levande
 * Input:       pekare till en yta
 *              pekare till FILE-struktur för infil
 * Output:      int som markör för om inläsningen lyckades
 *
 * modifierad för att använda sig av field-strukturen
 *
 * Krav från uppgiftsektionen för Mästarprov 9:
 *   6: flytta flertalet funktioner från mp9.c till game_of_life.c
 */
int init_field(field *nuvarande_situation, FILE *infil)
{
   /* if (nuvarande_situation != 0 && */
   /*     nuvarande_situation->cells != 0) */
   /* { */
   /*    avallokera_celler(nuvarande_situation); */
   /* } */

   int resultat = 1; /* returnera FAIL i normalfallet
		      */
   if (!load_config_from_file(nuvarande_situation, infil))
   {
      resultat = 0;
   }

   return( resultat);
}

/*
 * Local Variables:
 * c-file-style: "ellemtel"
 * compile-command: "make"
 * End:
 */
